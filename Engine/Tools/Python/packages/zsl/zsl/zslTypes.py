

from enum import Enum
import math
import typing as typing
from pyparsing import *
from abc import ABC, abstractmethod
from dataclasses import dataclass, field

from pyparsing import Any

#region Common Types

# Generic HLSL
IDENTIFIER = Word(alphas + "_", alphanums + "_")
STRUCT = Keyword("struct")
CBUFFER = Keyword("cbuffer")
REGISTER = Keyword("register")
SEMICOLON = Suppress(";")
QUOTE = Suppress("\"")
OPEN_BRACE = Suppress("{")
CLOSE_BRACE = Suppress("}")
OPEN_PARENTHESIS = Suppress("(")
CLOSE_PARENTHESIS = Suppress(")")
COMMA = Suppress(",")
COMMENT_INLINE = Suppress('//')
DIRECTIVE = Suppress("#")

class CustomFormatTypes(Enum):
    Color = 'color'

    @classmethod
    def has_value(cls, value):
        return value in cls._value2member_map_

class Primitive(Enum):
    Float = 'float'
    Float2 = 'float2'
    Float3 = 'float3'
    Float4 = 'float4'
    Float4X4 = 'float4x4'
    UInt = 'uint'
    Int = 'int'
    Int2 = 'int2'
    Int3 = 'int3'
    Int4 = 'int4'

    @classmethod
    def has_value(cls, value):
        return value in cls._value2member_map_
    
    @classmethod
    def getSize(cls, key: str) -> typing.Optional[int]:
        if Primitive.has_value(key):
            found = cls._value2member_map_[key]
            return found.size
        return None

    @property
    def size(self):
        if self == Primitive.Float:
            return 4
        elif self == Primitive.Float2:
            return 4 * 2
        elif self == Primitive.Float3:
            return 4 * 3
        elif self == Primitive.Float4:
            return 4 * 4
        elif self == Primitive.Float4X4:
            return 4 * 16
        elif self == Primitive.UInt:
            return 4
        elif self == Primitive.Int:
            return 4
        elif self == Primitive.Int2:
            return 4 * 2
        elif self == Primitive.Int3:
            return 4 * 3
        elif self == Primitive.Int4:
            return 4 * 4
        else:
            raise ZSLError(f"Primitive size is not defined for {self.name}")


class ShaderStage(Enum):
    Vertex = "vertex"
    Fragment = "fragment"
    Compute = "compute"

class BindingType(Enum):
    PushConstant = 'pushConstant'
    Uniform = 'uniform'
    Sampler = 'sampler'
    Texture = 'texture'
    StorageBuffer = 'storageBuffer'

class RegisterType(Enum):
    ConstantBuffer = 'b'
    Sampler = 's'
    Texture = 't'
    UnorderedAccessView = 'u'

class SamplerType(Enum):
    SamplerState = "SamplerState"

class TextureType(Enum):
    Texture1D = "Texture1D"
    Texture1DArray = "Texture1DArray"
    Texture2D = "Texture2D"

class StorageBufferType(Enum):
    StructuredBuffer = "StructuredBuffer"
    RWStructuredBuffer = "RWStructuredBuffer"

# ZSL
class ZSLSyntax():
    META = Suppress('//@')
    SPIRV_OPTION = 'ZE_SHADER_SPIRV'

    @staticmethod
    def getRegister(input: str):
        return f'zsl_register({input})'

    @staticmethod
    def getLocation(input: str):
        return f'zsl_location({input})'
    
    @staticmethod
    def getBinding(input: str, set: typing.Optional[str] = None):
        return f'zsl_binding({input}, {set if set else 0})'
    
    @staticmethod
    def getPushConstant():
        return f'zsl_pushConstant'

@dataclass
class LayoutEntry():
    type: str
    semantic: str
    location: int 
    slot: int = 0
    isInstanced: typing.Optional[bool] = None

    def getDict(self):
        data = {
            'type': self.type,
            'semantic': self.semantic,
            'location': self.location,
        }
        if self.isInstanced:
            data['instanced'] = True
        if self.slot:
            data['slot'] = self.slot

        return data
