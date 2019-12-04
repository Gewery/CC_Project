dotnet restore
dotnet build
dotnet publish -r win-x64
.\bin\Debug\netcoreapp3.0\win-x64\publish\CodeGeneration.exe