#ifndef FOODDATA_H
#define FOODDATA_H

#include <string>

class FoodData
{
public:
    FoodData();
    FoodData(const std::string& id, const std::string& name, const std::string& serving, const std::string& date, double numServings, double calories, double fats, double carbs, double fiber, double protein);
    FoodData(const std::string& name, const std::string& serving, double calories, double fats, double carbs, double fiber, double protein);

    void setID(const std::string& id) { m_id = id;}
    std::string getID() const { return m_id; }

    void setName(const std::string& name) { m_name = name; }
    std::string getName() const { return m_name; }

    void setServing(const std::string& serving) { m_serving = serving; }
    std::string getServing() const { return m_serving; }

    void setNumServings(double numServings) { m_numServings = numServings; }
    double getNumServings() const { return m_numServings; }

    void setCalories(double cals) { m_calories = cals; }
    double getCalories() const { return m_calories; }

    void setFats(double fats) { m_fats = fats; }
    double getFats() const { return m_fats; }

    void setCarbs(double carbs) { m_carbs = carbs; }
    double getCarbs() const { return m_carbs; }

    void setFiber(double fiber) { m_fiber = fiber; }
    double getFiber() const { return m_fiber; }

    void setProtein(double protein) { m_protein = protein; }
    double getProtein() const { return m_protein; }

    void setDate(std::string date) { m_date = date; }
    std::string getDate() const { return m_date; }

private:
    std::string m_id;
    std::string m_name;
    std::string m_serving;
    std::string m_date;
    double m_numServings;
    double m_calories;
    double m_fats;
    double m_carbs;
    double m_fiber;
    double m_protein;
};

#endif // FOODDATA_H
