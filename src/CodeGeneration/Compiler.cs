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
            String functionName = declaration.Name;
            Console.WriteLine(functionName);
            //if function is main - execute it
            if (functionName == "main")
            {
                if (ivasiq.Type == "BodyInRoutineDeclaration")
                {
                    this.EmitBodyInRoutineDeclaration(ivasiq);
                }
            }
        }

        private void EmitBodyInRoutineDeclaration(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
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

        private void EmitReturnInRoutine(JsonEntity declaration)
        {
            var ivasiq = declaration.Children[0];
            //TODO 
        }

        private void EmitBody(JsonEntity declaration)
        {
            Console.WriteLine("Ya zashel suda");

            foreach (var ivasiq in declaration.Children)
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

        private void EmitStatement(JsonEntity ivasiq)
        {
            throw new NotImplementedException();
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

            if (declaration.Children.Count > 1)
            {
                ivasiq = declaration.Children[1];

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
            var bootstrapIP = this.bootstrap.Body.GetILProcessor();
            Console.Write("Storing whatever is on the stack into a field named: ");
            Console.WriteLine(declaration.Name);
            bootstrapIP.Emit(OpCodes.Stfld, fieldDefinition);
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

                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

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
            this.EmitRelation(declaration.Children[1])

            if (declaration.Children.Count > 1) {
                this.EmitMultipleRelationsInExpression(declaration.Children[2]);
                var operation = declaration.Children[2].Children[0].Value; // operation = multiplerelationsinexpression->multiplerelationsinexpression->logicaloperator

                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

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
            if (declaration.Children.Count > 1) {
                this.EmitComparisonInRelation(declaration.Children[1]);
                var operation = declaration.Children[1].Children[0].Value; // opearation = relation->comparisoninrelation->comparisonoperator

                var lhs = new ArgumentVariable(integer, 0);
                var rhs = new ArgumentVariable(integer, 1);

                // prepare lhs
                lhs.Load(ip);
                integer.Unboxed(ip);
                // prepare rhs
                rhs.Load(ip);
                integer.Unboxed(ip);

                if (operation == "<") {
                    ip.Emit(OpCodes.Clt);
                }
                else if (operation == "<=") {
                    //TODO
                }
                else if (opearation == ">") {
                    ip.Emit(OpCodes.Cgt);
                }
                else if (opearation == ">=") {
                    //TODO
                }
                else if (opearation == "=") {
                    ip.Emit(OpCodes.Ceq);
                }
                else if (opearation == "/=") {
                    //TODO
                }

                var result = integer.Boxed(ip);
                // return result;
                result.Load(ip);
            }
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
                
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

                if (operation == "*") {
                    ip.Emit(OpCodes.mul);
                }
                else if (operation == "/") {
                    ip.Emit(OpCodes.div);
                }
                else if (operation == "%") {
                    ip.Emit(OpCodes.rem);
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
                
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

                if (operation == "*") {                        
                    ip.Emit(OpCodes.mul);
                }
                else if (operation == "/") {
                    ip.Emit(OpCodes.div);
                }
                else if (operation == "%") {
                    ip.Emit(OpCodes.rem);
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
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

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
                // lhs.value
                ip.Emit(OpCodes.Ldarg_0);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);
                // rhs.value
                ip.Emit(OpCodes.Ldarg_1);
                ip.Emit(OpCodes.Ldfld, integer.ValueField);

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