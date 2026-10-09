#include "pch.h"

#ifdef TMATRIX_TESTS
#include "TMatrix.h"

//Унаследованные от TMathVector методы

TEST(TMatrixInherited, SizeAndCapacityReflectRows) {
    // size() и capacity()
    TMatrix<double> m(3, 4);

    EXPECT_EQ(m.size(), 3);      
    EXPECT_GE(m.capacity(), 3);
}

TEST(TMatrixInherited, RowAccessViaBracketOperator) {
    // operator[] унаследован и возвращает целую строку (TMathVector<T>)
    TMatrix<int> m = { {1, 2, 3}, {4, 5, 6} };

    EXPECT_EQ(m[0].size(), 3);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[0][2], 3);

    m[1][1] = 99;
    EXPECT_EQ(m[1][1], 99);
    EXPECT_EQ(m(1, 1), 99); 
}

TEST(TMatrixInherited, EqualityAndInequalityOperators) {
    // operator== и operator!= унаследованы и сравнивают вектор векторов поэлементно
    TMatrix<double> m1 = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> m2 = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> m3 = { {1.0, 2.0}, {3.0, 5.0} }; // Отличается один элемент
    TMatrix<double> m4 = { {1.0, 2.0} };              // Другое количество строк

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 == m3);
    EXPECT_FALSE(m1 == m4);

    EXPECT_TRUE(m1 != m3);
    EXPECT_TRUE(m1 != m4);
}

TEST(TMatrixInherited, StreamOutputOperator) {
    // operator<< унаследован. TMathVector выводит себя как "{ a, b, c }"
    TMatrix<int> m = { {1, 2}, {3, 4} };
    std::ostringstream oss;
    oss << m;

    std::string output = oss.str();
    EXPECT_TRUE(output.find("1") != std::string::npos);
    EXPECT_TRUE(output.find("2") != std::string::npos);
    EXPECT_TRUE(output.find("3") != std::string::npos);
    EXPECT_TRUE(output.find("4") != std::string::npos);
    // Ожидаем наличие фигурных скобок от TMathVector
    EXPECT_TRUE(output.find("{") != std::string::npos);
}

TEST(TMatrixInherited, StreamInputOperator) {
    TMatrix<int> m;
    std::istringstream iss("2 2 10 20 0 2 30 40 0");
    // Расшифровка: 2 строки. Первая: размер 2, элементы 10, 20, start_idex = 0. Вторая: размер 2, элементы 30, 40, start_idex = 0.

    iss >> m;

    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 2);
    EXPECT_EQ(m[0][0], 10);
    EXPECT_EQ(m[0][1], 20);
    EXPECT_EQ(m[1][0], 30);
    EXPECT_EQ(m[1][1], 40);
}

//Тесты не унаследованных методов

TEST(TMatrixOwn, ScalarMultiplication) {
    TMatrix<double> m = { {1.0, 2.0}, {3.0, 4.0} };

    // operator*
    TMatrix<double> result = m * 2.0;
    EXPECT_DOUBLE_EQ(result(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 6.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 8.0);

    // Исходная матрица не изменилась
    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);

    // operator*=
    m *= 3.0;
    EXPECT_DOUBLE_EQ(m(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(m(0, 1), 6.0);
    EXPECT_DOUBLE_EQ(m(1, 0), 9.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 12.0);
}
TEST(TMatrixOwn, MatrixMultiplication) {
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} };//почему capacity = 15?
    TMatrix<double> b = { {5.0, 6.0}, {7.0, 8.0} };

     //a * b = [[1*5+2*7, 1*6+2*8], [3*5+4*7, 3*6+4*8]] = [[19, 22], [43, 50]]
    TMatrix<double> result = a * b;
    EXPECT_DOUBLE_EQ(result(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 50.0);

}

TEST(TMatrixOwn, MatrixMultiplicationNonSquare) {
    TMatrix<double> a = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} }; // 2x3
    TMatrix<double> b = { {7.0, 8.0}, {9.0, 10.0}, {11.0, 12.0} }; // 3x2

    // Результат должен быть 2x2
    TMatrix<double> result = a * b;
    EXPECT_EQ(result.getRows(), 2);
    EXPECT_EQ(result.getCols(), 2);
    EXPECT_DOUBLE_EQ(result(0, 0), 58.0);  // 1*7+2*9+3*11
    EXPECT_DOUBLE_EQ(result(0, 1), 64.0);  // 1*8+2*10+3*12
    EXPECT_DOUBLE_EQ(result(1, 0), 139.0); // 4*7+5*9+6*11
    EXPECT_DOUBLE_EQ(result(1, 1), 154.0); // 4*8+5*10+6*12
}

