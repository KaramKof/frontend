#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QFileDialog"

#include "fstream"
#include "string"

#include "Set_Assoc_Cache.h"
#include "memory.h"
#include "register_file.h"
#include "program_counter.h"
#include "clock.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //data_cache.print_segment_of_cache(0x0aaa4, 8);

    ui->instructionsTable->setColumnCount(3);
    QStringList name;
    name << "Line No." << "Text Instruction" << "Hex Instruction";
    ui->instructionsTable->setHorizontalHeaderLabels(name);
    ui->instructionsTable->setColumnWidth(0,70);
    ui->instructionsTable->setColumnWidth(1,103);
    ui->instructionsTable->setColumnWidth(2,103);

    ui->tableLabel1->setVisible(false);
    ui->tableLabelBot->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_program1()
{
    array<unsigned int, memory_constants::num_of_words> line_of_data_1 {1, 2, 3, 4};
    array<unsigned int, memory_constants::num_of_words> line_of_data_2 {5, 6, 7, 8};
    array<unsigned int, memory_constants::num_of_words> line_of_data_3 {1, 5, 2, 6};
    array<unsigned int, memory_constants::num_of_words> line_of_data_4 {3, 7, 4, 8};
    memory.write_a_line_as_side_door(0x0aaa0, line_of_data_1);
    memory.write_a_line_as_side_door(0x0aab0, line_of_data_2);

    memory.write_a_line_as_side_door(0x0aac0, line_of_data_1);
    memory.write_a_line_as_side_door(0x0aad0, line_of_data_2);
    //memory.print_segment_of_memory(0x0aaa0, 8);

    array<unsigned int, memory_constants::num_of_words> line_of_instruction1 {0x0000a637, 0x6a060613, 0x40060613, 0x00062903};
    array<unsigned int, memory_constants::num_of_words> line_of_instruction2 {0x0000a6b7, 0x6c068693, 0x40068693, 0x0006a983};
    array<unsigned int, memory_constants::num_of_words> line_of_instruction3 {0x00028293, 0x00430313, 0x0000a06F, 0x00460613};
    array<unsigned int, memory_constants::num_of_words> line_of_instruction4 {0x00468693, 0x00062903, 0x0006a983, 0x033909b3};
    array<unsigned int, memory_constants::num_of_words> line_of_instruction5 {0x01498a33, 0x00128293, 0xFE62C963, 0x0000a737};
    array<unsigned int, memory_constants::num_of_words> line_of_instruction6 {0x70070713, 0x40070713, 0x01472023, 0x00000000};

    vector<array<unsigned int, memory_constants::num_of_words>> segment_of_instructions {line_of_instruction1,
        line_of_instruction2,
        line_of_instruction3,
        line_of_instruction4,
        line_of_instruction5,
        line_of_instruction6, };

    memory.load_data_segment_into_memory(0x08aa0,segment_of_instructions);
    //memory_update();
    program_counter.set_instruction_address_to(0x08aa0);

    update_config();
}

void MainWindow::on_step1Cycle_clicked()
{
    if (!oneWayOrTwoWay) { clock_dm.run_one_cycle(); }
    else                 { clock_sa.run_one_cycle(); }
    update_config();
}

void MainWindow::on_stepNCycles_clicked()
{
    for(int f = 0; f < ui->numCycles->value(); f++){
        if (!oneWayOrTwoWay) { clock_dm.run_one_cycle(); }
        else                 { clock_sa.run_one_cycle(); }
    }
    update_config();
}

