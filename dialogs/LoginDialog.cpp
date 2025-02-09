#include "LoginDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSettings>
#include <QCryptographicHash>

#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include "UsersDAO.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), m_moving(false)
{
    initLoginDialog();
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        m_moving = true;
    }
    return QDialog::mousePressEvent(event);
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_moving && (event->buttons() & Qt::LeftButton)
        && ((event->globalPosition().toPoint()-m_dragPosition).manhattanLength() > QApplication::startDragDistance())) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }
    return QDialog::mouseMoveEvent(event);
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_moving = false;
    }
    return QDialog::mouseReleaseEvent(event);
}


void LoginDialog::initLoginDialog()
{
    editUserName = new QLineEdit("user", this);
    editPassword = new QLineEdit("", this);
    editPassword->setEchoMode(QLineEdit::Password);
    cBoxSaveUser = new QCheckBox(tr("Save Password"), this);
    QPushButton *btnConfirm = new QPushButton(tr("Login"), this);
    QPushButton *btnCancel = new QPushButton(tr("Exit"), this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(tr("User Name: "), editUserName);
    formLayout->addRow(tr("Password: "), editPassword);
    formLayout->addRow(cBoxSaveUser);
    formLayout->addRow(btnConfirm, btnCancel);
    setLayout(formLayout);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::SplashScreen);

    readSettings();

    connect(btnConfirm, &QPushButton::clicked, this, &LoginDialog::onLoginButtonClicked);
    connect(btnCancel, &QPushButton::clicked, this, &LoginDialog::reject);
}

void LoginDialog::readSettings()
{
    QString organization = "SeekGene";
    QString appName = "SeekCytometer";
    QSettings settings(organization, appName);
    settings.beginGroup("LoginDialog");
    QString userName = settings.value("username", "user").toString();
    QString defaultPassword = QString("123456");
    QString password = settings.value("password", defaultPassword).toString();
    bool savePassword = settings.value("savePassword", false).toBool();

    if (savePassword) {
        editUserName->setText(userName);
        editPassword->setText(password);
        cBoxSaveUser->setChecked(true);
    } else {
        editUserName->setText(userName);
        editPassword->setText("");
        cBoxSaveUser->setChecked(false);
    }

    settings.endGroup();

}

void LoginDialog::writeSettings()
{
    QString organization = "SeekGene";
    QString appName = "SeekCytometer";
    QSettings settings(organization, appName);
    settings.beginGroup("LoginDialog");
    settings.setValue("username", editUserName->text());
    if (cBoxSaveUser->isChecked()) {
        settings.setValue("password", editPassword->text());
        settings.setValue("savePassword", true);
    } else {
        settings.setValue("password", "");
        settings.setValue("savePassword", false);
    }
    settings.endGroup();
}

QString LoginDialog::encryptPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

void LoginDialog::onLoginButtonClicked()
{
    UsersDAO usersDAO;
    int id = usersDAO.checkUserPassword(editUserName->text(), encryptPassword(editPassword->text()));
    if (id > 0) {
        writeSettings();
        User::setLoginUser(usersDAO.fetchUser(id));
        accept();
    } else {
        QMessageBox::warning(this, tr("Login Failed"), tr("Invalid User Name or Password!"));
        editPassword->clear();
    }
}
