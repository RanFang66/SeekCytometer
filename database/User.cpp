#include "User.h"
User User::m_loginUser = User(1, "Admin", true);

User::User()
:m_id(0), m_name(""), m_isAdmin(false), m_department(""), m_email(""), m_createdDate(QDateTime::currentDateTime())
{
}

