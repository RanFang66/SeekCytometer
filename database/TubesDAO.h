#ifndef TUBESDAO_H
#define TUBESDAO_H

#include "BaseDAO.h"
#include "Tube.h"

class TubesDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit TubesDAO(QObject *parent = nullptr);

    bool insertTube(const Tube &tube);
    bool updateTube(const Tube &tube);
    bool deleteTube(int tubeId);
    QList<Tube> fetchTubes() const;
    Tube fetchTube(int tubeId) const;
    bool isTubeExists(const QString &name) const;
    bool isTubeExists(int tubeId) const;
};

#endif // TUBESDAO_H
