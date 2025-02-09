#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent=nullptr);


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initLoginDialog();

    void readSettings();

    void writeSettings();

    QString encryptPassword(const QString &password);

    QLineEdit *editUserName;
    QLineEdit *editPassword;
    QCheckBox *cBoxSaveUser;

    bool m_moving;
    QPoint m_dragPosition;


private slots:
    void onLoginButtonClicked();
};

#endif // LOGINDIALOG_H
