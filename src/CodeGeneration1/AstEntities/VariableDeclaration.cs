using System;
using System.IO;
using Mono.Cecil;
using Mono.Cecil.Cil;

namespace CodeGeneration
{
    public class VariableDeclaration
    {
//        public TypeReference NativeType => Type.NativeType;
//        public VariableDefinition NativeVariable { get; }
        public TypeReference typeReference;
        public void GenerateVariableDeclaration(dynamic variableDeclaration, ILProcessor ip)
        {
            //typeReference = new TypeReference("Boolean", variableDeclaration.name);
            //ip.Emit(OpCodes.Stloc, typeReference);
            Console.WriteLine("YA TUT!");
        }
        
//        public BodyVariable(string type, string name) : base(type, name)
//        {
//            NativeVariable = new VariableDefinition(Type.NativeType);
//        }
    }
}