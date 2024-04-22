#ifndef WRITE_BACK_H
#define WRITE_BACK_H

#include <set>
#include "Mem_Access.h"


using std::set;

class WriteBack {
public:
    WriteBack(RegisterFile& register_file, MemAccess& mem_access, InstrFetch& instr_fetch, bool pipeline_enabled = true);  // Default constructor
    ~WriteBack(); // Default destructor

    void reset();

    void set_pipeline_disabled();

    void set_pipeline_enabled();

    Instruction* proceed(string signal);

    Instruction* get_saved_instr();

private:
    RegisterFile& register_file;
    MemAccess& mem_access;
    InstrFetch& instr_fetch;
    // the instruction returned from Execution, could be a STALL
    Instruction* saved_instr;
    bool pipeline_enabled;
};

#endif // WRITE_BACK_H
