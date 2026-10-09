#pragma once
#include "TMathVector.h"
#include <exception>
#include <iostream>

template <typename T>
class TMatrix: public TMathVector<TMathVector<T>> {//Прямоугольная матрица
public:
    TMatrix(size_t rows = 0, size_t columns = 0) {// конструктор по размеру + по умолчанию 
        for (size_t i = 0; i < rows; ++i) {
            this->push_back(TMathVector<T>(columns));
        }
    }  
    TMatrix(std::initializer_list<std::initializer_list<T>> list) {
        if (list.size() == 0) { return; }
        size_t expected_cols = list.begin()->size();
        for (const auto& row_data : list) {
            if (row_data.size() != expected_cols) {
                throw std::invalid_argument("Все строки матрицы должны иметь одинаковую длину.");
            }
            this->push_back(TMathVector<T>(row_data));
        }
    }
    TMatrix(const TMatrix<T>& other) = default;         // конструктор копирования
    TMatrix(TMatrix<T>&& other) noexcept = default;     // конструктор с move-семантикой
    ~TMatrix() = default;

    // умножение на скаляр
    TMatrix<T> operator*(double val) const;
    TMatrix<T>& operator*=(double val);

    // умножение матрицы на матрицу
    TMatrix<T> operator*(const TMatrix<T>& other) const;

    // сложение
    TMatrix<T>& operator+=(const TMatrix<T>& other);
    TMatrix<T> operator+(const TMatrix<T>& other) const;

    // вычитание
    TMatrix<T>& operator-=(const TMatrix<T>& other);
    TMatrix<T> operator-(const TMatrix<T>& other) const;

    // оператор доступа к элементу
    T& operator()(size_t row, size_t col) {
        return (*this)[row][col];
    }

    const T& operator()(size_t row, size_t col) const {
        return (*this)[row][col];
    }

    // геттеры размеров
    size_t getRows() const { return this->size(); }
    size_t getCols() const { return this->size() > 0 ? (*this)[0].size() : 0; }
};




