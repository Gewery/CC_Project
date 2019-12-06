.assembly rabotai{}
.assembly extern System.Console{}
.assembly extern System.Runtime{}

.class private auto ansi beforefieldinit ConsoleApp1.Program extends [System.Runtime]System.Object
{
.field public static int32 b
.field public static bool c
.field public static float64 g

.method private hidebysig static specialname rtspecialname void .cctor() cil managed
{
.maxstack 8

IL_0000: ldc.i4.5
IL_0001: ldc.i4.6
IL_0002: add
IL_0003: ldc.i4.7
IL_0004: ldc.i4.123
IL_0005: sub
IL_0006: mul
IL_0007: stsfld int32 ConsoleApp1.Program::b
IL_0008: ldc.i4.0
IL_0009: stsfld bool ConsoleApp1.Program::c
IL_0010: ldc.i4.5
IL_0011: stsfld float64 ConsoleApp1.Program::g


}
}