void MainWindow::update_config()
{
    if (location == 0) {
        on_registerButton_clicked();
    }
    else if (location == 1) {
        on_cacheButton_clicked();
    }
    else if (location == 2) {
        on_memoryButton_clicked();
    }
    else if (location == 3) {
        on_pipelineButton_clicked();
    }

    if (!oneWayOrTwoWay) {
        ui->cacheHitsLabel->setText("Cache Hits: "+QString::number(data_cache_dm.get_hit_count()));
        ui->cacheMissesLabel->setText("Cache Misses: "+QString::number(data_cache_dm.get_miss_count()));

        ui->cycleLabel->setText("Cycle: "+QString::number(clock_dm.get_cycle_count()));
    }
    else
    {
        ui->cacheHitsLabel->setText("Cache Hits: "+QString::number(data_cache_sa.get_hit_count()));
        ui->cacheMissesLabel->setText("Cache Misses: "+QString::number(data_cache_sa.get_miss_count()));

        ui->cycleLabel->setText("Cycle: "+QString::number(clock_sa.get_cycle_count()));
    }

    std::stringstream stream;
    stream << std::hex << program_counter.get_current_instruction_address();
    std::string pcA(stream.str());
    ui->PCLabel->setText("Program Counter: "+QString::fromStdString("0x"+pcA));
}

void MainWindow::on_registerButton_clicked()
{
    location = 0;

    reset_tab_colors();
    ui->registerButton->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(32);
    ui->tableWidgetBot->setVisible(false);
    ui->tableLabel1->setVisible(true);
    ui->tableLabel1->setText("Registers");
    ui->tableLabelBot->setVisible(false);
    ui->tableWidgetBot->setVisible(false);

    QStringList name;
    name << "Decimal" << "Hexadecimal";
    ui->tableWidget->setHorizontalHeaderLabels(name);
    ui->tableWidget->setColumnWidth(0,61*4);
    ui->tableWidget->setColumnWidth(1,61*4);

    array dataArr = register_file.get_register_file();

    for(int i = 0; i < 32; i++) {
        auto decV = new QTableWidgetItem(QString::number(dataArr[i]));

        std::stringstream stream;
        stream << std::hex << dataArr[i];
        std::string hexadecV1(stream.str());

        auto hexadecV2 = hexadecV1;
        while(hexadecV2.length() < 8) hexadecV2 = "0"+hexadecV2;
        hexadecV2 = "0x"+hexadecV2;

        auto hexadecV = new QTableWidgetItem(QString::fromStdString(hexadecV2));

        ui->tableWidget->setItem(i, 0, decV);
        ui->tableWidget->setItem(i, 1, hexadecV);
    }
}


void MainWindow::on_cacheButton_clicked()
{
    location = 1;

    reset_tab_colors();
    ui->cacheButton->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");

    int numLines = 8;//n_lines;

    ui->tableLabel1->setVisible(true);
    ui->tableLabel1->setText("Data Cache");
    ui->tableLabelBot->setVisible(true);

    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(numLines);
    ui->tableWidgetBot->setColumnCount(7);
    ui->tableWidgetBot->setRowCount(numLines);
    ui->tableWidgetBot->setVisible(true);

    QStringList name;
    name << "Valid" << "Dirty" << "Tag" << "Word 0" << "Word 1" << "Word 2" << "Word 3";
    ui->tableWidget->setHorizontalHeaderLabels(name);
    ui->tableWidgetBot->setHorizontalHeaderLabels(name);

    for(int i = 0; i < 7; i++) {
        ui->tableWidget->setColumnWidth(i,75);
        ui->tableWidgetBot->setColumnWidth(i,75);
    }

    vector<string> cacheData;
    vector<string> cacheInstr;
    if (!oneWayOrTwoWay) {
        cacheData = data_cache_dm.print_segment_of_cache(0x08aa0 , numLines); //NOTE address is hardcoded
        cacheInstr = instr_cache_dm.print_segment_of_cache(0x08aa0 , numLines); //NOTE address is hardcoded
    }
    else {
        cacheData = data_cache_sa.print_segment_of_cache(0x08aa0 , numLines); //NOTE address is hardcoded
        cacheInstr = instr_cache_sa.print_segment_of_cache(0x08aa0 , numLines); //NOTE address is hardcoded
    }

    for(int i = 0; i < numLines*7; i++) {
        auto v1 = new QTableWidgetItem(QString::fromStdString(cacheData[i]));
        ui->tableWidget->setItem(floor(i/7), i%7,v1);

        auto v2 = new QTableWidgetItem(QString::fromStdString(cacheInstr[i]));
        ui->tableWidgetBot->setItem(floor(i/7), i%7,v2);
    }

    if (oneWayOrTwoWay == true) {
        ui->tableWidget->setColumnCount(14);
        ui->tableWidgetBot->setColumnCount(14);
        name << "Valid" << "Dirty" << "Tag" << "Word 0" << "Word 1" << "Word 2" << "Word 3" << "Valid" << "Dirty" << "Tag" << "Word 0" << "Word 1" << "Word 2" << "Word 3";
        ui->tableWidget->setHorizontalHeaderLabels(name);
        ui->tableWidgetBot->setHorizontalHeaderLabels(name);
        for(int i = 7; i < 14; i++) {
            ui->tableWidget->setColumnWidth(i,47);
            ui->tableWidgetBot->setColumnWidth(i,47);
        }

        vector<string> cacheData2 = data_cache_sa.print_segment_of_cache_2_way(0x00008aa0 , numLines); //NOTE address is hardcoded
        vector<string> cacheInstr2 = instr_cache_sa.print_segment_of_cache_2_way(0x00008aa0 , numLines); //NOTE address is hardcoded

        for(int i = 0; i < numLines*7; i++) {
            auto v1 = new QTableWidgetItem(QString::fromStdString(cacheData2[i]));
            ui->tableWidget->setItem(floor(i/7), (i%7)+7,v1);

            auto v2 = new QTableWidgetItem(QString::fromStdString(cacheInstr2[i]));
            ui->tableWidgetBot->setItem(floor(i/7), (i%7)+7,v2);
        }
    }
}


