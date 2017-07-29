#ifndef FileManager_H
#define FileManager_H

#include <vector>
#include <string>
#include <fstream>
#include "page.hpp"
#include "defs.hpp"

class FileManager {
private:
    /* PRIVATE FIELDS */
    /* Last read page data */
    Page current_page;

    /* Name of the file associated with the file manager */
    string filename;

    /* File pointer to the associated file */
    std::fstream file_p;

    /* Location of the last page read */
    int last_page_location = 0;

    /* ID to be given to the next page */
    int new_page_id = 0;

    /* ID to be given to the next record */
    int new_record_id = 0;

    /* Current number of pages */
    int page_count = 0;

    /* PRIVATE METHODS */
    /* Create a new page and append it to the file */
    void createNewPage();

    /* Read a page from the current position */
    void readNextPage();

    /* Iterate through the pages and find a page with an empty record */
    void findEmptyPage();

    /* Find the id to be given to the next page */
    void findNewPageID();

    /* Find the id to be given to the next record */
    void findNewRecordID();

    /* Calculate the current number of pages */
    void findPageCount();

    /* Write current page to the file */
    void writePage();

public:
    /* PUBLIC METHODS */
    /* Empty constructor, no file is associated */
    FileManager();

    /* Constructor with the filename to be associated */
    FileManager(string filename);

    /* Associate the given file with this file manager */
    void open(string name);

    /* Find and empty record and write the given record data */
    void writeRecord(vector<string> field_values, string name);

    /* Print the current records */
    void printRecords();

    /* Delete the record with the given name */
    int deleteRecord(string record_name);

    /* Delete the record with the given id */
    int deleteRecord(int delete_key);

    /* Display the record with the given key (along with field names) */
    void displayRecord(int search_key, vector<string> field_names);

    /* Display all records (along with field names) */
    void displayRecords(vector<string> field_names);

    /* Update the record data of the record with the given id */
    void updateRecord(int update_key, vector<string> record_data);

    /* Get fields of a record with the given name */
    vector<string> getFields(string type_name);

    /* Get names of all current records */
    vector<string> getNames();
};

#endif