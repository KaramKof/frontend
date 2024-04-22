#include "JAL.h"

JAL::JAL(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
         unsigned int rd_index, int imm, unsigned int pc_addr, bool pending_status_of_rd)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), imm(imm), pc_addr(pc_addr),
    pending_status_of_rd(pending_status_of_rd), rd_value(0) {}

JAL::JAL(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
         unsigned int rd_index, int imm, unsigned int pc_addr, bool pending_status_of_rd)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), imm(imm), pc_addr(pc_addr),
    pending_status_of_rd(pending_status_of_rd), rd_value(0) {}

JAL::~JAL() {}

unsigned int JAL::get_rd_index() {
    return rd_index;
}

void JAL::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

unsigned int JAL::get_rd_value() {
    return rd_value;
}

void JAL::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

void JAL::set_immediate_value(int imm) {
    this->imm = imm;
}

int JAL::get_immediate_value() {
    return imm;
}

void JAL::set_pending_status_of_rd(bool pending_status_of_rd) {
    this->pending_status_of_rd = pending_status_of_rd;
}

void JAL::set_pc_addr(unsigned int pc_addr) {
    this->pc_addr = pc_addr;
}

unsigned int JAL::get_pc_addr() {
    return pc_addr;
}

void JAL::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        pc_addr = pc_addr + imm;
    }else{
        int abs_of_imm = -imm;
        pc_addr = pc_addr - abs_of_imm;
    }
}

map<string, unsigned int> JAL::get_member_map() {
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rd_value"] = rd_value;
    member_map["imm"] = imm;
    member_map["pc_addr"] = pc_addr;
    return member_map;
}
