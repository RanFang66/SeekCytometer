#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QString>
#include <QDateTime>

class Experiment
{
public:
    Experiment();
    Experiment(int id, const QString &name, int userId, const QDateTime &createdDate = QDateTime::currentDateTime())
        : m_id(id), m_name(name), m_userId(userId), m_createdDate(createdDate)
    {}
    int id() const { return m_id; }
    QString name() const { return m_name; }
    int userId() const { return m_userId; }
    const QDateTime &createdDate() const { return m_createdDate; }

    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }
    void setUserId(int userId) { m_userId = userId; }
    void setCreatedDate(const QDateTime &createdDate) { m_createdDate = createdDate; }

private:
    int m_id;
    QString m_name;
    int m_userId;
    QDateTime m_createdDate;
};

#endif // EXPERIMENT_H
