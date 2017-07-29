#include "fileManager.hpp"

#include <iostream>

FileManager::FileManager() {}

FileManager::FileManager(string name) {
    open(name);
}

void FileManager::open(string name) {
    filename = name;
    file_p.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    file_p.seekg(0, ios_base::end);

    if (file_p.tellg() == 0) {
        createNewPage();
    }

    file_p.close();
    file_p.open(filename, std::fstream::in | std::fstream::out);

    findPageCount();

    findNewPageID();
    findNewRecordID();
}

void FileManager::findPageCount() {
    file_p.seekg(0, ios_base::end);
    page_count = file_p.tellg() / PAGE_SIZE;
    file_p.seekg(0, ios_base::beg);
}

void FileManager::findNewPageID() {
    new_page_id = 0;

    for (int i = 0; i < page_count; i++) {
        readNextPage();
        if (current_page.getID() >= new_page_id) {
            new_page_id = current_page.getID() + 1;
        }
    }
}

void FileManager::findNewRecordID() {
    new_record_id = 0;

    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        int max_id = current_page.findMaxID();
        if (max_id >= new_record_id) {
            new_record_id = max_id + 1;
        }
    }
}

void FileManager::readNextPage() {
    char* buffer = new char[PAGE_SIZE];

    last_page_location = file_p.tellg();

    file_p.read(buffer, PAGE_SIZE);

    vector<char> page_data;

    for (int i = 0; i < PAGE_SIZE; i++) {
        page_data.push_back(buffer[i]);
    }

    current_page.parse(page_data);

    delete[] buffer;
}

void FileManager::createNewPage() {
    ofstream file_app(filename, std::fstream::in | std::fstream::app);

    Page new_page(new_page_id);
    new_page_id++;

    file_app.seekp(0, ios_base::end);

    vector<char> page_data = new_page.getByteArray();
    unsigned char* buffer = new unsigned char[page_data.size()];

    for (int i = 0; i < page_data.size(); i++) {
        buffer[i] = (unsigned char)page_data[i];
    }

    file_app.write((const char*)buffer, page_data.size());

    file_app.flush();

    delete[] buffer;

    file_app.close();

    findPageCount();
}

void FileManager::findEmptyPage() {
    file_p.seekg(0, ios_base::beg);
    readNextPage();

    for (int i = 1; i < page_count && !current_page.isAvailable(); i++) {
        readNextPage();
    }

    if (!current_page.isAvailable()) {
        createNewPage();
        readNextPage();
    }
}

void FileManager::writeRecord(vector<string> field_values, string name = string()) {
    findEmptyPage();

    current_page.writeRecord(new_record_id, field_values, name);
    new_record_id++;

    writePage();
}

void FileManager::printRecords() {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        current_page.printRecords();
    }
}

int FileManager::deleteRecord(string record_name) {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        if (current_page.deleteRecord(record_name) == 0) {
            writePage();

            return 0;
        }
    }

    return -1;
}

int FileManager::deleteRecord(int delete_key) {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        if (current_page.deleteRecord(delete_key) == 0) {
            writePage();

            return 0;
        }
    }

    return -1;
}

vector<string> FileManager::getFields(string type_name) {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        if (current_page.checkRecord(type_name)) {
            return current_page.getFields(type_name);
        }
    }
}

vector<string> FileManager::getNames() {
    vector<string> names;

    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        vector<string> current_names = current_page.getNames();
        names.insert(begin(names), begin(current_names), end(current_names));
    }

    return names;
}

void FileManager::updateRecord(int update_key, vector<string> record_data) {
    vector<string> names;

    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        if (current_page.checkRecord(update_key)) {
            current_page.updateRecord(update_key, record_data);

            writePage();

            return;
        }
    }
}

void FileManager::displayRecord(int search_key, vector<string> field_names) {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        if (current_page.checkRecord(search_key)) {
            vector<string> field_values = current_page.getFields(search_key);

            for (int i = 0; i < field_values.size(); i++) {
                cout << field_names[i] << ": " << field_values[i] << endl;
            }
        }
    }
}

void FileManager::displayRecords(vector<string> field_names) {
    file_p.seekg(0, ios_base::beg);

    for (int i = 0; i < page_count; i++) {
        readNextPage();

        vector<int> field_ids = current_page.getIDs();
        vector<vector<string>> field_values = current_page.getFields();

        for (int j = 0; j < field_values.size(); j++) {
            cout << "ID: " << field_ids[j] << endl;
            for (int k = 0; k < field_values[j].size(); k++) {
                cout << field_names[k] << ": " << field_values[j][k] << endl;
            }
            cout << endl;
        }
    }
}

void FileManager::writePage() {
    file_p.seekp(last_page_location, ios_base::beg);

    vector<char> page_data = current_page.getByteArray();
    unsigned char* buffer = new unsigned char[page_data.size()];

    for (int i = 0; i < page_data.size(); i++) {
        buffer[i] = (unsigned char)page_data[i];
    }

    file_p.write((const char*)buffer, page_data.size());

    file_p.flush();

    return;
}