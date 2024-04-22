#include "U_AUIPC.h"

U_AUIPC::U_AUIPC(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
                 unsigned int rd_index, int imm, unsigned int pc_addr)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), imm(imm), pc_addr(pc_addr), rd_value(0){
    // Add your implementation here
}

U_AUIPC::U_AUIPC(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
                 unsigned int rd_index, int imm, unsigned int pc_addr)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), imm(imm),
    pc_addr(pc_addr), rd_value(0){
    // Add your implementation here
}

U_AUIPC::~U_AUIPC() {
    // Add your implementation here
}

unsigned int U_AUIPC::get_rd_index() {
    return rd_index;
}

void U_AUIPC::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

unsigned int U_AUIPC::get_rd_value() {
    return rd_value;
}

void U_AUIPC::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

void U_AUIPC::set_immediate_value(int imm) {
    this->imm = imm;
}

int U_AUIPC::get_immediate_value() {
    return imm;
}

void U_AUIPC::set_pc_addr(unsigned int pc_addr) {
    this->pc_addr = pc_addr;
}

unsigned int U_AUIPC::get_pc_addr() {
    return pc_addr;
}

void U_AUIPC::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        rd_value = pc_addr + imm;
    }else{
        int abs_of_imm = -imm;
        rd_value = pc_addr - abs_of_imm;
    }
}

map<string, unsigned int> U_AUIPC::get_member_map(){
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rd_value"] = rd_value;
    member_map["imm"] = imm;
    member_map["pc_addr"] = pc_addr;
    return member_map;
}