void MainWindow::on_memoryButton_clicked()
{
    /*int numWords = 32;//n_lines;
    string addrStr = "0x0aaa4";
    int addrInt = stoi(addrStr);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(numWords/4);

    ui->tableWidget2->setVisible(false);
    ui->dataCacheLabel->setVisible(false);
    ui->instructionCacheLabel->setVisible(false);

    QStringList name;
    name << "Address" << "Word 0" << "Word 1" << "Word 2" << "Word 3";
    ui->tableWidget->setHorizontalHeaderLabels(name);

    for(int i = 0; i < 5; i++) { ui->tableWidget->setColumnWidth(i,57); }

    vector<unsigned int> memArr = memory.get_memory_segment(addrInt, numWords);
    string arrIns[memArr.size()];
    for(int i = 0; i < memArr.size(); i++) { arrIns[i] = std::to_string(memArr[i]); }

    for(int i = 0; i < numWords; i++) {
        auto addr = new QTableWidgetItem(QString::fromStdString(addrStr));
        auto v = new QTableWidgetItem(QString::fromStdString(arrIns[i]));

        ui->tableWidget->setItem(i,0,addr);
        ui->tableWidget->setItem(floor(i/4),(i%4)+1,v);
    }*/

    memory_update();
}

template <typename T>
string get_value_from_map(map<string, unsigned>& a_map, string key){
    if(a_map.contains(key)){
        return to_string(static_cast<T>(a_map[key]));
    }
    return "N/A";
}

