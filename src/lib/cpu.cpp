//
// Created by armen on 1/27/21.
//

#include <fstream>
#include <iostream>
#include <cmath>
#include "cpu.h"

void cpu::execute(std::string inputPath) {
    std::ifstream infile(inputPath, std::ios::in | std::ios::binary);
    std::vector<char> contents((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    m_binaryInstructions.assign(contents.begin(), contents.end());

    char code;
    while (!isTerminated()) {
        code = getCode();
        if (checkIfLabel(code)) {
            // skips labels
            m_currentPosition += sizeof(size_t);
        } else {
            executeCommand(code);
        }
    }
}

void cpu::reset() {
    m_currentPosition = 0;
    m_mainStack.reset();
    m_mainStack.reset();
}

cpu::cpu() {
    m_currentPosition = 0;
    m_terminated = false;
    this->setupRegisters();
    this->setupCommands();
}

void cpu::setupRegisters() {
    m_registers.emplace('a', 0);
    m_registers.emplace('b', 0);
    m_registers.emplace('c', 0);
    m_registers.emplace('d', 0);
}

char cpu::getCode(size_t position) {
    m_currentPosition = position + 1;

    if (position >= m_binaryInstructions.size())
        throw cpu_exception("No halt command specified");

    return m_binaryInstructions.at(position);
}

char cpu::getCode() {
    return getCode(m_currentPosition);
}

bool cpu::checkIfLabel(char code) {
    return code == LABEL_CODE;
}

bool cpu::checkIfHalt(char code) {
    return code == HALT_CODE;
}

void cpu::setupCommands() {
    addNewBinaryMathFunction("add", 'a', [](double a, double b) { return a + b; });
    addNewBinaryMathFunction("sub", 'b', [](double a, double b) { return a - b; });
    addNewBinaryMathFunction("mul", 'c', [](double a, double b) { return a * b; });
    addNewBinaryMathFunction("div", 'd', [](double a, double b) { return a / b; });
    addNewUnaryMathFunction("sqrt", 'e', [](double a) { return sqrt(a); });

    addNewCommand("in", 'f', [this]() {
        std::string tmp;
        double a;
        std::cout << "Please enter double for [in] command: \n";
        std::cin >> tmp;
        m_mainStack.push(std::stod(tmp));
    });

    addNewCommand("out", 'g', [this]() {
        double a = m_mainStack.pop();
        std::cout << "Result of [out] command: " << a << "\n";
    });

    addNewCommand("ret", 'h', [this]() {
        ret();
    });

    addNewCommand("hlt", HALT_CODE, [this]() {
        std::cout << "\nCPU halting...\n"
                     "Dumping main stack: \n\n";
        std::cout << m_mainStack.dump() << "\n";
        std::cout << "Dumping registers: \n\n";
        for (auto &iter: m_registers) {
            std::cout << iter.first << "\t" << iter.second << "\n";
        }
        std::cout << "\nCPU halted.\n";

        m_terminated = true;
    });

    addNewCommand("push", 'j', [this]() {
        char valueType = 0;
        getValueFromBinaryInstructions(valueType);

        if (valueType == 'r') {
            char reg;
            getValueFromBinaryInstructions(reg);
            m_mainStack.push(getRegisterValue(reg));
        } else if (valueType == 'd') {
            double value;
            getValueFromBinaryInstructions(value);
            m_mainStack.push(value);
        } else {
            throw cpu_exception("Invalid argument for command: push");
        }
    });

    addNewCommand("pop", 'k', [this]() {
        char valueType = 0;
        getValueFromBinaryInstructions(valueType);

        if (valueType == 'r') {
            char reg;
            getValueFromBinaryInstructions(reg);

            double value = m_mainStack.pop();
            setRegisterValue(reg, value);
        } else if (valueType == 'd') {
            m_mainStack.pop();
        } else {
            throw cpu_exception("Invalid argument for command: pop");
        }
    });

    addNewCommand("jmp", 'l', [this]() {
        size_t position = 0;
        getValueFromBinaryInstructions(position);
        jumpTo(position);
    });

    addNewConditionalJump("je", 'm', [](double a, double b) {
        return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
    });
    addNewConditionalJump("jne", 'n', [](double a, double b) {
        return std::fabs(a - b) >= std::numeric_limits<double>::epsilon();
    });
    addNewConditionalJump("ja", 'o', [](double a, double b) {
        return b > a && std::fabs(a - b) >= std::numeric_limits<double>::epsilon();
    });
    addNewConditionalJump("jae", 'p', [](double a, double b) {
        return (b > a && std::fabs(a - b) >= std::numeric_limits<double>::epsilon())
               || std::fabs(a - b) < std::numeric_limits<double>::epsilon();
    });
    addNewConditionalJump("jb", 'q', [](double a, double b) {
        return b < a && std::fabs(a - b) >= std::numeric_limits<double>::epsilon();
    });
    addNewConditionalJump("jbe", 'r', [](double a, double b) {
        return (b < a && std::fabs(a - b) >= std::numeric_limits<double>::epsilon())
               || std::fabs(a - b) < std::numeric_limits<double>::epsilon();
    });

    addNewCommand("call", 's', [this]() {
        size_t position = 0;
        getValueFromBinaryInstructions(position);
        callTo(position);
    });
}

double cpu::getRegisterValue(char code) {
    if (!isValidRegister(code)) {
        throw cpu_exception("Invalid register.");
    }
    return m_registers.at(code);
}

bool cpu::isValidRegister(char code) {
    return m_registers.find(code) != m_registers.end();
}

auto cpu::getCommandExecutor(char code) {
    if (m_commandExecutors.find(code) == m_commandExecutors.end()) {
        throw assembly_exception("No command executor with given code.");
    }
    return m_commandExecutors.at(code);
}

void cpu::executeCommand(char code) {
    auto renderer = getCommandExecutor(code);
    renderer();
}

bool cpu::isTerminated() const {
    return m_terminated;
}

void cpu::addNewCommand(const string &name, char code, const CommandBinaryExecutor &executor) {
    if (m_commandExecutors.find(code) != m_commandExecutors.end()) {
        throw disassembly_exception("Command already exists.");
    }

    if (m_commandRendererNames.find(code) != m_commandRendererNames.end()) {
        throw disassembly_exception("Command already exists.");
    }

    if (m_allCommandRendererNames.find(name) != m_allCommandRendererNames.end()) {
        throw disassembly_exception("Name for command already exists.");
    }

    m_allCommandRendererNames.emplace(name);
    m_commandRendererNames.emplace(code, name);
    m_commandExecutors.emplace(code, executor);
}

void cpu::addNewBinaryMathFunction(const string &name, char code, CommandBinaryMathFunction f) {
    addNewCommand(name, code, [this, f]() {
        double a = m_mainStack.pop();
        double b = m_mainStack.pop();
        double c = f(a, b);

        m_mainStack.push(c);
    });
}

void cpu::addNewUnaryMathFunction(const string &name, char code, CommandUnaryMathFunction f) {
    addNewCommand(name, code, [this, f]() {
        double a = m_mainStack.pop();
        double c = f(a);

        m_mainStack.push(c);
    });
}

void cpu::jumpTo(size_t position) {
    m_currentPosition = position;
}

void cpu::callTo(size_t position) {
    m_returnStack.push(m_currentPosition);
    jumpTo(position);
}

void cpu::ret() {
    m_currentPosition = m_returnStack.pop();
}

template<typename T>
void cpu::getValueFromBinaryInstructions(T &value) {
    union {
        char data[sizeof(T)];
        T value;
    } converter{};

    for (char &i : converter.data) {
        i = getCode();
    }

    value = converter.value;
}

void cpu::setRegisterValue(char code, double value) {
    if (!isValidRegister(code)) {
        throw cpu_exception("Invalid register.");
    }
    m_registers[code] = value;
}

void cpu::addNewConditionalJump(const string &name, char code, CommandBinaryLogicalFunction f) {
    addNewCommand(name, code, [this, f]() {
        double a = m_mainStack.pop();
        double b = m_mainStack.pop();

        if (f(a, b)) {
            executeCommand('l');
        } else {
            m_currentPosition += sizeof(size_t) + 1;
        }
    });
}

