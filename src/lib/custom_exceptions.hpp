//
// Created by armen on 1/26/21.
//

#ifndef ISPRAS_CPP_CPU_CUSTOM_EXCEPTIONS_HPP
#define ISPRAS_CPP_CPU_CUSTOM_EXCEPTIONS_HPP

class mexception : public std::runtime_error {
public:
    explicit mexception(const char *msg)
            : std::runtime_error(msg) {
    }
};

class stack_exception : public mexception {
public:
    explicit stack_exception(const char *msg)
            : mexception(msg) {
    }
};

class assembly_exception : public mexception {
public:
    explicit assembly_exception(const char *msg)
            : mexception(msg) {
    }
};

class disassembly_exception : public mexception {
public:
    explicit disassembly_exception(const char *msg)
            : mexception(msg) {
    }
};

class cpu_exception : public mexception {
public:
    explicit cpu_exception(const char *msg)
            : mexception(msg) {
    }
};

#endif //ISPRAS_CPP_CPU_CUSTOM_EXCEPTIONS_HPP
