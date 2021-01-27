//
// Created by armen on 1/27/21.
//

#include <sstream>
#include <iostream>
#include <algorithm>
#include "disassembly.h"
#include "custom_exceptions.hpp"

disassembly::disassembly(std::string inputPath) {
    this->inputPath = inputPath;
    this->setupRegisters();
    this->setupCommands();
}

void disassembly::convertToAssemblySourceCode() {
    infile.open(inputPath, std::ios::in | std::ios::binary);

    char code;
    while (infile.get(code)) {
        if (checkIfLabel(code)) {
            size_t position = 0;
            getValueFromBinaryInstructions(position);
            m_sourceCodeLines.push_back("label_" + std::to_string(position) + ":");
        } else {
            executeCommand(code);
        }
    }

    std::cout << "Done\n";
}

void disassembly::writeSourceCodeToFile(const std::string &outputPath) {
    std::ofstream fout(outputPath, std::ios::out);
    for (auto &line: m_sourceCodeLines)
        fout << line << "\n";
    fout.close();
}

void disassembly::setupRegisters() {
    m_registerNames.emplace('a', "ax");
    m_registerNames.emplace('b', "bx");
    m_registerNames.emplace('c', "cx");
    m_registerNames.emplace('d', "dx");
}

bool disassembly::isValidRegister(char code) {
    return m_registerNames.find(code) != m_registerNames.end();
}

std::string disassembly::getRegisterName(char code) {
    if (!isValidRegister(code)) {
        throw cpu_exception("Invalid register.");
    }
    return m_registerNames.at(code);
}

void disassembly::addNewCommand(const std::string &name, char code, const CommandBinaryRenderer &renderer) {
    if (m_commandRenderers.find(code) != m_commandRenderers.end()) {
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
    m_commandRenderers.emplace(code, renderer);
}

void disassembly::addNewDefaultCommandWithoutArgument(const std::string &name, char code) {
    addNewCommand(name, code, [this, name]() {
        m_sourceCodeLines.push_back(name);
    });
}

void disassembly::addNewDefaultCallableCommand(const std::string &name, char code) {
    addNewCommand(name, code, [this, name]() {
        size_t position = 0;
        getValueFromBinaryInstructions(position);
        m_sourceCodeLines.push_back(name + " " + "label_" + std::to_string(position));
    });
}

template<typename T>
void disassembly::getValueFromBinaryInstructions(T &value) {
    union {
        char data[sizeof(T)];
        T value;
    } converter{};

    for (char &i : converter.data) {
        infile.get(i);
    }

    value = converter.value;
}

bool disassembly::checkIfLabel(char code) {
    return code == LABEL_CODE;
}

void disassembly::setupCommands() {
    addNewDefaultCommandWithoutArgument("add", 'a');
    addNewDefaultCommandWithoutArgument("sub", 'b');
    addNewDefaultCommandWithoutArgument("mul", 'c');
    addNewDefaultCommandWithoutArgument("div", 'd');
    addNewDefaultCommandWithoutArgument("sqrt", 'e');
    addNewDefaultCommandWithoutArgument("in", 'f');
    addNewDefaultCommandWithoutArgument("out", 'g');
    addNewDefaultCommandWithoutArgument("ret", 'h');
    addNewDefaultCommandWithoutArgument("hlt", HALT_CODE);

    addNewCommand("push", 'j', [this]() {
        char valueType = 0;
        getValueFromBinaryInstructions(valueType);

        if (valueType == 'r') {
            char reg;
            getValueFromBinaryInstructions(reg);
            m_sourceCodeLines.push_back("push " + getRegisterName(reg));
        } else if (valueType == 'd') {
            double value;
            getValueFromBinaryInstructions(value);
            m_sourceCodeLines.push_back("push " + std::to_string(value));
        } else {
            throw disassembly_exception("Invalid argument for command: push");
        }
    });

    addNewCommand("pop", 'k', [this]() {
        char valueType = 0;
        getValueFromBinaryInstructions(valueType);

        if (valueType == 'r') {
            char reg;
            getValueFromBinaryInstructions(reg);
            m_sourceCodeLines.push_back("pop " + getRegisterName(reg));
        } else if (valueType == 'd') {
            m_sourceCodeLines.push_back("pop");
        } else {
            throw disassembly_exception("Invalid argument for command: pop");
        }
    });

    addNewDefaultCallableCommand("jmp", 'l');
    addNewDefaultCallableCommand("je", 'm');
    addNewDefaultCallableCommand("jne", 'n');
    addNewDefaultCallableCommand("ja", 'o');
    addNewDefaultCallableCommand("jae", 'p');
    addNewDefaultCallableCommand("jb", 'q');
    addNewDefaultCallableCommand("jbe", 'r');
    addNewDefaultCallableCommand("call", 's');
}

auto disassembly::getCommandRenderer(char code) {
    if (m_commandRenderers.find(code) == m_commandRenderers.end()) {
        throw assembly_exception("No command with given code.");
    }
    return m_commandRenderers.at(code);
}

void disassembly::executeCommand(char code) {
    auto renderer = getCommandRenderer(code);
    renderer();
}


