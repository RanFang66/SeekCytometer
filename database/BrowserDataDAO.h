#ifndef BROWSERDATADAO_H
#define BROWSERDATADAO_H

#include "BaseDAO.h"

#include "BrowserData.h"
#include "User.h"


class BrowserDataDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit BrowserDataDAO(QObject *parent = nullptr);

    BrowserData *getBrowserData(const User &user);

    BrowserData* getNewNode(NodeType type, const QString &name, BrowserData* parent);

private:
    BrowserData *buildTree(const QList<QVariantMap> &treeData);
    static const QString queryUserData;
};

#endif // BROWSERDATADAO_H
