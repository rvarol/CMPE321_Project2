#ifndef Defs_H
#define Defs_H

#define MAX_LEN 30
#define MAX_RECORD 10
#define MAX_FIELD 10

#define INITIAL_SIZE 300

#define RECORD_SIZE (8+MAX_LEN+MAX_LEN*MAX_FIELD)
#define PAGE_SIZE (8+RECORD_SIZE*MAX_RECORD)

#endif