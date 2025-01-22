#ifndef DETECTORSDAO_H
#define DETECTORSDAO_H

#include "BaseDAO.h"
#include "Detector.h"

class DetectorsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit DetectorsDAO(QObject *parent = nullptr);

    bool insertDetector(const Detector &detector);
    bool updateDetector(const Detector &detector);
    bool deleteDetector(int detectorId);
    QList<Detector> fetchDetectors() const;
    Detector fetchDetector(int detectorId) const;
    Detector fetchDetector(const QString &name) const;
    bool isDetectorExists(const QString &name) const;
    bool isDetectorExists(int detectorId) const;
};

#endif // DETECTORSDAO_H
