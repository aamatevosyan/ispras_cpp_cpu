//
// Created by armen on 1/26/21.
//

#include <iostream>
#include "lib/disassembly.h"

static void showHelpMessage() {
    std::cout << "Usage: dasm [input] [output]\n"
            "Generates assembly source code from binary code\n"
            "\n"
            "Example:\n"
            "\tdasm code.dasm code.asm\n";
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Incorrect number of arguments.\n";
        showHelpMessage();
        return -1;
    }

    std::string inputFilePath(argv[1]);
    std::string outputFilePath(argv[2]);

    disassembly disassembly(inputFilePath);
    disassembly.convertToAssemblySourceCode();
    disassembly.writeSourceCodeToFile(outputFilePath);

    return 0;
}

