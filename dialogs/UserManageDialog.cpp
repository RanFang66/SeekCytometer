#include "UserManageDialog.h"
#include "UsersDAO.h"
#include <QSqlTableModel>
#include <QSqlTableModel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>



UserManageDialog::UserManageDialog(QWidget *parent)
    :QDialog{parent}
{
    initDialog();
}

void UserManageDialog::initDialog()
{
    stackedWidget = new QStackedWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
    initUserListView();
    initUserEditView();
}

void UserManageDialog::initUserListView()
{
    userListWidget = new QWidget(this);
    userListIndex = stackedWidget->addWidget(userListWidget);

    userListView = new QTableView(userListWidget);
    QSqlTableModel *model = new QSqlTableModel(this, UsersDAO().database());

    if (!model) {
        qDebug() << "Failed to create QSqlTableModel";
    }
    model->setTable("users");
    bool ok = model->select();
    if (!ok) {
        qDebug() << "Failed to select data from Users table";
        qDebug() << model->lastError().text();
    }
    userListView->setModel(model);
    userListView->setSelectionBehavior(QAbstractItemView::SelectRows);
    userListView->setSelectionMode(QAbstractItemView::SingleSelection);
    userListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    userListView->setSortingEnabled(true);

    // hide user id
    userListView->hideColumn(0);
    // hide password
    userListView->hideColumn(model->fieldIndex("user_password"));

    userListView->resizeColumnsToContents();
    userListView->horizontalHeader()->setStretchLastSection(true);
    userListView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userListView->verticalHeader()->hide();
    userListView->setAlternatingRowColors(true);
    userListView->setShowGrid(false);
    userListView->update();
    stackedWidget->setCurrentIndex(userListIndex);

    QVBoxLayout *userListLayout = new QVBoxLayout(userListWidget);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton(tr("Add"), userListWidget);
    QPushButton *editButton = new QPushButton(tr("Edit"), userListWidget);
    QPushButton *deleteButton = new QPushButton(tr("Delete"), userListWidget);
    btnLayout->addWidget(addButton);
    btnLayout->addWidget(editButton);
    btnLayout->addWidget(deleteButton);
    userListLayout->addLayout(btnLayout);
    userListLayout->addWidget(userListView);

    connect(addButton, &QPushButton::clicked, this, &UserManageDialog::showUserEditViewForAdd);
    connect(editButton, &QPushButton::clicked, this, &UserManageDialog::showUserEditViewForEdit);
    userListWidget->setLayout(userListLayout);
}

void UserManageDialog::initUserEditView()
{
    userEditWidget = new QWidget(this);
    userEditIndex = stackedWidget->addWidget(userEditWidget);


    QFormLayout *formLayout = new QFormLayout();
    nameEdit = new QLineEdit(userEditWidget);
    departmentEdit = new QLineEdit(userEditWidget);
    emailEdit = new QLineEdit(userEditWidget);
    adminCombo = new QComboBox(userEditWidget);
    adminCombo->addItem("Admin");
    adminCombo->addItem("Normal");

    formLayout->addRow(new QLabel(tr("Name")), nameEdit);
    formLayout->addRow(new QLabel(tr("Department")), departmentEdit);
    formLayout->addRow(new QLabel(tr("Email")), emailEdit);
    formLayout->addRow(new QLabel(tr("Admin")), adminCombo);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton(tr("Ok"), userEditWidget);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), userEditWidget);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *userEditLayout = new QVBoxLayout(userEditWidget);
    userEditLayout->addLayout(formLayout);
    userEditLayout->addLayout(buttonLayout);
    userEditWidget->setLayout(userEditLayout);

    connect(saveButton, &QPushButton::clicked, this, &UserManageDialog::saveUser);
    connect(cancelButton, &QPushButton::clicked, this, [this](){stackedWidget->setCurrentIndex(userListIndex);});
}


void UserManageDialog::saveUser()
{
    qDebug() << "UserManageDialog::saveUser()";
    stackedWidget->setCurrentIndex(userListIndex);
}

void UserManageDialog::showUserEditView()
{

}

void UserManageDialog::showUserEditViewForAdd()
{
    stackedWidget->setCurrentIndex(userEditIndex);
    nameEdit->clear();
    departmentEdit->clear();
    emailEdit->clear();
    adminCombo->setCurrentIndex(1);
}

void UserManageDialog::showUserEditViewForEdit()
{
    stackedWidget->setCurrentIndex(userEditIndex);
    QModelIndex index = userListView->currentIndex();
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(userListView->model());
    if (!model) {
        qDebug() << "Failed to cast model to QSqlTableModel";
    }
    nameEdit->setText(model->data(model->index(index.row(), model->fieldIndex("user_name"))).toString());
    departmentEdit->setText(model->data(model->index(index.row(), model->fieldIndex("user_department"))).toString());
    emailEdit->setText(model->data(model->index(index.row(), model->fieldIndex("user_email"))).toString());
    QString admin = model->data(model->index(index.row(), model->fieldIndex("user_admin"))) == true ? "Admin" : "Normal";
    adminCombo->setCurrentText(admin);
}