@dataclass
class StageLayout():
    type: ShaderStage
    input: typing.List[LayoutEntry] =  field(default_factory=list)
    output: typing.List[LayoutEntry] = field(default_factory=list)

    def getDict(self) -> dict:
        out = {
            self.type.value.lower(): {
                'input': [input.getDict() for input in self.input],
                'output': [output.getDict() for output in self.output],
            }
        }
        return out

@dataclass
class BindingPoint():
    set: int
    index: int
    typed: int

class MetaData(dict):
    PARSER = (
            OneOrMore(Group(ZSLSyntax.META + IDENTIFIER('key') + restOfLine('value')))
        ).setParseAction(
            lambda t: MetaData.fromParsed(t)
        )
    
    def get(self, key: str):
        return self[key].replace(" ", "")
    
    @staticmethod
    def fromParsed(element: ParserElement) -> 'MetaData':
        meta = MetaData()
        for item in element:
            meta[item[0]] = item[1]
        return meta

#endregion

#region Section
class StageSection():
    PARSER = (
        Group(ZSLSyntax.META + Suppress('begin_stage')) 
            + IDENTIFIER('stage')
            + SkipTo(Group(ZSLSyntax.META + Suppress('end_stage')))('body') 
            + Group(ZSLSyntax.META + Suppress('end_stage'))
        ).setParseAction(
            lambda t: StageSection(t.stage, t.body)
        )

    def __init__(self, stage, body):
        try:
            self.stage: ShaderStage = ShaderStage[stage]
        except KeyError:
            raise ZSLError(f'Invalid shader stage: {stage}')
        self.body: str = body

@dataclass
class SharedSection():
    body: str

    PARSER = (
            SkipTo(Group(ZSLSyntax.META + Suppress('begin_stage'))  | Group(ZSLSyntax.META + Suppress('end_stage')))('body')
        ).setParseAction(
            lambda t: SharedSection(t.body)
        )

#endregion

#region Definition

class Definition(ABC):
    @abstractmethod
    def stringify(self) -> str:
        """Convert defintion to a HLSL string representation."""
        pass


@dataclass
class VariableDefinition(Definition):
    type: str
    name: str
    semantic: typing.Optional[str] = None
    value: typing.Optional[str] = None
    isList: bool = False
    listSize: typing.Optional[int] = None
    metadata: MetaData = field(default_factory=MetaData)

    PARSER = (
        Optional(MetaData.PARSER('metadata')) 
        + IDENTIFIER("type") 
        + IDENTIFIER("name") + 
          Optional((Suppress('[') + Word(nums)('listSize') + Suppress(']'))('isList'))
    )

    def getListStr(self):
        listSize = f'{self.listSize}' if self.listSize else ''
        return f"[{listSize}]" if self.isList else ''

    def stringify(self):
        valueStr = f' = {self.value}' if self.value else ''
        semanticStr = f' : {self.semantic}' if self.semantic else ''
        return f"{self.type} {self.name}{self.getListStr()}{semanticStr}{valueStr}"
    
    @staticmethod
    def asArrayFromParsed(parsedVariables) : 
        return [
            VariableDefinition(type=variable.type, 
                               name=variable.name, 
                               metadata=variable.metadata,
                               semantic=variable.semantic,
                               isList=(True if variable.isList else False),
                               listSize=(int(variable.listSize) if variable.listSize else None),
                               value=variable.value,
                               ) 
            for variable in parsedVariables
        ]

class CommentDefinition(Definition):
    PARSER = (
            Suppress( Regex(r'//[^@]')) + restOfLine
        )
    
#endregion

#region BindingDefinition

