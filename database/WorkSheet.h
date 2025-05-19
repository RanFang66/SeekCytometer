#ifndef WORKSHEET_H
#define WORKSHEET_H

#include <QString>
#include <QDateTime>


class WorkSheet
{
public:
    WorkSheet();
    explicit WorkSheet(int id, QString name, bool isGlobal, int parentId, const QDateTime &createdDate = QDateTime::currentDateTime());

    int id() const;
    QString name() const;
    bool isGlobal() const;
    int parentId() const;
    QDateTime createdAt() const;

    void setId(int id);
    void setName(const QString &name);
    void setIsGlobal(bool isGlobal);
    void setParentId(int parentId);
    void setCreatedAt(const QDateTime &createdAt);

private:
    int m_id;
    QString m_name;
    bool m_isGlobal;
    int  m_parentId;
    QDateTime m_createdAt;
};


inline int WorkSheet::id() const
{
    return m_id;
}

inline QString WorkSheet::name() const
{
    return m_name;
}

inline bool WorkSheet::isGlobal() const
{
    return m_isGlobal;
}

inline int WorkSheet::parentId() const
{
    return m_parentId;
}

inline QDateTime WorkSheet::createdAt() const
{
    return m_createdAt;
}

inline void WorkSheet::setId(int id)
{
    m_id = id;
}

inline void WorkSheet::setName(const QString &name)
{
    m_name = name;
}

inline void WorkSheet::setIsGlobal(bool isGlobal)
{
    m_isGlobal = isGlobal;
}

inline void WorkSheet::setParentId(int parentId)
{
    m_parentId = parentId;
}

inline void WorkSheet::setCreatedAt(const QDateTime &createdAt)
{
    m_createdAt = createdAt;
}



#endif // WORKSHEET_H
