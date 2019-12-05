using System;
using System.Collections.Generic;
using System.Threading;
using CodeGeneration.Parser;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Mono.Cecil.Rocks;

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
            this.Types.Add("Real", asm.MainModule.ImportReference(typeof(Double)));
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
            var scope = false; //0 - global, 1 - local
            switch (ivasiq.Type)
            {
                case "SimpleDeclaration":
                    this.EmitSimpleDeclaration(ivasiq, scope);
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
                    this.EmitSimpleDeclaration(ivasiq, true);
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
                    this.EmitExpression(ivasiq);
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

        public void EmitSimpleDeclaration(JsonEntity simpleDeclaration, bool scope)
        {
            var ivasiq = simpleDeclaration.Children[0];
            switch (ivasiq.Type)
            {
                case "VariableDeclaration":
                    this.EmitVariableDeclaration(ivasiq, scope);
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

        private void EmitVariableDeclaration(JsonEntity variableDeclaration, bool scope)
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
            
            var cil = this.bootstrap.Body.GetILProcessor();
            if (scope)
            {
                var local = new VariableDefinition(this.Types[type]);
                this.bootstrap.Body.Variables.Add(local);

                cil.Emit(OpCodes.Ldloca, local);
                cil.Emit(OpCodes.Initobj, this.Types[type]);
                cil.Emit(OpCodes.Ldloc, local);
                cil.Emit(OpCodes.Ret);
            }
            else
            {
                var fieldDefinition = new FieldDefinition(variableDeclaration.Name, FieldAttributes.Static | FieldAttributes.Public, this.Types[type]);
                this.Variables.Add(variableDeclaration.Name, fieldDefinition);
                Console.Write("Storing whatever is on the stack into a field named: ");
                Console.WriteLine(variableDeclaration.Name);
                cil.Emit(OpCodes.Stfld, fieldDefinition);
            }

            if (variableDeclaration.Children.Count > 1)
            {
                ivasiq = variableDeclaration.Children[1];

                switch (ivasiq.Type)
                {
                    case "InitialValue":
                        this.EmitInitialValue(ivasiq);
                        break;
                    case "Expression":
                        this.EmitExpression(ivasiq);
                        break;
                    default:
                        throw new Exception("Error");
                }
            }
        }

        private string GetType(JsonEntity type)
        {
            return type.Name;
        }

        private void EmitInitialValue(JsonEntity initialValue)
        {
            var ivasiq = initialValue.Children[0];
            switch (ivasiq.Type)
            {
                case "Expression":
                    this.EmitExpression(ivasiq);
                    break;

                default:
                    throw new Exception("Initial Value Error");
            }
        }

        // DanyaDone
        private void EmitExpression(JsonEntity declaration)
        {
            this.EmitRelation(declaration.Children[0]);
            if (declaration.Children.Count > 1) {
                this.EmitMultipleRelationsInExpression(declaration.Children[1]);
                var operation = declaration.Children[1].Children[0].Value; // operation = expression->multiplerelationsinexpression->logicaloperator

                var ip = this.bootstrap.Body.GetILProcessor();
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (operation == "and") {
                    ip.Emit(OpCodes.And);
                }
                else if (operation == "or") {
                    ip.Emit(OpCodes.Or);
                }
                else if (operation == "xor") {
                    ip.Emit(OpCodes.Xor);
                }
            }
        }

        // DanyaDone
        private void EmitMultipleRelationsInExpression(JsonEntity declaration)
        {
            this.EmitRelation(declaration.Children[1]);

            if (declaration.Children.Count > 1) {
                this.EmitMultipleRelationsInExpression(declaration.Children[2]);
                var operation = declaration.Children[2].Children[0].Value; // operation = multiplerelationsinexpression->multiplerelationsinexpression->logicaloperator
                var ip = this.bootstrap.Body.GetILProcessor();
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (operation == "and") {
                    ip.Emit(OpCodes.And);
                }
                else if (operation == "or") {
                    ip.Emit(OpCodes.Or);
                }
                else if (operation == "xor") {
                    ip.Emit(OpCodes.Xor);
                }
            }
        }

        // Danya[Kind A]Done
        private void EmitRelation(JsonEntity declaration)
        {
            this.EmitSimple(declaration.Children[0]);
//            if (declaration.Children.Count > 1) {
//                this.EmitComparisonInRelation(declaration.Children[1]);
//                var operation = declaration.Children[1].Children[0].Value; // opearation = relation->comparisoninrelation->comparisonoperator
//
//                var lhs = new ArgumentVariable(integer, 0);
//                var rhs = new ArgumentVariable(integer, 1);
//
//                var ip = this.bootstrap.Body.GetILProcessor();
//                // prepare lhs
//                lhs.Load(ip);
//                integer.Unboxed(ip);
//                // prepare rhs
//                rhs.Load(ip);
//                integer.Unboxed(ip);
//
//                if (operation == "<") {
//                    ip.Emit(OpCodes.Clt);
//                }
//                else if (operation == "<=") {
//                    //TODO
//                }
//                else if (operation == ">") {
//                    ip.Emit(OpCodes.Cgt);
//                }
//                else if (operation == ">=") {
//                    //TODO
//                }
//                else if (operation == "=") {
//                    ip.Emit(OpCodes.Ceq);
//                }
//                else if (operation == "/=") {
//                    //TODO
//                }
//
//                var result = integer.Boxed(ip);
//                // return result;
//                result.Load(ip);
//            }
        }

        // DanyaDone
        private void EmitComparisonInRelation(JsonEntity declaration) {
            this.EmitSimple(declaration.Children[1]);
        }

        // DanyaDone
        private void EmitSimple(JsonEntity declaration)
        {
            this.EmitFactor(declaration.Children[0]);
            if (declaration.Children.Count > 1) {
                this.EmitFactors(declaration.Children[1]);
                var operation = declaration.Children[1].Children[0].Value; // operation = declaration->factors->sign

                var ip = this.bootstrap.Body.GetILProcessor();
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (operation == "*") {
                    ip.Emit(OpCodes.Mul);
                }
                else if (operation == "/") {
                    ip.Emit(OpCodes.Div);
                }
                else if (operation == "%") {
                    ip.Emit(OpCodes.Rem);
                }  
            }
        }

        // DanyaDone
        private void EmitFactors(JsonEntity declaration)
        {
            this.EmitFactor(declaration.Children[0]);
            if (declaration.Children.Count > 1) {
                this.EmitFactors(declaration.Children[1]);
                var operation = declaration.Children[1].Children[0].Value; // operation = declaration->factors->sign

                var ip = this.bootstrap.Body.GetILProcessor();
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (operation == "*") {                        
                    ip.Emit(OpCodes.Mul);
                }
                else if (operation == "/") {
                    ip.Emit(OpCodes.Div);
                }
                else if (operation == "%") {
                    ip.Emit(OpCodes.Rem);
                }   
            }
        }

        // DanyaDone
        private void EmitFactor(JsonEntity declaration)
        {
            this.EmitSummand(declaration.Children[0]);
            if (declaration.Children.Count > 1) {
                var sign = declaration.Children[1].Children[0].Value; // sign = summands->summands->sign
                this.EmitSummands(declaration.Children[1]);
                var ip = this.bootstrap.Body.GetILProcessor();
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (sign == "+") {
                    ip.Emit(OpCodes.Add);
                }
                else if (sign == "-") {
                    ip.Emit(OpCodes.Sub);
                }
            }
        }
        
        // DanyaDone
        private void EmitSummands(JsonEntity declaration)
        {
            this.EmitSummand(declaration.Children[1]);
            if (declaration.Children.Count > 2) {
                var sign = declaration.Children[2].Children[0].Value; // sign = summands->summands->sign
                this.EmitSummands(declaration.Children[2]);
                var ip = this.bootstrap.Body.GetILProcessor();
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, declaration.Value);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, declaration.Value);

                if (sign == "+") {
                    ip.Emit(OpCodes.Add);
                }
                else if (sign == "-") {
                    ip.Emit(OpCodes.Sub);
                }
            }
        }

        // DanyaDone
        private void EmitSummand(JsonEntity declaration)
        {
            if (declaration.Children[0].Type == "Primary")
                this.EmitPrimary(declaration.Children[0]);
            else if (declaration.Children[0].Type == "Expression")
                this.EmitExpression(declaration.Children[1]);
        }

        // DanyaDone
        private void EmitPrimary(JsonEntity declaration)
        {
            Console.Write("Storing this onto the stack: ");
            Console.WriteLine(declaration.Value);
            this.bootstrap.Body.GetILProcessor().Emit(
                        OpCodes.Ldc_R4, float.Parse(declaration.Value)); //Store value of type float32 into memory at address
        }
    }
}