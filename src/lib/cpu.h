//
// Created by armen on 1/27/21.
//

#ifndef ISPRAS_CPP_CPU_CPU_H
#define ISPRAS_CPP_CPU_CPU_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include "mstack.hpp"

typedef std::function<void()> CommandBinaryExecutor;
typedef std::function<double(double, double)> CommandBinaryMathFunction;
typedef std::function<double(double)> CommandUnaryMathFunction;
typedef std::function<bool(double, double)> CommandBinaryLogicalFunction;

class cpu {
public:
    cpu();
    void execute(std::string inputPath);
    char getCode();
    char getCode(size_t position);
    double getRegisterValue(char code);
    void setRegisterValue(char code, double value);
    static bool checkIfLabel(char code);
    static bool checkIfHalt(char code);
    bool isValidRegister(char code);
    auto getCommandExecutor(char code);
    void executeCommand(char code);
    bool isTerminated() const;
    void addNewCommand(const std::string &name, char code, const CommandBinaryExecutor & executor);
    void addNewBinaryMathFunction(const std::string &name, char code, CommandBinaryMathFunction f);
    void addNewUnaryMathFunction(const std::string &name, char code, CommandUnaryMathFunction f);
    void addNewConditionalJump(const std::string &name, char code, CommandBinaryLogicalFunction f);
    void jumpTo(size_t position);
    void callTo(size_t position);
    void ret();

    template<typename T>
    void getValueFromBinaryInstructions(T &value);

protected:
    static const char LABEL_CODE = '0';
    static const char HALT_CODE = '1';

    void reset();
    void setupRegisters();
    void setupCommands();

    bool m_terminated;
    std::string inputPath;
    std::unordered_map<char, double> m_registers;
    std::vector<char> m_binaryInstructions;
    mstack<double> m_mainStack;
    mstack<size_t> m_returnStack;
    size_t m_currentPosition;
    std::unordered_map<char, std::string> m_registerNames;
    std::unordered_map<char, CommandBinaryExecutor> m_commandExecutors;
    std::unordered_map<char, std::string> m_commandRendererNames;
    std::set<std::string> m_allCommandRendererNames;
};

#endif //ISPRAS_CPP_CPU_CPU_H
