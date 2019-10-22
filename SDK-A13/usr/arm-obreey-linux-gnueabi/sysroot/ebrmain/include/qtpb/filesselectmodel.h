#ifndef FILESSELECTMODEL_H
#define FILESSELECTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QHash>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QList>
#include <QSet>

class FilesSelectModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QUrl rootFolder READ rootFolder WRITE setRootFolder NOTIFY rootFolderChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

    enum FilesSelectModelRoles {
        NameRole = Qt::UserRole + 1,
        FilePathRole,
        IsFolderRole,
        IsExpandedRole,
        IsSelectedRole,
        IsPartiallySelectedRole,
        IsLastSubItemRole,
        LevelRole,
        MarkersRole
    };

    struct FileItem {
        QString name;
        QString file_path;
        bool is_folder;
        bool is_expanded;
        bool is_selected;
        bool is_partially_selected;
        bool is_last_subitem;
        unsigned int level;
        QList<bool> markers;
    };

    explicit FilesSelectModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString filter() const;
    QUrl rootFolder() const;

public slots:
    void setFilter(QString filter);
    void setRootFolder(QUrl rootFolder);
    void select(unsigned int index, bool value);
    void expand(unsigned int index);
    void collapseAll();
    void clearSelectedFiles();
    QStringList getSelectedFiles();

signals:
    void filterChanged(QString filter);
    void rootFolderChanged(QUrl rootFolder);

private:
    void addSubItems(unsigned int index);
    void removeSubItems(unsigned int index);

    unsigned int getSelectedSubItemsCount(unsigned int index, unsigned int &subItemsCount, bool &has_partially_selected);
    unsigned int findParent(unsigned int index);
    void updateParents(unsigned int index);
    void updateSiblings(unsigned int index);
    void selectFolder(unsigned int index, bool value);
    void selectFile(unsigned int index, bool value);

    QList<FileItem> files_;
    QSet<QString> selected_;
    QString filter_;
    QUrl root_folder_;

};

#endif // FILESSELECTMODEL_H
