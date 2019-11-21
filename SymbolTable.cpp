#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;


struct Variable {
    string type;
    bool scope; // 0 - global, 1 - local
    float value;
    Variable(string type_of_var, bool scope_of_var, float value_of_var) {
        type = type_of_var;
        scope = scope_of_var;
        value = value_of_var;
    }
};

struct Function {
    string return_type;
    float return_value;
    int number_of_args;
    vector<Variable*> arguments;
    Function(string return_type_of_func, float return_value_of_func, int number_of_args_of_func, vector<Variable*> &arguments_of_func) {
        return_type = return_type_of_func;
        return_value = return_value_of_func;
        number_of_args = number_of_args_of_func;
        arguments = arguments_of_func;
    }
};

int main() {
    unordered_map<string, Variable* > variables;
    unordered_map<string, Function* > functions;
    Variable *v1 = new Variable("int", 0, 5);
    vector<Variable*> vect;
    vect.push_back(v1);
    Function *f1 = new Function("bool", 1, 1, vect);

    variables["a"] = v1;
    functions["main"] = f1;
    for (auto x : variables)
        cout << x.first << " " << x.second->type << endl;

    for (auto x : functions)
        cout << x.first << " " << x.second->arguments[0]->scope << endl;
}



