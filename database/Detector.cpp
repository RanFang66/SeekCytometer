#include "Detector.h"

Detector::Detector()
: m_id{0}, m_name{""}, m_type{"PMT"}, m_filterPeak{0}, m_filterBandwidth{0}, m_defaultGain{100}, m_defaultOffset{0}
{
}

Detector::Detector(int id, int filterPeak, int filterBandwidth, QString type, int defaultGain, int defaultOffset, QString name)
: m_id{id}, m_name{name}, m_type{type}, m_filterPeak{filterPeak}, m_filterBandwidth{filterBandwidth}, m_defaultGain{defaultGain}, m_defaultOffset{defaultOffset}
{
    if (m_name.isEmpty()) {
        m_name = QString("Detector-%1").arg(m_id);
    }
}

QString Detector::type() const
{
    return m_type;
}




