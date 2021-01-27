//
// Created by armen on 1/26/21.
//

#include <sstream>
#include <iostream>
#include "assembly.h"
#include "utils.hpp"
#include "custom_exceptions.hpp"

assembly::assembly(const std::string inputPath) {
    this->inputPath = inputPath;
    this->setupRegisters();
    this->setupLabelEngine();
    this->setupCommands();
}

void assembly::convertToBinaryCode() {
    std::ifstream infile(inputPath);
    std::string line;
    std::string command;
    std::string arg;

    while (std::getline(infile, line)) {
        if (!getCommandAndArgumentFromLine(line, command, arg)) {
            continue;
        } else if (checkIfLabel(command)) {
            // case when label
            addNewLabel(command);
        } else if (checkIfCallableCommand(command)) {
            // case when callable command
            executeCommand(command, arg);
        } else {
            // other commands
            executeCommand(command, arg);
        }
    }

    for (auto& iter: m_notFoundCodeLabels) {
        auto labelIter = m_codeLabels.find(iter.first);
        if (labelIter == m_codeLabels.end()) {
            throw assembly_exception("Label's definition is missing.");
        } else {
            for (auto& position: iter.second) {
                editValueFromBinaryInstructions(labelIter->second, position);
            }
        }
    }
    std::cout << "Done\n";
}

void assembly::writeBinaryInstructionsToFile(const std::string &outputPath) {
    std::ofstream fout(outputPath, std::ios::out | std::ios::binary);
    fout.write(m_binaryInstructions.data(), m_binaryInstructions.size());
    fout.close();
}

void assembly::setupCommands() {
    addNewDefaultCommandWithoutArgument("add", 'a');
    addNewDefaultCommandWithoutArgument("sub", 'b');
    addNewDefaultCommandWithoutArgument("mul", 'c');
    addNewDefaultCommandWithoutArgument("div", 'd');
    addNewDefaultCommandWithoutArgument("sqrt", 'e');
    addNewDefaultCommandWithoutArgument("in", 'f');
    addNewDefaultCommandWithoutArgument("out", 'g');
    addNewDefaultCommandWithoutArgument("ret", 'h');
    addNewDefaultCommandWithoutArgument("hlt", HALT_CODE);

    addNewCommand("push", 'j', [this](std::string arg) {
        pushValueToBinaryInstructions('j');

        if (isValidRegister(arg)) {
            pushValueToBinaryInstructions('r');
            pushValueToBinaryInstructions(getRegisterCode(arg));
        } else {
            pushValueToBinaryInstructions('d');
            pushValueToBinaryInstructions(std::stod(arg));
        }
    });

    addNewCommand("pop", 'k', [this](std::string arg) {
        pushValueToBinaryInstructions('k');

        if (isValidRegister(arg)) {
            pushValueToBinaryInstructions('r');
            pushValueToBinaryInstructions(getRegisterCode(arg));
        } else if (arg.empty()) {
            pushValueToBinaryInstructions('d');
        } else {
            throw assembly_exception("Invalid argument for command.");
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

template<typename T>
void assembly::pushValueToBinaryInstructions(T value) {
    union {
        char data[sizeof(T)];
        T value;
    } converter{};
    converter.value = value;

    for (size_t i = 0; i < sizeof(T); i++) {
        m_binaryInstructions.push_back(converter.data[i]);
    }
}

void assembly::setupRegisters() {
    m_registerCodes.emplace("ax", 'a');
    m_registerCodes.emplace("bx", 'b');
    m_registerCodes.emplace("cx", 'c');
    m_registerCodes.emplace("dx", 'd');
}

bool assembly::isValidRegister(std::string reg) {
    return m_registerCodes.find(reg) != m_registerCodes.end();
}

char assembly::getRegisterCode(std::string reg) {
    return m_registerCodes.at(reg);
}

char assembly::getCommandRenderersCode(std::string name) {
    return m_commandRendererCodes.at(name);
}

auto assembly::getCommandRenderer(std::string name) {
    if (m_commandRenderers.find(name) == m_commandRenderers.end()) {
        throw assembly_exception("No command with given name.");
    }
    return m_commandRenderers.at(name);
}

void assembly::executeCommand(std::string command, std::string arg) {
    auto renderer = getCommandRenderer(command);
    renderer(arg);
}

void assembly::addNewCommand(const std::string &name, char code, const CommandAssemblyRenderer &renderer) {
    if (m_commandRenderers.find(name) != m_commandRenderers.end()) {
        throw assembly_exception("Command already exists.");
    }

    if (m_commandRendererCodes.find(name) != m_commandRendererCodes.end()) {
        throw assembly_exception("Command already exists.");
    }

    if (m_allCommandRendererCodes.find(code) != m_allCommandRendererCodes.end()) {
        throw assembly_exception("Code for command already exists.");
    }

    m_allCommandRendererCodes.emplace(code);
    m_commandRendererCodes.emplace(name, code);
    m_commandRenderers.emplace(name, renderer);
}

void assembly::addNewDefaultCommandWithoutArgument(const std::string &name, const char code) {
    addNewCommand(name, code, [this, code](const std::string &arg) {
        if (!arg.empty()) {
            throw assembly_exception("Command do not support an argument");
        }
        pushValueToBinaryInstructions(code);
    });
}

bool assembly::checkIfLabel(std::string name) {
    return name[name.length() - 1] == ':';
}

bool assembly::checkIfCallableCommand(std::string name) {
    return m_allCallableCommands.find(name) != m_allCallableCommands.end();
}

void assembly::addNewLabel(std::string label) {
    if (m_codeLabels.find(label) != m_codeLabels.end()) {
        throw assembly_exception("Label already exists.");
    }

    size_t position = m_binaryInstructions.size();
    m_codeLabels.emplace(label.substr(0, label.length() - 1), position + sizeof(size_t) + 1);
    pushValueToBinaryInstructions(LABEL_CODE);
    pushValueToBinaryInstructions(position + sizeof(size_t) + 1);
}

bool assembly::getCommandAndArgumentFromLine(std::string line, std::string &command, std::string &arg) {
    trim(line);
    if (line.empty()) {
        return false;
    }

    std::istringstream ss(line);
    command = "";
    arg = "";

    ss >> command;
    ss >> arg;
    if (ss >> arg) {
        throw assembly_exception("Too many arguments for command.");
    }

    return true;
}

void assembly::addNewDefaultCallableCommand(const std::string &name, char code) {
    addNewCommand(name, code, [this, code](const std::string &label) {
        if (label.empty()) {
            throw assembly_exception("Command must have an argument");
        }

        pushValueToBinaryInstructions(code);
        auto iter = m_codeLabels.find(label);
        size_t position = 0;

        if (iter != m_codeLabels.end()) {
            position = iter->second;
        } else if (m_notFoundCodeLabels.find(label) == m_notFoundCodeLabels.end()) {
            m_notFoundCodeLabels.emplace(label, std::vector<size_t>());
            m_notFoundCodeLabels.at(label).push_back(m_binaryInstructions.size());
        } else {
            m_notFoundCodeLabels.at(label).push_back(m_binaryInstructions.size());
        }

        pushValueToBinaryInstructions(position);
    });
}

template<typename T>
void assembly::editValueFromBinaryInstructions(T value, size_t position) {
    union {
        char data[sizeof(T)];
        T value;
    } converter{};
    converter.value = value;

    for (size_t i = 0; i < sizeof(T); i++) {
        m_binaryInstructions.at(position + i) = converter.data[i];
    }
}

void assembly::setupLabelEngine() {
    m_allCommandRendererCodes.emplace(LABEL_CODE);
}


