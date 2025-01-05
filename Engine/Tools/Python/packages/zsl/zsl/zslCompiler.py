

import json
import os
import shutil
import subprocess

from zslShader import ShaderParser
from zslTypes import ShaderStage, ZSLSyntax, ZSLError

class ShaderCompiler:
    def __init__(self, shaderName, srcDir, outDir):
        self.shaderName = shaderName
        self.srcDir = srcDir
        self.outDir = outDir

        self.glslcExec = os.getenv("ZE_GLSLC_EXEC")
        self.fxcExec = os.getenv("ZE_FXC_EXEC")
        if not self.glslcExec and not self.fxcExec:
            raise ZSLError('Expected a shader compiler to have been defined')

    def compile(self):
        metafile = os.path.join(self.srcDir, f'{self.shaderName}.zsl.json')
        if not os.path.exists(metafile):
            raise ZSLError(f"Shader metadata was not found")
        
        os.makedirs(self.outDir, exist_ok = True)
        
        with open(metafile, 'r') as file:
            data = json.load(file)
            if data["stages"]:
                for stage in data["stages"]:
                    shaderStage = ShaderStage(stage)
                    extension = ShaderParser.getShaderExtension(shaderStage)
                    input = os.path.abspath(os.path.join(self.srcDir, f'{self.shaderName}.{extension}.hlsl'))
                    output = os.path.abspath(os.path.join(self.outDir, f'{self.shaderName}.{extension}'))
                    if self.glslcExec:
                        self._compileSPIRV(input, f'{output}.spv')
                    if self.fxcExec:
                        self._compileHLSLv5(input, f'{output}.cso', shaderStage)
            else:
                ZSLError('Expected shader meta data stages')

        shutil.copy(metafile, self.outDir)

            
    def _compileSPIRV(self, input, output):
        try:
            subprocess.run(
                [self.glslcExec,
                 "-x", "hlsl",
                 input, 
                 "-o", output,
                #  '-I', includeDirs,
                 f'-D{ZSLSyntax.SPIRV_OPTION}=1', 
                 '-MD'
                 ],
                check=True,
                stdout=subprocess.DEVNULL
            )
        except subprocess.CalledProcessError as e:
            raise ZSLError(f"Error compiling shader: {input}")
            

    def _compileHLSLv5(self, input: str, output: str, shaderStage: ShaderStage):
        shaderType = ""
        if shaderStage is ShaderStage.Vertex:
            shaderType = "vs"
        elif shaderStage is ShaderStage.Fragment:
            shaderType = "ps"
        elif shaderStage is ShaderStage.Compute:
            shaderType = "cs"
        
        if not shaderType:
            raise ZSLError(f'Shader type {shaderStage.value} is not supported')

        try:
            subprocess.run(
                [self.fxcExec, 
                 '/Fo', output, 
                 '/T', f'{shaderType}_5_0',
                #  f'/I{includeDirs}', 
                 '/E', 'main', 
                 '/Zi',
                 input
                 ],
                check=True,
                stdout=subprocess.DEVNULL
            )
        except subprocess.CalledProcessError as e:
            raise ZSLError(f"Error compiling shader: {input}")