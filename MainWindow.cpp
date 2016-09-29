#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QSqlError>
#include <QUuid>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_masterFoodRows(0), m_foodRows(0), m_mealFoodRows(0), m_mealCreatorName(""), m_mealCreatorServings(0.0)
{
    ui->setupUi(this);

    QStringList headers;
    headers << "Item" << "Serving" << "Calories" << "Fats" << "Carbs" << "Fiber" << "Protein";

    QStringList dailyFoodsHeaders;
    dailyFoodsHeaders << "Num Servings" << "Item" << "Serving" << "Calories" << "Fats" << "Carbs" << "Fiber" << "Protein";

    QStringList totalsHeaders;
    totalsHeaders << "Calories" << "Fats" << "Carbs" << "Fiber" << "Protein";

    ui->masterFoodTableWidget->setColumnCount(7);
    ui->masterFoodTableWidget->setHorizontalHeaderLabels(headers);

    ui->foodListTableWidget->setColumnCount(8);
    ui->foodListTableWidget->setHorizontalHeaderLabels(dailyFoodsHeaders);

    ui->mealCreatorFoodList->setColumnCount(7);
    ui->mealCreatorFoodList->setHorizontalHeaderLabels(headers);

    ui->foodTotalsTableWidget->setColumnCount(5);
    ui->foodTotalsTableWidget->setHorizontalHeaderLabels(totalsHeaders);

    ui->mealTotalTableWidget->setColumnCount(5);
    ui->mealTotalTableWidget->setHorizontalHeaderLabels(totalsHeaders);

    ui->createdMealFoodList->setColumnCount(8);
    ui->createdMealFoodList->setHorizontalHeaderLabels(dailyFoodsHeaders);

    for (int c = 0; c < ui->foodListTableWidget->horizontalHeader()->count(); ++c)
    {
        ui->foodListTableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int c = 0; c < ui->masterFoodTableWidget->horizontalHeader()->count(); ++c)
    {
        ui->masterFoodTableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int c = 0; c < ui->mealCreatorFoodList->horizontalHeader()->count(); ++c)
    {
        ui->mealCreatorFoodList->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int c = 0; c < ui->createdMealFoodList->horizontalHeader()->count(); ++c)
    {
        ui->createdMealFoodList->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int c = 0; c < ui->foodTotalsTableWidget->horizontalHeader()->count(); ++c)
    {
        ui->foodTotalsTableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    for (int c = 0; c < ui->mealTotalTableWidget->horizontalHeader()->count(); ++c)
    {
        ui->mealTotalTableWidget->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
    }

    ui->masterFoodTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->masterFoodTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->masterFoodTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->mealCreatorFoodList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->mealCreatorFoodList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mealCreatorFoodList->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->foodTotalsTableWidget->setFixedHeight(50);
    ui->foodTotalsTableWidget->setRowCount(1);

    ui->mealTotalTableWidget->setFixedHeight(50);
    ui->mealTotalTableWidget->setRowCount(1);

    ui->foodTotalsTableWidget->setItem(0,0, new QTableWidgetItem(QString("0")));
    ui->foodTotalsTableWidget->setItem(0,1, new QTableWidgetItem(QString("0")));
    ui->foodTotalsTableWidget->setItem(0,2, new QTableWidgetItem(QString("0")));
    ui->foodTotalsTableWidget->setItem(0,3, new QTableWidgetItem(QString("0")));
    ui->foodTotalsTableWidget->setItem(0,4, new QTableWidgetItem(QString("0")));

    ui->mealTotalTableWidget->setItem(0,0, new QTableWidgetItem(QString("0")));
    ui->mealTotalTableWidget->setItem(0,1, new QTableWidgetItem(QString("0")));
    ui->mealTotalTableWidget->setItem(0,2, new QTableWidgetItem(QString("0")));
    ui->mealTotalTableWidget->setItem(0,3, new QTableWidgetItem(QString("0")));
    ui->mealTotalTableWidget->setItem(0,4, new QTableWidgetItem(QString("0")));

    m_dbManager = new DatabaseManager(QCoreApplication::applicationDirPath() + QDir::separator() + "masterFoodList.db");

    populateInitialFoodList();
    populateInitialDailyFoodList();
    updateDailyTotal();

    if(m_masterFoodRows > 0)
    {
        ui->addToDailyFoodPushButton->setEnabled(true);
        ui->foodTabEditFoodButton->setEnabled(true);
        ui->foodTabDeleteFoodButton->setEnabled(true);
        ui->addToMealArrowButton->setEnabled(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addFoodPushButton_clicked()
{

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        ui->masterFoodTableWidget->selectRow(0);
    }
    else if(index == 2)
    {
        ui->mealCreatorFoodList->selectRow(0);
    }
}

void MainWindow::on_foodTabAddFoodButton_clicked()
{
    AddFoodFormDialog* newFoodForm = new AddFoodFormDialog(this);
    if(newFoodForm->exec() == QDialog::Accepted)
    {
        FoodData* newFoodItem = newFoodForm->getNewFood();
        populateMasterFoodsRow(newFoodItem);

        m_dbManager->addFood(newFoodItem);

        if(m_masterFoodRows == 1)
        {
            ui->addToDailyFoodPushButton->setEnabled(true);
            ui->foodTabEditFoodButton->setEnabled(true);
            ui->foodTabDeleteFoodButton->setEnabled(true);

            ui->addToMealArrowButton->setEnabled(true);
            ui->removeFromMealArrowButton->setEnabled(true);

            ui->masterFoodTableWidget->selectRow(0);
            ui->mealCreatorFoodList->selectRow(0);
        }
    }
}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    while(ui->foodListTableWidget->rowCount() > 0)
    {
        ui->foodListTableWidget->removeRow(0);
    }
    m_foodRows = 0;
    populateInitialDailyFoodList();
}

void MainWindow::populateInitialFoodList()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM FOODLIST");
    query.exec();

    int nameFieldNo = query.record().indexOf("NAME");
    int servingFieldNo = query.record().indexOf("SERVING");
    int caloriesFieldNo = query.record().indexOf("CALORIES");
    int fatsFieldNo = query.record().indexOf("FAT");
    int carbsFieldNo = query.record().indexOf("CARBS");
    int fiberFieldNo = query.record().indexOf("FIBER");
    int proteinFieldNo = query.record().indexOf("PROTEIN");

    bool ok;
    if(query.first())
    {
        QString name = query.value(nameFieldNo).toString();
        QString serving = query.value(servingFieldNo).toString();
        double calories = query.value(caloriesFieldNo).toDouble(&ok);
        double fat = query.value(fatsFieldNo).toDouble(&ok);
        double carbs = query.value(carbsFieldNo).toDouble(&ok);
        double fiber = query.value(fiberFieldNo).toDouble(&ok);
        double protein = query.value(proteinFieldNo).toDouble(&ok);

        FoodData* newFoodItem = new FoodData(name.toStdString(), serving.toStdString(), calories, fat, carbs, fiber, protein);
        populateMasterFoodsRow(newFoodItem);

        while (query.next()) {
            QString name = query.value(nameFieldNo).toString();
            QString serving = query.value(servingFieldNo).toString();
            double calories = query.value(caloriesFieldNo).toDouble(&ok);
            double fat = query.value(fatsFieldNo).toDouble(&ok);
            double carbs = query.value(carbsFieldNo).toDouble(&ok);
            double fiber = query.value(fiberFieldNo).toDouble(&ok);
            double protein = query.value(proteinFieldNo).toDouble(&ok);

            FoodData* newFoodItem = new FoodData(name.toStdString(), serving.toStdString(), calories, fat, carbs, fiber, protein);
            populateMasterFoodsRow(newFoodItem);
        }
    }
}

void MainWindow::populateInitialDailyFoodList()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM DAILYFOODLIST WHERE DATE=:DATE");
    query.bindValue(":DATE", QString::fromStdString(getSelectedDate()));
    query.exec();

    if(query.first())
    {
        int idFieldNo = query.record().indexOf("ID");
        int nameFieldNo = query.record().indexOf("NAME");
        int servingFieldNo = query.record().indexOf("SERVING");
        int numServingsFieldNo = query.record().indexOf("NUMSERVINGS");
        int caloriesFieldNo = query.record().indexOf("CALORIES");
        int fatsFieldNo = query.record().indexOf("FAT");
        int carbsFieldNo = query.record().indexOf("CARBS");
        int fiberFieldNo = query.record().indexOf("FIBER");
        int proteinFieldNo = query.record().indexOf("PROTEIN");
        int dateFieldNo = query.record().indexOf("DATE");

        bool ok;

        QString id = query.value(idFieldNo).toString();
        QString name = query.value(nameFieldNo).toString();
        QString serving = query.value(servingFieldNo).toString();
        QString date = query.value(dateFieldNo).toString();
        double numServings = query.value(numServingsFieldNo).toDouble(&ok);
        double calories = query.value(caloriesFieldNo).toDouble(&ok);
        double fat = query.value(fatsFieldNo).toDouble(&ok);
        double carbs = query.value(carbsFieldNo).toDouble(&ok);
        double fiber = query.value(fiberFieldNo).toDouble(&ok);
        double protein = query.value(proteinFieldNo).toDouble(&ok);

        FoodData* newFoodItem = new FoodData(id.toStdString(), name.toStdString(), serving.toStdString(), date.toStdString(), numServings, calories, fat, carbs, fiber, protein);
        m_idFoodDataMap.insert(make_pair(newFoodItem->getID(), newFoodItem));

        populateDailyFoodsRow(newFoodItem);

        while (query.next()) {
            QString id = query.value(idFieldNo).toString();
            QString name = query.value(nameFieldNo).toString();
            QString serving = query.value(servingFieldNo).toString();
            QString date = query.value(dateFieldNo).toString();
            double numServings = query.value(numServingsFieldNo).toDouble(&ok);
            double calories = query.value(caloriesFieldNo).toDouble(&ok);
            double fat = query.value(fatsFieldNo).toDouble(&ok);
            double carbs = query.value(carbsFieldNo).toDouble(&ok);
            double fiber = query.value(fiberFieldNo).toDouble(&ok);
            double protein = query.value(proteinFieldNo).toDouble(&ok);

            FoodData* newFoodItem = new FoodData(id.toStdString(), name.toStdString(), serving.toStdString(), date.toStdString(), numServings, calories, fat, carbs, fiber, protein);
            m_idFoodDataMap.insert(make_pair(newFoodItem->getID(), newFoodItem));

            populateDailyFoodsRow(newFoodItem);
        }
    }
    else
    {
        qDebug() << "Populate initial daily food list exec failed:" << query.lastError().text();
    }
}

