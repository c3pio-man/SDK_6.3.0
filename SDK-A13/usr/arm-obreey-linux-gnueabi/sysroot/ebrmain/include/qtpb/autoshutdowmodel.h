#ifndef AUTOSHUTDOWMODEL_H
#define AUTOSHUTDOWMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>

class AutoShutdownModel : public QAbstractListModel
{
    Q_OBJECT
public:

    Q_PROPERTY(bool enableSpecials READ enableSpecials WRITE setEnableSpecials NOTIFY enableSpecialsChanged)

    AutoShutdownModel();

    enum AutoShutdownModelRoles {
        NameRole = Qt::DisplayRole,
        IntervalRole = Qt::UserRole + 1,
        IsSpecialRole,
        IsEnabledRole,
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool enableSpecials() const;
    static void shutdownTimerHandler(void *context);

public slots:
    void setShutdownInterval(int index);
    void clearShutdownInterval();
    void setSpecialShutdownInterval(int index, int seconds);
    void setEnableSpecials(bool enableSpecials);
    void emitShutdownTimeout();

signals:
    void enableSpecialsChanged(bool enableSpecials);
    void shutdownTimeout();

private:
    QList<int> shutdown_intervals_list_;

    void loadIntervals();

    static const QMap<int, QString> special_intervals_names_;
    static const QMap<int, QString> FillSpecialIntervalsNames();

    bool IsSpecial(int index);
    QString GetReadableName(const int index) const;
    int GetHours(const int minutes) const;
    int GetMinutesForLastHour(const int minutes) const;

    bool enable_specials_ = false;
};

#endif // AUTOSHUTDOWMODEL_H
