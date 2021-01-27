//
// Created by armen on 1/27/21.
//

#ifndef ISPRAS_CPP_CPU_DISASSEMBLY_H
#define ISPRAS_CPP_CPU_DISASSEMBLY_H

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <fstream>
#include <set>

typedef std::function<void()> CommandBinaryRenderer;

class disassembly {
public:
    explicit disassembly(std::string inputPath);
    std::string inputPath;
    void convertToAssemblySourceCode();
    void writeSourceCodeToFile(const std::string& outputPath);
    bool isValidRegister(char code);
    std::string getRegisterName(char code);
    void addNewCommand(const std::string &name, char code, const CommandBinaryRenderer& renderer);
    void addNewDefaultCommandWithoutArgument(const std::string& name, char code);
    void addNewDefaultCallableCommand(const std::string& name, char code);
    static bool checkIfLabel(char code);
    void executeCommand(char code);
    auto getCommandRenderer(char code);

    template<typename T>
    void getValueFromBinaryInstructions(T &value);

protected:
    char static constexpr LABEL_CODE = '0';
    char static constexpr HALT_CODE = '1';

    void setupCommands();
    void setupRegisters();
    std::ifstream infile;
    std::vector<std::string> m_sourceCodeLines;
    std::unordered_map<char, std::string> m_registerNames;
    std::unordered_map<char, CommandBinaryRenderer> m_commandRenderers;
    std::unordered_map<char, std::string> m_commandRendererNames;
    std::set<std::string> m_allCommandRendererNames;
    std::set<char> m_allCallableCommands;
};

#endif //ISPRAS_CPP_CPU_DISASSEMBLY_H
