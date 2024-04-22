#include "Write_Back.h"

WriteBack::WriteBack(RegisterFile& register_file, MemAccess& mem_access, InstrFetch& instr_fetch, bool pipeline_enabled)
    : register_file(register_file), mem_access(mem_access), instr_fetch(instr_fetch), saved_instr(new STALL()),
    pipeline_enabled(pipeline_enabled){
    // Constructor body
}

WriteBack::~WriteBack() {
    // Destructor body
}

void WriteBack::reset() {
    if(saved_instr) {
        delete saved_instr;
        saved_instr = new STALL();
    }
    mem_access.reset();
}

Instruction* WriteBack::get_saved_instr(){
    return saved_instr;
}

void WriteBack::set_pipeline_disabled(){
    pipeline_enabled = false;
}

void WriteBack::set_pipeline_enabled(){
    pipeline_enabled = true;
}

Instruction* WriteBack::proceed(string signal){
    Instruction* an_instr_object = saved_instr;
    Instr_Type instr_type = an_instr_object->get_instr_type();
    /*instr_types in this set do not need to write register rd's value to register_file at this stage*/
    set<Instr_Type> set_of_instr_types {Instr_Type::STALL,
        Instr_Type::NO_OP,
        Instr_Type::HALT,
        Instr_Type::B,
        Instr_Type::S,
        Instr_Type::JAL, // register rd is already written duing Instr_Decode stage.
        Instr_Type::I_JALR, // register rd is already written duing Instr_Decode stage.
    };

    if(!set_of_instr_types.contains(instr_type)){
        unsigned int rd_index = an_instr_object->get_rd_index();
        unsigned int rd_value = an_instr_object->get_rd_value();
        register_file.write_a_word_by_reg_index(rd_index, rd_value);
        register_file.remove_register_from_pending_write(rd_index);
    }
    saved_instr = mem_access.proceed(signal);

    if(!pipeline_enabled && instr_type != Instr_Type::STALL) instr_fetch.turn_on_switch();

    return an_instr_object;
}
