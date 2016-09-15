#include "DatabaseManager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDateTime>

DatabaseManager::DatabaseManager(const QString &path)
{
    m_sqlDb = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDb.setDatabaseName(path);

    if (!m_sqlDb.open())
    {
       qDebug() << "ERROR: connection with database failed with path:" << path;
       qDebug() << "";
    }
    else
    {
       qDebug() << "Database connection successful from path:" << path;
       qDebug() << "";
    }

    if(!m_sqlDb.tables().contains(QString("FOODLIST")))
    {
        QSqlQuery query;
        if(query.exec("CREATE TABLE FOODLIST(NAME TEXT PRIMARY KEY NOT NULL, SERVING TEXT, CALORIES REAL NOT NULL, FAT REAL NOT NULL, CARBS REAL NOT NULL, FIBER REAL NOT NULL, PROTEIN REAL NOT NULL)"))
        {
            qDebug() << "Successful creaiton of table FOODLIST";
            qDebug() << "";
        }
        else
        {
            qDebug() << "ERROR: failed to create table FOODLIST" << query.lastError().text();
            qDebug() << "";
        }
    }

    if(!m_sqlDb.tables().contains(QString("DAILYFOODLIST")))
    {
        QSqlQuery query;
        if(query.exec("CREATE TABLE DAILYFOODLIST(ID TEXT PRIMARY KEY NOT NULL, DATE TEXT NOT NULL, NAME TEXT NOT NULL, NUMSERVINGS REAL NOT NULL, SERVING TEXT, CALORIES REAL NOT NULL, FAT REAL NOT NULL, CARBS REAL NOT NULL, FIBER REAL NOT NULL, PROTEIN REAL NOT NULL)"))
        {
            qDebug() << "Successful creation of table DAILYFOODLIST";
            qDebug() << "";
        }
        else
        {
            qDebug() << "ERROR: failed to create table DAILYFOODLIST" << query.lastError().text();
            qDebug() << "";
        }
    }
}

bool DatabaseManager::addFood(const FoodData* food)
{
    QSqlQuery query;
    query.prepare("INSERT INTO FOODLIST (NAME, SERVING, CALORIES, FAT, CARBS, FIBER, PROTEIN) "
                  "VALUES (:NAME, :SERVING, :CALORIES, :FAT, :CARBS, :FIBER, :PROTEIN)");
    query.bindValue(":NAME", QString::fromStdString(food->getName()));
    query.bindValue(":SERVING", QString::fromStdString(food->getServing()));
    query.bindValue(":CALORIES", food->getCalories());
    query.bindValue(":FAT", food->getFats());
    query.bindValue(":CARBS", food->getCarbs());
    query.bindValue(":FIBER", food->getFiber());
    query.bindValue(":PROTEIN", food->getProtein());
    if(query.exec())
    {
        qDebug() << "Succesful query exec on food insert for " << food->getName().c_str();
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to insert food into DB with" << query.lastError().text();
        qDebug() << "";
        return false;
    }
}

bool DatabaseManager::updateFood(const FoodData *food)
{
    QSqlQuery query;
    query.prepare("UPDATE FOODLIST SET SERVING=:SERVING, CALORIES=:CALORIES, FAT=:FAT, CARBS=:CARBS, FIBER=:FIBER, PROTEIN=:PROTEIN WHERE NAME=:NAME");
    query.bindValue(":SERVING", QString::fromStdString(food->getServing()));
    query.bindValue(":CALORIES", food->getCalories());
    query.bindValue(":FAT", food->getFats());
    query.bindValue(":CARBS", food->getCarbs());
    query.bindValue(":FIBER", food->getFiber());
    query.bindValue(":PROTEIN", food->getProtein());
    query.bindValue(":NAME",QString::fromStdString(food->getName()));
    if(query.exec())
    {
        qDebug() << "Succesful query exec on food update for " << food->getName().c_str();
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to update food into DB with" << query.lastError().text();
        qDebug() << "";
        return false;
    }

}

bool DatabaseManager::deleteFood(const QString &id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FOODLIST WHERE NAME=:NAME");
    query.bindValue(":NAME",id);
    if(query.exec())
    {
        qDebug() << "Succesful query exec on food delete for" << id;
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to delete food" << id;
        qDebug() << "";
        return false;
    }
}

bool DatabaseManager::deleteDailyFood(const QString &id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM DAILYFOODLIST WHERE ID=:ID");
    query.bindValue(":ID",id);
    if(query.exec())
    {
        qDebug() << "Succesful query exec on daily food delete for" << id;
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to delete daily food" << id << query.lastError();
        qDebug() << "";
        return false;
    }
}

bool DatabaseManager::updateDailyFood(const FoodData *food)
{
    QSqlQuery query;
    query.prepare("UPDATE DAILYFOODLIST SET DATE=:DATE, NAME=:NAME, NUMSERVINGS=:NUMSERVINGS, SERVING=:SERVING, CALORIES=:CALORIES, FAT=:FAT, CARBS=:CARBS, FIBER=:FIBER, PROTEIN=:PROTEIN WHERE ID=:ID");
    query.bindValue(":DATE", QString::fromStdString(food->getDate()));
    query.bindValue(":NAME", QString::fromStdString(food->getName()));
    query.bindValue(":NUMSERVINGS", food->getNumServings());
    query.bindValue(":SERVING", QString::fromStdString(food->getServing()));
    query.bindValue(":CALORIES", food->getCalories());
    query.bindValue(":FAT", food->getFats());
    query.bindValue(":CARBS", food->getCarbs());
    query.bindValue(":FIBER", food->getFiber());
    query.bindValue(":PROTEIN", food->getProtein());
    query.bindValue(":ID", QString::fromStdString(food->getID()));

    qDebug() << food->getDate().c_str() << food->getName().c_str() << food->getNumServings() << food->getServing().c_str() << food->getCalories() << food->getFats() << food->getCarbs() << food->getFiber() << food->getProtein() << food->getID().c_str();

    if(query.exec())
    {
        qDebug() << "Succesful query exec on food update for " << food->getName().c_str();
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to update food into DB with" << query.lastError().text();
        qDebug() << "";
        return false;
    }
}

bool DatabaseManager::addDailyFood(const FoodData *food)
{
    QSqlQuery query;
    query.prepare("INSERT INTO DAILYFOODLIST (ID, DATE, NAME, NUMSERVINGS, SERVING, CALORIES, FAT, CARBS, FIBER, PROTEIN) "
                  "VALUES (:ID, :DATE, :NAME, :NUMSERVINGS, :SERVING, :CALORIES, :FAT, :CARBS, :FIBER, :PROTEIN)");
    query.bindValue(":ID", QString::fromStdString(food->getID()));
    query.bindValue(":DATE", QString::fromStdString(food->getDate()));
    query.bindValue(":NAME", QString::fromStdString(food->getName()));
    query.bindValue(":NUMSERVINGS", food->getNumServings());
    query.bindValue(":SERVING", QString::fromStdString(food->getServing()));
    query.bindValue(":CALORIES", food->getCalories());
    query.bindValue(":FAT", food->getFats());
    query.bindValue(":CARBS", food->getCarbs());
    query.bindValue(":FIBER", food->getFiber());
    query.bindValue(":PROTEIN", food->getProtein());

    if(query.exec())
    {
        qDebug() << "Succesful query exec on daily food insert for" << food->getName().c_str();
        qDebug() << "";
        return true;
    }
    else
    {
        qDebug() << "ERROR: Failed to insert daily food into DB with" << query.lastError().text();
        qDebug() << "";
        return false;
    }
}
