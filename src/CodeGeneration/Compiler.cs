using System;
using System.Collections.Generic;
using System.Threading;
using CodeGeneration.Parser;
using Mono.Cecil;
using Mono.Cecil.Cil;

namespace CodeGeneration
{
    public class Compiler
    {
        public Dictionary<string, MethodDefinition> Methods = new Dictionary<string, MethodDefinition>();
        public Dictionary<string, FieldDefinition> Variables = new Dictionary<string, FieldDefinition>();

        public Dictionary<string, TypeReference> Types = new Dictionary<string, TypeReference>();

        private MethodDefinition bootstrap = null;

        public void EmitRoot(JsonEntity el)
        {
            var nameDef = new AssemblyNameDefinition("SuperGreeterBinary", new Version(1, 0, 0, 0));
            var asm = AssemblyDefinition.CreateAssembly(nameDef, "result.exe", ModuleKind.Console);

            this.Types.Add("Integer", asm.MainModule.ImportReference(typeof(Int32)));
//            this.Types.Add("Integer", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Int32));
//            this.Types.Add("Void", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Void));
            this.Types.Add("Real", asm.MainModule.ImportReference(typeof(Double)));
            this.Types.Add("Boolean", asm.MainModule.ImportReference(typeof(Boolean)));
            this.Types.Add("Void", asm.MainModule.ImportReference(typeof(void)));
            Console.WriteLine(this.Types["Void"]);

            bootstrap = new MethodDefinition("Main",
                MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, this.Types["Void"]);

            foreach (JsonEntity declaration in el.Children)
            {
                this.EmitDeclaration(declaration);
            }
            
            var ip = bootstrap.Body.GetILProcessor();
            
            ip.Emit(OpCodes.Pop);
            ip.Emit(OpCodes.Ret);
            
            var type = new TypeDefinition("supergreeter", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.ImportReference(typeof(object)));
            type = this.ImportStuffIntoModule(type);
            asm.MainModule.Types.Add(type);
            type.Methods.Add(bootstrap);

            asm.EntryPoint = bootstrap;
            asm.Write("./result.exe");
        }

        private TypeDefinition ImportStuffIntoModule(TypeDefinition type)
        {
            foreach (var variable in this.Variables)
            {
                type.Fields.Add(variable.Value);
            }

            return type;
        }

        public void EmitDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "SimpleDeclaration":
                    this.EmitSimpleDeclaration(ivasiq);
                    break;
                
                case "RoutineDeclaration":
                    this.EmitRoutineDeclaration(ivasiq);
                    break;
                
                default:
                    throw new Exception("Declaration Error");
            }
        }

        public void EmitRoutineDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            String functionName = declaration.Value;
            Console.WriteLine(functionName);
        }

        public void EmitSimpleDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "VariableDeclaration":
                    this.EmitVariableDeclaration(ivasiq);
                    break;

                default:
                    throw new Exception("Simple Declaration Error");
            }
        }

        private void EmitVariableDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            String type = null;
            
            if (ivasiq.Type == "type")
            {
                type = this.GetType(ivasiq);
            }
            if (type == null)
            {
                throw new Exception("Variable Decalrarion Error");
            }
            Console.WriteLine(declaration.Value);

            var fieldDefinition = new FieldDefinition(declaration.Value, FieldAttributes.Private, this.Types[type]);
            this.Variables.Add(declaration.Value, fieldDefinition);

            ivasiq = declaration.Children[0];

            switch (ivasiq.Type)
            {
                case "InitialValue":
                    this.EmitInitialValue(ivasiq);
                    break;

                default:
                    throw new Exception("Error");
            }

            var bootstrapIP = this.bootstrap.Body.GetILProcessor();
            switch (type)
            {
                case "Integer":
                    Console.Write("Storing whatever is on the stack into a field named: ");
                    Console.WriteLine(declaration.Name);
                    bootstrapIP.Emit(OpCodes.Stfld, fieldDefinition);
                    break;
            }
        }

        private string GetType(JsonEntity declaration)
        {
            return declaration.Value;
        }

        private void EmitInitialValue(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Expression":
                    this.EmitExpression(ivasiq);
                    break;

                default:
                    throw new Exception("Initial Value Error");
            }
        }

        private void EmitExpression(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Relation":
                    this.EmitRelation(ivasiq);
                    break;

                default:
                    throw new Exception("Expression Error");
            }
        }

        private void EmitRelation(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Simple":
                    this.EmitSimple(ivasiq);
                    break;

                default:
                    throw new Exception("Relation Error");
            }
        }

        private void EmitSimple(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Factor":
                    this.EmitFactor(ivasiq);
                    break;

                default:
                    throw new Exception("Simple error");
            }
        }

        private void EmitFactor(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Summand":
                    this.EmitSummand(ivasiq);
                    break;

                default:
                    throw new Exception("Factor error");
            }
        }

        private void EmitSummand(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Primary":
                    this.EmitPrimary(ivasiq);
                    break;

                default:
                    throw new Exception("Summand Error");
            }
        }

        private void EmitPrimary(JsonEntity declaration)
        {
            Console.Write("Storing this onto the stack: ");
            Console.WriteLine(declaration.Value);
            this.bootstrap.Body.GetILProcessor().Emit(
                OpCodes.Ldc_I4, int.Parse(declaration.Value));
        }
    }
}