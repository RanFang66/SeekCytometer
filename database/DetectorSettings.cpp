#include "DetectorSettings.h"

DetectorSettings::DetectorSettings()
:m_configId(0), m_detectorId{0}, m_parameter{""}, m_detectorGain{100}, m_detectorOffset{0}, m_enableThreshold{false}, m_thresholdValue{0},
m_enableHeight{true}, m_enableWidth{false}, m_enableArea{false}
{
}

DetectorSettings::DetectorSettings(int configId, int detectorId, QString parameter, int detectorGain, int detectorOffset,
                                   bool enableThreshold, int thresholdVal, bool enableHeight, bool enableWidth, bool enableArea)
:m_configId(configId), m_detectorId{detectorId}, m_parameter{parameter}, m_detectorGain{detectorGain}, m_detectorOffset{detectorOffset},
m_enableThreshold{enableThreshold}, m_thresholdValue{thresholdVal},
m_enableHeight{enableHeight}, m_enableWidth{enableWidth}, m_enableArea{enableArea}
{

}

