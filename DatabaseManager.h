#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include "FoodData.h"

class DatabaseManager
{
public:
    DatabaseManager(const QString& path);

    bool addFood(const FoodData *food);
    bool updateFood(const FoodData *food);
    bool deleteFood(const QString& id);
    bool deleteDailyFood(const QString& id);

    bool updateDailyFood(const FoodData *food);

    bool addDailyFood(const FoodData *food);

private:
    QSqlDatabase m_sqlDb;
};

#endif // DATABASEMANAGER_H
