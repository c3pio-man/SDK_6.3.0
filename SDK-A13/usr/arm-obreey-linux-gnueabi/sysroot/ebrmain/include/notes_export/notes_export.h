#ifndef NOTES_EXPORT_H
#define NOTES_EXPORT_H

#include <string>

namespace pocketbook {

class NotesExporterImpl;
class NotesExporter {
public:
    NotesExporter();
    ~NotesExporter();

    void setExportDir(const std::string& export_dir);
    int export_bookmarks_to_html(const std::string& fast_hash);

    /**
     * @brief import_from_file import from html with embedded binary or from binary dump file
     * @param filename
     * @return number of imported items or negative number on error
     */
    int import_from_file(const std::string& filename);
private:
    NotesExporterImpl* impl_;
};

}
#endif // NOTES_EXPORT_H
