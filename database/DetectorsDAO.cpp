#include "DetectorsDAO.h"

DetectorsDAO::DetectorsDAO(QObject *parent)
    : BaseDAO{parent}
{}

bool DetectorsDAO::insertDetector(const Detector &detector)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Detectors (detector_name, detector_type, filter_peak, filter_bandwidth, default_gain, default_offset) "
                  "VALUES (:detector_name, :detector_type, :filter_peak, :filter_bandwidth, :default_gain, :default_offset)");
    query.bindValue(":detector_name", detector.name());
    query.bindValue(":detector_type", detector.type());
    query.bindValue(":filter_peak", detector.filterPeak());
    query.bindValue(":filter_bandwidth", detector.filterBandwidth());
    query.bindValue(":default_gain", detector.defaultGain());
    query.bindValue(":default_offset", detector.defaultOffset());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

bool DetectorsDAO::updateDetector(const Detector &detector)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Detectors SET detector_name = :detector_name, detector_type = :detector_type, filter_peak = :filter_peak, filter_bandwidth = :filter_bandwidth, default_gain = :default_gain, default_offset = :default_offset WHERE detector_id = :detector_id");
    query.bindValue(":detector_name", detector.name());
    query.bindValue(":detector_type", detector.type());
    query.bindValue(":filter_peak", detector.filterPeak());
    query.bindValue(":filter_bandwidth", detector.filterBandwidth());
    query.bindValue(":default_gain", detector.defaultGain());
    query.bindValue(":default_offset", detector.defaultOffset());
    query.bindValue(":detector_id", detector.id());

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}


bool DetectorsDAO::deleteDetector(int detectorId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Detectors WHERE detector_id = :detector_id");
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }
    return true;
}

QList<Detector> DetectorsDAO::fetchDetectors() const
{
    QList<Detector> detectors;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Detectors");

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return detectors;
    }

    while (query.next()) {
        Detector detector{
            query.value("detector_id").toInt(),
            query.value("filter_peak").toInt(),
            query.value("filter_bandwidth").toInt(),
            query.value("detector_type").toString(),
            query.value("default_gain").toInt(),
            query.value("default_offset").toInt(),
            query.value("detector_name").toString()
        };
        detectors.append(detector);
    }

    return detectors;
}


Detector DetectorsDAO::fetchDetector(int detectorId) const
{
    Detector detector;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Detectors WHERE detector_id = :detector_id");
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return detector;
    }

    if (query.next()) {
        detector.setId(query.value("detector_id").toInt());
        detector.setFilterPeak(query.value("filter_peak").toInt());
        detector.setFilterBandwidth(query.value("filter_bandwidth").toInt());
        detector.setType(query.value("detector_type").toString());
        detector.setDefaultGain(query.value("default_gain").toInt());
        detector.setDefaultOffset(query.value("default_offset").toInt());
        detector.setName(query.value("detector_name").toString());
    }

    return detector;
}

Detector DetectorsDAO::fetchDetector(const QString &name) const
{
    Detector detector;

    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Detectors WHERE detector_name = :detector_name");
    query.bindValue(":detector_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return detector;
    }

    if (query.next()) {
        detector.setId(query.value("detector_id").toInt());
        detector.setFilterPeak(query.value("filter_peak").toInt());
        detector.setFilterBandwidth(query.value("filter_bandwidth").toInt());
        detector.setType(query.value("detector_type").toString());
        detector.setDefaultGain(query.value("default_gain").toInt());
        detector.setDefaultOffset(query.value("default_offset").toInt());
        detector.setName(query.value("detector_name").toString());
    }

    return detector;
}


bool DetectorsDAO::isDetectorExists(const QString &name) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Detectors WHERE detector_name = :detector_name");
    query.bindValue(":detector_name", name);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

bool DetectorsDAO::isDetectorExists(int detectorId) const
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM Detectors WHERE detector_id = :detector_id");
    query.bindValue(":detector_id", detectorId);

    if (!query.exec()) {
        handleError(__FUNCTION__, query);
        return false;
    }

    return query.next();
}

// End of the file

