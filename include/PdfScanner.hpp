#ifndef IMGPOPPER_PDFSCANNER_DEFINED
#define IMGPOPPER_PDFSCANNER_DEFINED

#include <iostream>
#include <string>
#include <glib.h>
#include <poppler/glib/poppler.h>
#include "Util.hpp"

using namespace std;

class PdfScanner {
    string filepath;
    GFile* file_pointer;
    void scan(string);
    void save_image(cairo_surface_t*, string, string);
    public:
        PdfScanner(string, string);
};

#endif
