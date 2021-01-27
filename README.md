# Educational Project: CPU

Simple custom made CPU implementation on C++, which uses Reverse Polish notation for evaluating given expressions.

## Natively supported commands

| Mnemonic  | Op code (char) | Description  |
| --------- |:-------------:| ------------ |
| {label}:  | 0            | Support for labels |
| hlt       | 1            | Stops the execution |
| add       | a            | Pops two last values from stack and pushes their sum |
| sub       | b            | Pops two last values from stack and pushes their subtraction result |
| mul       | c            | Pops two last values from stack and pushes their multiplication result |
| div       | d            | Pops two last values from stack and pushes their division result |
| sqrt      | e            | Pops last value from stack and pushes it's square root |
| in        | f            | Reads double value from console and pushes to stack |
| out       | g            | Pops last value from stack and writes to console |
| ret       | h            | Returns to the last saved position in call stack, just after the last executed call command |
| push      | j            | Pushes double value to stack |
| push      | j            | Pushes value of given register (ax, bx, cx or dx) to stack |
| pop       | k            | Pops last value from stack |
| pop       | k            | Pops last value from stack and writes to given register (ax, bx, cx or dx) |
| jmp       | l            | Unconditionally jumps to the label |
| je        | m            | Pops two last values from stack and jumps to given label if second popped value is equal to the first one |
| jne       | n            | Pops two last values from stack and jumps to given label if second popped value is not equal to the first one |
| ja        | o            | Pops two last values from stack and jumps to given label if second popped value is greater then the first one |
| jae       | p            | Pops two last values from stack and jumps to given label if second popped value is greater then or equal to the first one |
| jb        | q            | Pops two last values from stack and jumps to given label if second popped value is less then the first one |
| jbe       | r            | Pops two last values from stack and jumps to given label if second popped value is less then or equal to the first one |
| call      | s            | Saves current position in call stack and jumps to given label |

Project uses lambda functions for registering callbacks for realizing commands functionality in assembly, disassembly and execution. It enables easy and dynamic management of functionality of the project. 

## Usage

### Prerequisites

You will need:

* A modern C/C++ compiler
* CMake 3.1+ installed
* If you prefer to code in a great IDE, I highly recommend [Jetbrains CLion](https://www.jetbrains.com/clion/). It is fully compatible with this project.

### Building The Project

#### Git Clone

First we need to check out the git repo:

```bash
❯ mkdir ~/workspace
❯ cd ~/workspace
❯ git clone \
    https://github.com/aamatevosyan/ispras_cpp_cpu \
    ispras_cpp_cpu
❯ cd ispras_cpp_cpu
```

Now we can build this project, and below we show three separate ways to do so.

#### Building Manually

```bash
❯ rm -rf build && mkdir build
❯ cd build
❯ cmake ..
❯ make
❯ cd ..
```

#### Running  Executable

##### asm

```bash
❯ build/asm
# Usage: asm [input] [output]
# Generates binary code from assembly source code
# 
# Example:
#        asm code.asm code.dasm
```

##### dasm

```bash
❯ build/asm
# Usage: dasm [input] [output]
# Generates assembly source code from binary code
# 
# Example:
#        dasm code.dasm code.asm
```

##### cpu

```bash
❯ build/cpu
# Usage: cpu [input]
# Runs binary code
# 
# Example:
#        cpu code.dasm
```


#### Running  Tests

```bash
❯ build/tests/tests
``` 

Currently only stack tests were added to gtest. To test you can use assembly/binary/disassembly files from /scripts.

### Building in CLion

Assuming you've done the above two steps, you can start CLion, and open the project's top level folder. CLion should automatically detect the top level `CMakeLists.txt` file and provide you with the full set of build targets.

Select menu option **Build   ➜ Build Project**.

#### Contributing

**Pull Requests are WELCOME!** Please submit any fixes or improvements, and I promise to review it as soon as I can at the project URL:

* [Project Github Home](https://github.com/aamatevosyan/ispras_cpp_cpu)
* [Submit Issues](https://github.com/aamatevosyan/ispras_cpp_cpu/issues)
* [Pull Requests](https://github.com/aamatevosyan/ispras_cpp_cpu/pulls)

### License

&copy; 2021 Armen Matevosyan

Open sourced under MIT license, the terms of which can be read here — [MIT License](http://opensource.org/licenses/MIT).