void MainWindow::on_pipelineButton_clicked()
{
    location = 3;

    reset_tab_colors();
    ui->pipelineButton->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");

    ui->tableWidget->setVisible(true);
    ui->tableWidgetBot->setVisible(false);
    ui->tableLabel1->setVisible(false);
    ui->tableLabelBot->setVisible(false);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(18);

    for(int i = 0; i < 5; i++) {
        ui->tableWidget->setColumnWidth(i,147);

        for(int j = 0; j < 18; j++) {
            auto na = new QTableWidgetItem("N/A");
            ui->tableWidget->setItem(j,i,na);
        }

        auto v0 = new QTableWidgetItem("Instruction Name:");
        ui->tableWidget->setItem(0,i,v0);

        auto v2 = new QTableWidgetItem("RS1 Index:");
        ui->tableWidget->setItem(2,i,v2);

        auto v4 = new QTableWidgetItem("RS1 Value:");
        ui->tableWidget->setItem(4,i,v4);

        auto v6 = new QTableWidgetItem("RS2 Index:");
        ui->tableWidget->setItem(6,i,v6);

        auto v8 = new QTableWidgetItem("RS2 Value:");
        ui->tableWidget->setItem(8,i,v8);

        auto v10 = new QTableWidgetItem("RD Index:");
        ui->tableWidget->setItem(10,i,v10);

        auto v12 = new QTableWidgetItem("RD Value:");
        ui->tableWidget->setItem(12,i,v12);

        auto v14 = new QTableWidgetItem("Immediate Value:");
        ui->tableWidget->setItem(14,i,v14);

        auto v16 = new QTableWidgetItem("Program Counter Address:");
        ui->tableWidget->setItem(16,i,v16);
    }
    auto v = new QTableWidgetItem("Program Counter Address:");
    ui->tableWidget->setItem(0,0,v);

    // Execution
    Instruction* instr_at_ex;
    if (!oneWayOrTwoWay) { instr_at_ex = execution_dm.get_next_instr(); }
    else                 { instr_at_ex = execution_sa.get_next_instr(); }
    string name_of_instr_at_ex = instr_at_ex->convert_instr_name_to_string();
    map<string, unsigned int> member_map_of_instr_at_ex = instr_at_ex->get_member_map();
    auto nameEX = new QTableWidgetItem(QString::fromStdString(name_of_instr_at_ex));
    ui->tableWidget->setItem(1,2,nameEX);
    auto rs1IEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rs1_index")));;
    ui->tableWidget->setItem(3,2,rs1IEX);
    auto rs1VEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rs1_value")));;
    ui->tableWidget->setItem(5,2,rs1VEX);
    auto rs2IEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rs2_index")));;
    ui->tableWidget->setItem(7,2,rs2IEX);
    auto rs2VEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rs2_value")));;
    ui->tableWidget->setItem(9,2,rs2VEX);
    auto rdIEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rd_index")));;
    ui->tableWidget->setItem(11,2,rdIEX);
    auto rdVEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_ex, "rd_value")));;
    ui->tableWidget->setItem(13,2,rdVEX);
    auto immEX = new QTableWidgetItem(QString::fromStdString(get_value_from_map<int>(member_map_of_instr_at_ex, "imm")));;
    ui->tableWidget->setItem(15,2,immEX);
    auto pcEX = new QTableWidgetItem(QString::number(member_map_of_instr_at_ex.contains("pc_addr")? member_map_of_instr_at_ex["pc_addr"] : NAN));
    ui->tableWidget->setItem(17,2,pcEX);

    // Memory
    Instruction* instr_at_mem;
    if (!oneWayOrTwoWay) { instr_at_mem = mem_access_dm.get_next_instr(); }
    else                 { instr_at_mem = mem_access_sa.get_next_instr(); }
    string name_of_instr_at_mem = instr_at_mem->convert_instr_name_to_string();
    map<string, unsigned int> member_map_of_instr_at_mem = instr_at_mem->get_member_map();
    auto nameMEM = new QTableWidgetItem(QString::fromStdString(name_of_instr_at_mem));
    ui->tableWidget->setItem(1,3,nameMEM);
    auto rs1IMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rs1_index")));;
    ui->tableWidget->setItem(3,3,rs1IMEM);
    auto rs1VMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rs1_value")));;
    ui->tableWidget->setItem(5,3,rs1VMEM);
    auto rs2IMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rs2_index")));;
    ui->tableWidget->setItem(7,3,rs2IMEM);
    auto rs2VMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rs2_value")));;
    ui->tableWidget->setItem(9,3,rs2VMEM);
    auto rdIMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rd_index")));;
    ui->tableWidget->setItem(11,3,rdIMEM);
    auto rdVMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_mem, "rd_value")));;
    ui->tableWidget->setItem(13,3,rdVMEM);
    auto immMEM = new QTableWidgetItem(QString::fromStdString(get_value_from_map<int>(member_map_of_instr_at_mem, "imm")));;
    ui->tableWidget->setItem(15,3,immMEM);
    auto pcMEM = new QTableWidgetItem(QString::number(member_map_of_instr_at_mem.contains("pc_addr")? member_map_of_instr_at_mem["pc_addr"] : NAN));
    ui->tableWidget->setItem(17,3,pcMEM);

    // Writeback
    Instruction* instr_at_wb;
    if (!oneWayOrTwoWay) { instr_at_wb = write_back_dm.get_saved_instr(); }
    else                 { instr_at_wb = write_back_sa.get_saved_instr(); }
    string name_of_instr_at_wb = instr_at_wb->convert_instr_name_to_string();
    map<string, unsigned int> member_map_of_instr_at_wb = instr_at_mem->get_member_map();
    auto nameWB = new QTableWidgetItem(QString::fromStdString(name_of_instr_at_wb));
    ui->tableWidget->setItem(1,4,nameWB);
    auto rs1IWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rs1_index")));;
    ui->tableWidget->setItem(3,4,rs1IWB);
    auto rs1VWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rs1_value")));;
    ui->tableWidget->setItem(5,4,rs1VWB);
    auto rs2IWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rs2_index")));;
    ui->tableWidget->setItem(7,4,rs2IWB);
    auto rs2VWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rs2_value")));;
    ui->tableWidget->setItem(9,4,rs2VWB);
    auto rdIWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rd_index")));;
    ui->tableWidget->setItem(11,4,rdIWB);
    auto rdVWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_wb, "rd_value")));;
    ui->tableWidget->setItem(13,4,rdVWB);
    auto immWB = new QTableWidgetItem(QString::fromStdString(get_value_from_map<int>(member_map_of_instr_at_wb, "imm")));;
    ui->tableWidget->setItem(15,4,immWB);
    auto pcWB = new QTableWidgetItem(QString::number(member_map_of_instr_at_wb.contains("pc_addr")? member_map_of_instr_at_wb["pc_addr"] : NAN));
    ui->tableWidget->setItem(17,4,pcWB);

    if((!oneWayOrTwoWay && instr_decode_dm.get_on_hold_instr()) || (oneWayOrTwoWay && instr_decode_sa.get_on_hold_instr())){
        Instruction* instr_at_decode;
        if (!oneWayOrTwoWay) { instr_at_decode = instr_decode_dm.get_on_hold_instr(); }
        else                 { instr_at_decode = instr_decode_sa.get_on_hold_instr(); }
        string name_of_instr_at_decode = instr_at_decode->convert_instr_name_to_string();
        map<string, unsigned int> member_map_of_instr_at_decode = instr_at_decode->get_member_map();
        auto nameID = new QTableWidgetItem(QString::fromStdString(name_of_instr_at_decode));
        ui->tableWidget->setItem(1,1,nameID);
        auto rs1IID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rs1_index")));;
        ui->tableWidget->setItem(3,1,rs1IID);
        auto rs1VID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rs1_value")));;
        ui->tableWidget->setItem(5,1,rs1VID);
        auto rs2IID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rs2_index")));;
        ui->tableWidget->setItem(7,1,rs2IID);
        auto rs2VID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rs2_value")));;
        ui->tableWidget->setItem(9,1,rs2VID);
        auto rdIID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rd_index")));;
        ui->tableWidget->setItem(11,1,rdIID);
        auto rdVID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<unsigned int>(member_map_of_instr_at_decode, "rd_value")));;
        ui->tableWidget->setItem(13,1,rdVID);
        auto immID = new QTableWidgetItem(QString::fromStdString(get_value_from_map<int>(member_map_of_instr_at_decode, "imm")));;
        ui->tableWidget->setItem(15,1,immID);
        auto pcID = new QTableWidgetItem(QString::number(member_map_of_instr_at_decode.contains("pc_addr")? member_map_of_instr_at_decode["pc_addr"] : NAN));
        ui->tableWidget->setItem(17,1,pcID);
    }

    QStringList name;
    name << "IF" << "ID" << "EX" << "MEM" << "WB";
    ui->tableWidget->setHorizontalHeaderLabels(name);
}

