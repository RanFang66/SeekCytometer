#include "NodeTypeHelper.h"

const std::unordered_map<QString, NodeType> NodeTypeHelper::stringToEnumMap = {
    {"Root", NodeType::Root},
    {"User", NodeType::User},
    {"Experiment", NodeType::Experiment},
    {"Specimen", NodeType::Specimen},
    {"Tube", NodeType::Tube},
    {"Settings", NodeType::Settings}
};

const std::unordered_map<NodeType, QString> NodeTypeHelper::enumToStringMap = {
    {NodeType::Root, "Root"},
    {NodeType::User, "User"},
    {NodeType::Experiment, "Experiment"},
    {NodeType::Specimen, "Specimen"},
    {NodeType::Tube, "Tube"},
    {NodeType::Settings, "Settings"},
    {NodeType::Unknown, "Unknown"}
};


// 字符串转换为枚举值
NodeType NodeTypeHelper::stringToNodeType(const QString& str) {
    auto it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
        return it->second;
    }
    return NodeType::Unknown; // 如果找不到匹配值，返回 Unknown
}

// 枚举值转换为字符串
QString NodeTypeHelper::nodeTypeToString(NodeType type) {
    auto it = enumToStringMap.find(type);
    if (it != enumToStringMap.end()) {
        return it->second;
    }
    return "Unknown"; // 如果没有匹配值，返回 "Unknown"
}

int NodeTypeHelper::nodeTypeDepth(NodeType type)
{
    return static_cast<int>(type);
}
