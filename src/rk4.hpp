#ifndef RK4_H
#define RK4_H

#include "exprtk.hpp"
#include <string>
#include <vector>
#include <type_traits>

template <typename T, typename Enable = std::enable_if_t<std::is_floating_point_v<T>>>
class RungeKutta4
{
private:
    exprtk::symbol_table<T> symbol_table;
    exprtk::expression<T> expression;
    std::vector<T> k1, k2, k3, k4;
    T x, step, expr_x;
    std::vector<T> y_i, expr_y_i;

public:
    RungeKutta4(const std::string& expression, const T& x0, const std::vector<T>& y0_i, const T& step);
    ~RungeKutta4();

    T makeStep();

    inline void setStep(T newStep){ step = newStep; }
    inline void setInitialConditions(T newX0, std::vector<T>&& newY0){ x = newX0; y_i = newY0; }

    inline T getK1() const { return k1; }
    inline T getK2() const { return k2; }
    inline T getK3() const { return k3; }
    inline T getK4() const { return k4; }
    inline T getX() const { return x; }
    inline std::vector<T> getY() const { return y_i; }
};

template <typename T, typename Enable>
RungeKutta4<T, Enable>::RungeKutta4(const std::string& expression, const T& x0, const std::vector<T>& y0_i, const T& step){
    // Initialize exprtk objects
    this->symbol_table = exprtk::symbol_table<T>();
    this->expression = exprtk::expression<T>();
    exprtk::parser<T> parser;

    // Add variables to symbol table
    this->symbol_table.add_variable("x", this->expr_x);
    this->y_i = std::vector<T>(y0_i);
    for(uint i = 0; i < this->y_i.size(); i++){
        this->expr_y_i.push_back(y0_i.at(i));
        this->symbol_table.add_variable(std::string("y") + std::to_string(i), expr_y_i.at(i));
    }

    // Register variables and compile expression
    this->expression.register_symbol_table(this->symbol_table);
    if(!parser.compile(expression, this->expression)){
        throw std::runtime_error("Invalid expression");
    }

    // Post init
    this->x = x0;
    this->step = step;
}

template <typename T, typename Enable>
RungeKutta4<T, Enable>::~RungeKutta4(){
    // nothing to do here
}

template <typename T, typename Enable>
T RungeKutta4<T, Enable>::makeStep(){
    std::vector<T> newY_i;
    for(uint i = 0; i < this->y_i.size() - 1; i++){
        T iter_k1 = this->y_i.at(i+1);
        T iter_k2 = this->y_i.at(i+1) + this->step / 2 * iter_k1;
        T iter_k3 = this->y_i.at(i+1) + this->step / 2 * iter_k2;
        T iter_k4 = this->y_i.at(i+1) + this->step * iter_k3;
        newY_i.push_back(this->y_i.at(i) + this->step / 6 * (iter_k1 + 2 * iter_k2 + 2 * iter_k3 + iter_k4));
    }
    
    // Calculate k1
    this->expr_x = this->x;
    for(uint i = 0; i < this->expr_y_i.size(); i++){
        this->expr_y_i.at(i) = this->y_i.at(i);
    }
    T k1 = this->expression.value();

    // Calculate k2
    this->expr_x = this->x + this->step / 2;
    for(uint i = 0; i < this->expr_y_i.size(); i++){
        this->expr_y_i.at(i) = this->y_i.at(i) + this->step / 2 * k1;
    }
    T k2 = this->expression.value();

    // Calculate k3
    for(uint i = 0; i < this->expr_y_i.size(); i++){
        this->expr_y_i.at(i) = this->y_i.at(i) + this->step / 2 * k2;
    }
    T k3 = this->expression.value();

    // Calculate k4
    this->expr_x = this->x + this->step;
    for(uint i = 0; i < this->expr_y_i.size(); i++){
        this->expr_y_i.at(i) = this->y_i.at(i) + this->step * k3;
    }
    T k4 = this->expression.value();

    // Update y_i
    for(uint i = 0; i < newY_i.size(); i++){
        this->y_i.at(i) = newY_i.at(i);
    }
    this->y_i.at(this->y_i.size() - 1) += this->step / 6 * (k1 + 2*k2 + 2*k3 + k4);
    // Update x
    this->x += this->step;

    return this->y_i.at(0);
}

#endif // RK4_H