#ifndef NOTESITEM_H
#define NOTESITEM_H

#include <QObject>
#include <QQuickItem>

#include <string>

#include <common_utilities/reader_db_manager.h>

namespace pocketbook {

namespace content {

class NoteType : public QQuickItem
{
    Q_OBJECT
public:
    enum EnNoteType {
        UNKNOWN,
        MARKER,
        PEN,
        COMMENT,
        SNAPSHOT
    };
    Q_ENUMS(EnNoteType)
};

class NotesItem
{
public:
    NotesItem();

    void setBookmark(pocketbook::utilities::Note bmk);
    pocketbook::utilities::Note getBookmark() const;

    std::string getTitle() const;

    void setComment(const std::string &comment);
    std::string getComment() const;

    NoteType::EnNoteType getType() const;
    std::string getStartPage() const;

    int getBookPageNumber() const;

    void setWrapped(bool value);
    bool getWrapped() const;

    void setWrapEnable(bool value);
    bool getWrapEnable() const;

    int getIndex() const;
    std::string getSnapshot() const;

private:
    pocketbook::utilities::Note bookmark_;

    std::string comment_;
    bool wrapped_ = true;
    bool wrap_enable_ = true;

    NoteType::EnNoteType type_ = NoteType::UNKNOWN;
};

}
}

#endif // NOTESITEM_H
