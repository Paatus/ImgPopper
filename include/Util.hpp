#ifndef IMGPOPPER_UTIL_DEFINED
#define IMGPOPPER_UTIL_DEFINED

#include <iostream>
#include <poppler/glib/poppler.h>
#include <string>
#include <sys/stat.h>

using namespace std;

class Util {
    public:
        enum FILES {FILE_PDF = 1, FILE_DOCX = 2, FILE_PPTX = 3, FILE_DIR = 4};
        static bool dir_compare(string, string);
        static bool is_bigger_than(cairo_surface_t*, int, int);
        static bool is_dir(string);
        static bool is_file(string);
        static int get_filetype(string);
        static string get_basename(string);
        static string get_ending(string);
        static string strip_slash(string);
        static void create_dir(string);
        static void show_usage(string);
        static void show_version();
};

#endif
