#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Execution.h"
#include "Instr_Decode.h"
#include "Instr_Fetch.h"
#include "Write_Back.h"
#include "memory.h"
#include "Set_Assoc_Cache.h"
#include "Direct_Mapped_Cache.h"
#include "register_file.h"
#include "program_counter.h"
#include "clock.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool cacheEnabled = true;
    bool oneWayOrTwoWay = false; //false = 1-way, true = 2-way
    bool oneWayOrTwoWayLast = false;
    bool pipelineOn = true;
    int location = 2; //0 = registers, 1 = cache, 2 = memory, 3 = pipeline

private slots:
    void on_step1Cycle_clicked();

    void on_stepNCycles_clicked();

    void on_registerButton_clicked();

    void on_cacheButton_clicked();

    void on_cache1Or2Way_clicked();

    void on_memoryButton_clicked();

    void on_loadInstructions_clicked();

    void on_pipelineEnabled_clicked();

    void on_cacheEnabled_clicked();

    void memory_update();

    void load_program1();

    void reset_tab_colors();

    void on_pipelineButton_clicked();

    void update_config();

    void on_stepToEnd_clicked();

    void on_resetState_clicked();

    void on_confirm_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;

public:
    unsigned int delay = 2;
    Memory memory = delay;

    DirectMappedCache data_cache_dm{memory,"data_cache",1,true};
    DirectMappedCache instr_cache_dm{memory,"instruction_cache",1,true};
    RegisterFile register_file;
    ProgramCounter program_counter;
    InstrFetch instr_fetch_dm{program_counter, instr_cache_dm, true};
    InstrDecode instr_decode_dm{register_file, instr_fetch_dm};
    Execution execution_dm{program_counter, instr_decode_dm};
    MemAccess mem_access_dm{data_cache_dm, execution_dm};
    WriteBack write_back_dm{register_file, mem_access_dm, instr_fetch_dm, true};
    Clock clock_dm{write_back_dm, program_counter};

    SetAssocCache data_cache_sa{memory,"data_cache",1,true};
    SetAssocCache instr_cache_sa{memory,"instruction_cache",1,true};
    InstrFetch instr_fetch_sa{program_counter, instr_cache_sa, true};
    InstrDecode instr_decode_sa{register_file, instr_fetch_sa};
    Execution execution_sa{program_counter, instr_decode_sa};
    MemAccess mem_access_sa{data_cache_sa, execution_sa};
    WriteBack write_back_sa{register_file, mem_access_sa, instr_fetch_sa, true};
    Clock clock_sa{write_back_sa, program_counter};
};
#endif // MAINWINDOW_H
