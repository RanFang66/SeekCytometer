#include "WorkSheet.h"

WorkSheet::WorkSheet() {}

WorkSheet::WorkSheet(int id, QString name, bool isGlobal, int parentId, const QDateTime &createdDate)
: m_id{id}, m_name{name}, m_isGlobal{isGlobal}, m_parentId{parentId}, m_createdAt{createdDate}
{}

