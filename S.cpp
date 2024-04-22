#include "S.h"

S::S(const Instr_Name& instr_identifier, const Instr_Type& instr_type, unsigned int rs1_index, unsigned int rs2_index,
     int imm, bool pending_status_of_src_reg)
    : Instruction(instr_identifier, instr_type), rs1_index(rs1_index), rs2_index(rs2_index), imm(imm),
    pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rs2_value(0), addr(0) {}

S::S(Instr_Name&& instr_identifier, Instr_Type&& instr_type, unsigned int rs1_index, unsigned int rs2_index, int imm,
     bool pending_status_of_src_reg)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rs1_index(rs1_index), rs2_index(rs2_index),
    imm(imm), pending_status_of_src_reg(pending_status_of_src_reg), rs1_value(0), rs2_value(0), addr(0) {}

S::~S() {}

unsigned int S::get_rs1_index() {
    return rs1_index;
}

unsigned int S::get_rs2_index() {
    return rs2_index;
}

void S::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

void S::set_rs2_index(unsigned int rs2_index) {
    this->rs2_index = rs2_index;
}

unsigned int S::get_rs1_value() {
    return rs1_value;
}

unsigned int S::get_rs2_value() {
    return rs2_value;
}

void S::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void S::set_rs2_value(unsigned int rs2_value) {
    this->rs2_value = rs2_value;
}

void S::set_immediate_value(int imm) {
    this->imm = imm;
}

int S::get_immediate_value() {
    return imm;
}

void S::set_pending_status_of_src_reg(bool pending_status_of_src_reg) {
    this->pending_status_of_src_reg = pending_status_of_src_reg;
}

unsigned int S::get_addr() {
    return addr;
}

void S::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        addr = rs1_value + imm;
    }else{
        int abs_of_imm = -imm;
        addr = rs1_value - abs_of_imm;
    }

    if(instr_identifier == Instr_Name::NO_OP){
        cout << "Invalid bits pattern is detected. Instr_Name::NO_OP in Instr_Type::S." << endl;
    }
}

map<string, unsigned int> S::get_member_map(){
    return map<string, unsigned int> {{"rs1_index",rs1_index}, {"rs1_value", rs1_value},
                                     {"rs2_index",rs2_index}, {"rs2_value", rs2_value},
                                     {"imm", imm}, {"addr", addr}};
}
