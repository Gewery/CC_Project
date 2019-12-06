.assembly rabotai{}
.assembly extern System.Console{}
.assembly extern System.Runtime{}
.assembly extern mscorlib{}

.class private auto ansi beforefieldinit ConsoleApp1.Program extends [System.Runtime]System.Object
{
.field public static int32 b

.method private hidebysig static specialname rtspecialname void .cctor() cil managed
{
.maxstack 100

IL_0000: ldc.i4.7
IL_0010: ldc.i4.2
IL_0020: ldc.i4.5
IL_0030: ldc.i4.9
IL_0040: add
IL_0050: add
IL_0060: add
IL_0070: stsfld int32 ConsoleApp1.Program::b

IL_0900: ret
}
}