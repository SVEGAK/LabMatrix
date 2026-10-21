#pragma once
#include "TMathVector.h"
template <typename T>
class TTriangleMatrix : private TMathVector<TMathVector<T>> {
    public:
        TTriangleMatrix(size_t rows = 0, size_t columns = 0);
        TTriangleMatrix(std::initializer_list<std::initializer_list<T>> list);
        TTriangleMatrix(size_t rows,size_t columns, T** data);
        TTriangleMatrix(const TTriangleMatrix<T>& other) = default;        // конструктор копирования
        TTriangleMatrix(TTriangleMatrix<T>&& other) noexcept = default;   // конструктор с move-семантикой
        ~TTriangleMatrix() = default;

        using TMathVector<TMathVector<T>>::size;
        using TMathVector<TMathVector<T>>::capacity;
        using TMathVector<TMathVector<T>>::shrink_to_fit;

        TTriangleMatrix& operator=(const TTriangleMatrix& other) = default;
        TTriangleMatrix& operator=(TTriangleMatrix&& other) noexcept = default;

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

        friend std::ostream& operator<<(std::ostream& os, const TTriangleMatrix<T>& mat) {
            for (size_t i = 0; i < mat.getRows(); i++) {
                os << "[ ";
                for (size_t j = 0; j < mat.getCols(); j++) {
                    os << mat(i, j) << " ";
                }
                os << "]\n";
            }
            return os;
        }

        friend std::istream& operator>>(std::istream& is, TTriangleMatrix<T>& mat) {
            size_t rows, columns;
            is >> rows >> columns;
            mat = TTriangleMatrix<T>(rows, columns);
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < columns; j++) {
                    T val;
                    is >> val;
                    mat(i, j) = val; // если j < i, оператор () просто проигнорирует запись
                }
            }
            return is;
        }
};

template<typename T>
TTriangleMatrix<T>::TTriangleMatrix(size_t rows, size_t columns)
{
    for (size_t i = 0; i < rows; ++i) {
        size_t row_size = (i <= columns) ? (columns - i) : 0;
        TMathVector<T> row(row_size, i);
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

template<typename T>
TTriangleMatrix<T>& TTriangleMatrix<T>::operator*=(double val) {
    TMathVector<TMathVector<T>>::operator*=(val);
    return *this;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator*(double val) const {
    TTriangleMatrix<T> copy(*this);
    copy *= val;
    return copy;
}

template<typename T>
inline TTriangleMatrix<T> TTriangleMatrix<T>::operator*(const TTriangleMatrix<T>& other) const
{
    if (this->getCols() != other.getRows()) {
        throw std::logic_error("Left operand columns count ought to be equal right operand rows count.");
    }
    TTriangleMatrix<T> result(this->getRows(), other.getCols());

    for (size_t res_row = 0; res_row < this->getRows(); res_row++) {
        for (size_t res_col = res_row; res_col < other.getCols(); res_col++) { 
            T sum = 0;
            for (size_t operand_i = res_row; operand_i <= res_col; operand_i++) {
                sum += (*this)(res_row, operand_i) * other(operand_i, res_col);
            }
            result(res_row, res_col) = sum;
        }
    }
    return result;
}

template<typename T>
TTriangleMatrix<T>& TTriangleMatrix<T>::operator+=(const TTriangleMatrix<T>& other) {
    // Проверяем, что размеры матриц совпадают
    if (this->size() != other.size()) {
        throw std::invalid_argument("Matrix dimensions must match.");
    }
    TMathVector<TMathVector<T>>::operator+=(other);
    return *this;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator+(const TTriangleMatrix<T>& other) const {
    TTriangleMatrix<T> copy(*this);
    copy += other;
    return copy;
}

template<typename T>
TTriangleMatrix<T>& TTriangleMatrix<T>::operator-=(const TTriangleMatrix<T>& other) {
    if (this->size() != other.size()) {
        throw std::invalid_argument("Matrix dimensions must match.");
    }
    TMathVector<TMathVector<T>>::operator-=(other);
    return *this;
}

template<typename T>
TTriangleMatrix<T> TTriangleMatrix<T>::operator-(const TTriangleMatrix<T>& other) const {
    TTriangleMatrix<T> copy(*this);
    copy -= other;
    return copy;
}

template<typename T>
T& TTriangleMatrix<T>::operator()(size_t row, size_t col)
{
    if (row >= getRows() || col >= getCols()) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    if (col < row) {
        // возвращаем ссылку на глобальную переменную-пустышку
        static T dummy = 0;
        dummy = 0; // сбрасываем её на случай, если кто-то туда писал
        return dummy; 
    }
    return (*this)[row][col-row];
}

template<typename T>
const T& TTriangleMatrix<T>::operator()(size_t row, size_t col) const
{
    if (row >= getRows() || col >= getCols()) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    if (col < row) {
        // возвращаем ссылку на глобальную переменную-пустышку
        static T dummy = 0;
        dummy = 0; // сбрасываем её на случай, если кто-то туда писал
        return dummy;
    }
    return (*this)[row][col-row];
}

    