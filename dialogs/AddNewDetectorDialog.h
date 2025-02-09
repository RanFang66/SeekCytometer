#ifndef ADDNEWDETECTORDIALOG_H
#define ADDNEWDETECTORDIALOG_H

#include <QDialog>
#include <DetectorModel.h>
#include <QItemSelectionModel>

class AddNewDetectorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddNewDetectorDialog(QWidget *parent = nullptr);

    QList<Detector> getSelectedDetectors();

private:
    void initDialog();
    DetectorModel *m_model;

    QItemSelectionModel *m_selection;

};

#endif // ADDNEWDETECTORDIALOG_H
