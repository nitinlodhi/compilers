#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>

using std::vector;
using std::string;
using std::stringstream;


string trim(const std::string &str) {
    string res = "";
    int start = 0;
    int end = str.size() - 1;
    for (int i = 0; i < end; i++) {
        if (isspace(str[i]))
            start++;
        else
            break;
    }
    int j = end;
    for (; j > start; j--) {
        if (!isspace(str[j]))
            break;
    }

    return str.substr(start, j - start + 1);
}


vector<string> split(const string &str, char delim = ' ') {
    string current = "";
    vector<string> result;

    for (auto ch: str) {
        if (ch == delim) {
            result.push_back(trim(current));
            current = "";
        } else {
            current += ch;
        }
    }
    result.push_back(trim(current));
    return result;
}

string toLowerCase(const string &src) {
    string temp = src;
    std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
    return temp;
}

void defineVisitor(std::stringstream &ss,
                   const std::string &baseName,
                   const vector<string> &types)
{
    ss << "\tclass Visitor {\n";
    ss << "\tpublic:\n";
    for (auto &type: types) {
        vector<string> name = split(type, ':');
        ss << "\t\tvirtual any visit" + name[0] + baseName
                + "(" + name[0] + baseName + "* " + toLowerCase(name[0]) + baseName + ") = 0;\n";
    }
    ss << "\t};\n\n";
}

void defineType(std::stringstream &ss,
                const std::string &baseName,
                const std::string &className,
                std::string &fields)
{
    vector<string> members = split(fields, ',');

    ss << "\n";
    ss << "class " + className + baseName + " : public " + baseName + " {\n";
    ss << "public:\n";
    ss << "\t" + className + baseName + "(";
    for (int i = 0; i < members.size(); i++) {
        vector<string> member = split(members[i], ' ');
        if (i == members.size() - 1)
            ss << members[i] + ") {\n";
        else
            ss << members[i] << ", ";
    }
    for (auto &field: members) {
        vector<string> info = split(field, ' ');
        ss << "\t\tthis->" << info[1] + " = " + info[1] + ";\n";
    }
    ss << "\t}\n\n";

    // Destructor
    ss << "\t~" + className + baseName + "() {\n";
    for (auto &field: members) {
        vector<string> info = split(field, ' ');
        if (info[0] == "any" || info[0] == "Token*")
            continue;
        if (info[0].find("vector") != string::npos) {
            if (info[0] != "vector<Token*>") {
                ss << "\t\tfor (auto _x : " + info[1] + ") {\n";
                ss << "\t\t\t delete _x;\n\t\t}\n";
            }
            continue;
        }
        ss << "\t\tif (" + info[1] + ") delete " << info[1] + ";\n";
    }
    ss << "\t}\n\n";

    ss << "\tany accept(Visitor* visitor) {\n";
    ss << "\t\treturn visitor->visit" + className + baseName + "(this);\n";
    ss << "\t}";
    ss << "\n\n";
    // ss << "private:\n";
    for (auto &field: members) {
        ss << "\t" + field + ";\n";
    }
    ss << "};\n";
}

void defineAST(const std::string& outputDir,
               const std::string& baseName,
               std::vector<std::string> expressions)
{

    std::stringstream ss;
    ss << "#pragma once\n\n";
    if (baseName == "Expr")
        ss << "#include <token.hpp>\n";
    else if (baseName == "Stmt")
        ss << "#include <expr.hpp>\n";
    ss << "\n";

    // Forward declare types
    for (auto& expr: expressions) {
        vector<string> classInfo = split(expr, ':');
        ss << "class " + classInfo[0] + baseName << ";\n";
    }
    ss << "\n";

    ss << "class " + baseName + " {\n";
    ss << "public:\n";
    
    // Define Visitor Interface
    defineVisitor(ss, baseName, expressions);

    ss << "\tvirtual ~" + baseName + "() { }\n";
    ss << "\tvirtual any accept(Visitor* visitor) = 0;\n";
    ss << "};\n\n";

#if 1
    for (auto& expr: expressions) {
        vector<string> classInfo = split(expr, ':');
        defineType(ss, baseName, classInfo[0], classInfo[1]);
    }
#endif
    const std::string &content = ss.str();

    std::cout << content;

    // return;

    std::string filename = baseName;
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    std::string file = outputDir + "/" + filename + ".hpp";
    std::ofstream out(file);

    if (out.is_open()) {
        out << ss.str();
        out.close();
    } else {
        std::cerr << "Failed to create " << file << std::endl;
    }
}

int main(int argc, char *argv[]) {

    const string& path = "../include";
    defineAST(path, "Expr", {
        "Assign   : Token* name, Expr* value",
        "Binary   : Expr* left, Token* op, Expr* right",
        "Call     : Expr* callee, Token* paren, vector<Expr*> arguments",
        "Grouping : Expr* expr",
        "Literal  : any value",
        "Unary    : Token* op, Expr* right",
        "Variable : Token* name"
    });

    defineAST(path, "Stmt", {
      "Block      : vector<Stmt*> statements",
      "Expression : Expr* expression",
      "Function   : Token* name, vector<Token*> params, BlockStmt* body",
      "If         : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
      "While      : Expr* condition, Stmt* body",
      "Print      : Expr* expression",
      "Return     : Token* keyword, Expr* value",
      "Var        : Token* name, Expr* initializer"
    });
    
    return 0;
}
