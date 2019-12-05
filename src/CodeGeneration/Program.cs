using System;
using System.IO;
using CodeGeneration.Parser;
using Mono.Cecil;
using Mono.Cecil.Cil;
using Newtonsoft.Json;

namespace CodeGeneration
{
    public class CodeGenerator
    {
        static void Main(params string[] args)
        {
            TextReader inputStream = new StreamReader(
                    new BufferedStream(
                        new FileStream("../../tree.json", FileMode.Open, FileAccess.Read, FileShare.Read)));

            JsonEntity ir;
            Console.WriteLine("Im ok");

            using (inputStream)
            {
                using (var reader = new JsonTextReader(inputStream))
                {
                    JsonSerializer serializer = new JsonSerializer();
                    ir = (JsonEntity) serializer.Deserialize(reader, typeof(JsonEntity));
                }
            }
            
            var compiler = new Compiler();
            compiler.EmitRoot(ir);
            Console.ReadLine();
        }
    }
}