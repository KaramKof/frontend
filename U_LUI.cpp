#include "U_LUI.h"

U_LUI::U_LUI(const Instr_Name& instr_identifier, const Instr_Type& instr_type,
             unsigned int rd_index, int imm)
    : Instruction(instr_identifier, instr_type), rd_index(rd_index), imm(imm), rd_value(0) {
    // Add your implementation here
}

U_LUI::U_LUI(Instr_Name&& instr_identifier, Instr_Type&& instr_type,
             unsigned int rd_index, int imm)
    : Instruction(std::move(instr_identifier), std::move(instr_type)), rd_index(rd_index), imm(imm), rd_value(0){
    // Add your implementation here
}

U_LUI::~U_LUI() {
    // Add your implementation here
}

unsigned int U_LUI::get_rd_index() {
    return rd_index;
}

void U_LUI::set_rd_index(unsigned int rd_index) {
    this->rd_index = rd_index;
}

unsigned int U_LUI::get_rd_value() {
    return rd_value;
}

void U_LUI::set_rd_value(unsigned int rd_value) {
    this->rd_value = rd_value;
}

void U_LUI::set_immediate_value(int imm) {
    this->imm = imm;
}

int U_LUI::get_immediate_value() {
    return imm;
}

void U_LUI::execute() {
    rd_value = imm;
}

map<string, unsigned int> U_LUI::get_member_map() {
    map<string, unsigned int> member_map;
    member_map["rd_index"] = rd_index;
    member_map["rd_value"] = rd_value;
    member_map["imm"] = imm;
    return member_map;
}
