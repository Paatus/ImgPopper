#ifndef IMGPOPPER_ZIPSCANNER_DEFINED
#define IMGPOPPER_ZIPSCANNER_DEFINED

#include <iostream>
#include <string>
#include "Util.hpp"
#include <archive.h>
#include <archive_entry.h>

using namespace std;

class ZipScanner {
    string filepath;
    void scan(string, string);
    int copy_data(struct archive*, struct archive*);
    public:
        ZipScanner(string, string);
};

#endif
