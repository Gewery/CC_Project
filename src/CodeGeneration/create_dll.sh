dotnet restore
dotnet build
dotnet publish
cp CodeGeneration.runtimeconfig.json ./bin/Debug/netcoreapp3.0/win-x64/
dotnet run
