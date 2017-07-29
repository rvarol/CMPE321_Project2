#ifndef CliManager_H
#define CliManager_H

#include <sstream>
#include <algorithm>
#include "fileManager.hpp"
#include "defs.hpp"

class CliManager {
private:
    vector<string> type_names;

public:
    FileManager catalogue;

    CliManager(); 

    void initialize();
    void showMainMenu();
    void promptCreateType();
    void promptDeleteType();
    void promptCreateRecord();
    void promptDeleteRecord();
    void promptUpdateRecord();
    void promptSearchRecord();
    void promptListRecords();
    bool checkType(string type_name);
};

#endif