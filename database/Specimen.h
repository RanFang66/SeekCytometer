#ifndef SPECIMEN_H
#define SPECIMEN_H

#include <QString>
#include <QDateTime>

class Specimen
{
public:
    Specimen();
    Specimen(int id, const QString &name, int experimentId, const QDateTime &createdDate = QDateTime::currentDateTime())
        : m_id(id), m_name(name), m_experimentId(experimentId), m_createdDate(createdDate)
    {}

    int id() const { return m_id; }

    QString name() const { return m_name; }

    int experimentId() const { return m_experimentId; }

    const QDateTime &createdDate() const { return m_createdDate; }

    void setId(int id) { m_id = id; }

    void setName(const QString &name) { m_name = name; }

    void setExperimentId(int experimentId) { m_experimentId = experimentId; }

    void setCreatedDate(const QDateTime &createdDate) { m_createdDate = createdDate; }

private:
    int m_id;
    QString m_name;
    int m_experimentId;
    QDateTime m_createdDate;
};

#endif // SPECIMEN_H
