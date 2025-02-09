#include "CytometerSettings.h"

CytometerSettings::CytometerSettings()
: m_id{0}, m_name{"CytometerSettings"}, m_parentType{NodeType::Experiment}, m_parentId{0}, m_thresholdType{ThresholdType::ThresholdOr}
{

}


CytometerSettings::CytometerSettings(int id, NodeType type, int parentId, ThresholdType thresholdType, QString name)
    :m_id(id), m_name{name}, m_parentType{type}, m_parentId{parentId}, m_thresholdType{thresholdType}
{

}

QString CytometerSettings::thresholdTypeToString(ThresholdType type)
{
    if (type == ThresholdType::ThresholdOr) {
        return "OR";
    } else {
        return "AND";
    }
}

ThresholdType CytometerSettings::stringToThresholdType(const QString &type)
{
    if (type == "OR") {
        return ThresholdType::ThresholdOr;
    } else {
        return ThresholdType::ThresholdAnd;
    }
}

