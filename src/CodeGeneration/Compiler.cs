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

        private TypeDefinition ImportStuffIntoModule(TypeDefinition typeDefinition)
        {
            foreach (var variable in this.Variables)
            {
                typeDefinition.Fields.Add(variable.Value);
            }

            return typeDefinition;
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

        public void EmitRoutineDeclaration(JsonEntity routineDeclaration)
        {
            var ivasiq = routineDeclaration.Children[0];
            String functionName = routineDeclaration.Name;
            Console.WriteLine(functionName);
            if (functionName == "main")
            {
                if (ivasiq.Type == "BodyInRoutineDeclaration")
                {
                    this.EmitBodyInRoutineDeclaration(ivasiq);
                }
            }
        }

        private void EmitBodyInRoutineDeclaration(JsonEntity bobyInRoutineDeclararion)
        {
            var ivasiq = bobyInRoutineDeclararion.Children[0];
            switch (ivasiq.Type)
            {
                case "Body":
                    this.EmitBody(ivasiq);
                    break;
                case "ReturnInRoutine":
                    this.EmitReturnInRoutine(ivasiq);
                    break;
                default:
                    throw new Exception("Body in Routine Declaration Error");
            }
        }

        private void EmitReturnInRoutine(JsonEntity returnInRoutine)
        {
            var ivasiq = returnInRoutine.Children[0];
            //TODO 
        }

        private void EmitBody(JsonEntity body)
        {
            foreach (var ivasiq in body.Children)
            {
                if (ivasiq.Type == "SimpleDeclaration")
                {
                    this.EmitSimpleDeclaration(ivasiq);
                }

                if (ivasiq.Type == "Statement")
                {
                    this.EmitStatement(ivasiq);
                }

                if (ivasiq.Type == "Body")
                {
                    this.EmitBody(ivasiq);
                }
            }
        }

        private void EmitStatement(JsonEntity statement)
        {
            var ivasiq = statement.Children[0];
            switch (ivasiq.Type)
            {
                case "Assignment":
                    this.EmitAssignment(ivasiq);
                    break;
                case "RoutineCall":
                    this.EmitRoutineCall(ivasiq);
                    break;
                case "WhileLoop":
                    this.EmitWhileLoop(ivasiq);
                    break;
                case "ForLoop":
                    this.EmitForLoop(ivasiq);
                    break;
                case "IfStatement":
                    this.EmitIfStatement(ivasiq);
                    break;
                default:
                    throw new Exception("Statement Error");
            }
        }

        private void EmitIfStatement(JsonEntity ifStatement)
        {
            foreach (var ivasiq in ifStatement.Children)
            {
                if (ivasiq.Type == "Expression")
                {
                    this.EmitExpression(ivasiq, "");
                }

                if (ivasiq.Type == "Body")
                {
                    this.EmitBody(ivasiq);
                }

                if (ivasiq.Type == "ElseInIfStatement")
                {
                    this.EmitElseInIfStatement(ivasiq);
                }
            }
        }

        private void EmitElseInIfStatement(JsonEntity elseInIfStatement)
        {
            var ivasiq = elseInIfStatement.Children[0];
            switch (ivasiq.Type)
            {
                case "Body":
                    this.EmitBody(ivasiq);
                    break;
                default:
                    throw new Exception("Body Error");
            }
        }

        private void EmitForLoop(JsonEntity forLoop)
        {
            foreach (var ivasiq in forLoop.Children)
            {
                if (ivasiq.Type == "Reverse")
                {
//                    this.EmitReverse(ivasiq, "");
                }

                if (ivasiq.Type == "Range")
                {
//                    this.EmitRange(ivasiq);
                }

                if (ivasiq.Type == "Body")
                {
                    this.EmitBody(ivasiq);
                }
            }
            
        }

        private void EmitWhileLoop(JsonEntity whileLoop)
        {
            throw new NotImplementedException();
        }

        private void EmitRoutineCall(JsonEntity routineCall)
        {
            throw new NotImplementedException();
        }

        private void EmitAssignment(JsonEntity assigment)
        {
            throw new NotImplementedException();
        }

        public void EmitSimpleDeclaration(JsonEntity simpleDeclaration)
        {
            var ivasiq = simpleDeclaration.Children[0];
            switch (ivasiq.Type)
            {
                case "VariableDeclaration":
                    this.EmitVariableDeclaration(ivasiq);
                    break;
                case "TypeDeclaration":
                    this.EmitTypeDeclaration(ivasiq);
                    break;
                default:
                    throw new Exception("Simple Declaration Error");
            }
        }

        //TODO 
        public void EmitTypeDeclaration(JsonEntity typeDeclaration)
        {
            var ivasiq = typeDeclaration.Children[0];
            String type = null;
            String nameOfType = typeDeclaration.Name;
            if (ivasiq.Type == "type")
            {
                type = this.GetType(ivasiq);
            }
            Console.Write("Name of type: ");
            Console.WriteLine(nameOfType);
            Console.Write("Type of type: ");
            Console.WriteLine(type);
        }

        private void EmitVariableDeclaration(JsonEntity variableDeclaration)
        {
            var ivasiq = variableDeclaration.Children[0];
            string type = null;
            
            if (ivasiq.Type == "type")
            {
                type = this.GetType(ivasiq);
            }
            if (type == null)
            {
                throw new Exception("Variable Declaration Error");
            }

            var fieldDefinition = new FieldDefinition(variableDeclaration.Name, FieldAttributes.Static | FieldAttributes.Public, this.Types[type]);
            this.Variables.Add(variableDeclaration.Name, fieldDefinition);

            if (variableDeclaration.Children.Count > 1)
            {
                ivasiq = variableDeclaration.Children[1];

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
            }
            var bootstrapIP = this.bootstrap.Body.GetILProcessor();
            Console.Write("Storing whatever is on the stack into a field named: ");
            Console.WriteLine(variableDeclaration.Name);
            bootstrapIP.Emit(OpCodes.Stfld, fieldDefinition);
        }

        private string GetType(JsonEntity type)
        {
            return type.Name;
        }

        private void EmitInitialValue(JsonEntity initialValue, string type)
        {
            var ivasiq = initialValue.Children[0];
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
        private void EmitExpression(JsonEntity expression, string type)
        {
            var ivasiq = expression.Children[0];
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
        private void EmitMultipleRelationsInExpression(JsonEntity multipleRelationsInExpression, string type)
        {
            var ivasiq = multipleRelationsInExpression.Children[0];
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
        private void EmitLogicalOperator(JsonEntity logicalOperatoEntity, string type)
        {
            throw new NotImplementedException();
        }

        //TODO Danya
        private void EmitRelation(JsonEntity relation, string type)
        {
            var ivasiq = relation.Children[0];
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
        private void EmitSimple(JsonEntity simple, string type)
        {
            var ivasiq = simple.Children[0];
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
        private void EmitFactors(JsonEntity factors, string type)
        {
            var ivasiq = factors.Children[0];
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
        private void EmitSimpleOperator(JsonEntity simpleOperator, string type)
        {
            throw new NotImplementedException();
        }

        //TODO DANYA
        private void EmitFactor(JsonEntity factor,  string type)
        {
            var ivasiq = factor.Children[0];
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
        private void EmitSummands(JsonEntity summands, string type)
        {
            var ivasiq = summands.Children[0];
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
        private void EmitSign(JsonEntity sign, string type)
        {
            throw new NotImplementedException();
        }

        // TODO Danya
        private void EmitSummand(JsonEntity summand, string type)
        {
            var ivasiq = summand.Children[0];
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
        private void EmitPrimary(JsonEntity primary, string type)
        {
            Console.Write("Storing this onto the stack: ");
            Console.WriteLine(primary.Value);
            switch (type)
            {
                case "Integer":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_I4, (int)Math.Floor(float.Parse(primary.Value))); //Store value of type int32 into memory at address
                    break;
                case "Real":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_R4, float.Parse(primary.Value)); //Store value of type float32 into memory at address
                    break;
                case "Boolean":
                    this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_I4, (int)Math.Floor(float.Parse(primary.Value))); //Store value of type int32 into memory at address
                    break;
            }
        }
    }
}