void MainWindow::populateMasterFoodsRow(const FoodData *newFoodItem)
{
    ui->masterFoodTableWidget->setRowCount(m_masterFoodRows+1);
    ui->mealCreatorFoodList->setRowCount(m_masterFoodRows+1);

    ui->masterFoodTableWidget->setItem(m_masterFoodRows,0, new QTableWidgetItem(QString::fromStdString(newFoodItem->getName())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,1, new QTableWidgetItem(QString::fromStdString(newFoodItem->getServing())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,2, new QTableWidgetItem(QString::number(newFoodItem->getCalories())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,3, new QTableWidgetItem(QString::number(newFoodItem->getFats())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,4, new QTableWidgetItem(QString::number(newFoodItem->getCarbs())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,5, new QTableWidgetItem(QString::number(newFoodItem->getFiber())));
    ui->masterFoodTableWidget->setItem(m_masterFoodRows,6, new QTableWidgetItem(QString::number(newFoodItem->getProtein())));

    ui->mealCreatorFoodList->setItem(m_masterFoodRows,0, new QTableWidgetItem(QString::fromStdString(newFoodItem->getName())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,1, new QTableWidgetItem(QString::fromStdString(newFoodItem->getServing())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,2, new QTableWidgetItem(QString::number(newFoodItem->getCalories())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,3, new QTableWidgetItem(QString::number(newFoodItem->getFats())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,4, new QTableWidgetItem(QString::number(newFoodItem->getCarbs())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,5, new QTableWidgetItem(QString::number(newFoodItem->getFiber())));
    ui->mealCreatorFoodList->setItem(m_masterFoodRows,6, new QTableWidgetItem(QString::number(newFoodItem->getProtein())));

    ++m_masterFoodRows;
}

void MainWindow::populateDailyFoodsRow(const FoodData *newFoodItem)
{
    if(getSelectedDate() == newFoodItem->getDate())
    {
        ui->foodListTableWidget->blockSignals(true);
        ui->foodListTableWidget->setRowCount(m_foodRows+1);

        ui->foodListTableWidget->setItem(m_foodRows,0, new QTableWidgetItem(QString::number(newFoodItem->getNumServings())));
        ui->foodListTableWidget->setItem(m_foodRows,1, new QTableWidgetItem(QString::fromStdString(newFoodItem->getName())));
        ui->foodListTableWidget->setItem(m_foodRows,2, new QTableWidgetItem(QString::fromStdString(newFoodItem->getServing())));
        ui->foodListTableWidget->setItem(m_foodRows,3, new QTableWidgetItem(QString::number(newFoodItem->getCalories())));
        ui->foodListTableWidget->setItem(m_foodRows,4, new QTableWidgetItem(QString::number(newFoodItem->getFats())));
        ui->foodListTableWidget->setItem(m_foodRows,5, new QTableWidgetItem(QString::number(newFoodItem->getCarbs())));
        ui->foodListTableWidget->setItem(m_foodRows,6, new QTableWidgetItem(QString::number(newFoodItem->getFiber())));
        ui->foodListTableWidget->setItem(m_foodRows,7, new QTableWidgetItem(QString::number(newFoodItem->getProtein())));
        ui->foodListTableWidget->blockSignals(false);

        m_idRowMap.insert(make_pair(newFoodItem->getID(), m_foodRows));

        ++m_foodRows;

        updateDailyTotal();
    }
}

void MainWindow::populateNewMealRow(const FoodData *newFoodItem)
{
    ui->createdMealFoodList->setRowCount(m_mealFoodRows+1);

    ui->createdMealFoodList->setItem(m_mealFoodRows,0, new QTableWidgetItem(QString::number(newFoodItem->getNumServings())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,1, new QTableWidgetItem(QString::fromStdString(newFoodItem->getName())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,2, new QTableWidgetItem(QString::fromStdString(newFoodItem->getServing())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,3, new QTableWidgetItem(QString::number(newFoodItem->getCalories())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,4, new QTableWidgetItem(QString::number(newFoodItem->getFats())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,5, new QTableWidgetItem(QString::number(newFoodItem->getCarbs())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,6, new QTableWidgetItem(QString::number(newFoodItem->getFiber())));
    ui->createdMealFoodList->setItem(m_mealFoodRows,7, new QTableWidgetItem(QString::number(newFoodItem->getProtein())));

    ++m_mealFoodRows;

    if(m_mealFoodRows == 1)
    {
        ui->removeFromMealArrowButton->setEnabled(true);
    }
}

void MainWindow::updateDailyTotal()
{
    double caloriesTotal = 0.0;
    double fatsTotal = 0.0;
    double carbsTotal = 0.0;
    double fiberTotal = 0.0;
    double proteinTotal = 0.0;
    bool ok = false;

    for(int i = 0; i < ui->foodListTableWidget->rowCount(); ++i)
    {
        caloriesTotal += ui->foodListTableWidget->item(i,3)->text().toDouble(&ok);
        fatsTotal += ui->foodListTableWidget->item(i,4)->text().toDouble(&ok);
        carbsTotal += ui->foodListTableWidget->item(i,5)->text().toDouble(&ok);
        fiberTotal += ui->foodListTableWidget->item(i,6)->text().toDouble(&ok);
        proteinTotal += ui->foodListTableWidget->item(i,7)->text().toDouble(&ok);
    }

    ui->foodTotalsTableWidget->item(0,0)->setText(QString::number(caloriesTotal));
    ui->foodTotalsTableWidget->item(0,1)->setText(QString::number(fatsTotal));
    ui->foodTotalsTableWidget->item(0,2)->setText(QString::number(carbsTotal));
    ui->foodTotalsTableWidget->item(0,3)->setText(QString::number(fiberTotal));
    ui->foodTotalsTableWidget->item(0,4)->setText(QString::number(proteinTotal));
}

void MainWindow::updateMealTotal()
{
    double caloriesTotal = 0.0;
    double fatsTotal = 0.0;
    double carbsTotal = 0.0;
    double fiberTotal = 0.0;
    double proteinTotal = 0.0;
    bool ok = false;

    for(int i = 0; i < ui->createdMealFoodList->rowCount(); ++i)
    {
        caloriesTotal += ui->createdMealFoodList->item(i,3)->text().toDouble(&ok);
        fatsTotal += ui->createdMealFoodList->item(i,4)->text().toDouble(&ok);
        carbsTotal += ui->createdMealFoodList->item(i,5)->text().toDouble(&ok);
        fiberTotal += ui->createdMealFoodList->item(i,6)->text().toDouble(&ok);
        proteinTotal += ui->createdMealFoodList->item(i,7)->text().toDouble(&ok);
    }

    ui->mealTotalTableWidget->item(0,0)->setText(QString::number(caloriesTotal));
    ui->mealTotalTableWidget->item(0,1)->setText(QString::number(fatsTotal));
    ui->mealTotalTableWidget->item(0,2)->setText(QString::number(carbsTotal));
    ui->mealTotalTableWidget->item(0,3)->setText(QString::number(fiberTotal));
    ui->mealTotalTableWidget->item(0,4)->setText(QString::number(proteinTotal));
}

void MainWindow::updateEditedDailyFoods(const FoodData* newFoodItem)
{
    for(int i = 0; i < ui->foodListTableWidget->rowCount(); ++i)
    {
         if(ui->foodListTableWidget->item(i,1)->text().toStdString() == newFoodItem->getName())
         {
             ui->foodListTableWidget->blockSignals(true);
             bool ok;
             double numServings = ui->foodListTableWidget->item(i, 0)->text().toDouble(&ok);

             double calories = newFoodItem->getCalories() * numServings;
             double fats = newFoodItem->getFats() * numServings;
             double carbs = newFoodItem->getCarbs() * numServings;
             double fiber = newFoodItem->getFiber() * numServings;
             double protein = newFoodItem->getProtein() * numServings;

             ui->foodListTableWidget->item(i, 3)->setText(QString::number(calories));
             ui->foodListTableWidget->item(i, 4)->setText(QString::number(fats));
             ui->foodListTableWidget->item(i, 5)->setText(QString::number(carbs));
             ui->foodListTableWidget->item(i, 6)->setText(QString::number(fiber));
             ui->foodListTableWidget->item(i, 7)->setText(QString::number(protein));

             ui->foodListTableWidget->blockSignals(false);

             updateDailyTotal();

             string selectedDate = getSelectedDate();
             std::map<std::string, int>::const_iterator iter = m_idRowMap.begin();
             while(iter != m_idRowMap.end())
             {
                 if(iter->second == i && m_idFoodDataMap[iter->first]->getDate() == selectedDate)
                 {
                     m_idFoodDataMap[iter->first]->setCalories(calories);
                     m_idFoodDataMap[iter->first]->setFats(fats);
                     m_idFoodDataMap[iter->first]->setCarbs(carbs);
                     m_idFoodDataMap[iter->first]->setFiber(fiber);
                     m_idFoodDataMap[iter->first]->setProtein(protein);

                     m_dbManager->updateDailyFood(m_idFoodDataMap[iter->first]);
                     break;
                 }
             }
         }
    }
}

double MainWindow::getServingNumber(int row) const
{
    bool ok = false;
    return ui->foodListTableWidget->item(row,0)->text().toDouble(&ok);
}

std::__1::string MainWindow::getSelectedDate() const
{
    int day, month, year = 0;
    ui->calendarWidget->selectedDate().getDate(&year, &month, &day);
    return (std::to_string(month) + "/" + std::to_string(day) + "/" + std::to_string(year));
}

void MainWindow::on_foodTabEditFoodButton_clicked()
{
    AddFoodFormDialog* newFoodForm = new AddFoodFormDialog(this);
    FoodData* editedFood = new FoodData();

    int row = ui->masterFoodTableWidget->currentRow();
    bool ok;

    editedFood->setName(ui->masterFoodTableWidget->item(row,0)->text().toStdString());
    editedFood->setServing(ui->masterFoodTableWidget->item(row,1)->text().toStdString());
    editedFood->setCalories(ui->masterFoodTableWidget->item(row,2)->text().toDouble(&ok));
    editedFood->setFats(ui->masterFoodTableWidget->item(row,3)->text().toDouble(&ok));
    editedFood->setCarbs(ui->masterFoodTableWidget->item(row,4)->text().toDouble(&ok));
    editedFood->setFiber(ui->masterFoodTableWidget->item(row,5)->text().toDouble(&ok));
    editedFood->setProtein(ui->masterFoodTableWidget->item(row,6)->text().toDouble(&ok));
    newFoodForm->addFoodToBeEdited(editedFood);

    if(newFoodForm->exec() == QDialog::Accepted)
    {
        editedFood = newFoodForm->getNewFood();
        m_dbManager->updateFood(editedFood);

        ui->masterFoodTableWidget->item(row,0)->setText(QString::fromStdString(editedFood->getName()));
        ui->masterFoodTableWidget->item(row,1)->setText(QString::fromStdString(editedFood->getServing()));
        ui->masterFoodTableWidget->item(row,2)->setText(QString::number(editedFood->getCalories()));
        ui->masterFoodTableWidget->item(row,3)->setText(QString::number(editedFood->getFats()));
        ui->masterFoodTableWidget->item(row,4)->setText(QString::number(editedFood->getCarbs()));
        ui->masterFoodTableWidget->item(row,5)->setText(QString::number(editedFood->getFiber()));
        ui->masterFoodTableWidget->item(row,6)->setText(QString::number(editedFood->getProtein()));

        updateEditedDailyFoods(editedFood);
    }

}

void MainWindow::on_foodTabDeleteFoodButton_clicked()
{
    int row = ui->masterFoodTableWidget->currentRow();
    QString name = ui->masterFoodTableWidget->item(row,0)->text();

    QMessageBox msgBox;
    QString msg;
    msg += QString("Are you sure you want to delete ");
    msg += name;
    msg += "?";

    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        m_dbManager->deleteFood(name);
        ui->masterFoodTableWidget->removeRow(row);
        --m_masterFoodRows;
        ui->masterFoodTableWidget->setRowCount(m_masterFoodRows+1);
    }
}
void MainWindow::on_addToDailyFoodPushButton_clicked()
{
    std::string stringDate = getSelectedDate();

    int row = ui->masterFoodTableWidget->currentRow();
    bool ok = false;

    QUuid id = id.createUuid();

    qDebug() << "Adding daily food item with date:" << stringDate.c_str() << "and ID =" << id.toString();
    qDebug() << "";

    FoodData* newDailyFood = new FoodData(id.toString().toStdString(),
                                          ui->masterFoodTableWidget->item(row,0)->text().toStdString(),
                                          ui->masterFoodTableWidget->item(row,1)->text().toStdString(),
                                          stringDate,
                                          1.0,
                                          ui->masterFoodTableWidget->item(row,2)->text().toDouble(&ok),
                                          ui->masterFoodTableWidget->item(row,3)->text().toDouble(&ok),
                                          ui->masterFoodTableWidget->item(row,4)->text().toDouble(&ok),
                                          ui->masterFoodTableWidget->item(row,5)->text().toDouble(&ok),
                                          ui->masterFoodTableWidget->item(row,6)->text().toDouble(&ok));

    m_idFoodDataMap.insert(make_pair(newDailyFood->getID(), newDailyFood));

    populateDailyFoodsRow(newDailyFood);
    updateDailyTotal();
    m_dbManager->addDailyFood(newDailyFood);
}

void MainWindow::on_foodListTableWidget_cellChanged(int row, int column)
{
    if(column == 0)
    {
        string selectedDate = getSelectedDate();
        std::map<std::string, int>::const_iterator iter = m_idRowMap.begin();
        while(iter != m_idRowMap.end())
        {
            if(iter->second == row && m_idFoodDataMap[iter->first]->getDate() == selectedDate)
            {
                double oldNumServings = m_idFoodDataMap[iter->first]->getNumServings();

                bool ok;
                double numServings = ui->foodListTableWidget->item(row, 0)->text().toDouble(&ok);
                double newNumServings = (numServings/oldNumServings);

                double calories = ui->foodListTableWidget->item(row, 3)->text().toDouble(&ok) * newNumServings;
                double fats = ui->foodListTableWidget->item(row, 4)->text().toDouble(&ok) * newNumServings;
                double carbs = ui->foodListTableWidget->item(row, 5)->text().toDouble(&ok) * newNumServings;
                double fiber = ui->foodListTableWidget->item(row, 6)->text().toDouble(&ok) * newNumServings;
                double protein = ui->foodListTableWidget->item(row, 7)->text().toDouble(&ok) * newNumServings;

                ui->foodListTableWidget->item(row, 3)->setText(QString::number(calories));
                ui->foodListTableWidget->item(row, 4)->setText(QString::number(fats));
                ui->foodListTableWidget->item(row, 5)->setText(QString::number(carbs));
                ui->foodListTableWidget->item(row, 6)->setText(QString::number(fiber));
                ui->foodListTableWidget->item(row, 7)->setText(QString::number(protein));

                m_idFoodDataMap[iter->first]->setNumServings(numServings);
                m_idFoodDataMap[iter->first]->setCalories(calories);
                m_idFoodDataMap[iter->first]->setFats(fats);
                m_idFoodDataMap[iter->first]->setCarbs(carbs);
                m_idFoodDataMap[iter->first]->setFiber(fiber);
                m_idFoodDataMap[iter->first]->setProtein(protein);

                updateDailyTotal();
                m_dbManager->updateDailyFood(m_idFoodDataMap[iter->first]);

                break;
            }
            ++iter;
        }
    }
}

void MainWindow::on_foodListTableWidget_cellPressed(int row, int column)
{
    ui->deleteDailyFoodPushButton->setEnabled(true);
    m_selectedDailyFoodRow = row;
}

void MainWindow::on_deleteDailyFoodPushButton_clicked()
{
    int row = ui->foodListTableWidget->currentRow();

    QString name = ui->foodListTableWidget->item(row,1)->text();
    string id = "";

    QMessageBox msgBox;
    QString msg;
    msg += QString("Are you sure you want to delete ");
    msg += name;
    msg += "?";

    msgBox.setText(msg);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);

    int ret = msgBox.exec();
    if(ret == QMessageBox::Yes)
    {
        string selectedDate = getSelectedDate();
        std::map<std::string, int>::iterator iter = m_idRowMap.begin();
        while(iter != m_idRowMap.end())
        {
            if(iter->second == row && m_idFoodDataMap[iter->first]->getDate() == selectedDate)
            {
                id = m_idFoodDataMap[iter->first]->getID();
                m_dbManager->deleteDailyFood(QString::fromStdString(id));
                ui->foodListTableWidget->removeRow(row);
                ui->masterFoodTableWidget->setRowCount(ui->foodListTableWidget->rowCount() - 1);
            }
            else if(iter->second > row && m_idFoodDataMap[iter->first]->getDate() == selectedDate)
            {
                --(iter->second);
            }
            ++iter;
        }

        delete m_idFoodDataMap[id];
        m_idFoodDataMap.erase(id);

        m_idRowMap.erase(id);
     }
}

void MainWindow::on_mealsTabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        ui->masterFoodTableWidget->selectRow(0);
    }
    else if(index == 2)
    {
        ui->mealCreatorFoodList->selectRow(0);
    }
}

void MainWindow::on_addToMealArrowButton_clicked()
{
    int row = ui->mealCreatorFoodList->currentRow();
    bool ok = false;

    FoodData* newMealFood = new FoodData( ui->mealCreatorFoodList->item(row,0)->text().toStdString(),
                                          ui->mealCreatorFoodList->item(row,1)->text().toStdString(),
                                          1.0,
                                          ui->mealCreatorFoodList->item(row,2)->text().toDouble(&ok),
                                          ui->mealCreatorFoodList->item(row,3)->text().toDouble(&ok),
                                          ui->mealCreatorFoodList->item(row,4)->text().toDouble(&ok),
                                          ui->mealCreatorFoodList->item(row,5)->text().toDouble(&ok),
                                          ui->mealCreatorFoodList->item(row,6)->text().toDouble(&ok));

    populateNewMealRow(newMealFood);
    updateMealTotal();

    if(m_mealCreatorServings > 0.0 && m_mealCreatorName.size() > 0)
    {
        ui->addToFoodListMealCreatorPushButton->setEnabled(true);
    }
}

void MainWindow::on_mealCreatorNameLineEdit_textEdited(const QString &arg1)
{
    m_mealCreatorName = arg1;

    if(m_mealCreatorServings > 0.0 && ui->createdMealFoodList->rowCount() > 0)
    {
        ui->addToFoodListMealCreatorPushButton->setEnabled(true);
    }
}

void MainWindow::on_numServingsLineEdit_textEdited(const QString &arg1)
{
    bool ok = false;
    m_mealCreatorServings = arg1.toDouble(&ok);

    if(m_mealCreatorName.size() > 0 && ui->createdMealFoodList->rowCount() > 0)
    {
        ui->addToFoodListMealCreatorPushButton->setEnabled(true);
    }
}
