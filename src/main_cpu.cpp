//
// Created by armen on 1/26/21.
//

#include <iostream>
#include "lib/cpu.h"

static void showHelpMessage() {
    std::cout << "Usage: cpu [input]\n"
            "Runs binary code\n"
            "\n"
            "Example:\n"
            "\tcpu code.dasm\n";
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect number of arguments.\n";
        showHelpMessage();
        return -1;
    }

    std::string inputFilePath(argv[1]);

    cpu cpu;
    cpu.execute(inputFilePath);

    return 0;
}

