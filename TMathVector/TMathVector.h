#pragma once

#include "TVector.h"
#include "TMemData.h"
#include <exception>
#include <initializer_list>

template <typename T>
class TMathVector: public TVector<T>{
    size_t _start_index;
public:
    TMathVector(size_t size = 0);                                 // конструктор по размеру + по умолчанию
    TMathVector(size_t size, T* data);                           // конструктор по размеру + по умолчанию 
    TMathVector(std::initializer_list<T> list);                      // конструктор по списку инициализации
    TMathVector(const TMathVector<T>& other) = default;        // конструктор копирования
    TMathVector(TMathVector<T>&& other) noexcept = default;   // конструктор с move-семантикой
    ~TMathVector() = default;

    TMathVector<T> operator*(double val) const;                  //Умножение на скаляр
    TMathVector<T>& operator*=(double val);                      //Умножение на скаляр

    T operator*(const TMathVector<T>& other) const;              //Скалярное умножение на вектор

    TMathVector<T> operator+(const TMathVector<T> &other) const; //Сложение векторов
    TMathVector<T>& operator+=(const TMathVector<T>& other);     //Сложение векторов

    TMathVector<T> operator-(const TMathVector<T>& other) const; //Вычитание векторов
    TMathVector<T>& operator-=(const TMathVector<T>& other);     //Вычитание векторов

    TMathVector<T>& operator=(const TMathVector<T>& other) noexcept;           // оператор присваивания
    TMathVector<T>& operator=(TMathVector<T>&& other) noexcept;                // оператор присваивания с move-семантикой

    bool operator==(const TMathVector<T>& other) const noexcept;              // оператор сравнения
    bool operator!=(const TMathVector<T>& other) const noexcept;              // оператор сравнения

    friend std::ostream& operator<<(std::ostream& os, const TMathVector<T>& v) {
        os << static_cast<const TVector<T>&>(v);
        return os;
    }
    friend std::istream& operator>>(std::istream& is, TMathVector<T>& v) {
        is >> static_cast<TVector<T>&>(v);
        is >> v._start_index;
        v.shrink_to_fit();
        return is;
    }
};

template<typename T>
inline TMathVector<T>::TMathVector(size_t size): TVector<T>(size)
{
    (*this).shrink_to_fit();
    _start_index = 0;
}

template<typename T>
TMathVector<T>::TMathVector(size_t size, T* data): TVector<T>(data,size)
{
    (*this).shrink_to_fit();
    _start_index = 0;
}

template<typename T>
TMathVector<T>::TMathVector(std::initializer_list<T> list): TVector<T>(list)
{
    _start_index = 0;
    (*this).shrink_to_fit();
}

template<typename T>
TMathVector<T> TMathVector<T>::operator*(double val) const
{
    TMathVector<T> copy((*this));
    for (size_t i = 0; i < (*this).size(); i++) {
        copy[i] *= val;
    }
    return copy;
}

template<typename T>
TMathVector<T>& TMathVector<T>::operator*=(double val)
{
    for (size_t i = 0; i < (*this).size(); i++) {
        (*this)[i] *= val;
    }
    return (*this);
}

template<typename T>
T TMathVector<T>::operator*(const TMathVector<T>& other) const
{
    if ((*this).size() == other.size()) {
        T result = 0;
        for (size_t i = 0; i < (*this).size(); i++) {
            result += (*this)[i]* other[i];
        }
        return result;
    }
    throw std::invalid_argument("Vectors sizes are not equal.");
}

template<typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector<T>& other) const
{
    TMathVector<T> copy((*this));
    return copy += (other);
}

template<typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector<T>& other)
{
    if ((*this).size() == other.size()) {
        for (size_t i = 0; i < (*this).size(); i++) {
            (*this)[i] += other[i];
        }
        return (*this);
    }
    throw std::invalid_argument("Vectors sizes are not equal.");
}

template<typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector<T>& other) const
{
    TMathVector<T> copy((*this));
    return copy -= (other);
}

template<typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector<T>& other)
{
    if ((*this).size() == other.size()) {
        for (size_t i = 0; i < (*this).size(); i++) {
            (*this)[i] -= other[i];
        }
        return (*this);
    }
    throw std::invalid_argument("Vectors sizes are not equal.");
}

template<typename T>
TMathVector<T>& TMathVector<T>::operator=(const TMathVector<T>&other) noexcept
{
    if (this != &other) {
        TVector<T>::operator=(other);
        _start_index = other._start_index;
    }
    return *this;
}

template<typename T>
TMathVector<T>& TMathVector<T>::operator=(TMathVector<T>&& other) noexcept
{
    if (this != &other) {
        TVector<T>::operator=(std::move(other));

        _start_index = other._start_index;
        other._start_index = 0;
    }
    return *this;
}

template<typename T>
inline bool TMathVector<T>::operator==(const TMathVector<T>& other) const noexcept
{
    if ((*this).size() != other.size()) return false;
    for (size_t i = 0; i < (*this).size(); i++) {
        if ((*this)[i] != other[i]) return false;
    }
    return true;
}

template<typename T>
inline bool TMathVector<T>::operator!=(const TMathVector<T>& other) const noexcept
{
    return !(*this == other);
}


