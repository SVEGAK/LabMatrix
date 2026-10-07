#pragma once
#include "TMathVector.h"
#include <exception>

template <typename T>
class TMatrix: public TMathVector<TMathVector<T>> {//Прямоугольная матрица
public:
    TMatrix(size_t rows = 0,size_t columns = 0) : TMathVector<TMathVector<T>>(rows,columns) {};// конструктор по размеру + по умолчанию
    TMatrix(size_t size, T* data) : TMathVector<TMathVector<T>>(size, data) {};                        // конструктор по размеру + по умолчанию 
    TMatrix(std::initializer_list<std::initializer_list<T>> list)
        : TMathVector<TMathVector<T>>(list) {};// конструктор по списку инициализации
    TMatrix(const TMatrix<T>& other) = default;         // конструктор копирования
    TMatrix(TMatrix<T>&& other) noexcept = default;     // конструктор с move-семантикой
    ~TMatrix() = default;

    // умножение на скаляр
    TMatrix<T> operator*(double val) const;
    TMatrix<T>& operator*=(double val);

    // умножение матрицы на матрицу
    TMatrix<T> operator*(const TMatrix<T>& other) const;

    // сложение
    TMatrix<T> operator+(const TMatrix<T>& other) const;
    TMatrix<T>& operator+=(const TMatrix<T>& other);

    // вычитание
    TMatrix<T> operator-(const TMatrix<T>& other) const;
    TMatrix<T>& operator-=(const TMatrix<T>& other);

    // операторы присваивания
    TMatrix<T>& operator=(const TMatrix<T>& other);
    TMatrix<T>& operator=(TMatrix<T>&& other) noexcept;

    // операторы сравнения
    bool operator==(const TMatrix<T>& other) const noexcept;
    bool operator!=(const TMatrix<T>& other) const noexcept;

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

    friend std::ostream& operator<<(std::ostream& os, const TMatrix<T>& m);
    friend std::istream& operator>>(std::istream& is, TMatrix<T>& m);
};


