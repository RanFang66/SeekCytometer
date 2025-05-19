#ifndef WORKSHEETSDAO_H
#define WORKSHEETSDAO_H

#include "BaseDAO.h"
#include "WorkSheet.h"

// CREATE TABLE WorkSheets (
//     worksheet_id        SERIAL PRIMARY KEY NOT NULL,
//     is_global           BOOLEAN NOT NULL DEFAULT TRUE,
//     experiment_id       INT,
//     tube_id             INT,
//     worksheet_name      VARCHAR(64) NOT NULL,
//     created_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
//     updated_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

//     CHECK (
//         (is_global = TRUE AND experiment_id IS NOT NULL AND tube_id IS NULL) OR
//         (is_global = FALSE AND experiment_id IS NULL AND tube_id IS NOT NULL)
//         ),
//     UNIQUE (experiment_id, worksheet_name),
//     UNIQUE (tube_id, worksheet_name)
//     );


class WorkSheetsDAO : public BaseDAO
{
    Q_OBJECT
public:
    explicit WorkSheetsDAO(QObject *parent = nullptr);

    int insertWorkSheet(const WorkSheet &workSheet);
    int insertWorkSheet(const QString &workSheetName, bool isGlobal, int parentId);
    bool updateWorkSheet(const WorkSheet &workSheet);
    bool deleteWorkSheet(int workSheetId);
    QList<WorkSheet> fetchWorkSheets(bool isGlobal, int parentId) const;
    WorkSheet fetchWorkSheet(int workSheetId) const;
    bool isWorkSheetExists(int workSheetId) const;
    bool isWorkSheetExists(bool isGlobal, int parentId, const QString &workSheetName) const;
};

#endif // WORKSHEETSDAO_H