class BindingDefinition(Definition):
    def __init__(self, bindingType:  BindingType, registerType: RegisterType, name: str, metadata: MetaData, dataType: typing.Optional[str] = None):
        self.bindingType = bindingType
        self.dataType = dataType
        self.registerType = registerType
        self.bindPoint = BindingPoint(0, 0, 0) 
        self.name: str = name
        self.tag: typing.Optional[str] = None
        self.hasRegister = False
        self.stage: typing.Optional[ShaderStage] = None
        
        # Process metadata
        if 'register' in metadata:
            self.hasRegister = True
            if 'set' in metadata:
                self.bindPoint.set = int(metadata.get('set'))

        if 'tag' in metadata:
            self.tag = metadata.get('tag')

    def getRegisterStr(self, hasRegister: bool) -> str:
        bindingStr = f'{self.registerType.value}{self.bindPoint.typed}'
        return f' {ZSLSyntax.getRegister(bindingStr)}' if hasRegister else ''

    def getBindingStr(self) -> str:
        return f'{ZSLSyntax.getBinding(self.bindPoint.index, self.bindPoint.set)} ' if self.hasRegister else ''
    
    def getDict(self):
        data = {
            'stage': self.stage.value,
            'bindingType': self.bindingType.value,
            'registerType': self.registerType.value,
            'index': self.bindPoint.index,
            'typeIndex': self.bindPoint.typed,
            'set': self.bindPoint.set,
            'name': self.name,
        }
        if self.dataType:
            data['dataType'] = self.dataType
        return data


class SamplerBinding(BindingDefinition):
    TYPES = oneOf(
        [type.value for type in SamplerType]
    )
    PARSER = (
            Optional(MetaData.PARSER('metadata')) + TYPES("dataType") + IDENTIFIER("name") + SEMICOLON
        ).setParseAction(
            lambda t: SamplerBinding(name=t.name, dataType=t.dataType, metadata=t.metadata)
        )
    
    def __init__(self, name: str, dataType: str, metadata: MetaData):
        super().__init__(bindingType=BindingType.Sampler, registerType=RegisterType.Sampler, name=name, metadata=metadata, dataType=dataType)

    def stringify(self):
        return f"{self.getBindingStr()} {self.dataType} {self.name}{self.getRegisterStr(self.hasRegister)};"


class TextureBinding(BindingDefinition):
    TYPES = oneOf(
        [type.value for type in TextureType]
    )
    PARSER = (
            Optional(MetaData.PARSER('metadata')) + TYPES("dataType") + IDENTIFIER("name") + SEMICOLON
        ).setParseAction(
            lambda t: TextureBinding(name=t.name, dataType=t.dataType, metadata=t.metadata)
        )
    
    def __init__(self, name: str, dataType: str, metadata: MetaData):
        super().__init__(bindingType=BindingType.Texture, registerType=RegisterType.Texture, name=name, metadata=metadata, dataType=dataType)

    def stringify(self):
        return f'{self.getBindingStr()} {self.dataType} {self.name}{self.getRegisterStr(self.hasRegister)};'


class ConstantBufferDefinition(BindingDefinition):
    """
    Represents a cbuffer for HLSL. It can be used as uniform or push constant.
    """
    PARSER = (
            Optional(MetaData.PARSER('metadata')) +
            "cbuffer" + IDENTIFIER("name") +
            OPEN_BRACE + 
            OneOrMore(Group(
                        VariableDefinition.PARSER
                      + Optional(Suppress("=") + SkipTo(";")("value"))
                      + SEMICOLON)
                )("members") + 
            CLOSE_BRACE + Optional(SEMICOLON)
        ).setParseAction(
            lambda t: ConstantBufferDefinition(name=t.name, members=VariableDefinition.asArrayFromParsed(t.members), metadata=t.metadata)
        )

    def __init__(self, name, members, metadata):
        super().__init__(bindingType=BindingType.Uniform, registerType=RegisterType.ConstantBuffer, name=name, metadata=metadata)
        self.members = members
        self.stride: typing.Optional[int] = None
        self.usePaddding: bool = False
        self._isPushConstant = False
        if 'push_constant' in metadata:
            if self.hasRegister:
                raise ZSLError(f"Push constants cannot use register tag (name: {self.name})")
            self._isPushConstant = True
            self.bindingType = BindingType.PushConstant

    @property
    def isPushConstant(self):
        return self._isPushConstant
    
    def getPaddedStride(self):
        return self.stride + (16 - self.stride % 16) if self.stride % 16 != 0 else self.stride
    
    def getBindingStr(self):
        return f'{ZSLSyntax.getPushConstant()} ' if self.isPushConstant else super().getBindingStr() 
    
    def getDict(self):
        dict = super().getDict() 
        if self.isPushConstant:
            dict.pop('set')
            dict.pop('index')
        if self.stride:
            stride = self.stride
            if self.usePaddding:
               stride = self.getPaddedStride()
            dict['stride'] = stride
        return dict

    def stringify(self):
        membersStr = "\n".join(f'\t{member.stringify()};' for member in self.members)
        if self.usePaddding and self.stride:
            b4Padding = 0 if (self.stride % 16) == 0 else 16 - (self.stride % 16)
            if b4Padding != 0:
                b4Var = VariableDefinition(type=Primitive.UInt.value, name=f"_padding4b{self.name}", isList=True, listSize=math.floor(b4Padding/4))
                membersStr += f"\n\t{b4Var.stringify()};"
        
        return f"{self.getBindingStr()}cbuffer {self.name}{self.getRegisterStr(self.hasRegister or self.isPushConstant)}\n{{\n{membersStr}\n}}"

