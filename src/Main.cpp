#include <iostream>
#include <string>
#include <poppler/glib/poppler.h>
#include "Util.hpp"
#include <glib.h>
#include <vector>
#include <algorithm>
#include "PdfScanner.hpp"
#include "ZipScanner.hpp"
#include <signal.h>
#include <fstream>

using namespace std;

class ImgPopper
{
    bool _DEBUG;
    public:
        ImgPopper(string, string, int, int);
        void set_debug(bool debug){_DEBUG = debug;};
};

ImgPopper::ImgPopper(string input_path, string output_path, int input_type, int last_id)
{
    // START STUFF
    if(input_type == Util::FILE_DIR)
    {
        GError* dir_error = NULL;
        GDir* dir_pointer = g_dir_open(input_path.c_str(), 0, &dir_error);
        const char* file = NULL;
        vector<string> files;
        while((file = g_dir_read_name(dir_pointer)) != NULL)
        {
            string s = file;
            files.push_back(s);
        }
        g_dir_close(dir_pointer);
        // sort in "human" order
        sort(files.begin(), files.end(), Util::dir_compare);
        for(int f = 0; f < files.size(); f++)
        {
            int file_id = Util::get_file_id(files[f]);
            if(file_id >= last_id)
            {
                fstream id_file;
                id_file.open("current_id", ios::out | ios::trunc);
                id_file << Util::get_file_id(files[f]) << endl;
                id_file.close();
                string full_path = input_path + "/" + files[f];
                int TYPE = Util::get_filetype(full_path);
                if(TYPE == Util::FILE_PDF)
                {
                    cout << "Scanning " << full_path << endl;
                    PdfScanner(full_path, output_path);
                }
                else if(TYPE == Util::FILE_DOCX || TYPE == Util::FILE_PPTX)
                {
                    cout << "Scanning " << full_path << endl;
                    ZipScanner(full_path, output_path);
                }
            }
        }
        //cout << "Directory " << input_path << " has " << files.size() << " files." << endl;
    }
    else
    {
        cout << "Input file: " << input_path << endl;
        int TYPE = Util::get_filetype(input_path);
        if(TYPE == Util::FILE_PDF)
        {
            cout << "Scanning " << input_path << endl;
            PdfScanner(input_path, output_path);
        }
        else if(TYPE == Util::FILE_DOCX || TYPE == Util::FILE_PPTX)
        {
            cout << "Scanning " << input_path << endl;
            ZipScanner(input_path, output_path);
        }
    }
}

int main(int argc, char* argv[])
{
    int FILETYPE = 0;
    string source_path;
    string output_path = ".";
    bool debug = false;
    // If there are arguments supplied
    if(argc > 1)
    {
        int output_pos = -1;
        for(int i = 1; i < argc; i++)
        {
            bool arg_found = false;
            string arg = argv[i];
            // HELP
            if(arg == "-h" || arg == "--help")
            {
                Util::show_usage(argv[0]);
                return 0;
            }
            // VERSION
            if(arg == "-v" || arg == "--version")
            {
                Util::show_version();
                return 0;
            }
            // DEBUG
            if(arg == "-D" || arg == "--debug")
            {
                debug = true;
                arg_found = true;
            }
            // OUTPUT_PATH
            if(arg == "-d" || arg == "--directory")
            {
                output_pos = i+1;
                if(argc > output_pos)
                {
                    output_path = Util::strip_slash(argv[output_pos]);
                    arg_found = true;
                }
                else
                {
                    cerr << "Error: No output directory supplied." << endl;
                    Util::show_usage(argv[0]);
                    return 0;
                }
            }
            // input directory
            int f_type = Util::get_filetype(arg);
            if(f_type && i != output_pos && !arg_found)
            {
                // directory
                FILETYPE = f_type;
                source_path = arg;
            }
        }
        if(!FILETYPE)
        {
            cerr << "Error: No file supplied or invalid file" << endl;
            Util::show_usage(argv[0]);
            return 0;
        }
        else
        {
            fstream id_file;
            id_file.open("current_id", ios::in);
            string curr;
            id_file >> curr;
            id_file.close();
            //ImgPopper(source_path, output_path, FILETYPE, stoi(curr));
            ImgPopper(source_path, output_path, FILETYPE, 1);
            //cout << "Source path: '" << source_path << "'" << endl
            //     << "Output path: '" << output_path << "'" << endl;
        }
    }
    else
    {
        Util::show_usage(argv[0]);
        return 0;
    }
}
