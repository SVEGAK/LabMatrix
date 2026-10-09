#include "pch.h"
#ifdef TMATHVECTOR_TESTS

#include "TMathVector.h"
#include <sstream>

TEST(TMathVector, constructors_maintain_size_equals_capacity) {
    TMathVector<double> v1(5);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);

    TMathVector<double> v2 = { 1.0, 2.0, 3.0, 4.0 };
    EXPECT_EQ(v2.size(), 4);
    EXPECT_EQ(v2.capacity(), 4);
}

TEST(TMathVector, scalar_multiplication) {
    TMathVector<double> v1 = { 1.0, 2.0, 3.0 };

    TMathVector<double> v2 = v1 * 2.0;
    EXPECT_EQ(v2.size(), v2.capacity());
    EXPECT_DOUBLE_EQ(v2[0], 2.0);
    EXPECT_DOUBLE_EQ(v2[1], 4.0);
    EXPECT_DOUBLE_EQ(v2[2], 6.0);

    v1 *= 3.0;
    EXPECT_EQ(v1.size(), v1.capacity());
    EXPECT_DOUBLE_EQ(v1[0], 3.0);
    EXPECT_DOUBLE_EQ(v1[1], 6.0);
    EXPECT_DOUBLE_EQ(v1[2], 9.0);
}

TEST(TMathVector, dot_product) {
    TMathVector<double> v1 = { 1.0, 2.0, 3.0 };
    TMathVector<double> v2 = { 4.0, 5.0, 6.0 };

    double result = v1 * v2;
    EXPECT_DOUBLE_EQ(result, 32.0);
}

TEST(TMathVector, dot_product_throws_on_size_mismatch) {
    TMathVector<double> v1 = { 1.0, 2.0 };
    TMathVector<double> v2 = { 1.0, 2.0, 3.0 };

    EXPECT_THROW(v1 * v2, std::invalid_argument);
}

TEST(TMathVector, vector_addition_and_subtraction) {
    TMathVector<double> v1 = { 1.0, 2.0, 3.0 };
    TMathVector<double> v2 = { 4.0, 5.0, 6.0 };

    TMathVector<double> v3 = v1 + v2;
    EXPECT_EQ(v3.size(), v3.capacity());
    EXPECT_DOUBLE_EQ(v3[0], 5.0);
    EXPECT_DOUBLE_EQ(v3[1], 7.0);
    EXPECT_DOUBLE_EQ(v3[2], 9.0);

    v1 += v2;
    EXPECT_EQ(v1.size(), v1.capacity());
    EXPECT_DOUBLE_EQ(v1[0], 5.0);

    TMathVector<double> v4 = v2 - v1;
    EXPECT_EQ(v4.size(), v4.capacity());
    EXPECT_DOUBLE_EQ(v4[0], -1.0);
    EXPECT_DOUBLE_EQ(v4[1], -2.0);
    EXPECT_DOUBLE_EQ(v4[2], -3.0);

    v2 -= v1;
    EXPECT_EQ(v2.size(), v2.capacity());
    EXPECT_DOUBLE_EQ(v2[0], -1.0);
}

TEST(TMathVector, assignment_operators) {
    TMathVector<double> v1 = { 1.0, 2.0, 3.0 };

    TMathVector<double> v2;
    v2 = v1;
    EXPECT_EQ(v2.size(), v2.capacity());
    EXPECT_EQ(v2.size(), 3);
    EXPECT_DOUBLE_EQ(v2[0], 1.0);

    TMathVector<double> v3 = { 4.0, 5.0 };
    v2 = std::move(v3);
    EXPECT_EQ(v2.size(), v2.capacity());
    EXPECT_EQ(v2.size(), 2);
    EXPECT_DOUBLE_EQ(v2[0], 4.0);

    EXPECT_EQ(v3.size(), 0);
    EXPECT_EQ(v3.capacity(), 0);
}

TEST(TMathVector, comparison_operators) {
    TMathVector<double> v1 = { 1.0, 2.0, 3.0 };
    TMathVector<double> v2 = { 1.0, 2.0, 3.0 };
    TMathVector<double> v3 = { 1.0, 2.0, 4.0 };
    TMathVector<double> v4 = { 1.0, 2.0 };

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);

    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);
}

TEST(TMathVector, stream_operators_and_shrink_to_fit_invariant) {
    TMathVector<double> v1;

    std::istringstream in("3 1.5 2.5 3.5 10");
    in >> v1;

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1.capacity(), 3);

    std::ostringstream out;
    out << v1;
    EXPECT_EQ(out.str(), "{ 1.5, 2.5, 3.5 }");
}

TEST(TMathVector, arithmetic_operations_maintain_invariant_after_reallocation) {
    TMathVector<double> v1 = { 1.0, 2.0 };
    TMathVector<double> v2 = { 3.0, 4.0, 5.0 };

    TMathVector<double> v3(5);
    v3 = { 1.0, 2.0, 3.0 };

    EXPECT_THROW(v1 += v3, std::invalid_argument);
}

#endif