void MainWindow::on_cache1Or2Way_clicked()
{
    if (oneWayOrTwoWay) {
        oneWayOrTwoWay = false;
        ui->cache1Or2Way->setText("Direct Mapped");
    }
    else {
        oneWayOrTwoWay = true;
        ui->cache1Or2Way->setText("2-Way Set Associative");
    }
    ui->confirm->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");
}

void MainWindow::on_loadInstructions_clicked()
{
    string loadIns;
    string loadStr = "";

    string filename = QFileDialog::getOpenFileName(this,"Open a file", "C://").toStdString();
    // Read from the text file
    ifstream MyReadFile(filename);

    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, loadIns)) {
        // Output the text from the file
        loadStr += loadIns;
        loadStr += "\n";
    }

    vector<array<unsigned int, memory_constants::num_of_words>> vecIns;
    vector<string> vecInsHex;
    int pos = 0;
    while(pos < loadStr.size()){
        pos = loadStr.find("\n");

        string s;
        vector<string> words;
        stringstream ss(loadStr.substr(0,pos));
        while(getline(ss,s,' ')) {
            words.push_back(s);
        }

        array<unsigned int, memory_constants::num_of_words> arrWords = {unsigned(stoi(words[0],0,16)),
                                                                        unsigned(stoi(words[1],0,16)),
                                                                        unsigned(stoi(words[2],0,16)),
                                                                        unsigned(stoi(words[3],0,16))};

        //array<unsigned int, memory_constants::num_of_words> arrWords = {unsigned(stoi(loadStr.substr(0,pos),0,16))};

        vecIns.push_back(arrWords);
        vecInsHex.push_back(loadStr.substr(0,pos));
        loadStr.erase(0,pos+1); // 2 is the length of the delimiter, "\n"
    }

    /*memory.load_data_segment_into_memory(0x08aa0,vecIns);
    memory_update();
    program_counter.set_instruction_address_to(0x08aa0);
    Clock clock(write_back, program_counter);*/

    ui->instructionsTable->setRowCount(vecInsHex.size());

    string arrIns[vecIns.size()];
    string arrInsHex[vecIns.size()];
    for(int i = 0; i < vecIns.size(); i++) {
        arrIns[i].clear();
        for(int j = 0; j < memory_constants::num_of_words; j++) {
            arrIns[i] += std::to_string(vecIns[i][j]);
        }
        arrInsHex[i] = vecInsHex[i];
    }

    for(int i = 0; i < vecIns.size(); i++) {
        auto v2 = new QTableWidgetItem(QString::fromStdString(arrInsHex[i]));
        ui->instructionsTable->setItem(i,1,v2);
    }

    // Close the file
   MyReadFile.close();

}


