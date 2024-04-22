#include "B.h"

B::B(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
     unsigned int rs1_index, unsigned int rs2_index, int imm, unsigned int pc_addr, bool pending_status_of_src_reg)
    : Instruction(instr_identifier, instr_type), rs1_index(rs1_index), rs2_index(rs2_index), imm(imm),
    pc_addr(pc_addr), pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rs2_value(0) {}

B::B(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
     unsigned int rs1_index, unsigned int rs2_index, int imm, unsigned int pc_addr, bool pending_status_of_src_reg)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rs1_index(rs1_index), rs2_index(rs2_index),
    imm(imm), pc_addr(pc_addr), pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rs2_value(0) {}

B::~B() {}

unsigned int B::get_rs1_index() {
    return rs1_index;
}

unsigned int B::get_rs2_index() {
    return rs2_index;
}

void B::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

void B::set_rs2_index(unsigned int rs2_index) {
    this->rs2_index = rs2_index;
}

unsigned int B::get_rs1_value() {
    return rs1_value;
}

unsigned int B::get_rs2_value() {
    return rs2_value;
}

void B::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void B::set_rs2_value(unsigned int rs2_value) {
    this->rs2_value = rs2_value;
}

void B::set_immediate_value(int imm) {
    this->imm = imm;
}

int B::get_immediate_value() {
    return imm;
}

bool B::is_any_src_reg_pending_write() {
    return pending_status_of_src_reg;
}

void B::set_pending_status_of_src_reg(bool pending_status_of_src_reg) {
    this->pending_status_of_src_reg = pending_status_of_src_reg;
}

void B::set_pc_addr(unsigned int pc_addr) {
    this->pc_addr = pc_addr;
}

unsigned int B::get_pc_addr() {
    return pc_addr;
}

void B::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        pc_addr = pc_addr + imm;
    }else{
        int abs_of_imm = -imm;
        pc_addr = pc_addr - abs_of_imm;
    }
}

bool B::is_branch_taken() {
    // {
    // {0x00000000, Instr_Name::BEQ },
    // {0x00001000, Instr_Name::BNE },
    // {0x00005000, Instr_Name::BGE },
    // {0x00007000, Instr_Name::BGEU },
    // {0x00004000, Instr_Name::BLT },
    // {0x00006000, Instr_Name::BLTU }, };

    switch (instr_identifier)
    {
    case Instr_Name::BEQ:
        return static_cast<int>(rs1_value) == static_cast<int>(rs2_value);
    case Instr_Name::BNE:
        return static_cast<int>(rs1_value) != static_cast<int>(rs2_value);
    case Instr_Name::BGE:
        return static_cast<int>(rs1_value) >= static_cast<int>(rs2_value);
    case Instr_Name::BGEU:
        return rs1_value >= rs2_value;
    case Instr_Name::BLT:
        return static_cast<int>(rs1_value) < static_cast<int>(rs2_value);
    case Instr_Name::BLTU:
        return rs1_value < rs2_value;
    default:
        return false;
    }
}

map<string, unsigned int> B::get_member_map(){
    return map<string, unsigned int> {{"rs1_index",rs1_index}, {"rs1_value", rs1_value},
                                     {"rs2_index",rs2_index}, {"rs2_value", rs2_value},
                                     {"imm", imm}, {"pc_addr", pc_addr}};
}
