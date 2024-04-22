#pragma once
#ifndef INSTR_DECODE_H
#define INSTR_DECODE_H

#include <functional>
#include "register_file.h"
#include "Instruction.h"
#include "Instr_Fetch.h"
#include "R_or_M.h"
#include "I_Basic.h"
#include "I_Load.h"
#include "S.h"
#include "B.h"
#include "JAL.h"
#include "I_JALR.h"
#include "U_AUIPC.h"
#include "U_LUI.h"
#include "NO-OP.h"
#include "STALL.h"
#include "HALT.h"

using std::function, std::bind, std::tie ;
using namespace std::placeholders;

class InstrDecode {
public:
    InstrDecode(RegisterFile& register_file, InstrFetch& instr_fetch);

    ~InstrDecode(); // Destructor

    void reset();

    tuple<bool, unsigned int, unsigned int> get_saved_instr();

    Instruction* get_on_hold_instr();


    // Add your methods here
    Instruction* proceed(string signal);

    tuple<R_or_M*, bool> decode_R_or_M_type(unsigned int instr_word, unsigned int instr_addr);

    // redecode the R or M type instruction because its source register(s) were pending write in last cycle.
    // returns true if any of its source register(s) are pending write, thus decode cannot finish.
    bool redecode_R_or_M_type(Instruction* instr_obj);

    tuple<I_Basic*, bool> decode_I_Basic_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_I_Basic_type(Instruction* instr_obj);

    tuple<I_Load*, bool> decode_I_Load_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_I_Load_type(Instruction* instr_obj);

    tuple<S*, bool> decode_S_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_S_type(Instruction* instr_obj);

    tuple<B*, bool> decode_B_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_B_type(Instruction* instr_obj);

    tuple<JAL*, bool> decode_JAL_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_JAL_type(Instruction* instr_obj);

    tuple<I_JALR*, bool> decode_I_JALR_type(unsigned int instr_word, unsigned int instr_addr);

    bool redecode_I_JALR_type(Instruction* instr_obj);

    tuple<U_AUIPC*, bool> decode_U_AUIPC_type(unsigned int instr_word, unsigned int instr_addr);

    tuple<U_LUI*, bool> decode_U_LUI_type(unsigned int instr_word, unsigned int instr_addr);

    tuple<HALT*, bool> decode_HALT_type(unsigned int instr_word, unsigned int instr_addr);

private:
    RegisterFile& register_file;
    InstrFetch& instr_fetch;
    // map from opcode to instruction type
    map<unsigned int, Instr_Type> map_of_opcode_to_instr_type;
    // map from funct7~funct3 value to instruction identifier
    // R type and M type share the same 7-bit OPCODE
    map<unsigned int, Instr_Name> map_for_R_or_M_type;
    map<unsigned int, Instr_Name> map_for_I_Basic_type;
    map<unsigned int, Instr_Name> map_for_I_Load_type;
    map<unsigned int, Instr_Name> map_for_S_type;
    map<unsigned int, Instr_Name> map_for_B_type;
    // JAL type is special, it has no funct3 or funct7
    map<unsigned int, Instr_Name> map_for_JAL_type;
    map<unsigned int, Instr_Name> map_for_I_JALR_type;
    // U type has no funct3 or funct7
    map<unsigned int, Instr_Name> map_for_U_AUIPC_type;
    // U type has no funct3 or funct7
    map<unsigned int, Instr_Name> map_for_U_LUI_type;

    map<Instr_Type, function<tuple<Instruction*, bool>(unsigned int, unsigned int)>> map_of_various_decode_funcs;
    // an instruction in decode stage stalls in last cycle, it will be redecode in new cycle
    map<Instr_Type, function<bool(Instruction*)>> map_of_various_redecode_funcs;
    // tuple<is_valid_instr, instr_word, instr_addr> returned from Fetch stage on the left
    tuple<bool, unsigned int, unsigned int> saved_instr;
    // on_hold_instr from last cycle
    Instruction* on_hold_instr;
    // indicates if InstrDecode has done processing the previous cycle's instruction
    bool is_last_cycle_instr_finished;
    // indicates if the previous cycle's instruction has been passed on to the next stage
    bool is_last_cycle_instr_passed_over;

    // 7-bit opcode
    static constexpr unsigned int num_of_bits_to_indicate_opcode = 7;
    static constexpr unsigned int mask_for_opcode = (1 << num_of_bits_to_indicate_opcode) -1;

    static constexpr unsigned int num_of_bits_to_indicate_register_index = 5;
    static constexpr unsigned int mask_for_register_index = (1 << num_of_bits_to_indicate_register_index) -1;

    static constexpr unsigned int num_of_bits_to_indicate_funct3 = 3;
    static constexpr unsigned int num_of_bits_to_indicate_funct7 = 7;
    static constexpr unsigned int mask_for_funct3 = ((1 << num_of_bits_to_indicate_funct3) -1) \
                                                    << (num_of_bits_to_indicate_opcode + num_of_bits_to_indicate_register_index);
    static constexpr unsigned int mask_for_funct7_and_funct3 = 0xFE007000;
    static constexpr unsigned int mask_for_funct7 = 0xFE000000;
    static constexpr unsigned int mask_for_upper_20_bits = 0xFFFFF000;
    static constexpr unsigned int num_of_bits_to_indicate_a_word = 32;

    static constexpr unsigned int mask_for_B_type_imm_10_to_5 = 0x7E000000;
    static constexpr unsigned int mask_for_B_type_imm_4_to_1 = 0xF00;
    static constexpr unsigned int mask_for_B_type_imm_11 = 0x080;

    static constexpr unsigned int mask_for_J_type_imm_10_to_1 = 0x7FE00000;
    static constexpr unsigned int mask_for_J_type_imm_11 = 0x00100000;
    static constexpr unsigned int mask_for_J_type_imm_19_to_12 = 0x000FF000;
};

#endif // INSTR_DECODE_H
