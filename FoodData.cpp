#include "FoodData.h"

FoodData::FoodData()
{
    m_id = "";
    m_name = "";
    m_date = "";
    m_numServings = 1.0;
    m_serving = "";
    m_calories = 0.0;
    m_fats = 0.0;
    m_carbs = 0.0;
    m_fiber = 0.0;
    m_protein = 0.0;
}

FoodData::FoodData(const std::string &id, const std::__1::string &name, const std::__1::string &serving, const std::__1::string &date, double numServings, double calories, double fats, double carbs, double fiber, double protein) :
        m_id(id), m_name(name), m_serving(serving), m_date(date), m_numServings(numServings), m_calories(calories), m_fats(fats), m_carbs(carbs), m_fiber(fiber), m_protein(protein)
{

}

FoodData::FoodData(const std::__1::string &name, const std::__1::string &serving, double calories, double fats, double carbs, double fiber, double protein) :
    m_name(name), m_serving(serving), m_calories(calories), m_fats(fats), m_carbs(carbs), m_fiber(fiber), m_protein(protein)
{
    m_id = "";
    m_date = "";
    m_numServings = 1.0;
}
