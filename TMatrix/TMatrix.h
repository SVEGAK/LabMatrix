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
        this->shrink_to_fit();
    }  
    TMatrix(std::initializer_list<std::initializer_list<T>> list) {
        if (list.size() == 0) { return; }
        size_t expected_cols = list.begin()->size();
        for (const auto& row_data : list) {
            if (row_data.size() != expected_cols) {
                throw std::invalid_argument("Все строки матрицы должны иметь одинаковую длину.");
            }
            TMathVector<T> row_vector(row_data);
            this->push_back(std::move(row_vector));
        }
        this->shrink_to_fit();
    }
    TMatrix(const TMatrix<T>& other) = default;         // конструктор копирования
    TMatrix(TMatrix<T>&& other) noexcept = default;     // конструктор с move-семантикой
    ~TMatrix() = default;

    // умножение на скаляр
    TMatrix<T>& operator*=(double val);
    TMatrix<T> operator*(double val) const;

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
        if (row >= this->size() || col >= (*this)[row].size()) {
            throw std::out_of_range("Matrix index out of bounds");
        }
        return (*this)[row][col];
    }

    const T& operator()(size_t row, size_t col) const {
        if (row >= this->size() || col >= (*this)[row].size()) {
            throw std::out_of_range("Matrix index out of bounds");
        }
        return (*this)[row][col];
    }

    // геттеры размеров
    size_t getRows() const { return this->size(); }
    size_t getCols() const { return this->size() > 0 ? (*this)[0].size() : 0; }
};




template <typename T>
TMatrix<T>& TMatrix<T>::operator*=(double val) {
    for (size_t i = 0; i < this->size(); i++) {
        // (*this)[i] возвращает ссылку на i-ю строку далее вызывается оператор *= TMathVector
        (*this)[i] *= val;
    }
    return *this;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(double val) const {
    TMatrix<T> result = *this;
    result *= val;
    return result;
}

template<typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix<T>& other) const
{
    if (this->getCols() != other.getRows()) {
        throw std::logic_error("Left operand columns count ought to be equal right operand rows count.");
    }
    TMatrix<T> result(this->getRows(), other.getCols());
    for (size_t res_row = 0; res_row < this->getRows(); res_row++) {
        for (size_t res_col = 0; res_col < other.getCols(); res_col++) {
            T sum = 0;
            for (size_t operand_i = 0; operand_i < this->getCols(); operand_i++) {
                sum += (*this)(res_row, operand_i) * other(operand_i, res_col);
            }
            result(res_row, res_col) = sum;
        }
    }
    return result;

}

template<typename T>
TMatrix<T>& TMatrix<T>::operator+=(const TMatrix<T>& other)
{
    if ((this->getCols() != other.getCols()) || (this->getRows() != other.getRows())) {
        throw std::logic_error("Left operand rows and columns count ought to be equal right operand.");
    }
    for (size_t row = 0; row < this->getRows(); row++) {
        for (size_t col = 0; col < this->getCols(); col++) {
            (*this)(row, col) += other(row, col);
        }
    }
    return (*this);

}
template<typename T>
inline TMatrix<T> TMatrix<T>::operator+(const TMatrix<T>& other) const
{
    TMatrix<T> result((*this));
    result += other;
    return result;
}

template<typename T>
inline TMatrix<T>& TMatrix<T>::operator-=(const TMatrix<T>& other)
{
    if ((this->getCols() != other.getCols()) || (this->getRows() != other.getRows())) {
        throw std::logic_error("Left operand rows and columns count ought to be equal right operand.");
    }
    for (size_t row = 0; row < this->getRows(); row++) {
        for (size_t col = 0; col < this->getCols(); col++) {
            (*this)(row, col) -= other(row, col);
        }
    }
    return (*this);
}

template<typename T>
inline TMatrix<T> TMatrix<T>::operator-(const TMatrix<T>& other) const
{
    TMatrix<T> result((*this));
    result -= other;
    return result;
}