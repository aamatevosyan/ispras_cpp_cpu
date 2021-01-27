//
// Created by armen on 1/26/21.
//

#ifndef ISPRAS_CPP_CPU_ASSEMBLY_H
#define ISPRAS_CPP_CPU_ASSEMBLY_H

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <fstream>
#include <set>

typedef std::function<void(std::string arg)> CommandAssemblyRenderer;

class assembly {
public:
    const char LABEL_CODE = '0';
    const char HALT_CODE = '1';

    explicit assembly(std::string inputPath);

    void convertToBinaryCode();

    void writeBinaryInstructionsToFile(const std::string& outputPath);

    void executeCommand(std::string command, std::string arg);

    std::string inputPath;

    bool isValidRegister(std::string reg);

    char getRegisterCode(std::string reg);

    char getCommandRenderersCode(std::string name);

    auto getCommandRenderer(std::string name);

    void addNewCommand(const std::string& name, char code, const CommandAssemblyRenderer& renderer);

    void addNewDefaultCommandWithoutArgument(const std::string& name, char code);

    void addNewDefaultCallableCommand(const std::string& name, char code);

    static bool checkIfLabel(std::string name);

    bool checkIfCallableCommand(std::string name);

    void addNewLabel(std::string label);

    static bool getCommandAndArgumentFromLine(std::string line, std::string& command, std::string& arg);

    void setupLabelEngine();

protected:

    void setupRegisters();

    void setupCommands();

    template<typename T>
    void pushValueToBinaryInstructions(T value);

    template<typename T>
    void editValueFromBinaryInstructions(T value, size_t position);

    std::vector<char> m_binaryInstructions;
    std::unordered_map<std::string, size_t> m_codeLabels;
    std::unordered_map<std::string, CommandAssemblyRenderer> m_commandRenderers;
    std::unordered_map<std::string, char> m_registerCodes;
    std::unordered_map<std::string, char> m_commandRendererCodes;
    std::set<char> m_allCommandRendererCodes;
    std::set<std::string> m_allCallableCommands;
    std::unordered_map<std::string, std::vector<size_t>> m_notFoundCodeLabels;
};

#endif //ISPRAS_CPP_CPU_ASSEMBLY_H
