#include "CytometerSettings.h"

CytometerSettings::CytometerSettings()
: m_id{0}, m_name{"CytometerSettings"}, m_parentType{ParentType::Experiment}, m_parentId{0}, m_thresholdType{ThresholdType::ThresholdSingle}
{

}

CytometerSettings::CytometerSettings(int id, ParentType type, int parentId, ThresholdType thresholdType, QString name)
    :m_id(id), m_name{name}, m_parentType{type}, m_parentId{parentId}, m_thresholdType{thresholdType}
{

}

