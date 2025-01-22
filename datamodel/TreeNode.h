#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QDateTime>

enum class NodeType {
    Root,
    User,
    Experiment,
    Specimen,
    Tube,
    Config
};



class TreeNode
{
public:
    explicit TreeNode(NodeType type, int id, const QString &name, const QDateTime &date, TreeNode *parent = nullptr);
    ~TreeNode();

    void addChild(TreeNode *child);
    TreeNode* getChild(int row) const;
    TreeNode* getSettings() const;
    int childCount() const;
    int row() const;
    TreeNode* getParent() const;
    NodeType getType() const;
    QString getTypeString()  const;
    int getId()         const;
    QString getName()   const;
    const QDateTime &getCreatedDate()     const;

private:
    TreeNode *m_parent;
    QList<TreeNode*> m_children;
    NodeType    m_type;
    int         m_id;
    QString     m_name;
    QDateTime   m_createdDate;
};

#endif // TREENODE_H
