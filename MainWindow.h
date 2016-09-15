#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AddFoodFormDialog.h"
#include "FoodData.h"
#include "DatabaseManager.h"

#include <map>
#include <vector>

#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addFoodPushButton_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_foodTabAddFoodButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_foodTabEditFoodButton_clicked();

    void on_foodTabDeleteFoodButton_clicked();

    void on_addToDailyFoodPushButton_clicked();

    void on_foodListTableWidget_cellChanged(int row, int column);

    void on_foodListTableWidget_cellPressed(int row, int column);

    void on_deleteDailyFoodPushButton_clicked();

private:

    void populateInitialFoodList();
    void populateInitialDailyFoodList();
    void populateMasterFoodsRow(const FoodData* newFoodItem);
    void populateDailyFoodsRow(const FoodData* newFoodItem);
    void updateDailyTotal();

    double getServingNumber(int row) const;

    std::string getSelectedDate() const;

    typedef std::vector<FoodData*> FoodDataVec;
    typedef std::map<std::string, FoodDataVec> DateFoodDataMap;

    Ui::MainWindow *ui;
    int m_masterFoodRows;
    int m_foodRows;
    DateFoodDataMap m_dateFoodsMap;
    DatabaseManager* m_dbManager;
    std::map<std::string, FoodData*> m_idFoodDataMap;
    std::map<std::string, int> m_idRowMap;
    int m_selectedDailyFoodRow;
};

#endif // MAINWINDOW_H
