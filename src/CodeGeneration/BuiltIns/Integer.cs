using System;
using Mono.Cecil;
using Mono.Cecil.Cil;

namespace CodeGeneration.BuiltIns
{
    public class IntegerBuiltInUnitDefinition : BuiltInUnitDefinition
    {
        
        public IntegerBuiltInUnitDefinition(MethodDefinition ctx)
            : base(ctx, UnitName, ctx.Module.TypeSystem.Int32)
        {
        }
        
        public static readonly string UnitName = "Integer";
        
        public bool CanLoadFromLiteral => true;

        public void LoadFromLiteral(string literal, ILProcessor ip)
        {
            if (!int.TryParse(literal, out var result))
                throw new Exception(literal);

            ip.Emit(OpCodes.Ldc_I4, result);
        }
    }
}