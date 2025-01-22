#ifndef TUBE_H
#define TUBE_H
#include <QString>
#include <QDateTime>

class Tube
{
public:
    Tube();
    Tube(int id, const QString &name, int specimenId, const QDateTime &createdDate = QDateTime::currentDateTime())
        : m_id(id), m_name(name), m_specimenId(specimenId), m_createdDate(createdDate)
    {}
    int id() const { return m_id; }
    QString name() const { return m_name; }
    int specimenId() const { return m_specimenId; }
    const QDateTime &createdDate() const { return m_createdDate; }

    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }
    void setSpecimenId(int specimenId) { m_specimenId = specimenId; }

private:
    int m_id;
    QString m_name;
    int m_specimenId;
    QDateTime m_createdDate;
};
#endif // TUBE_H
