#include "pch.h"

#ifdef TTRIANGLEMATRIX_TESTS
#include "TTriangleMatrix.h"

TEST(TTriangleMatrixConstructors, SizeConstructorAndGetters) {
    // Квадратная матрица
    TTriangleMatrix<int> mat1(3, 3);
    EXPECT_EQ(mat1.getRows(), 3);
    EXPECT_EQ(mat1.getCols(), 3);
    EXPECT_EQ(mat1(0, 0), 0);
    EXPECT_EQ(mat1(2, 2), 0);

    // Прямоугольная(2x4)
    TTriangleMatrix<int> mat2(2, 4);
    EXPECT_EQ(mat2.getRows(), 2);
    EXPECT_EQ(mat2.getCols(), 4);

    // Прямоугольная(4x2)
    TTriangleMatrix<int> mat3(4, 2);
    EXPECT_EQ(mat3.getRows(), 2);
    EXPECT_EQ(mat3.getCols(), 2);

    TTriangleMatrix<int> mat4(0, 0);
    EXPECT_EQ(mat4.getRows(), 0);
    EXPECT_EQ(mat4.getCols(), 0);
}

TEST(TTriangleMatrixConstructors, InitializerListConstructor) {
    TTriangleMatrix<int> mat({
        { 3, 4, 3, 0, 0 },
        { 0, 4, 3, 5, 1 },
        { 0, 0, 3, 5, 1 },
        { 0, 0, 0, 5, 1 },
        { 0, 0, 0, 0, 1 }
        });

    EXPECT_EQ(mat.getRows(), 5);
    EXPECT_EQ(mat.getCols(), 5);

    // Проверка значений над диагональю и на ней
    EXPECT_EQ(mat(0, 0), 3);
    EXPECT_EQ(mat(0, 1), 4);
    EXPECT_EQ(mat(1, 1), 4);
    EXPECT_EQ(mat(1, 4), 1);
    EXPECT_EQ(mat(2, 3), 5);
    EXPECT_EQ(mat(4, 4), 1);

    //под диагональю возвращаются нули
    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(2, 0), 0);
    EXPECT_EQ(mat(4, 1), 0);
}

TEST(TTriangleMatrixConstructors, InitializerListIgnoresBelowDiagonal) {
    TTriangleMatrix<int> mat({
        { 1, 2, 3 },
        { 9, 4, 5 }, // 9 под диагональю - должно быть проигнорировано
        { 8, 7, 6 }  // 8 и 7 под диагональю - проигнорированы
        });

    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(2, 0), 0);
    EXPECT_EQ(mat(2, 1), 0);

    // Значения на диагонали и выше сохранились
    EXPECT_EQ(mat(1, 1), 4);
    EXPECT_EQ(mat(2, 2), 6);
}

TEST(TTriangleMatrixConstructors, RawArrayConstructor) {

    int** data = new int* [3];
    data[0] = new int[3] { 1, 2, 3 };
    data[1] = new int[3] { 4, 5, 6 };
    data[2] = new int[3] { 7, 8, 9 };

    TTriangleMatrix<int> mat(3, 3, data);

    EXPECT_EQ(mat.getRows(), 3);
    EXPECT_EQ(mat.getCols(), 3);


    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 2), 3);
    EXPECT_EQ(mat(1, 1), 5);
    EXPECT_EQ(mat(2, 2), 9);


    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(2, 0), 0);
    EXPECT_EQ(mat(2, 1), 0);

    for (int i = 0; i < 3; ++i) delete[] data[i];
    delete[] data;
}

TEST(TTriangleMatrixConstructors, NullDataThrows) {

    EXPECT_THROW(TTriangleMatrix<int>(3, 3, nullptr), std::invalid_argument);

    EXPECT_NO_THROW(TTriangleMatrix<int>(0, 0, nullptr));
}

#endif