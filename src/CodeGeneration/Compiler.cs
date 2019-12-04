using System;
using System.Collections.Generic;
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
            var nameDef = new AssemblyNameDefinition("CodeGenerator", new Version(1, 0, 0, 0));
            var asm = AssemblyDefinition.CreateAssembly(nameDef, "CodeGenerator", ModuleKind.Console);

//            this.Types.Add("Integer", asm.MainModule.ImportReference(typeof(Int32)));
            this.Types.Add("Integer", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Int32));
            this.Types.Add("Void", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Void));
//            this.Types.Add("Real", asm.MainModule.ImportReference(typeof(Double)));
//            this.Types.Add("Boolean", asm.MainModule.ImportReference(typeof(Boolean)));
//            this.Types.Add("Void", asm.MainModule.ImportReference(typeof(void)));
            

            bootstrap = new MethodDefinition("Context",
                MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, this.Types["Void"]);

            foreach (JsonEntity declaration in el.Children)
            {
                this.EmitDeclaration(declaration);
            }
            var ip = bootstrap.Body.GetILProcessor();
            ip.Emit(OpCodes.Pop);
            ip.Emit(OpCodes.Ret);
            
            var type = new TypeDefinition("CodeGenerator", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.ImportReference(typeof(object)));
            asm.MainModule.Types.Add(type);
            type.Methods.Add(bootstrap);

            this.ImportStuffIntoModule(type);
            

            // указываем точку входа для исполняемого файла
            asm.EntryPoint = bootstrap;
            asm.Write("./CodeGeneration.dll");
        }

        private void ImportStuffIntoModule(TypeDefinition type)
        {
            foreach (var variable in this.Variables)
            {
                type.Fields.Add(variable.Value);
            }
        }

        public void EmitDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "SimpleDeclaration":
                    this.EmitSimpleDeclaration(ivasiq);
                    break;

                default:
                    throw new Exception("sosi jopy");
            }
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
                    throw new Exception("sosi jopy");
            }
        }

        private void EmitVariableDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[1];
            String type = null;

            if (ivasiq.Type == "Type")
            {
                type = this.GetType(ivasiq);
            }

            if (type == null)
            {
                throw new Exception("sosi joopy");
            }

            var fieldDefinition = new FieldDefinition(declaration.Name, FieldAttributes.Private, this.Types[type]);
            this.Variables.Add(declaration.Name, fieldDefinition);

            ivasiq = declaration.Children[0];

            switch (ivasiq.Type)
            {
                case "InitialValue":
                    this.EmitInitialValue(ivasiq);
                    break;

                default:
                    throw new Exception("sosi jopy");
            }

            var bootstrapIP = this.bootstrap.Body.GetILProcessor();
            switch (type)
            {
                case "Integer":
//                    Console.Write("Storing whatever is on the stack into a field named: ");
//                    Console.WriteLine(declaration.Name);
                    bootstrapIP.Emit(OpCodes.Stfld, fieldDefinition);
                    break;
            }
        }

        private string GetType(JsonEntity declaration)
        {
            return declaration.Name;
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
                    throw new Exception("sosi jopy");
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
                    throw new Exception("sosi jopy");
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
                    throw new Exception("sosi jopy");
            }

            // 00))0)000)0)0))00)))0)0)0)0))0))0)))00))0))0)0))))))))))
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
                    throw new Exception("sosi jopy");
            }

            // 00))0)000)0)0))00)))0)0)0)0))0))0)))00))0))0)0))))))))))
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
                    throw new Exception("sosi jopy");
            }

            // 00))0)000)0)0))00)))0)0)0)0))0))0)))00))0))0)0))))))))))
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
                    throw new Exception("sosi jopy");
            }

            // 00))0)000)0)0))00)))0)0)0)0))0))0)))00))0))0)0))))))))))
        }

        private void EmitPrimary(JsonEntity declaration)
        {
//            Console.Write("Storing this onto the stack: ");
//            Console.WriteLine(declaration.Value);
            this.bootstrap.Body.GetILProcessor().Emit(
                OpCodes.Ldc_I4, int.Parse(declaration.Value));

            // 00))0)000)0)0))00)))0)0)0)0))0))0)))00))0))0)0))))))))))
        }
    }
}