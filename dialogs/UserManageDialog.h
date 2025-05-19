#ifndef USERMANAGEDIALOG_H
#define USERMANAGEDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QStackedWidget>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

class UserManageDialog : public QDialog
{
    Q_OBJECT
public:
    UserManageDialog(QWidget *parent = nullptr);

private:
    void            initDialog();
    void            initUserListView();
    void            initUserEditView();


    QStackedWidget  *stackedWidget;
    QWidget         *userEditWidget;
    QWidget         *userListWidget;
    QLineEdit       *nameEdit;
    QLineEdit       *departmentEdit;
    QLineEdit       *emailEdit;
    QComboBox       *adminCombo;


    QTableView      *userListView;
    QListView       *userInfoView;

    int             userListIndex;
    int             userEditIndex;


private slots:
    void            saveUser();
    void            showUserEditView();
    void            showUserEditViewForAdd();
    void            showUserEditViewForEdit();
};

#endif // USERMANAGEDIALOG_H