void MainWindow::on_pipelineEnabled_clicked()
{
    if (pipelineOn) {
        pipelineOn = false;
        ui->pipelineEnabled->setText("Pipeline Enabled [  ]");
    }
    else {
        pipelineOn = true;
        ui->pipelineEnabled->setText("Pipeline Enabled [x]");
    }

    ui->confirm->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");
}


void MainWindow::on_cacheEnabled_clicked()
{
    if (cacheEnabled) {
        cacheEnabled = false;
        ui->cacheEnabled->setText("Cache Enabled [  ]");
    }
    else {
        cacheEnabled = true;
        ui->cacheEnabled->setText("Cache Enabled [x]");
    }
    ui->confirm->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");
}

void MainWindow::reset_tab_colors(){
    ui->registerButton->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");
    ui->cacheButton->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");
    ui->memoryButton->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");
    ui->pipelineButton->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");
}

void MainWindow::memory_update(){
    location = 2;

    reset_tab_colors();
    ui->memoryButton->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);""");

    int addrr = 0x08aa0;

    ui->tableWidget->setColumnCount(3);
    QStringList name;
    name << "Memory Address" << "Text Instruction" << "Hex Instruction";
    ui->tableWidget->setHorizontalHeaderLabels(name);

    ui->tableWidget->setColumnWidth(0,110);
    ui->tableWidget->setColumnWidth(1,220);
    ui->tableWidget->setColumnWidth(2,360);
    ui->tableWidget->setRowCount(0);
    ui->tableLabel1->setVisible(false);
    ui->tableLabelBot->setVisible(false);
    ui->tableWidgetBot->setVisible(false);

    vector<unsigned int> mem = memory.get_memory_segment(addrr,6*4*4);

    vector<string> vecInsHex;
    for(int i = 0; i < mem.size(); i++){
        std::stringstream stream;
        stream << std::hex << mem[i];
        std::string result(stream.str());

        vecInsHex.push_back(result);
    }
    ui->tableWidget->setRowCount(vecInsHex.size()/memory_constants::num_of_words);
    for(int i = 0; i < vecInsHex.size(); i++) {

        string str = "";
        string tempAppend = vecInsHex[i];
        while(tempAppend.length() < 8) tempAppend = "0"+tempAppend;
        tempAppend = "0x"+tempAppend;
        str += tempAppend;

        auto v = new QTableWidgetItem(QString::fromStdString(str));
        ui->tableWidget->setItem(i,2,v);

        std::stringstream stream;
        stream << std::hex << (stoi(std::to_string(addrr+4*i),0,10));
        std::string addrVStr(stream.str());
        //while(addrVStr.length() < 8) addrVStr = "0"+addrVStr;
        addrVStr = "0x"+addrVStr;

        auto addrV = new QTableWidgetItem(QString::fromStdString(addrVStr));
        ui->tableWidget->setItem(i,0,addrV);
    }

    return;
}

void MainWindow::on_stepToEnd_clicked()
{
    int k = 0;
    Instruction* an_instr = nullptr;
    do{
        k++;
        if(an_instr) {
            delete an_instr;
            an_instr = nullptr;
        }
        if (!oneWayOrTwoWay) { an_instr = clock_dm.run_one_cycle(); }
        else                 { an_instr = clock_sa.run_one_cycle(); }
    } while(((an_instr->get_instr_name()) != Instr_Name::HALT) && k < 10000);

    update_config();
}

void MainWindow::on_resetState_clicked()
{
    ui->confirm->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");

    ui->comboBox->setCurrentIndex(0);
    oneWayOrTwoWayLast = oneWayOrTwoWay;
    register_file.reset_register_file();
    memory.reset();
    data_cache_dm.reset();
    data_cache_sa.reset();
    instr_cache_dm.reset();
    instr_cache_sa.reset();
    clock_dm.reset();
    clock_sa.reset();
    program_counter.reset_instruction_address();

    update_config();
}


void MainWindow::on_confirm_clicked()
{
    ui->confirm->setStyleSheet("""background-color: rgb(43, 43, 43);color: rgb(255, 255, 255);""");

    int loadP = ui->comboBox->currentIndex();
    on_resetState_clicked();

    switch(loadP) {
    case 1:
        load_program1();
        break;
    }

    if (pipelineOn) {
        if (!oneWayOrTwoWay) { instr_fetch_dm.set_pipeline_enabled(); }
        else { instr_fetch_sa.set_pipeline_enabled(); }
    }
    else {
        if (!oneWayOrTwoWay) { instr_fetch_dm.set_pipeline_disabled(); }
        else { instr_fetch_sa.set_pipeline_disabled(); }
    }

    if (cacheEnabled) {
        if (!oneWayOrTwoWay) {
            data_cache_dm.set_cache_enabled();
            instr_cache_dm.set_cache_enabled();
        }
        else
        {
            data_cache_sa.set_cache_enabled();
            instr_cache_sa.set_cache_enabled();
        }
    }
    else {
        if (!oneWayOrTwoWay) {
            data_cache_dm.set_cache_disabled();
            instr_cache_dm.set_cache_disabled();
        }
        else
        {
            data_cache_sa.set_cache_disabled();
            instr_cache_sa.set_cache_disabled();
        }
    }
}


void MainWindow::on_comboBox_activated(int index)
{
    if (index != 0) { ui->confirm->setStyleSheet("""background-color: #e3e3e3;color: rgb(25,25,25);"""); };
}

