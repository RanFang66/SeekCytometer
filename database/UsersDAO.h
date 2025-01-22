#ifndef USERSDAO_H
#define USERSDAO_H

#include "BaseDAO.h"
#include "User.h"

// CREATE TABLE Users (
//     user_id          SERIAL PRIMARY KEY NOT NULL,
//     user_name        VARCHAR(32) NOT NULL UNIQUE,
//     user_admin       BOOLEAN NOT NULL,
//     department       VARCHAR(32),
//     email            VARCHAR(64)
//     );


class UsersDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit UsersDAO(QObject *parent = nullptr);

    bool insertUser(const User &user);
    bool updateUser(const User &user);
    bool deleteUser(int userId);
    QList<User> fetchUsers() const;
    User fetchUser(int userId) const;
    bool isUserAdmin(int userId) const;
    bool isUserAdmin(const QString &name) const;
    bool isUserExists(const QString &name) const;
    bool isUserExists(int userId) const;
};

#endif // USERSDAO_H
