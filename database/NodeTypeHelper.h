#ifndef NODETYPEHELPER_H
#define NODETYPEHELPER_H

#include <QString>
#include <unordered_map>

#include <QHash>

enum class NodeType {
    Root,
    User,
    Experiment,
    Specimen,
    Tube,
    Settings,
    Unknown,
};

class NodeTypeHelper {
public:
    static NodeType stringToNodeType(const QString &str);
    static QString nodeTypeToString(NodeType type);
    static bool isValidNodeType(NodeType type);
    static int  nodeTypeDepth(NodeType type);
private:
    NodeTypeHelper() = delete;
    static const std::unordered_map<QString, NodeType> stringToEnumMap;
    static const std::unordered_map<NodeType, QString> enumToStringMap;
};


#endif // NODETYPEHELPER_H