class StorageBufferBinding(BindingDefinition):
    TYPES = oneOf(
        [type.value for type in StorageBufferType]
    )

    PARSER = (
            Optional(MetaData.PARSER('metadata')) + (TYPES('bufferType') + '<' + IDENTIFIER('dataType') + '>') + IDENTIFIER("name") + SEMICOLON
        ).setParseAction(
            lambda t: StorageBufferBinding(name=t.name, bufferType=t.bufferType, dataType=t.dataType, metadata=t.metadata)
        )
    
    def __init__(self, name: str, bufferType: str,  dataType: str, metadata: MetaData):
        super().__init__(bindingType=BindingType.StorageBuffer, 
                         registerType=StorageBufferBinding.getRegisterType(bufferType), 
                         name=name, metadata=metadata, 
                         dataType=dataType)
        self.bufferType = bufferType

    def getRegisterType(bufferType: str):
        if bufferType == StorageBufferType.StructuredBuffer.value:
            return RegisterType.Texture
        if bufferType == StorageBufferType.RWStructuredBuffer.value:
            return RegisterType.UnorderedAccessView
        
        raise ZSLError(f"Storage buffer type '{bufferType}' is not supported")


    def stringify(self):
        return f'{self.getBindingStr()} {self.bufferType}<{self.dataType}> {self.name}{self.getRegisterStr(self.hasRegister)};'

#endregion

@dataclass
class ConditionDirectiveDefinition(Definition):
    condition: str
    ifBody: str
    elseBody: typing.Optional[str] = None

    PARSER = (
            Suppress("#if") 
            + IDENTIFIER("condition")
            + (SkipTo("#else") | SkipTo("#endif"))("ifBody")
            + Optional(Suppress("#else") + SkipTo("#endif")("elseBody")) 
            + Suppress("#endif")
        ).setParseAction(
            lambda t: ConditionDirectiveDefinition(condition=t.condition, ifBody=t.ifBody, elseBody=t.elseBody)
        )

    def stringify(self):
        ifbody = self.ifBody.rstrip('\n')
        elseBody = f'#else\n{self.elseBody}'.rstrip('\n') if self.elseBody else ''
        return f"#if {self.condition}\n{ifbody}\n{elseBody}\n#endif"

class PragmaDirectiveDefinition(Definition):
    PARSER = (
            Suppress("#pragma") + restOfLine("value")
        ).setParseAction(
            lambda t: PragmaDirectiveDefinition(t.value)
        )

    def __init__(self, value):
        self.value = value

    def stringify(self):
        return f"#pragma {self.value}"

@dataclass
class DefineDirectiveDefinition(Definition):
    key: str
    value: str

    PARSER = (
            Suppress("#define") + IDENTIFIER("key") + Optional(IDENTIFIER)("value")
        ).setParseAction(
            lambda t: DefineDirectiveDefinition(t.key, t.value)
        )

    def stringify(self):
        return f"#define {self.key} {self.value}"

