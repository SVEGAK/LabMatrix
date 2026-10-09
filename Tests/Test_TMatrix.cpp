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


#endif