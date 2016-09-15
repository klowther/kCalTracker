#include "AddFoodFormDialog.h"
#include "ui_AddFoodFormDialog.h"

#include <QDebug>
#include <QIntValidator>

AddFoodFormDialog::AddFoodFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFoodFormDialog)
{
    ui->setupUi(this);

    ui->caloriesLineEdit->setValidator(new QIntValidator);
    ui->fatsLineEdit->setValidator(new QIntValidator);
    ui->carbsLineEdit->setValidator(new QIntValidator);
    ui->fiberLineEdit->setValidator(new QIntValidator);
    ui->proteinLineEdit->setValidator(new QIntValidator);

    setTabOrder(ui->nameLineEdit, ui->servingLineEdit);
    setTabOrder(ui->servingLineEdit, ui->caloriesLineEdit);
    setTabOrder(ui->caloriesLineEdit, ui->fatsLineEdit);
    setTabOrder(ui->fatsLineEdit, ui->carbsLineEdit);
    setTabOrder(ui->carbsLineEdit, ui->fiberLineEdit);
    setTabOrder(ui->fiberLineEdit, ui->proteinLineEdit);
    setTabOrder(ui->proteinLineEdit, ui->nameLineEdit);

    ui->nameLineEdit->setFocus();
}

AddFoodFormDialog::~AddFoodFormDialog()
{
    delete ui;
}

FoodData *AddFoodFormDialog::getNewFood() const
{
    return m_newFood;
}

void AddFoodFormDialog::addFoodToBeEdited(const FoodData *selectedFood)
{
    ui->nameLineEdit->setText((QString::fromStdString(selectedFood->getName())));
    ui->servingLineEdit->setText((QString::fromStdString(selectedFood->getServing())));
    ui->caloriesLineEdit->setText((QString::number(selectedFood->getCalories())));
    ui->fatsLineEdit->setText((QString::number(selectedFood->getFats())));
    ui->carbsLineEdit->setText((QString::number(selectedFood->getCarbs())));
    ui->fiberLineEdit->setText((QString::number(selectedFood->getFiber())));
    ui->proteinLineEdit->setText((QString::number(selectedFood->getProtein())));
}

void AddFoodFormDialog::on_buttonBox_accepted()
{
    bool ok = false;
    FoodData* newFoodItem = new FoodData(ui->nameLineEdit->text().toStdString(),
                                         ui->servingLineEdit->text().toStdString(),
                                         ui->caloriesLineEdit->text().toDouble(&ok),
                                         ui->fatsLineEdit->text().toDouble(&ok),
                                         ui->carbsLineEdit->text().toDouble(&ok),
                                         ui->fiberLineEdit->text().toDouble(&ok),
                                         ui->proteinLineEdit->text().toDouble(&ok));


    m_newFood = newFoodItem;
}
