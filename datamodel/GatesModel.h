#ifndef GATESMODEL_H
#define GATESMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include "GatesDAO.h"
#include <QMutex>


class GatesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum GateColumn {
        GateIDColumn = 0,
        WorkSheetIdColumn,
        GateNameColumn,
        GateTypeColumn,
        XAxisColumn,
        YAxisColumn,
        GatePointsColumn,
        GateColumnCount,
    };
    static GatesModel *instance() {
        static GatesModel instance;
        return &instance;
    }
    GatesModel(const GatesModel &) = delete;
    GatesModel &operator=(const GatesModel &) = delete;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;

    // Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void resetGateModel(int worksheetId);
    int addGate(const Gate &gate);
    bool removeGate(int row);
    bool removeGate(const Gate &gate);
    void updateGate(int row, const Gate &gate);

    const Gate getGate(int row) const;
    const QList<Gate> &getGateList() const;

private:
    explicit GatesModel(QObject *parent = nullptr);


    QList<Gate> m_gateList;
    GatesDAO gatesDao;
    const QStringList m_headerData = {"ID", "WorkSheetID", "Name", "Type", "X Axis", "Y Axis", "Points"};

    QMutex m_mutex;
};

#endif // GATESMODEL_H
