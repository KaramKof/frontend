#include "I_Load.h"

I_Load::I_Load(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
               unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), rs1_index(rs1_index), imm(imm),
    pending_status_of_src_reg(pending_status_of_src_reg), addr(0), rs1_value(0), rd_value(0) {}

I_Load::I_Load(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
               unsigned int rd_index, unsigned int rs1_index, int imm, bool pending_status_of_src_reg)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), rs1_index(rs1_index), imm(imm),
    pending_status_of_src_reg(pending_status_of_src_reg), addr(0), rs1_value(0), rd_value(0) {}

I_Load::~I_Load() {}

unsigned int I_Load::get_rd_index() {
    return rd_index;
}

unsigned int I_Load::get_rs1_index() {
    return rs1_index;
}

void I_Load::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

void I_Load::set_rs1_index(unsigned int rs1_index) {
    this->rs1_index = rs1_index;
}

unsigned int I_Load::get_rd_value() {
    return rd_value;
}

unsigned int I_Load::get_rs1_value() {
    return rs1_value;
}

void I_Load::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

void I_Load::set_rs1_value(unsigned int rs1_value) {
    this->rs1_value = rs1_value;
}

void I_Load::set_immediate_value(int imm) {
    this->imm = imm;
}

int I_Load::get_immediate_value() {
    return imm;
}

void I_Load::set_pending_status_of_src_reg(bool pending_status_of_src_reg) {
    this->pending_status_of_src_reg = pending_status_of_src_reg;
}

unsigned int I_Load::get_addr() {
    return addr;
}

void I_Load::execute() {
    // In C++, when an int and an unsigned int are used together in an operation,
    // the int value is automatically implicitly converted to an unsigned int before the operation is performed.
    if(imm >= 0){
        addr = rs1_value + imm;
    }else{
        int abs_of_imm = -imm;
        addr = rs1_value - abs_of_imm;
    }

    if(instr_identifier == Instr_Name::NO_OP){
        cout << "Invalid bits pattern is detected. Instr_Name::NO_OP in Instr_Type::I_LOAD." << endl;
    }

}

map<string, unsigned int> I_Load::get_member_map() {
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rd_value"] = rd_value;
    member_map["rs1_index"] = rs1_index;
    member_map["rs1_value"] = rs1_value;
    member_map["imm"] = imm;
    member_map["addr"] = addr;
    return member_map;
}
