#include "PdfScanner.hpp"

PdfScanner::PdfScanner(string in_path, string out_path)
{
    this->filepath = in_path;
    this->file_pointer = g_file_new_for_commandline_arg(in_path.c_str());
    if(this->file_pointer != NULL)
    {
        this->scan(out_path);
    }
    g_object_unref(this->file_pointer);
}

void PdfScanner::scan(string output_dir)
{
    GError* open_error = NULL;
    PopplerDocument* document_pointer = poppler_document_new_from_gfile(this->file_pointer, NULL, NULL, &open_error);
    if(document_pointer == NULL)
    {
        cout << "Error: Could not open file <" << this->filepath << ">" << endl;
        g_object_unref(document_pointer);
        return;
    }
    // get amount of pages in document
    int pages = poppler_document_get_n_pages(document_pointer);
    // get the file name
    string file_name = Util::get_basename(this->filepath);
    for(int p = 0; p < pages; p++)
    {
        PopplerPage* page_pointer = poppler_document_get_page(document_pointer, p);
        // if page exists, should always be true
        if(page_pointer)
        {
            GList* image_list = poppler_page_get_image_mapping(page_pointer);
            // if there is a list
            if(image_list)
            {
                int images = g_list_length(image_list);
                //cout << images << " images on page " << p << endl;
                for(int i = 0; i < images; i++)
                {
                    if(i > 20)
                    {
                        break;
                    }
                    PopplerImageMapping* img = (PopplerImageMapping*) g_list_nth(image_list, i)->data;
                    int image_id = img->image_id;
                    cairo_surface_t* img_surface = poppler_page_get_image(page_pointer, image_id);
                    string output_name = file_name + "_page_" + to_string(p) + "_image_" + to_string(image_id+1) + ".png";
                    this->save_image(img_surface, output_dir, output_name);
                    cairo_surface_destroy(img_surface);
                }
            }
            poppler_page_free_image_mapping(image_list);
        }
        g_object_unref(page_pointer);
    }
    // free document_object
    g_object_unref(document_pointer);
}

void PdfScanner::save_image(cairo_surface_t* image, string out_dir, string filename)
{
    //cout << "Saving " << filename << " to " << out_dir << endl;
    if(image)
    {
        if(!Util::is_bigger_than(image, 200,200))
        {
            return;
        }
        // create dir if it does not exist
        Util::create_dir(out_dir);
        string out = out_dir+"/"+filename;
        cairo_surface_write_to_png(image, out.c_str());
    }
}
