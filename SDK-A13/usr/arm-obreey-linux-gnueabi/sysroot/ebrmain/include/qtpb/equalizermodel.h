#ifndef EQUALIZERMODEL_H
#define EQUALIZERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector>
#include <QHash>

class EqualizerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QString presetName READ presetName WRITE setPresetName NOTIFY presetNameChanged)
    Q_PROPERTY(QStringList presetsList READ presetsList NOTIFY presetsListChanged)
    Q_PROPERTY(bool isPresetModified READ isPresetModified NOTIFY isPresetModifiedChanged)

    explicit EqualizerModel(QObject *parent = 0);

    enum EqualizerModelRoles {
        NameRole = Qt::DisplayRole,
        GainRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString presetName() const;
    QStringList presetsList() const;

    bool isPresetModified() const;

public slots:
    void setPreset(int index);
    void savePreset(QString presetName);
    void removePreset(int index);
    void setPresetName(QString presetName);
    void setPresetItemValue(int index, int value);
    QString getPresetNameByIndex(int index);
    void resetPreset();
    bool isPredefinedPreset(QString presetName);
    bool isPresetExists(QString presetName);
    QVector<int> getCurrentPreset();
    QVector<int> getPresetItemsNames();

signals:
    void presetNameChanged(QString presetName);
    void presetsListChanged();
    void isPresetModifiedChanged(bool isPresetModified);
    void presetChanged();

private:
    void fillPredefinedPresets();
    QString getPresetItemName(const int index) const;
    void loadPresetsFromFile(QString file_path);
    void savePresetsToFile(QString file_path);

    QHash<QString, QVector<int> > predefined_presets_;
    QHash<QString, QVector<int> > presets_;
    QString preset_name_;
    QString selected_preset_name_;
    QVector<int> current_preset_;
    QVector<int> preset_items_names_;
    QStringList presets_list_;
    bool is_current_preset_modified_ = false;
};

#endif // EQUALIZERMODEL_H
