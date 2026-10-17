#pragma once
#include "TMathVector.h"
template <typename T>
class TTriangleMatrix : private TMathVector<TMathVector<T>> {
    public:
        TTriangleMatrix(size_t rows = 0, size_t columns = 0);
        TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list);
        TTriangleMatrix(size_t rows,size_t columns, T** data);
        TTriangleMatrix(const TMathVector<T>& other) = default;        // конструктор копирования
        TTriangleMatrix(TMathVector<T>&& other) noexcept = default;   // конструктор с move-семантикой
        ~TTriangleMatrix() = default;

        using TMathVector<T>::size();
        using TMathVector<T>::capcity();
        using TMathVector<T>::shrink_to_fit();
        using TMathVector<T>::operator=();      // операторы присваивания

        // умножение на скаляр
        TTriangleMatrix<T>& operator*=(double val);
        TTriangleMatrix<T> operator*(double val) const;

        // умножение матрицы на матрицу
        TTriangleMatrix<T> operator*(const TTriangleMatrix<T>& other) const;

        // сложение
        TTriangleMatrix<T>& operator+=(const TTriangleMatrix<T>& other);
        TTriangleMatrix<T> operator+(const TTriangleMatrix<T>& other) const;

        // вычитание
        TTriangleMatrix<T>& operator-=(const TTriangleMatrix<T>& other);
        TTriangleMatrix<T> operator-(const TTriangleMatrix<T>& other) const;

        // оператор доступа к элементу
        T& operator()(size_t row, size_t col);
        const T& operator()(size_t row, size_t col) const;

        // геттеры размеров
        size_t getRows() const { return this->size(); }
        size_t getCols() const { return this->size() > 0 ? (*this)[0].size() : 0; }
};

template<typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns)
{
    for (size_t i = 0; i < n; ++i) {
        // Верхнетреугольная: в строке i храним элементы с i-го по (n-1)-й
        TMathVector<Q> row(n - i,i);
        this->push_back(std::move(row));
    }
    this->shrink_to_fit();
}

template<typename T>
TTriangleMatrix<T>::TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list)
{
    if (list.size() == 0) { return; }
    size_t max_cols = list.begin()->size();
    size_t i = 0;
    for (const auto& row_list : list) {
        if (i >= max_cols) {
            break;//обрезаем если строк больше чем столбцов
        }
        TMathVector<T> row(max_cols - i, i);
        size_t j = 0;
        for (const auto& val : row_list) {
            if (j >= max_cols) break; // если строка длиннее max_cols - обрезаем

            if (j >= i) {
                row[j - i] = val;
            }
            ++j;
        }
        this->push_back(std::move(row));
        ++i;
    }
    this->shrink_to_fit();
}

template<typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns, T** data)
{
    if (data == nullptr || rows == 0 || columns == 0) { return; }

    for (size_t i = 0; i < rows; ++i) {
        if (i >= columns) {
            break;
        }

        TMathVector<T> row(columns - i, i);

        for (size_t j = i; j < columns; ++j) {
            row[j - i] = data[i][j];
        }

        row.shrink_to_fit();
        this->push_back(std::move(row));
    }
    this->shrink_to_fit();
}

