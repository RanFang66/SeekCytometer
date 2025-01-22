#include "TreeNode.h"

TreeNode::TreeNode(NodeType type, int id, const QString &name, const QDateTime &date, TreeNode *parent)
    : m_parent(parent), m_type(type), m_id(id), m_name(name), m_createdDate(date)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(m_children);
}


void TreeNode::addChild(TreeNode *child)
{
    m_children.append(child);
}


TreeNode* TreeNode::getChild(int row) const
{
    return m_children.value(row);
}

TreeNode *TreeNode::getSettings() const
{
    switch (m_type) {
    case NodeType::Experiment:
        for (TreeNode *node: m_children) {
            if (node->getType() == NodeType::Config) {
                return node;
            }
        }
        break;
    case NodeType::Specimen:
        for (TreeNode *node: m_children) {
            if (node->getType() == NodeType::Config) {
                return node;
            }
        }
        return m_parent->getSettings();
        break;
    case NodeType::Tube:
        for (TreeNode *node: m_children) {
            if (node->getType() == NodeType::Config) {
                return node;
            }
        }
        return m_parent->getSettings();
        break;
    default:
        break;
    }
    return nullptr;
}


int TreeNode::childCount() const
{
    return m_children.count();
}

int TreeNode::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
    return 0;
}

TreeNode* TreeNode::getParent() const
{
    return m_parent;
}

NodeType TreeNode::getType() const
{
    return m_type;
}

QString TreeNode::getTypeString() const
{
    static const QStringList typeNames = {"Root", "User", "Experiment", "Specimen", "Tube", "Config"};
    return typeNames.at(static_cast<int>(m_type));
}

int TreeNode::getId() const
{
    return m_id;
}

QString TreeNode::getName() const
{
    return m_name;
}

const QDateTime &TreeNode::getCreatedDate() const
{
    return m_createdDate;
}



