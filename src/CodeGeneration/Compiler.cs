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
        public Dictionary<string, Type> Variables = new Dictionary<string, Type>();
        public Dictionary<string, Type> Types = new Dictionary<string, Type>();

        private MethodDefinition bootstrap = null;
        private MethodDefinition CurrentMethod = null;

        public void EmitRoot(JsonEntity el)
        {
            // create library and specify its name, version and type - console app
            var nameDef = new AssemblyNameDefinition("CodeGeneration", new Version(1, 0, 0, 0));
            var asm = AssemblyDefinition.CreateAssembly(nameDef, "result.exe", ModuleKind.Console);

            // import necessary types to the library
            this.Types.Add("integer", typeof(Int32));
            this.Types.Add("real", typeof(Double));
            this.Types.Add("boolean", typeof(Boolean));
            this.Types.Add("void", typeof(void));

            // void create static private void Main method
            bootstrap = new MethodDefinition("Main",
                MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, asm.MainModule.ImportReference(typeof(void)));
            
            // go through the JSON calculating values and putting the results onto the stack
            foreach (JsonEntity declaration in el.Children)
            {
                this.EmitDeclaration(declaration);
            }

            // create variable for the code generator
            var ip = this.bootstrap.Body.GetILProcessor();

            // write final result to program output, wait until any input is received
            ip.Emit(OpCodes.Call, asm.MainModule.ImportReference(typeof(Console).GetMethod("WriteLine", new Type[] { this.Variables["b"] })));
            ip.Emit(OpCodes.Call, asm.MainModule.ImportReference(typeof(Console).GetMethod("ReadLine", new Type[] { })));
            ip.Emit(OpCodes.Pop);
            ip.Emit(OpCodes.Ret);

            // register a type which method will be connected to
            var type = new TypeDefinition("CodeGenerationResult", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.ImportReference(typeof(object)));

            // add type to build
            asm.MainModule.Types.Add(type);

            // connect method with type
            type.Methods.Add(bootstrap);

            // specify entry point for a program
            asm.EntryPoint = bootstrap;

            // save the build 
            asm.Write("./result.exe");
        }

        public void EmitDeclaration(JsonEntity declaration)
        {
            var subDeclaration = declaration.Children[0];
            var scope = false; //0 - global, 1 - local
            switch (subDeclaration.Type)
            {
                case "SimpleDeclaration":
                    this.EmitSimpleDeclaration(subDeclaration, scope);
                    break;
                
                case "RoutineDeclaration":
                    // this.EmitRoutineDeclaration(ivasiq);
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
                //CurrentMethod = new MethodDefinition(routineDeclaration.Name,
                //  MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, this.Types["Void"]);
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
            var subSimpleDeclaration = simpleDeclaration.Children[0];
            switch (subSimpleDeclaration.Type)
            {
                case "VariableDeclaration":
                    this.EmitVariableDeclaration(subSimpleDeclaration, scope);
                    break;
                case "TypeDeclaration":
                    // this.EmitTypeDeclaration(ivasiq);
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
            //var ivasiq = variableDeclaration.Children[0];
            //string type = null;

            //if (ivasiq.Type == "type")
            //{
            //    type = this.GetType(ivasiq);
            //}
            //if (type == null)
            //{
            //    throw new Exception("Variable Declaration Error");
            //}

            //var cil = this.bootstrap.Body.GetILProcessor();
            //if (scope)
            //{
            //    var local = new VariableDefinition(this.Types[type]);
            //    this.bootstrap.Body.Variables.Add(local);

            //    cil.Emit(OpCodes.Ldloca, local);
            //    cil.Emit(OpCodes.Initobj, this.Types[type]);
            //    cil.Emit(OpCodes.Ldloc, local);
            //    cil.Emit(OpCodes.Ret);
            //}
            //else
            //{
            //    var fieldDefinition = new FieldDefinition(variableDeclaration.Name, FieldAttributes.Static | FieldAttributes.Public, this.Types[type]);
            //    this.Variables.Add(variableDeclaration.Name, fieldDefinition);
            //    Console.Write("Storing whatever is on the stack into a field named: ");
            //    Console.WriteLine(variableDeclaration.Name);
            //    cil.Emit(OpCodes.Stfld, fieldDefinition);
            //}

            //if (variableDeclaration.Children.Count > 1)
            //{
            var subVariableDeclaration = variableDeclaration.Children[1];
            switch (subVariableDeclaration.Type)
            {
                case "InitialValue":
                    this.EmitInitialValue(subVariableDeclaration);
                    this.Variables.Add(variableDeclaration.Name, this.Types[variableDeclaration.Children[0].Children[0].Name]);
                    break;
                case "Expression":      // if type is not specified
                    // this.EmitExpression(ivasiq);
                    break;
                default:
                    throw new Exception("Error");
            }
            //}
        }

        private string GetType(JsonEntity type)
        {
            return type.Name;
        }

        private void EmitInitialValue(JsonEntity initialValue)
        {
            var expression = initialValue.Children[0];
            if (expression.Type == "Expression")
            {
                this.EmitExpression(expression);
            }
            else
            {
                throw new Exception("Initial Value Error");
            }
        }

        private void EmitExpression(JsonEntity expression)
        {
            this.EmitRelation(expression.Children[0]);
            //if (expression.Children.Count > 1) {
            //    this.EmitMultipleRelationsInExpression(expression.Children[1]);
            //    var operation = expression.Children[1].Children[0].Value; // operation = expression->multiplerelationsinexpression->logicaloperator

            //    var ip = this.bootstrap.Body.GetILProcessor();
            //    // lhs.value
            //    ip.Emit(OpCodes.Ldarg_0);
            //    ip.Emit(OpCodes.Ldfld, expression.Value);
            //    // rhs.value
            //    ip.Emit(OpCodes.Ldarg_1);
            //    ip.Emit(OpCodes.Ldfld, expression.Value);

            //    if (operation == "and") {
            //        ip.Emit(OpCodes.And);
            //    }
            //    else if (operation == "or") {
            //        ip.Emit(OpCodes.Or);
            //    }
            //    else if (operation == "xor") {
            //        ip.Emit(OpCodes.Xor);
            //    }
            //}
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
        private void EmitRelation(JsonEntity relation)
        {
            this.EmitSimple(relation.Children[0]);
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
        private void EmitSimple(JsonEntity simple)
        {
            this.EmitFactor(simple.Children[0]);
            if (simple.Children.Count > 1) {
                this.EmitFactors(simple.Children[1]);
            }
        }

        // DanyaDone
        private void EmitFactors(JsonEntity factors)
        {
            this.EmitFactor(factors.Children[1]);
            if (factors.Children.Count > 2)
            {
                this.EmitFactors(factors.Children[2]);
            }

            var ip = this.bootstrap.Body.GetILProcessor();
            var op = factors.Children[0].Value;

            if (op == "*")
            {
                ip.Emit(OpCodes.Mul);
            }
            else if (op == "/")
            {
                ip.Emit(OpCodes.Div);
            }
            else if (op == "%")
            {
                ip.Emit(OpCodes.Rem);
            }
        }

        // DanyaDone
        private void EmitFactor(JsonEntity factor)
        {
            this.EmitSummand(factor.Children[0]);
            if (factor.Children.Count > 1) {
                this.EmitSummands(factor.Children[1]);
            }
        }
        
        // DanyaDone
        private void EmitSummands(JsonEntity summands)
        {
            this.EmitSummand(summands.Children[1]);
            if (summands.Children.Count > 2) {
                this.EmitSummands(summands.Children[2]);
            }

            var ip = this.bootstrap.Body.GetILProcessor();
            var sign = summands.Children[0].Value;

            if (sign == "+")
            {
                ip.Emit(OpCodes.Add);
            }
            else if (sign == "-")
            {
                ip.Emit(OpCodes.Sub);
            }
        }

        // DanyaDone
        private void EmitSummand(JsonEntity summand)
        {
            var subSummand = summand.Children[0];
            switch (subSummand.Type)
            {
                case "Primary":
                    this.EmitPrimary(subSummand);
                    break;
                case "Expression":
                    this.EmitExpression(subSummand);
                    break;
                default:
                    throw new Exception("Summand error");
            }
        }

        private void EmitPrimary(JsonEntity primary)
        {
            var ip = this.bootstrap.Body.GetILProcessor();
            switch(primary.Name)
            {
                case "integer":
                    ip.Emit(OpCodes.Ldc_I4, (Int32)Convert.ToDouble(primary.Value));
                    break;
                case "real":
                    ip.Emit(OpCodes.Ldc_R4, (float)Convert.ToDouble(primary.Value));
                    break;
                case "boolean":
                    ip.Emit(OpCodes.Ldc_I4, (Int32)Convert.ToDouble(primary.Value));
                    break;
                default:
                    throw new Exception("Primary error");
            }
        }
    }
}