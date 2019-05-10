#ifndef READER_DB_MANAGER_H
#define READER_DB_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "common_utilities/pb_sqlite.h"
#include <set>

namespace pocketbook {
namespace utilities {


class Note {
public:
    enum Tags {
        kInvalid = -1,
        kBookFastHash = 0, // string
        kUUID = 1, // string
        kBookmark = 101, // json
        kType = 102, // string
        kSubtype = 103, // string
        kQuotation = 104, // json
        kNote = 105,   // json
        kColor = 106,  // string
        kIcon = 107,    // string
        kVoice = 108,   // binary
        kImage = 109,   // binary
        kDraws = 110,   // string
    };

    enum TagsTypes {
        kString,
        kJson,
        kBinary,
    };

    static TagsTypes getTagType(Tags tag);
    std::string getTag(Tags tag) const;
    time_t getTagTimeStamp(Tags tag) const;
    void setTag(Tags tag, const std::string& value, time_t timestamp);

    const std::map<Tags, std::pair<std::string, time_t> >& tags() const { return values_; }

    std::string getPrimaryType()const;
    int64_t     getTime()       const;
    std::string getText()       const; // get note quotation text
    std::string getNote()       const; // get note comment text
    std::string getImage()      const;
    std::string getIcon()       const;
    std::string getDraws()      const;
    std::string getColor()      const;
    std::string getStartUri()   const;
    std::string getEndUri()     const;
    std::string getAnchorUri()  const;
    std::string getType()       const;

    int64_t getId() const;
    void setId(int64_t id);

    std::string getJsonTagElement(Tags tag, const std::string& element) const;

    void setText(const std::string& text, int64_t timestamp);

    // set note comment text
    void setNote(const std::string& comment, int64_t timestamp);
    time_t timestamp_;
    bool is_deleted_ = false;

private:
    std::map<Tags, std::pair<std::string, time_t> > values_;
    int64_t id_ = 0;
};

struct Book {
    int64_t uid_;
    std::string title_;
    std::string author_;
    std::string filename_;
    std::string fast_hash_;
};

class ReaderDbManager
{
public:
    ReaderDbManager();

    static std::string toUpperHex(const std::string& binary);
    std::vector<std::string> getChangedNotes(time_t timestamp);
    std::vector<std::pair<std::string, uint64_t> > getBookNotes(const std::string& fast_hash);
    std::vector<std::pair<std::string, uint64_t> > getBookNotesFilteredByText(const std::string& fast_hash, const std::string& text);
    Note getNote(const std::string& uuid);
    int setNote(const Note& note);
    int removeNote(const std::string& note_uuid);
    std::string getBookFastHashById(sqlite_int64 id);
    sqlite_int64 getItemId(const std::string& uuid);
    int addBook(const std::string& title, const std::string& authors, const std::string& fast_hash, time_t time_added);
    std::vector<Book> getBookByFastHash(const std::string& fast_hash);
    std::vector<Book> getBooksWhichHaveNotes();
    std::vector<std::pair<Book, int64_t>> getBooksWithNotesCount();
    std::vector<std::pair<Book, int64_t>> getBooksWithNotesCountFilteredByText(const std::string& text);
    std::set<std::string> findNotesByText(const std::string& text);
    void setTimeAlt(const std::string& uuid, time_t time);
private:
    pocketbook::utilities::Sqlite sqlite_;
};

}
}
#endif // READER_DB_MANAGER_H
