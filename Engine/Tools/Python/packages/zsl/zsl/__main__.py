import argparse
import os
import sys
from zslShader import ShaderParser
from zslCompiler import ShaderCompiler
from zslTypes import ZSLError
from pathlib import Path

def parse(input, output):
    if not os.path.exists(input):
        raise ZSLError(f"Shader file '{input}' not found!")

    with open(input, "r") as file:
        basicShader = file.read()
        parser = ShaderParser()
        parser.process(basicShader)
        parser.generateShader(Path(input).stem, output)

def compile(shaderName, srcDir, outDir):
    compiler = ShaderCompiler(shaderName, srcDir, outDir)
    compiler.compile()


if __name__ == "__main__":
    argParser = argparse.ArgumentParser(description='Zeron Shader Language')
    subparsers = argParser.add_subparsers(dest="command", help="ZSL commands")

    parseParser = subparsers.add_parser('parse', help="Parse ZSL shader")
    parseParser.add_argument('-shader', type=str, required=True, help='Path to the shader file')
    parseParser.add_argument('-outDir', type=str, required=True, help='Path to the generated output files')

    compileParser = subparsers.add_parser('compile', help="Compile ZSL shader")
    compileParser.add_argument('-shaderName', type=str, required=True, help='Name of the shader')
    compileParser.add_argument('-srcDir', type=str, required=True, help='Path to the generated output files')
    compileParser.add_argument('-outDir', type=str, required=True, help='Path to the compiled output files')

    args = argParser.parse_args()

    try:
        if args.command == "parse":
            parse(args.shader, args.outDir)
        elif args.command == "compile":
            compile(args.shaderName, args.srcDir, args.outDir)
        sys.exit(0)
    except Exception as e:
        print(e)
        sys.exit(1)
