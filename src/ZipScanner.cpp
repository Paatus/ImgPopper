#include "ZipScanner.hpp"

ZipScanner::ZipScanner(string in_path, string out_path)
{
    this->scan(in_path, out_path);
}

void ZipScanner::scan(string in_path, string out_dir)
{
    struct archive* a;
    struct archive* ext;
    struct archive_entry* entry;
    int r, flags;
    string basename = Util::get_basename(in_path);

    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    r = archive_read_open_filename(a, in_path.c_str(), 10240);

    if (r != ARCHIVE_OK)
    {
        cout << "ERRNO " << archive_errno(a) << archive_error_string(a) << endl;
        return;
    }

    int img_num = 0;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        string path = archive_entry_pathname(entry);
        //cout << path << endl;
        if(path.find("media") != string::npos && path.find("image") != string::npos)
        {
            //cout << Util::get_basename(path) << " is an image" << endl;
            string out_name = out_dir + "/" + basename + "_image_" + to_string(img_num) + Util::get_ending(path);
            //cout << out_name << endl;
            archive_entry_set_pathname(entry,out_name.c_str());
            r = archive_write_header(ext, entry);
            r = this->copy_data(a, ext);
            r = archive_write_finish_entry(ext);
            img_num++;
        }
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

}

int ZipScanner::copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
    int64_t offset;
#else
    off_t offset;
#endif

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r != ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r != ARCHIVE_OK) {
            printf("archive_write_data_block() %s", archive_error_string(aw));
            return (r);
        }
    }
}

