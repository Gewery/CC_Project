using System;
using System.IO;
using Newtonsoft.Json;
using Mono.Cecil;
using Mono.Cecil.Cil;
using CodeGeneration.AstEntities;

namespace CodeGeneration
{
    public class CodeGenerator
    {
        public static void Compile()
        {
            string treeJson = File.ReadAllText("../../tree.json");
            Console.WriteLine(treeJson);
            dynamic stuff = JsonConvert.DeserializeObject(treeJson);
            Console.WriteLine(stuff.Program);
            
            Compiler compiler = new Compiler();
            compiler.GenerateIL(stuff);
            

//            var name = new AssemblyNameDefinition("SuperGreeterBinary", new Version(1, 0, 0, 0));
//            var asm = AssemblyDefinition.CreateAssembly(name, "greeter.exe", ModuleKind.Console);
//
//            asm.MainModule.Import(typeof(String));
//            var void_import = asm.MainModule.Import(typeof(void));
//            
//            
//
//            var method = new MethodDefinition("Main", MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, void_import);
//            var ip = method.Body.GetILProcessor();
//
//            ip.Emit(OpCodes.Ldstr, "Hello, ");
//            ip.Emit(OpCodes.Ldstr, str);
//            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(String).GetMethod("Concat", new Type[] { typeof(string), typeof(string) })));
//            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(Console).GetMethod("WriteLine", new Type[] { typeof(string) })));
//            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(Console).GetMethod("ReadLine", new Type[] { })));
//            ip.Emit(OpCodes.Pop);
//            ip.Emit(OpCodes.Ret);
//
//            var type = new TypeDefinition("supergreeter", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.Import(typeof(object)));
//            asm.MainModule.Types.Add(type);
//            type.Methods.Add(method);
//
//            asm.EntryPoint = method;
//
//            asm.Write("greeter.exe");
        }

        static void Main(params string[] args)
        {
            Compile();
        }
    }
}