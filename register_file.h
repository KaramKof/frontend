#pragma once
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include "cache.h"
#include "memory.h"


static constexpr unsigned int num_of_registers = 32;


class RegisterFile {
public:
    RegisterFile();

    // Other class members, variables, and methods go here
    unsigned int read_a_word_by_reg_index(unsigned int reg_index);

    void write_a_word_by_reg_index(unsigned int reg_index, unsigned int data);

    void reset_register_file();

    array<unsigned int, num_of_registers> get_register_file();

    void set_register_as_pending_write(unsigned int reg_index);

    bool is_register_pending_write(unsigned int reg_index);

    void remove_register_from_pending_write(unsigned int reg_index);

private:
    // Register file data structure goes here
    array<unsigned int, num_of_registers> register_file;
    array<bool, num_of_registers> registers_pending_write;
};

#endif // REGISTER_FILE_H
