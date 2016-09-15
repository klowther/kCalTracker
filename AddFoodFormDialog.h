#ifndef ADDFOODFORMDIALOG_H
#define ADDFOODFORMDIALOG_H

#include <QDialog>
#include <FoodData.h>

namespace Ui {
class AddFoodFormDialog;
}

class AddFoodFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFoodFormDialog(QWidget *parent = 0);
    ~AddFoodFormDialog();

    FoodData* getNewFood() const;
    void addFoodToBeEdited(const FoodData* selectedFood);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddFoodFormDialog *ui;
    FoodData* m_newFood;
};

#endif // ADDFOODFORMDIALOG_H
