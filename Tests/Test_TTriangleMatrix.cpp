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
    EXPECT_EQ(mat3.getRows(), 4);
    EXPECT_EQ(mat3.getCols(), 2);

    TTriangleMatrix<int> mat4(0, 0);
    EXPECT_EQ(mat4.getRows(), 0);
    EXPECT_EQ(mat4.getCols(), 0);
}

TEST(TTriangleMatrixConstructors, InitializerListConstructor) {
    TTriangleMatrix<int> mat({
        { 1, 2, 3, 4, 5 },
        { 0, 6, 7, 8, 9 },
        { 0, 0, 10, 11, 12 },
        { 0, 0, 0, 13, 14 },
        { 0, 0, 0, 0, 15 }
        });

    EXPECT_EQ(mat.getRows(), 5);
    EXPECT_EQ(mat.getCols(), 5);

    // Проверка значений над диагональю и на ней
    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 1), 2);
    EXPECT_EQ(mat(1, 1), 6);
    EXPECT_EQ(mat(1, 4), 9);
    EXPECT_EQ(mat(2, 3), 11);
    EXPECT_EQ(mat(4, 4), 15);

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

TEST(TTriangleMatrixCore, GettersAndInheritedMethods) {
    TTriangleMatrix<int> mat(3, 3);

    EXPECT_EQ(mat.getRows(), 3);
    EXPECT_EQ(mat.getCols(), 3);

    EXPECT_EQ(mat.size(), 3);
    EXPECT_GE(mat.capacity(), 3);

    mat.shrink_to_fit();
    EXPECT_EQ(mat.capacity(), 3);

    TTriangleMatrix<int> mat2(0, 0);
    mat2 = mat;

    EXPECT_EQ(mat2.getRows(), 3);
    EXPECT_EQ(mat2.getCols(), 3);
    EXPECT_EQ(mat2(0, 0), 0);

    TTriangleMatrix<int> mat3(0, 0);
    mat3 = std::move(mat2);
    EXPECT_EQ(mat3.getRows(), 3);
}

TEST(TTriangleMatrixCore, ElementAccessAndExceptions) {
    TTriangleMatrix<int> mat(3, 3);

    mat(0, 0) = 1;
    mat(0, 2) = 3;
    mat(1, 1) = 5;
    mat(2, 2) = 9;

    EXPECT_EQ(mat(0, 0), 1);
    EXPECT_EQ(mat(0, 2), 3);
    EXPECT_EQ(mat(1, 1), 5);
    EXPECT_EQ(mat(2, 2), 9);

    EXPECT_THROW(mat(3, 0), std::out_of_range);
    EXPECT_THROW(mat(0, 3), std::out_of_range);

    EXPECT_EQ(mat(1, 0), 0);
    EXPECT_EQ(mat(2, 1), 0);
}

TEST(TTriangleMatrixMath, ScalarArithmetic) {
    TTriangleMatrix<int> mat({
        {1, 2},
        {0, 3}
        });

    mat *= 2;
    EXPECT_EQ(mat(0, 0), 2);
    EXPECT_EQ(mat(0, 1), 4);
    EXPECT_EQ(mat(1, 1), 6);

    TTriangleMatrix<int> mat2 = mat * 3;
    EXPECT_EQ(mat2(0, 0), 6);
    EXPECT_EQ(mat2(0, 1), 12);
    EXPECT_EQ(mat2(1, 1), 18);

    EXPECT_EQ(mat(0, 0), 2);
}

TEST(TTriangleMatrixMath, AdditionAndSubtraction) {
    TTriangleMatrix<int> A({
        {1, 2},
        {0, 3}
        });
    TTriangleMatrix<int> B({
        {10, 20},
        {0,  30}
        });

    A += B;
    EXPECT_EQ(A(0, 0), 11);
    EXPECT_EQ(A(0, 1), 22);
    EXPECT_EQ(A(1, 1), 33);

    A -= B;
    EXPECT_EQ(A(0, 0), 1);
    EXPECT_EQ(A(1, 1), 3);

    TTriangleMatrix<int> C = A + B;
    EXPECT_EQ(C(0, 0), 11);

    TTriangleMatrix<int> D = B - A;
    EXPECT_EQ(D(0, 0), 9);
    EXPECT_EQ(D(1, 1), 27);

    TTriangleMatrix<int> E(3, 3);
    EXPECT_THROW(A += E, std::invalid_argument);
    EXPECT_THROW(A - E, std::invalid_argument);
}

TEST(TTriangleMatrixMath, MatrixMultiplication) {
    TTriangleMatrix<int> A({
        {1, 2},
        {0, 3}
        });
    TTriangleMatrix<int> B({
        {4, 5},
        {0, 6}
        });

    TTriangleMatrix<int> C = A * B;

    EXPECT_EQ(C.getRows(), 2);
    EXPECT_EQ(C.getCols(), 2);

    EXPECT_EQ(C(0, 0), 4);
    EXPECT_EQ(C(0, 1), 17);
    EXPECT_EQ(C(1, 1), 18);

    TTriangleMatrix<int> D(2, 3);
    EXPECT_THROW(A * D, std::logic_error);
}
#endif