TEST(TMatrixOwn, MatrixMultiplicationThrowsOnSizeMismatch) {
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} }; // 2x2
    TMatrix<double> b = { {1.0, 2.0, 3.0} }; // 1x3

    EXPECT_THROW(a * b, std::logic_error);
}

TEST(TMatrixOwn, Addition) {
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> b = { {5.0, 6.0}, {7.0, 8.0} };

    // operator+
    TMatrix<double> result = a + b;
    EXPECT_DOUBLE_EQ(result(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 12.0);

    // Исходные матрицы не изменились
    EXPECT_DOUBLE_EQ(a(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(b(0, 0), 5.0);

    // operator+=
    a += b;
    EXPECT_DOUBLE_EQ(a(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(a(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(a(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(a(1, 1), 12.0);
}

TEST(TMatrixOwn, AdditionThrowsOnSizeMismatch) {
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} }; // 2x2
    TMatrix<double> b = { {1.0, 2.0, 3.0} }; // 1x3

    EXPECT_THROW(a + b, std::logic_error);
    EXPECT_THROW(a += b, std::logic_error);
}

TEST(TMatrixOwn, Subtraction) {
    TMatrix<double> a = { {5.0, 6.0}, {7.0, 8.0} };
    TMatrix<double> b = { {1.0, 2.0}, {3.0, 4.0} };

    // operator-
    TMatrix<double> result = a - b;
    EXPECT_DOUBLE_EQ(result(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 4.0);

    // Исходные матрицы не изменились
    EXPECT_DOUBLE_EQ(a(0, 0), 5.0);

    // operator-=
    a -= b;
    EXPECT_DOUBLE_EQ(a(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(a(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(a(1, 0), 4.0);
    EXPECT_DOUBLE_EQ(a(1, 1), 4.0);
}

TEST(TMatrixOwn, SubtractionThrowsOnSizeMismatch) {
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} }; // 2x2
    TMatrix<double> b = { {1.0, 2.0, 3.0} }; // 1x3

    EXPECT_THROW(a - b, std::logic_error);
    EXPECT_THROW(a -= b, std::logic_error);
}

TEST(TMatrixOwn, ChainedOperations) {
    // Проверка цепочек операций 
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> b = { {5.0, 6.0}, {7.0, 8.0} };
    TMatrix<double> c = { {9.0, 10.0}, {11.0, 12.0} };

    // a += b += c должно работать
    a += (b += c);
    EXPECT_DOUBLE_EQ(a(0, 0), 15.0); // 1 + (5+9)
    EXPECT_DOUBLE_EQ(a(0, 1), 18.0); // 2 + (6+10)
    EXPECT_DOUBLE_EQ(a(1, 0), 21.0); // 3 + (7+11)
    EXPECT_DOUBLE_EQ(a(1, 1), 24.0); // 4 + (8+12)
}

TEST(TMatrixOwn, ZeroMatrixMultiplication) {
    // Умножение на нулевую матрицу должно давать нулевую матрицу
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> zero(2, 2); // Нулевая матрица 2x2

    TMatrix<double> result = a * zero;
    EXPECT_DOUBLE_EQ(result(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 0.0);
}

TEST(TMatrixOwn, IdentityMatrixMultiplication) {
    // Умножение на единичную матрицу не должно изменять матрицу
    TMatrix<double> a = { {1.0, 2.0}, {3.0, 4.0} };
    TMatrix<double> identity = { {1.0, 0.0}, {0.0, 1.0} };

    TMatrix<double> result = a * identity;
    EXPECT_DOUBLE_EQ(result(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(result(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(result(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(result(1, 1), 4.0);
}

#endif