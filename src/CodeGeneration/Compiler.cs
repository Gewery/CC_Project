using System;
using System.IO;
using Mono.Cecil;
using Mono.Cecil.Cil;
using CodeGeneration.AstEntities;

namespace CodeGeneration.AstEntities
{
    public class Compiler
    {
        public ILProcessor ip;
        public void GenerateIL(dynamic root)
        {
            dynamic newroot = root;
            // VariableDeclaration declaration
            while ((newroot.Program) != null 
                   && (newroot.Program.Declaration) != null 
                   && (newroot.Program.Declaration.SimpleDeclaration) != null
                   && (newroot.Program.Declaration.SimpleDeclaration.VariableDeclaration) != null)
            {
                Console.WriteLine(newroot.Program.Declaration.SimpleDeclaration.VariableDeclaration);
                Console.WriteLine("#==============#");
                VariableDeclaration vd = new VariableDeclaration();
                vd.GenerateVariableDeclaration(newroot.Program.Declaration.SimpleDeclaration.VariableDeclaration, ip);
                newroot = newroot.Program;
            }
        }
    }
}