#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    explicit User();
    static void setLoginUser(const User &user) {
        m_loginUser = user;
    };
    static const User &loginUser() {
        return m_loginUser;
    }

    User(int id, const QString &name, bool admin, const QString &department="", const QString &email="", const QDateTime &createdDate = QDateTime::currentDateTime())
        : m_id(id), m_name(name), m_isAdmin(admin), m_department(department), m_email(email), m_createdDate(createdDate)
    {}


    int id() const { return m_id; }
    QString name() const { return m_name; }
    bool isAdmin() const { return m_isAdmin; }
    QString department() const { return m_department; }
    QString email() const { return m_email; }
    const QDateTime &createdDate() const { return m_createdDate; }


    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }
    void setAdmin(bool admin) { m_isAdmin = admin; }
    void setDepartment(const QString &department) { m_department = department; }
    void setEmail(const QString &email) { m_email = email; }
    void setCreatedDate(const QDateTime &createdDate) { m_createdDate = createdDate; }


private:
    static User m_loginUser;

    int m_id;
    QString m_name;
    bool m_isAdmin;
    QString m_department;
    QString m_email;
    QDateTime m_createdDate;
};

#endif // USER_H
