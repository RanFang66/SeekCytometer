#ifndef DETECTOR_H
#define DETECTOR_H

#include <QString>

class Detector
{
public:
    explicit Detector();
    explicit Detector(int id, int filterPeak, int filterBandwidth, QString type = "PMT", int defaultGain = 100, int defaultOffset = 0, QString name="");
    int id() const;
    QString name() const;
    QString type() const;
    int filterPeak() const;
    int filterBandwidth() const;
    int defaultGain() const;
    int defaultOffset() const;

    void setId(int id);
    void setName(const QString &name);
    void setType(const QString &type);
    void setFilterPeak(int filterPeak);
    void setFilterBandwidth(int filterBandwidth);
    void setDefaultGain(int defaultGain);
    void setDefaultOffset(int defaultOffset);

private:
    int             m_id;
    QString         m_name;
    QString         m_type;
    int             m_filterPeak;
    int             m_filterBandwidth;
    int             m_defaultGain;
    int             m_defaultOffset;
};

inline int Detector::id() const
{
    return m_id;
}

inline QString Detector::name() const
{
    return m_name;
}


inline int Detector::filterPeak() const
{
    return m_filterPeak;
}

inline int Detector::filterBandwidth() const
{
    return m_filterBandwidth;
}

inline int Detector::defaultGain() const
{
    return m_defaultGain;
}

inline int Detector::defaultOffset() const
{
    return m_defaultOffset;
}

inline void Detector::setId(int id)
{
    m_id = id;
}

inline void Detector::setName(const QString &name)
{
    m_name = name;
}

inline void Detector::setType(const QString &type)
{
    m_type = type;
}

inline void Detector::setFilterPeak(int filterPeak)
{
    m_filterPeak = filterPeak;
}


inline void Detector::setFilterBandwidth(int filterBandwidth)
{
    m_filterBandwidth = filterBandwidth;
}

inline void Detector::setDefaultGain(int defaultGain)
{
    m_defaultGain = defaultGain;
}

inline void Detector::setDefaultOffset(int defaultOffset)
{
    m_defaultOffset = defaultOffset;
}

#endif // DETECTOR_H
