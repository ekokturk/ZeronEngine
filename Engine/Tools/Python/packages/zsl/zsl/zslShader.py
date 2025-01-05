
import os

from pyparsing import OneOrMore
import zslTypes as zsl
import json


class ShaderParser:
    def __init__(self):
        # Shader data
        self.name = None
        self.countRegisterType = {type: 0 for type in zsl.RegisterType}
        self.countRegisterSet = [0, 0, 0]
        self.stageLayouts: list[zsl.StageLayout] = []
        self.bindings: list[zsl.BindingDefinition] = []
        self.pushConstants: list[zsl.ConstantBufferDefinition] = []

        # Parse types
        self.sharedContent: list[zsl.SharedSection] = []
        self.stageContent: list[zsl.StageSection] = []
        self.shaderOut = {stage: None for stage in zsl.ShaderStage}


    def process(self, content):
        self._preparse(content)
        self._parse()

    def generateShader(self, name, directory):
        self.name = name
        outDir = os.path.abspath(directory)
        os.makedirs(outDir, exist_ok=True)
        for stage, value in self.shaderOut.items():
            if not value:
                continue
            shaderFilename = f'{name}.{ShaderParser.getShaderExtension(stage)}.hlsl'
            with open(os.path.join(outDir, shaderFilename), 'w') as file:
                file.write(value)

        metadata = self._generateMeta()
        with open(os.path.join(outDir, f'{name}.zsl.json'), 'w') as f:
            json.dump(metadata, f, indent=4)

    def _preparse(self, content):
        """Extract shader stages and shared content."""
        preparseGrammer = OneOrMore(
            zsl.StageSection.PARSER | 
            zsl.SharedSection.PARSER
        )
        preparseData = preparseGrammer.parseString(content, parseAll=True)
        for entry in preparseData:
            if isinstance(entry, zsl.SharedSection):
                self.sharedContent.append(entry.body)
            if isinstance(entry, zsl.StageSection):
                self.stageContent.append(entry)

    def _parse(self):
        """Extract shader stages and shared content."""
        sharedContentStr = "\n".join(self.sharedContent)
        shaderGrammer = OneOrMore(
            # Bindings
              zsl.SamplerBinding.PARSER
            | zsl.TextureBinding.PARSER
            | zsl.ConstantBufferDefinition.PARSER 
            | zsl.StorageBufferBinding.PARSER 
            # Directives
            | zsl.IncludeDirectiveDefinition.PARSER
            | zsl.DefineDirectiveDefinition.PARSER
            # General
            | zsl.CommentDefinition.PARSER
            | zsl.StructDefinition.PARSER 
            | zsl.FunctionDefinition.PARSER 
            | zsl.GlobalVariableDefinition.PARSER
            | zsl.ConditionDirectiveDefinition.PARSER
        )

        for stageContent in self.stageContent:
            stage: zsl.ShaderStage = stageContent.stage
            stageLayout = zsl.StageLayout(type=stage)
            code = "\n".join([sharedContentStr, stageContent.body])
            parsedCode = shaderGrammer.parseString(code, parseAll=True)
            globalDirectives = self._getGlobalDirectives(stage)
            outCode = [item.stringify() for item in globalDirectives]
            for entry in parsedCode:
                if isinstance(entry, zsl.Definition):
                    if isinstance(entry, zsl.BindingDefinition):
                        self._registerBinding(entry, stage)
                    if isinstance(entry, zsl.StructDefinition):
                        self._registerStruct(entry, stageLayout)
                    defString = entry.stringify()
                    if defString:
                        outCode.append(defString)

            self.shaderOut[stage] = '\n'.join(outCode)
            if stageLayout.output or stageLayout.input:
                self.stageLayouts.append(stageLayout)


    def _registerBinding(self, binding: zsl.BindingDefinition, stage: zsl.ShaderStage):
        """Register binding definition to shader bindings."""

        # Type registers always incremented as push constants are not supported for some HLSL version
        binding.bindPoint.typed = self.countRegisterType[binding.registerType]
        self.countRegisterType[binding.registerType] +=1
        binding.stage = stage

        if isinstance(binding, zsl.ConstantBufferDefinition) and binding.isPushConstant:
            binding.stride = self._calculateStride(binding.members)
            binding.usePaddding = True
            if any(pc.stage == binding.stage for pc in self.pushConstants):
                raise zsl.ZSLError(f"Push constant is already defined for {binding.stage.value} stage (name: {binding.name})")
            if binding.stage == zsl.ShaderStage.Compute:
                raise zsl.ZSLError(f"Push constant is not supported for compute shader (name: {binding.name})")
            if binding.getPaddedStride() > 128:
                raise zsl.ZSLError(f"Push constant is too large  (name: {binding.name})")

            self.pushConstants.append(binding)
        else:
            binding.bindPoint.index = self.countRegisterSet[binding.bindPoint.set]
            self.countRegisterSet[binding.bindPoint.set] +=1
            self.bindings.append(binding)


    def _registerStruct(self, struct: zsl.StructDefinition, stageLayout: zsl.StageLayout):
        for member in struct.members:
            metadata = member.metadata
            isInstanced = True if 'instanced' in metadata else None
            slot = int(metadata.get('slot')) if 'slot' in metadata else 0
            # Override HLSL type if we have defined a custom type
            if 'type' in metadata and not zsl.CustomFormatTypes.has_value(metadata.get('type')):
                raise zsl.ZSLError(f"Custom format '{metadata.get('type')}' is not supported!")
            type = metadata.get('type') if 'type' in metadata else member.type

            if 'input' in metadata:
                location = metadata.get('input')

                for existingInput in stageLayout.input:
                    if existingInput.semantic == member.semantic or existingInput.location == location:
                        raise zsl.ZSLError(f'Shader input needs to be unique')

                stageLayout.input.append(
                    zsl.LayoutEntry(type=type, semantic=member.semantic, 
                                    location=location, slot=slot, isInstanced=isInstanced)
                )
            elif 'output' in metadata:
                stageLayout.output.append(
                    zsl.LayoutEntry(type=type, semantic=member.semantic, 
                                    location=metadata.get('output'))
                )
            
    def _calculateStride(self, members: list[zsl.VariableDefinition]):
        val = 0
        for member in members:
            if zsl.Primitive.has_value(member.type):
                count = member.listSize if member.listSize else 1
                val += zsl.Primitive.getSize(member.type) * count
            else:
                raise zsl.ZSLError(f"Custom type stride calculation is not supported. ({member.name}: {member.type})")
        return val


    def _generateMeta(self):
        return {
            "name": self.name,
            "stages": [content.stage.value.lower() for content in self.stageContent],
            "layout": {key: value for layout in self.stageLayouts for key, value in layout.getDict().items()},
            "bindings": [ binding.getDict() for binding in self.bindings ],
            "pushConstants": [ binding.getDict() for binding in self.pushConstants ],
        }

    def _getGlobalDirectives(self, stage: zsl.ShaderStage):
        return [
            zsl.PragmaDirectiveDefinition('pack_matrix(row_major)'),
            zsl.ConditionDirectiveDefinition(
                zsl.ZSLSyntax.SPIRV_OPTION, 
                # SPIRV defintions
                '\n'.join([
                    zsl.PragmaDirectiveDefinition(f'shader_stage({stage.value.lower()})').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getLocation('n'), '[[vk::location((n))]]').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getBinding('n', 's'), '[[vk::binding(n, s)]] ').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getPushConstant(), '[[vk::push_constant]] ').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getRegister('n'), '').stringify()
                ]),
                # HLSL defintions
                '\n'.join([
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getLocation('n'), '').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getBinding('n', 's'), '').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getPushConstant(), '').stringify(),
                    zsl.DefineDirectiveDefinition(zsl.ZSLSyntax.getRegister('n'), ': register(n)').stringify()
                ])
            )
        ]

    def getShaderExtension(stage: zsl.ShaderStage):
        if stage == zsl.ShaderStage.Vertex:
            return 'vert'
        elif stage == zsl.ShaderStage.Fragment:
            return 'frag'
        elif stage == zsl.ShaderStage.Compute:
            return 'comp'
        else:
            return None
        