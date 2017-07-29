#include "page.hpp"
#include <iostream>

std::vector<char> getBytes(int n) {
    std::vector<char> byte_array(4);
    
    /* Convert the integer to byte array */
    for (int i = 0; i < 4; i++) {
        byte_array[3-i] = (n >> (i*8));
    }

    /* Return the byte array */
    return byte_array;
}

Page::Page() {
    page_id = 0;
    record_count = 0;
}

Page::Page(int id) {
    page_id = id;
    record_count = 0;
}

bool Page::isAvailable() {
    return record_count < MAX_FIELD;
}

vector<char> Page::getByteArray() {
    vector<char> byte_array;

    vector<char> id_bytes = getBytes(page_id);
    vector<char> record_count_bytes = getBytes(record_count);

    byte_array.insert(end(byte_array), begin(id_bytes), end(id_bytes));
    byte_array.insert(end(byte_array), begin(record_count_bytes), end(record_count_bytes));

    for (int i = 0; i < records.size(); i++) {
        vector<char> record_bytes = records[i].getByteArray();

        byte_array.insert(end(byte_array), begin(record_bytes), end(record_bytes));
    }

    int empty_records = MAX_RECORD - records.size();
    for (int i = 0; i < empty_records; i++) {
        for (int j = 0; j < RECORD_SIZE; j++) {
            byte_array.push_back(0);
        }
    }

    return byte_array;
}

void Page::writeRecord(int id, vector<string> field_values, string name = string()) {
    if (isAvailable()) {
        Record new_record(id, field_values, name);

        records.push_back(new_record);

        record_count++;
    }
}

void Page::parse(vector<char> page_data) {
    records.clear();

    page_id = (page_data[0] << 24) | (page_data[1] << 16) | (page_data[2] << 8) | (page_data[3]);
    record_count = (page_data[4] << 24) | (page_data[5] << 16) | (page_data[6] << 8) | (page_data[7]);

    for (int i = 0; i < record_count; i++) {
        vector<char> record_data;

        for (int j = 8+(i*RECORD_SIZE); j < 8+((i+1)*RECORD_SIZE); j++) {
            record_data.push_back(page_data[j]);
        }

        Record new_record(record_data);
        records.push_back(new_record);
    }
}

int Page::getRecordCount() {
    return record_count;
}

int Page::getID() {
    return page_id;
}

void Page::printRecords() {
    for (int i = 0; i < records.size(); i++) {
        cout << "NAME: " << records[i].getName() << endl;
        cout << "ID: " << records[i].getID() << endl;
        cout << "FIELDS: " << endl;
        vector<string> fields = records[i].getFields();
        for (string& s: fields) {
            cout << "- " << s << endl;
        }
        cout << endl;
    }
}

int Page::deleteRecord(string record_name) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getName() == record_name) {
            records.erase(records.begin() + i);
            record_count--;
            return 0;
        }
    }

    return -1;
}

bool Page::checkRecord(string record_name) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getName() == record_name)
            return true;
    }

    return false;
}

bool Page::checkRecord(int record_key) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getID() == record_key)
            return true;
    }

    return false;
}

void Page::updateRecord(int record_key, vector<string> record_data) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getID() == record_key) {
            records[i].update(record_data);

            return;
        }
    }    
}

vector<string> Page::getFields(string record_name) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getName() == record_name)
            return records[i].getFields();
    }    
}

vector<string> Page::getFields(int record_key) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getID() == record_key)
            return records[i].getFields();
    }    
}

vector<vector<string>> Page::getFields() {
    vector<vector<string>> fields;

    for (int i = 0; i < records.size(); i++) {
        fields.push_back(records[i].getFields());
    }

    return fields;
}

vector<int> Page::getIDs() {
    vector<int> ids;

    for (int i = 0; i < records.size(); i++) {
        ids.push_back(records[i].getID());
    }

    return ids;
}

vector<string> Page::getNames() {
    vector<string> names;

    for (int i = 0; i < records.size(); i++) {
        names.push_back(records[i].getName());
    }

    return names;
}

int Page::deleteRecord(int delete_key) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getID() == delete_key) {
            records.erase(records.begin() + i);
            record_count--;
            return 0;
        }
    }

    return -1;
}

int Page::findMaxID() {
    int max_id = 0;

    for (int i = 0; i < records.size(); i++) {
        if (records[i].getID() > max_id) {
            max_id = records[i].getID();
        }
    }

    return max_id;
}