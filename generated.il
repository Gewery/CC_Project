.assembly rabotai{}
.assembly extern System.Console{}
.assembly extern System.Runtime{}
.assembly extern mscorlib{}

.class private auto ansi beforefieldinit ConsoleApp1.Program extends [System.Runtime]System.Object
{
.field public static int32 b
 .method private hidebysig static void
main(
string[] args
) cil managed
{
.entrypoint
.maxstack 100

.locals init(
[0] int32 a
)
IL_0000: ldc.i4.5
IL_0010: ldc.i4.6
IL_0020: ldc.i4.1
IL_0030: sub
IL_0040: add
IL_0050: stloc.0
IL_0880: call   void [System.Console]System.Console::WriteLine(int32)
IL_0900: ret
}

.method private hidebysig static specialname rtspecialname void .cctor() cil managed
{
.maxstack 100

IL_0000: ldc.i4.5
IL_0010: ldc.i4.6
IL_0020: add
IL_0030: ldc.i4.7
IL_0040: ldc.i4.2
IL_0050: sub
IL_0060: mul
IL_0070: stsfld int32 ConsoleApp1.Program::b

IL_0900: ret
}
}