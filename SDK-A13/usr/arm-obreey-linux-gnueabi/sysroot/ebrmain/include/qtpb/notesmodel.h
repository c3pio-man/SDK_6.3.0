#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>
#include <QObject>

#include <deque>
#include <string>
#include <vector>

#include <common_utilities/reader_db_manager.h>

#include "notesitem.h"
#include "resourceimageprovider.h"

namespace pocketbook {

namespace content {

class NotesModel: public QAbstractListModel, public pocketbook::utils::IImageProvider
{
    Q_OBJECT
    Q_PROPERTY(bool canCollapseAll READ canCollapseAll NOTIFY canCollapseAllChanged)

private:
    explicit NotesModel(QObject *parent = 0);

    std::vector<NotesItem> notes_;

    void collapseExpand(bool collapse);

    int current_item_;
    QString book_fast_hash_;

    pocketbook::utilities::ReaderDbManager reader_db_manager_;

public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    enum NotesRoles {
        TitleRole = Qt::UserRole + 1,
        PageNumberRole,
        NoteTypeRole,
        NoteCommentRole,
        NoteWrappedRole,
        NoteSnapshotIndexRole,
        NoteDateRole,
    };

    static NotesModel &Instance();
    ~NotesModel();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    bool canCollapseAll() const;

signals:

    void canCollapseAllChanged();
    void positionSelected(QString position);

public slots:

    void openNote(int index);

    int getSelectedNoteType(int index);

    QString getSelectedNoteComment(int index);
    void setSelectedNoteComment(int index, QString comment);

    QString getSelectedNoteTitle(int index);
    void setSelectedNoteTitle(int index, QString title);

    QString getHighlightedRichText(QString text, QString highlight);

    void deleteNote(int index);

    void collapseAll();
    void expandAll();

    void loadNotesList();
    void loadFilteredNotesList(QString filter);

    void selectBook(QString bookFastHash, QString searchQuery);

    bool hasComment(int index);

};

}
}

#endif // NOTESMODEL_H
