using Mono.Cecil;
using Mono.Cecil.Cil;

namespace CodeGeneration
{
    public class Compiler
    {
        static void Compile()
        {
            var name = new AssemblyNameDefinition("SuperGreeterBinary", new Version(1, 0, 0, 0));
            var asm = AssemblyDefinition.CreateAssembly(name, "greeter.exe", ModuleKind.Console);

            // импортируем в библиотеку типы string и void
            asm.MainModule.Import(typeof(String));
            var void_import = asm.MainModule.Import(typeof(void));

            // создаем метод Main, статический, приватный, возвращающий void
            var method = new MethodDefinition("Main", MethodAttributes.Static | MethodAttributes.Private | MethodAttributes.HideBySig, void_import);
            // сохраняем короткую ссылку на генератор кода
            var ip = method.Body.GetILProcessor();

            // магия ленор!
            ip.Emit(OpCodes.Ldstr, "Hello, ");
            ip.Emit(OpCodes.Ldstr, str);
            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(String).GetMethod("Concat", new Type[] { typeof(string), typeof(string) })));
            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(Console).GetMethod("WriteLine", new Type[] { typeof(string) })));
            ip.Emit(OpCodes.Call, asm.MainModule.Import(typeof(Console).GetMethod("ReadLine", new Type[] { })));
            ip.Emit(OpCodes.Pop);
            ip.Emit(OpCodes.Ret);

            // регистрируем тип, к которому будет привязан данный метод: все параметры выбраны
            // опытным путем из дизассемблированного экзешника
            var type = new TypeDefinition("supergreeter", "Program", TypeAttributes.AutoClass | TypeAttributes.Public | TypeAttributes.AnsiClass | TypeAttributes.BeforeFieldInit, asm.MainModule.Import(typeof(object)));
            // добавляем тип в сборку
            asm.MainModule.Types.Add(type);
            // привязываем метод к типу
            type.Methods.Add(method);

            // указываем точку входа для исполняемого файла
            asm.EntryPoint = method;

            // сохраняем сборку на диск
            asm.Write("greeter.exe");
        }

        static void Main(params string[] args)
        {
            Compile();
        }
    }
}