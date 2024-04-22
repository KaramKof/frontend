#include "register_file.h"


RegisterFile::RegisterFile() {
    register_file.fill(0);
    registers_pending_write.fill(false);
}

void RegisterFile::reset_register_file(){
    register_file.fill(0);
    registers_pending_write.fill(false);
    return;
}

unsigned int RegisterFile::read_a_word_by_reg_index(unsigned int reg_index){
    return register_file[reg_index];
}

void RegisterFile::write_a_word_by_reg_index(unsigned int reg_index, unsigned int data){
    register_file[reg_index] = data;
    return;
}


array<unsigned int, num_of_registers> RegisterFile::get_register_file(){
    return register_file;
}

void RegisterFile::set_register_as_pending_write(unsigned int reg_index){
    if(reg_index >= num_of_registers){
        return;
    }
    registers_pending_write[reg_index] = true;
    return;
}

bool RegisterFile::is_register_pending_write(unsigned int reg_index){
    return registers_pending_write[reg_index];
}

void RegisterFile::remove_register_from_pending_write(unsigned int reg_index){
    registers_pending_write[reg_index] = false;
    return;
}
