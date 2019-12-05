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
            var nameDef = new AssemblyNameDefinition("CodeGeneration", new Version(1, 0, 0, 0));
            var asm = AssemblyDefinition.CreateAssembly(nameDef, "result.exe", ModuleKind.Console);

            this.Types.Add("Integer", asm.MainModule.ImportReference(typeof(Int32)));
            //            this.Types.Add("Integer", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Int32));
            //            this.Types.Add("Void", asm.MainModule.ImportReference(asm.MainModule.TypeSystem.Void));
            //this.Types.Add("Real", asm.MainModule.ImportReference(typeof(Double)));
            this.Types.Add("Boolean", asm.MainModule.ImportReference(typeof(Boolean)));
            this.Types.Add("Void", asm.MainModule.ImportReference(typeof(void)));

            bootstrap = new MethodDefinition("Main",
                MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, this.Types["Void"]);

            foreach (JsonEntity declaration in el.Children)
            {
                this.EmitDeclaration(declaration);
            }
            var ip = bootstrap.Body.GetILProcessor();
//            ip.Emit(OpCodes.Pop, a);
//            Console.WriteLine(a);
            ip.Emit(OpCodes.Pop);
            ip.Emit(OpCodes.Ret);
            
            var type = new TypeDefinition("CodeGenerationResult", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.ImportReference(typeof(object)));
            type = this.ImportStuffIntoModule(type);
            asm.MainModule.Types.Add(type);
            type.Methods.Add(bootstrap);
            Console.WriteLine("Compiler.cs: Im ok");
            asm.EntryPoint = bootstrap;
            asm.Write("./result.exe");
        }

        private TypeDefinition ImportStuffIntoModule(TypeDefinition type)
        {
            Console.WriteLine("3");
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
            if (functionName == "main")
            {
                
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
                case "TypeDeclaration":
                    this.EmitTypeDeclarationn(ivasiq);
                    break;
                default:
                    throw new Exception("Simple Declaration Error");
            }
        }

        //TODO 
        public void EmitTypeDeclarationn(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            String type = null;
            String nameOfType = declaration.Name;
            if (ivasiq.Type == "type")
            {
                type = this.GetType(ivasiq);
            }
            Console.Write("Name of type: ");
            Console.WriteLine(nameOfType);
            Console.Write("Type of type: ");
            Console.WriteLine(type);
        }

        private void EmitVariableDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            string type = null;
            
            if (ivasiq.Type == "type")
            {
                type = this.GetType(ivasiq);
            }
            if (type == null)
            {
                throw new Exception("Variable Declaration Error");
            }

            var fieldDefinition = new FieldDefinition(declaration.Name, FieldAttributes.Public, this.Types[type]);
            this.Variables.Add(declaration.Name, fieldDefinition);

            ivasiq = declaration.Children[1];

            switch (ivasiq.Type)
            {
                case "InitialValue":
                    this.EmitInitialValue(ivasiq, type);
                    break;
                case "Expression":
                    this.EmitExpression(ivasiq, type);
                    break;
                default:
                    throw new Exception("Error");
            }

            var bootstrapIP = this.bootstrap.Body.GetILProcessor();
            Console.Write("Storing whatever is on the stack into a field named: ");
            Console.WriteLine(declaration.Name);
            bootstrapIP.Emit(OpCodes.Stfld, fieldDefinition);
        }

        private string GetType(JsonEntity declaration)
        {
            return declaration.Name;
        }

        private void EmitInitialValue(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Expression":
                    this.EmitExpression(ivasiq, type);
                    break;

                default:
                    throw new Exception("Initial Value Error");
            }
        }

        //TODO Danya
        private void EmitExpression(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Relation":
                    this.EmitRelation(ivasiq, type);
                    break;
                case "MultipleRelationsInExpression":
                    this.EmitMultipleRelationsInExpression(ivasiq, type);
                    break;

                default:
                    throw new Exception("Expression Error");
            }
        }

        //TODO Danya
        private void EmitMultipleRelationsInExpression(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "LogicalOperator":
                    this.EmitLogicalOperator(ivasiq, type);
                    break;
                case "Relation":
                    this.EmitRelation(ivasiq, type);
                    break;
                case "MultipleRelationsInExpression":
                    this.EmitMultipleRelationsInExpression(ivasiq, type);
                    break;

                default:
                    throw new Exception("Expression Error");
            }
        }

        //TODO Danya
        private void EmitLogicalOperator(JsonEntity ivasiq, string type)
        {
            throw new NotImplementedException();
        }

        //TODO Danya
        private void EmitRelation(JsonEntity declaration, String type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Simple":
                    this.EmitSimple(ivasiq, type);
                    break;

                default:
                    throw new Exception("Relation Error");
            }
        }

        //TODO Danya
        private void EmitSimple(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Factor":
                    this.EmitFactor(ivasiq, type);
                    break;
                case "Factors":
                    this.EmitFactors(ivasiq, type);
                    break;

                default:
                    throw new Exception("Simple error");
            }
        }

        //TODO Danya
        private void EmitFactors(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "SimpleOperator":
                    this.EmitSimpleOperator(ivasiq, type);
                    break;
                case "Factor":
                    this.EmitFactor(ivasiq, type);
                    break;
                case "Factors":
                    this.EmitFactors(ivasiq, type);
                    break;

                default:
                    throw new Exception("Simple error");
            }
        }

        //TODO DANYA
        private void EmitSimpleOperator(JsonEntity ivasiq, string type)
        {
            throw new NotImplementedException();
        }

        //TODO DANYA
        private void EmitFactor(JsonEntity declaration,  string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Summand":
                    this.EmitSummand(ivasiq, type);
                    break;
                case "Summands":
                    this.EmitSummands(ivasiq, type);
                    break;

                default:
                    throw new Exception("Factor error");
            }
        }
        //TODO DANYA
        private void EmitSummands(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Sign":
                    this.EmitSign(ivasiq, type);
                    break;
                case "Summand":
                    this.EmitSummand(ivasiq, type);
                    break;
                case "Summands":
                    this.EmitSummands(ivasiq, type);
                    break;

                default:
                    throw new Exception("Factor error");
            }
        }

        //TODO DANYA
        private void EmitSign(JsonEntity ivasiq, string type)
        {
            throw new NotImplementedException();
        }

        // TODO Danya
        private void EmitSummand(JsonEntity declaration, string type)
        {
            var ivasiq = declaration.Children[0];
            switch (ivasiq.Type)
            {
                case "Primary":
                    this.EmitPrimary(ivasiq, type);
                    break;
                case "Expression":
                    this.EmitExpression(ivasiq, type);
                    break;
                default:
                    throw new Exception("Summand Error");
            }
        }

        // TODO Danya
        private void EmitPrimary(JsonEntity declaration, string type)
        {
            Console.Write("Storing this onto the stack: ");
            Console.WriteLine(declaration.Value);
            switch (type)
            {
                case "Integer":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_I4, (int)Math.Floor(float.Parse(declaration.Value))); //Store value of type int32 into memory at address
                    break;
                case "Real":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_R4, float.Parse(declaration.Value)); //Store value of type float32 into memory at address
                    break;
                case "Boolean":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_I4, (int)Math.Floor(float.Parse(declaration.Value))); //Store value of type int32 into memory at address
                    break;
            }
        }
    }
}