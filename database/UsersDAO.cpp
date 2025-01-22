#include "UsersDAO.h"

UsersDAO::UsersDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool UsersDAO::insertUser(const User &user)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Users (user_name, user_admin, department, email) VALUES (:user_name, :user_admin, :department, :email)");
    query.bindValue(":user_name", user.name());
    query.bindValue(":user_admin", user.isAdmin());
    query.bindValue(":department", user.department());
    query.bindValue(":email", user.email());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool UsersDAO::updateUser(const User &user)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Users SET user_name = :user_name, user_admin = :user_admin, department = :department, email = :email WHERE user_id = :user_id");
    query.bindValue(":user_name", user.name());
    query.bindValue(":user_admin", user.isAdmin());
    query.bindValue(":department", user.department());
    query.bindValue(":email", user.email());
    query.bindValue(":user_id", user.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool UsersDAO::deleteUser(int userId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<User> UsersDAO::fetchUsers() const
{
    QList<User> users;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return users;
    }

    while (query.next()) {
        User user;
        user.setId(query.value("user_id").toInt());
        user.setName(query.value("user_name").toString());
        user.setAdmin(query.value("user_admin").toBool());
        user.setDepartment(query.value("department").toString());
        user.setEmail(query.value("email").toString());
        users.append(user);
    }

    return users;
}

User UsersDAO::fetchUser(int userId) const
{
    User user;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return user;
    }

    if (query.next()) {
        user.setId(query.value("user_id").toInt());
        user.setName(query.value("user_name").toString());
        user.setAdmin(query.value("user_admin").toBool());
        user.setDepartment(query.value("department").toString());
        user.setEmail(query.value("email").toString());
    }

    return user;
}

bool UsersDAO::isUserAdmin(int userId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT user_admin FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    if (query.next()) {
        return query.value("user_admin").toBool();
    }

    return false;
}

bool UsersDAO::isUserAdmin(const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT user_admin FROM Users WHERE user_name = :user_name");
    query.bindValue(":user_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    if (query.next()) {
        return query.value("user_admin").toBool();
    }

    return false;
}

bool UsersDAO::isUserExists(const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users WHERE user_name = :user_name");
    query.bindValue(":user_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool UsersDAO::isUserExists(int userId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Users WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

