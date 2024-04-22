#include "I_JALR.h"

I_JALR::I_JALR(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
               unsigned int rd_index, unsigned int rs1_index, int imm, unsigned int pc_addr, bool pending_status_of_rs1_rd)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), rs1_index(rs1_index),
    imm(imm), pc_addr(pc_addr), pending_status_of_rs1_rd(pending_status_of_rs1_rd), rs1_value(0), rd_value(0) {}

I_JALR::I_JALR(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
               unsigned int rd_index, unsigned int rs1_index, int imm, unsigned int pc_addr, bool pending_status_of_rs1_rd)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), rs1_index(rs1_index),
    imm(imm), pc_addr(pc_addr), pending_status_of_rs1_rd(pending_status_of_rs1_rd), rs1_value(0), rd_value(0) {}

I_JALR::~I_JALR() {
    // Destructor body
}

unsigned int I_JALR::get_rd_index() {
    return rd_index;
}

void I_JALR::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

unsigned int I_JALR::get_rd_value() {
    return rd_value;
}

void I_JALR::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

unsigned int I_JALR::get_rs1_index() {
    return rs1_index;
}

void I_JALR::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

unsigned int I_JALR::get_rs1_value() {
    return rs1_value;
}

void I_JALR::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void I_JALR::set_immediate_value(int imm) {
    this->imm = imm;
}

int I_JALR::get_immediate_value() {
    return imm;
}

void I_JALR::set_pending_status_of_rs1_rd(bool pending_status_of_rs1_rd) {
    this->pending_status_of_rs1_rd = pending_status_of_rs1_rd;
}

void I_JALR::set_pc_addr(unsigned int pc_addr) {
    this->pc_addr = pc_addr;
}

unsigned int I_JALR::get_pc_addr() {
    return pc_addr;
}

void I_JALR::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        pc_addr = rs1_value + imm;
    }else{
        int abs_of_imm = -imm;
        pc_addr = rs1_value - abs_of_imm;
    }
    if(instr_identifier == Instr_Name::NO_OP){
        cout << "Invalid bits pattern is detected. Instr_Name::NO_OP in Instr_Type::I_JALR." << endl;
    }
}

map<string, unsigned int> I_JALR::get_member_map() {
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rd_value"] = rd_value;
    member_map["rs1_index"] = rs1_index;
    member_map["rs1_value"] = rs1_value;
    member_map["imm"] = imm;
    member_map["pc_addr"] = pc_addr;
    return member_map;
}
