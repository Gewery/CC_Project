using System.Collections.Generic;

namespace CodeGeneration.Parser
{
    public class JsonEntity
    {
        public string Type { get; set; }
        public List<JsonEntity> Children { get; set; }
        public string Value { get; set; }
        public bool IsNot { get; set; }
        public string ValueType { get; set; }
        public string Name { get; set; }
        
    }
}