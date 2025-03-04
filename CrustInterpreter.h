#ifndef Crust_H
#define Crust_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <iomanip>



namespace Crust {



    struct Cpp {
        bool containsWord(const std::string& str, const std::string& word) {
            return str.find(word) != std::string::npos;
        }
        struct Variable {
            std::string name;
            std::string value;
        };

        struct Function {
            std::string name;
            std::vector<std::string> params;
            std::vector<std::string> body;
        };

        std::vector<Variable> variables;
        std::vector<Function> functions;

        void addVariable(const std::string& name, const std::string& value) {
            for (auto& var : variables) {
                if (var.name == name) {
                    var.value = value;
                    return;
                }
            }
            variables.push_back({ name, value });
        }

        std::string getVariableValue(const std::string& name) {
            for (const auto& var : variables) {
                if (var.name == name) {
                    return var.value;
                }
            }
            return "Variable not found";
        }

        void evaluateExpression(const std::string& expression, std::string& result) {
            std::istringstream iss(expression);
            std::string token;
            std::vector<std::string> tokens;

            while (iss >> token) {
                tokens.push_back(token);
            }

            std::vector<float> values;
            std::vector<char> operators;
            for (const auto& t : tokens) {
                if (isdigit(t[0]) || (t[0] == '-' && t.size() > 1)) {
                    values.push_back(std::stof(t));
                }
                else if (t == "+" || t == "-" || t == "*" || t == "/") {
                    operators.push_back(t[0]);
                }
                else {
                    std::string varValue = getVariableValue(t);
                    if (varValue != "Variable not found") {
                        values.push_back(std::stof(varValue));
                    }
                    else {
                        result = varValue;
                        return;
                    }
                }
            }

            if (values.size() != operators.size() + 1) {
                std::cout << "Invalid expression" << std::endl;
                return;
            }

            float computation = values[0];
            for (size_t i = 0; i < operators.size(); ++i) {
                switch (operators[i]) {
                case '+':
                    computation += values[i + 1];
                    break;
                case '-':
                    computation -= values[i + 1];
                    break;
                case '*':
                    computation *= values[i + 1];
                    break;
                case '/':
                    computation /= values[i + 1];
                    break;
                }
            }

            result = std::to_string(computation);
        }

        void executeFunction(const std::string& fnName, const std::vector<std::string>& params) {
            for (const auto& fn : functions) {
                if (fn.name == fnName) {
                    if (params.size() != fn.params.size()) {
                        std::cout << "Incorrect number of parameters for function " + fnName << std::endl;
                        return;
                    }

                    std::unordered_map<std::string, std::string> paramMap;
                    for (size_t i = 0; i < params.size(); ++i) {
                        paramMap[fn.params[i]] = params[i];
                    }

                    for (const auto& statement : fn.body) {
                        std::string processedStatement = statement;
                        for (const auto& pair : paramMap) {
                            size_t pos = processedStatement.find(pair.first);
                            while (pos != std::string::npos) {
                                processedStatement.replace(pos, pair.first.length(), pair.second);
                                pos = processedStatement.find(pair.first, pos + pair.second.length());
                            }
                        }
                        execute(processedStatement);
                    }
                    return;
                }
            }
            std::cout << "Function " + fnName + " not found" << std::endl;
        }

        void execute(const std::string& code) {
            if (containsWord(code, "import std"))
            {
                if (containsWord(code, "func")) {
                    size_t funcPos = code.find("func") + 4;
                    size_t paramsStart = code.find("(");
                    size_t paramsEnd = code.find(")");
                    std::string funcName = code.substr(funcPos, paramsStart - funcPos);

                    std::string paramsStr = code.substr(paramsStart + 1, paramsEnd - paramsStart - 1);
                    std::istringstream iss(paramsStr);
                    std::vector<std::string> params;
                    std::string param;
                    while (std::getline(iss, param, ',')) {
                        params.push_back(param);
                    }

                    size_t bodyStart = code.find("{");
                    size_t bodyEnd = code.find("}");
                    std::string body = code.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
                    functions.push_back({ funcName, params, {body} });

                }
                else if (containsWord(code, "call")) {
                    size_t callPos = code.find("call") + 4;
                    size_t paramsStart = code.find("(");
                    size_t paramsEnd = code.find(")");
                    std::string funcName = code.substr(callPos, paramsStart - callPos);

                    std::string paramsStr = code.substr(paramsStart + 1, paramsEnd - paramsStart - 1);
                    std::istringstream iss(paramsStr);
                    std::vector<std::string> params;
                    std::string param;
                    while (std::getline(iss, param, ',')) {
                        params.push_back(param);
                    }

                    executeFunction(funcName, params);

                }
                else if (containsWord(code, "print")) {
                    size_t startPos = code.find("(") + 1;
                    size_t endPos = code.find(")");
                    std::string content = code.substr(startPos, endPos - startPos);
                    std::string result;
                    evaluateExpression(content, result);
                    std::cout << result <<std::endl;

                }
                else if (containsWord(code, "var")) {
                    size_t varPos = code.find("var") + 3;
                    size_t equalsPos = code.find("=");
                    std::string varName = code.substr(varPos, equalsPos - varPos);
                    varName.erase(0, varName.find_first_not_of(" "));
                    varName.erase(varName.find_last_not_of(" ") + 1);

                    size_t valueStart = equalsPos + 1;
                    size_t semicolonPos = code.find(";");
                    std::string varValue = code.substr(valueStart, semicolonPos - valueStart);
                    addVariable(varName, varValue);

                }
                else if (containsWord(code, "if")) {
                    size_t ifPos = code.find("if") + 2;
                    size_t thenPos = code.find("then");
                    std::string condition = code.substr(ifPos, thenPos - ifPos);

                    size_t commandStart = thenPos + 4;
                    size_t commandEnd = code.find(";", commandStart);
                    std::string command = code.substr(commandStart, commandEnd - commandStart);

                    std::istringstream iss(condition);
                    std::string lhs, op, rhs;
                    iss >> lhs >> op >> rhs;

                    float lhsValue = std::stof(getVariableValue(lhs));
                    float rhsValue = std::stof(rhs);
                    bool conditionMet = false;

                    if (op == "==") conditionMet = (lhsValue == rhsValue);
                    else if (op == "!=") conditionMet = (lhsValue != rhsValue);
                    else if (op == "<") conditionMet = (lhsValue < rhsValue);
                    else if (op == "<=") conditionMet = (lhsValue <= rhsValue);
                    else if (op == ">") conditionMet = (lhsValue > rhsValue);
                    else if (op == ">=") conditionMet = (lhsValue >= rhsValue);

                    if (conditionMet) {
                        execute(command);
                    }

                }
                else if (containsWord(code, "read")) {
                    size_t startPos = code.find("(") + 1;
                    size_t endPos = code.find(")");
                    std::string varName = code.substr(startPos, endPos - startPos);

                    std::string inputValue;
                    std::cout << "|----> ";
                    std::getline(std::cin, inputValue);
                    addVariable(varName, inputValue);
                }
            }
        }

        void executeFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Could not open file: " << filename << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                execute(line);
            }

            file.close();
        }
    };

} 

#endif
