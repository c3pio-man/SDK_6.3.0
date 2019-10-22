#ifndef SIMPLEFILESYSTEMMODEL_H
#define SIMPLEFILESYSTEMMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QObject>

#ifdef EMULATOR
    #define DEFAULT_ROOT_FOLDER "./system/mnt"
#else
    #define DEFAULT_ROOT_FOLDER "/mnt"
#endif

#define BAD_FOLDERNAME_SYMBOLS "*!#$%&()@^`{}~'+,;=[]?<:>|\"/"

class SimpleFilesystemModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentFolderPath READ currentFolderPath WRITE setCurrentFolderPath NOTIFY currentFolderPathChanged)
    Q_PROPERTY(QString parentFolderPath READ parentFolderPath NOTIFY parentFolderPathChanged)
    Q_PROPERTY(QString rootFolderPath READ rootFolderPath WRITE setRootFolderPath NOTIFY rootFolderPathChanged)
    Q_PROPERTY(bool showFolders READ showFolders WRITE setShowFolders NOTIFY showFoldersChanged)
    Q_PROPERTY(bool showFiles READ showFiles WRITE setShowFiles NOTIFY showFilesChanged)
    Q_PROPERTY(QString filenameFilter READ filenameFilter WRITE setFilenameFilter NOTIFY filenameFilterChanged)
    Q_PROPERTY(bool currentFolderPathIsRootPath READ currentFolderPathIsRootPath NOTIFY currentFolderPathIsRootPathChanged)
public:

//    static SimpleFilesystemModel& instance();

    explicit SimpleFilesystemModel();

    enum SimpleFilesystemModelRoles {
        NameRole = Qt::UserRole + 1,
        PathRole,
        ParentPathRole,
        IsFolderRole,
    };

    struct FolderItem {
        QString itemName;
        QString itemPath;
        QString itemParentPath;
        bool isFolder;
    };


    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QString currentFolderPath() const;
    QString parentFolderPath() const;
    QString rootFolderPath() const;
    bool showFolders() const;
    bool showFiles() const;
    QString filenameFilter() const;
    bool currentFolderPathIsRootPath() const;

public slots:
    void setCurrentFolderPath(QString currentFolderPath);
    void setRootFolderPath(QString rootFolderPath);
    void setShowFolders(bool showFolders);
    void setShowFiles(bool showFiles);
    void setFilenameFilter(QString filenameFilter);
    QString createNewFolder(QString folder_path, QString folder_name);

signals:
    void currentFolderPathChanged(QString currentFolderPath);
    void parentFolderPathChanged(QString parentFolderPath);
    void rootFolderPathChanged(QString rootFolderPath);
    void showFoldersChanged(bool showFolders);
    void showFilesChanged(bool showFiles);
    void filenameFilterChanged(QString filenameFilter);
    void currentFolderPathIsRootPathChanged(bool currentFolderPathIsRootPath);

private:
    void populateCurrentFolderItems();

    QString current_folder_path_;
    QString parent_folder_path_;
    QString root_folder_path_;
    bool show_folders_;
    bool show_files_;
    QString filename_filter_;
    QVector<FolderItem> current_folder_items_;
};

#endif // SIMPLEFILESYSTEMMODEL_H
