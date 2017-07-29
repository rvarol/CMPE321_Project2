#ifndef Page_H
#define Page_H

#include <vector>
#include <string>
#include <iostream>
#include "defs.hpp"

using namespace std;

std::vector<char> getBytes(int n);

class Page {
private:
    class Record {
    private:
        int record_id;
        int field_count;
        string record_name;
        vector<string> fields;
    public:
        Record (vector<char> record_data) {
            parse(record_data);
        }

        Record (int id, vector<string> field_data, string name = "") {
            record_id = id;
            field_count = field_data.size();

            for (auto const& val: field_data) {
                fields.push_back(val);
            }

            record_name = name;
        }

        vector<char> getByteArray() {
            vector<char> byte_array;

            vector<char> record_bytes = getBytes(record_id);
            vector<char> field_count_bytes = getBytes(fields.size());

            byte_array.insert(end(byte_array), begin(record_bytes), end(record_bytes));
            byte_array.insert(end(byte_array), begin(field_count_bytes), end(field_count_bytes));

            for (int i = 0; i < record_name.length(); i++) {
                byte_array.push_back(record_name[i]);
            }

            for (int i = 0; i < MAX_LEN-record_name.length(); i++) {
                byte_array.push_back(0);
            }

            for (int i = 0; i < fields.size(); i++) {
                int null_count = MAX_LEN - fields[i].length();

                for (char& c : fields[i]) {
                    byte_array.push_back(c);
                }

                for (int j = 0; j < null_count; j++) {
                    byte_array.push_back(0);
                }
            }

            int empty_fields = MAX_FIELD - fields.size();
            for (int i = 0; i < empty_fields; i++) {
                for (int j = 0; j < MAX_LEN; j++) {
                    byte_array.push_back(0);
                }
            }

            return byte_array;
        }

        vector<string> getFields() {
            return fields;
        }

        string getName() {
            return record_name;
        }

        int getID() {
            return record_id;
        }

        void parse(vector<char> record_data) {
            record_id = (record_data[0] << 24) | (record_data[1] << 16) | (record_data[2] << 8) | (record_data[3]);
            field_count = (record_data[4] << 24) | (record_data[5] << 16) | (record_data[6] << 8) | (record_data[7]);

            for (int i = 0; i < MAX_LEN; i++) {
                char c = record_data[8+i];

                if (c == 0) break;

                record_name += c;
            }

            for (int i = 1; i < field_count+1; i++) {
                string field_string;

                for (int j = 0; j < MAX_LEN; j++) {
                    char c = record_data[8+(i*MAX_LEN)+j];

                    if (c == 0) break;

                    field_string += c;
                }

                fields.push_back(field_string);
            }
        }

        void update(vector<string> record_data) {
            fields.clear();

            for (string& s: record_data) {
                fields.push_back(s);
            }
        }
    };

    int page_id;
    int record_count;
    vector<Record> records;

public:
    Page();
    Page(int id);
    vector<char> getByteArray();
    bool isAvailable();
    void writeRecord(int id, vector<string> field_values, string name);
    void parse(vector<char> page_data);
    int getRecordCount();
    int getID();
    void printRecords();
    int deleteRecord(string record_name);
    int deleteRecord(int delete_key);
    bool checkRecord(string record_name);
    bool checkRecord(int record_key);
    void updateRecord(int record_key, vector<string> record_data);
    vector<string> getFields(string record_name);
    vector<string> getFields(int record_key);
    vector<vector<string>> getFields();
    vector<string> getNames();
    vector<int> getIDs();
    int findMaxID();
};

#endif