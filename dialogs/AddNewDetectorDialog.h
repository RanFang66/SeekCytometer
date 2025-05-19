#ifndef ADDNEWDETECTORDIALOG_H
#define ADDNEWDETECTORDIALOG_H

#include <QDialog>
#include <DetectorModel.h>
#include <QItemSelectionModel>

class AddNewDetectorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddNewDetectorDialog(int settingId, QWidget *parent = nullptr);

    QList<Detector> getSelectedDetectors();

private:
    void initDialog();
    int m_settingId;
    DetectorModel *m_model;

    QItemSelectionModel *m_selection;

};

#endif // ADDNEWDETECTORDIALOG_H
