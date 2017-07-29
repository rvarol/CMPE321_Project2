#include "cliManager.hpp"

#include <iostream>

using namespace std;

/* Retrieve an integer from he command line (helper function) */
int getInteger() {
    string input = "";
    int number = 0;

    while (true) {
        getline(cin, input);

        stringstream myStream(input);
        if (myStream >> number)
            break;
        cout << "Invalid number, please enter a valid number: ";
    }

    return number;
}

void CliManager::initialize() {
    type_names = catalogue.getNames();

    showMainMenu();
}

CliManager::CliManager() {
    catalogue.open("Data/catalogue.cat");
}

/* Display main menu options to the user */
void CliManager::showMainMenu() {
    // Variable to hold the option for user operation option
    int option = -1;


    while (true) {
        // Show user the main menu
        cout << endl;
        cout << "############ MAIN MENU ############" << endl;
        cout << "## 1: Create new type            ##" << endl;
        cout << "## 2: Delete a type              ##" << endl;
        cout << "## 3: List all types             ##" << endl;
        cout << "## 4: Create a record            ##" << endl;
        cout << "## 5: Delete a record            ##" << endl;
        cout << "## 6: Update a record            ##" << endl;
        cout << "## 7: Search for a record        ##" << endl;
        cout << "## 8: List all records of a type ##" << endl;
        cout << "## 0: Quit                       ##" << endl;
        cout << "###################################" << endl;

        // Prompt for an option
        cout << "Please enter the operation you would like to perform: ";
        option = getInteger();

        switch (option) {
        case 1: // Create a new type
            // Prompt the user for parameters of the new type to be created
            this->promptCreateType();
            break;
        case 2: // Delete a type
            // Prompt the user for the type name of the type to be deleted
            this->promptDeleteType();
            break;
        case 3: // List all types
        {
            catalogue.printRecords();

            break;
        }
        case 4: // Create a record
            this->promptCreateRecord();
            break;
        case 5: // Delete a record
            this->promptDeleteRecord();
            break;
        case 6: // Update a record
            this->promptUpdateRecord();
            break;
        case 7: // Search for a record
            this->promptSearchRecord();
            break;
        case 8: // List all records of a type
            this->promptListRecords();
            break;
        case 0: // Quit
            return;
        }
    }
}

/* Prompt the user for new type parameters */
void CliManager::promptCreateType() {
    string type_name, fieldName;
    int fieldCount;
    vector<string> fields;

    cout << endl << "### Creating a new type ###" << endl;

    // Prompt for type name
    cout << "Please enter the name of the new type (max " << MAX_LEN << " characters): ";
    getline(cin, type_name);


    // Prompt for field count
    cout << "Please enter the number of fields (max " << MAX_FIELD << " fields): ";
    fieldCount = getInteger();

    // Prompt for field names
    for (int i = 0; i < fieldCount; i++) {
        cout << "Please enter the field name for field no " << i+1 << " (max " << MAX_LEN << " characters): ";
        getline(cin, fieldName);

        fields.push_back(fieldName);
    }

    catalogue.writeRecord(fields, type_name);

    type_names.push_back(type_name);
}

/* Prompt the user for a type name to be deleted */
void CliManager::promptDeleteType() {
    string typeName;

    cout << endl << "### Deleting an existing type ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name to be deleted: ";
    getline(cin, typeName);

    catalogue.deleteRecord(typeName);
}

/* Prompt the user from the new record information */
void CliManager::promptCreateRecord() {
    string typeName;

    cout << endl << "### Creating a new record ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name for which a record is to be created: ";
    getline(cin, typeName);

    if (!checkType(typeName)) {
        cout << "No type with that name!" << endl;
        return;
    }

    // Prompt for field values
    cout << "Please enter values for the given fields." << endl;

    vector<string> rData;
    vector<string> fNames = catalogue.getFields(typeName);

    for (int i = 0; i < fNames.size(); i++) {
        if (fNames[i] != "") {
            string _input;
            cout << fNames[i] << ": ";
            getline(cin, _input);

            rData.push_back(_input);
        }
    }
    
    if (std::find(type_names.begin(), type_names.end(), typeName) != type_names.end()) {
        FileManager fm("Data/" + typeName + ".dat");

        fm.writeRecord(rData, "");
    }
}

void CliManager::promptDeleteRecord() {
    string typeName;

    cout << endl << "### Deleting a record ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name for which a record is to be deleted: ";
    getline(cin, typeName);

    if (!checkType(typeName)) {
        cout << "No type with that name!" << endl;
        return;
    }

    cout << "Please enter the primary key value of the record to be deleted: ";
    int delete_key = getInteger();

    if (std::find(type_names.begin(), type_names.end(), typeName) != type_names.end()) {
        FileManager fm("Data/" + typeName + ".dat");

        fm.deleteRecord(delete_key);
    }
}

/* Prompt the user from the new record information */
void CliManager::promptUpdateRecord() {
    string typeName;

    cout << endl << "### Updating a record ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name for which a record is to be updated: ";
    getline(cin, typeName);

    if (!checkType(typeName)) {
        cout << "No type with that name!" << endl;
        return;
    }

    cout << "Please enter the primary key value of the record to be updated: ";
    int updateKey = getInteger();

    // Prompt for field values
    cout << "Please enter values for the given fields." << endl;
    vector<string> rData;
    vector<string> fNames = catalogue.getFields(typeName);

    for (int i = 0; i < fNames.size(); i++) {
        if (fNames[i] != "") {
            string _input;
            cout << fNames[i] << ": ";
            getline(cin, _input);

            rData.push_back(_input);
        }
    }

    if (std::find(type_names.begin(), type_names.end(), typeName) != type_names.end()) {
        FileManager fm("Data/" + typeName + ".dat");
        
        fm.updateRecord(updateKey, rData);
    }
}

void CliManager::promptSearchRecord() {
    string typeName;

    cout << endl << "### Searching for a record ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name a record is to be searched: ";
    getline(cin, typeName);

    if (!checkType(typeName)) {
        cout << "No type with that name!" << endl;
        return;
    }

    cout << "Please enter the primary key value of the record you are seaching for: ";
    int searchKey = getInteger();

    if (std::find(type_names.begin(), type_names.end(), typeName) != type_names.end()) {
        FileManager fm("Data/" + typeName + ".dat");
        
        fm.displayRecord(searchKey, catalogue.getFields(typeName));
    }
}

void CliManager::promptListRecords() {
    string typeName;

    cout << endl << "### Listing records of a type ###" << endl;

    // Prompt for type name
    cout << "Please enter the type name for which the records are to be listed: ";
    getline(cin, typeName);

    if (!checkType(typeName)) {
        cout << "No type with that name!" << endl;
        return;
    }

    if (std::find(type_names.begin(), type_names.end(), typeName) != type_names.end()) {
        FileManager fm("Data/" + typeName + ".dat");

        vector<string> fields = catalogue.getFields(typeName);

        fm.displayRecords(fields);
    }
}

bool CliManager::checkType(string type_name) {
    if (std::find(type_names.begin(), type_names.end(), type_name) != type_names.end()) {
        return true;
    }
    return false;
}