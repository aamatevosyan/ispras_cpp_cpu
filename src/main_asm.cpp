//
// Created by armen on 1/26/21.
//

#include <iostream>
#include "lib/assembly.h"

static void showHelpMessage() {
    std::cout << "Usage: asm [input] [output]\n"
            "Generates binary code from assembly source code\n"
            "\n"
            "Example:\n"
            "\tasm code.asm code.dasm\n";
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Incorrect number of arguments.\n";
        showHelpMessage();
        return -1;
    }

    std::string inputFilePath(argv[1]);
    std::string outputFilePath(argv[2]);

    assembly assembly(inputFilePath);
    assembly.convertToBinaryCode();
    assembly.writeBinaryInstructionsToFile(outputFilePath);

    return 0;
}