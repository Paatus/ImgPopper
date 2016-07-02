#include "Util.hpp"

void Util::show_usage(string name)
{
    cout << "Usage: "<< name <<" [options] file" << endl
         << endl
         << "Available options:" << endl
         << "   -h  --help      Show this message." << endl
         << "   -v  --version   Show version." << endl
         << "   -d  --directory Specify a folder to save the images in." << endl
         << "   -D  --debug     Enables log messages." << endl
         << endl
         << "Example: " << name << " pdf.pdf -d output-dir" << endl;
}

void Util::show_version()
{
    cout << "ImgPopper v. 1.0" << endl
         << "Developed by Tommy Molin & Pontus Hjortskog" << endl;
}

string Util::strip_slash(string str)
{
    size_t pos = str.find_last_of('/');
    if(pos != string::npos)
    {
        return str.substr(0,pos);
    }
    return str;
}

bool Util::is_dir(string path)
{
    struct stat statbuf;
    if(stat(path.c_str(), &statbuf) == 0)
    {
        return statbuf.st_mode & S_IFDIR;
    }
    return false;
}

bool Util::is_file(string path)
{
    struct stat statbuf;
    if(stat(path.c_str(), &statbuf) == 0)
    {
        return statbuf.st_mode & S_IFREG;
    }
    return false;
}

int Util::get_filetype(string path) {
    // Add for other filetypes
    int dot_location = path.find_last_of(".");
    if(Util::is_dir(path))
    {
        return Util::FILE_DIR;
    }
    if(dot_location != string::npos)
    {
        if(path.substr(dot_location) == ".pdf" && Util::is_file(path))
        {
            return Util::FILE_PDF;
        }
        else if(path.substr(dot_location) == ".docx" && Util::is_file(path))
        {
            return Util::FILE_DOCX;
        }
        else if(path.substr(dot_location) == ".pptx" && Util::is_file(path))
        {
            return Util::FILE_PPTX;
        }
    }
    return -1;
}

bool Util::dir_compare(string arg1, string arg2)
{
    //string arg1 = *a1;
    //string arg2 = *a2;
    string arg1_body = arg1.substr(0, arg1.find('.'));
    string arg2_body = arg2.substr(0, arg2.find('.'));
    int arg1_nr = stoi(arg1.substr(0, arg1.find('_')), nullptr);
    int arg2_nr = stoi(arg2.substr(0, arg2.find('_')), nullptr);
    if(arg1_nr != arg2_nr)
    {
        return arg1_nr < arg2_nr;
    }
    else {
        int arg1_nr2 = stoi(arg1_body.substr(arg1_body.rfind('_')+1), nullptr);
        int arg2_nr2 = stoi(arg2_body.substr(arg2_body.rfind('_')+1), nullptr);
        return arg1_nr2 < arg2_nr2;
    }
}

string Util::get_basename(string path)
{
    int pos = path.find_last_of('/');
    if(pos == string::npos)
    {
        return path;
    }
    else
    {
        return path.substr(pos+1);
    }

}

bool Util::is_bigger_than(cairo_surface_t* image, int width, int height)
{
    int w = cairo_image_surface_get_width(image);
    int h = cairo_image_surface_get_height(image);
    return (w >= width && h >= height);
}

void Util::create_dir(string dir_name)
{
    if(!Util::is_dir(dir_name))
        mkdir(dir_name.c_str(), 0700);
}

string Util::get_ending(string file)
{
    return file.substr(file.rfind('.'));
}