@dataclass
class IncludeDirectiveDefinition(Definition):
    path: str
    expand: str

    PARSER = (
            Suppress("#include") + (QuotedString('"') | QuotedString('<', endQuoteChar='>'))("path")
        ).setParseAction(
            lambda t: IncludeDirectiveDefinition(t.path, False)
        )

    def stringify(self):
        return f"#include \"{self.path}\""

    
@dataclass
class GlobalVariableDefinition(Definition):
    type: str
    name: str
    metadata: MetaData

    PARSER = (
             IDENTIFIER("type") + IDENTIFIER("name") + SEMICOLON
        ).setParseAction(
            lambda t: GlobalVariableDefinition(name=t.name, type=t.type, metadata=t.metadata)
        )

    def stringify(self):
        return f"{self.type} {self.name};"

@dataclass
class StructDefinition(Definition):
    name: str
    members: list[VariableDefinition]

    PARSER = (
            "struct" + IDENTIFIER("name") +
            OPEN_BRACE + 
            OneOrMore(
                Group(VariableDefinition.PARSER 
                      + Optional(Suppress(":") + IDENTIFIER("semantic")) 
                      + Optional(Suppress("=") + SkipTo(";")("value")) 
                      + Suppress(";")
            ))("members") + 
            CLOSE_BRACE + SEMICOLON
        ).setParseAction(
            lambda t: StructDefinition(t.name, VariableDefinition.asArrayFromParsed(t.members))
        )

    def stringify(self):
        membersStr = "\n".join(f'\t{self.getMemberPrefixStr(member)}{member.stringify()};' for member in self.members)
        return f"struct {self.name}\n{{\n{membersStr}\n}};"

    def getMemberPrefixStr(self, member: VariableDefinition):
        if 'input' in member.metadata:
            location = member.metadata['input'].replace(" ", "")
            return ZSLSyntax.getLocation(location) + ' '
        elif 'output' in member.metadata:
            location = member.metadata['output'].replace(" ", "")
            return ZSLSyntax.getLocation(location) + ' '
        else: 
            return ''

@dataclass
class FunctionDefinition(Definition):
    name: str
    returnType: str
    args: Any
    body: str
    semantic: typing.Optional[str] = None
    decorator: typing.Optional[str] = None

    PARSER = (
            Optional(originalTextFor(
                Suppress('[') 
                    + (Suppress("numthreads") + OPEN_PARENTHESIS + Word(nums) + COMMA + Word(nums) + COMMA + Word(nums) + CLOSE_PARENTHESIS) 
                + Suppress(']')
                )('decorator')
            )
            + originalTextFor(IDENTIFIER | (IDENTIFIER + '<' + IDENTIFIER + '>'))('returnType') + IDENTIFIER('name') 
            + (OPEN_PARENTHESIS 
                + Optional(ZeroOrMore(Group(VariableDefinition.PARSER) + Optional(Suppress(":") + IDENTIFIER("semantic"))  + COMMA) 
                + Group(VariableDefinition.PARSER + Optional(Suppress(":") + IDENTIFIER("semantic")) )) 
            + CLOSE_PARENTHESIS)('args')
            + Optional(Suppress(":") + IDENTIFIER("semantic"))  
            + originalTextFor(nestedExpr('{', '}'))('body')
        ).setParseAction(
            lambda t: FunctionDefinition(
                name=t.name, 
                returnType=t.returnType, 
                args=VariableDefinition.asArrayFromParsed(t.args), 
                body=t.body, 
                semantic=t.semantic,
                decorator=t.decorator) 
        )

    def stringify(self):
        semanticStr = f' : {self.semantic}' if self.semantic else ''
        argStr = ", ".join(f'{arg.stringify()}' for arg in self.args)
        return f"{self.decorator}\n{self.returnType} {self.name}({argStr}){semanticStr}\n{self.body};"

class ZSLError(Exception):
    def __init__(self, message, code=None):
        super().__init__(message)

    def __str__(self):
        return f"[ZSL ERROR]: {self.args[0]}"

