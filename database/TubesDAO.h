#ifndef TUBESDAO_H
#define TUBESDAO_H

#include "BaseDAO.h"
#include "Tube.h"

class TubesDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit TubesDAO(QObject *parent = nullptr);

    int  insertTube(const Tube &tube);
    int  insertTube(const QString &name, int specimenId);
    bool updateTube(const Tube &tube);
    bool deleteTube(int tubeId);
    QList<Tube> fetchTubes() const;
    Tube fetchTube(int tubeId) const;
    Tube fetchTube(int specimenId, const QString &name);
    bool isTubeExists(const QString &name, int specimenId) const;
    bool isTubeExists(int tubeId) const;
};

#endif // TUBESDAO_H
