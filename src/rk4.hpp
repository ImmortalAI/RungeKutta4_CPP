#ifndef RK4_H
#define RK4_H

#include "exprtk.hpp"
#include <string>
#include <type_traits>

template <typename T, typename Enable = std::enable_if_t<std::is_arithmetic_v<T>>>
class RungeKutta4
{
private:
    exprtk::symbol_table<T> *symbol_table;
    exprtk::expression<T> *expression;
    exprtk::parser<T> *parser;
    T k1, k2, k3, k4;
    T x, y, step, expr_x, expr_y;

public:
    RungeKutta4(std::string expr, T x0, T y0, T step);
    ~RungeKutta4();

    T makeStep(){
        expr_x = x;
        expr_y = y;
        k1 = expression->value();

        expr_x += step / 2;
        expr_y += step * (k1 / 2);
        k2 = expression->value();

        expr_y = y + step * (k2 / 2);
        k3 = expression->value();

        expr_x = x + step;
        expr_y = y + step * k3;
        k4 = expression->value();

        x += step;

        return (y += step / 6 * (k1 + 2 * k2 + 2 * k3 + k4));
    }

    inline void setStep(T newStep){ step = newStep; }
    inline void setInitialConditions(T newX, T newY){ x = newX; y = newY; }

    inline T getK1() const { return k1; }
    inline T getK2() const { return k2; }
    inline T getK3() const { return k3; }
    inline T getK4() const { return k4; }
    inline T getX() const { return x; }
    inline T getY() const { return y; }
};

template <typename T, typename Enable>
RungeKutta4<T, Enable>::RungeKutta4(std::string expr, T x0, T y0, T step){
    symbol_table = new exprtk::symbol_table<T>();
    expression = new exprtk::expression<T>();
    parser = new exprtk::parser<T>();

    symbol_table->add_variable("x", expr_x);
    symbol_table->add_variable("y", expr_y);
    expression->register_symbol_table(*symbol_table);

    if (!parser->compile(expr, *expression)) {
        throw std::runtime_error("Invalid expression");
    }

    this->x = x0;
    this->y = y0;
    this->step = step;
}

template <typename T, typename Enable>
RungeKutta4<T, Enable>::~RungeKutta4(){
    delete symbol_table;
    delete expression;
    delete parser;
}

#endif // RK4_H