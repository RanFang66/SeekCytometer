#ifndef BROWSERDATADAO_H
#define BROWSERDATADAO_H

#include "BaseDAO.h"

#include "TreeNode.h"
#include "User.h"


class BrowserDataDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit BrowserDataDAO(QObject *parent = nullptr);

    TreeNode *getBrowserData(const User &user);

private:
    TreeNode *buildTree(const QList<QVariantMap> &treeData);
};

#endif // BROWSERDATADAO_H
