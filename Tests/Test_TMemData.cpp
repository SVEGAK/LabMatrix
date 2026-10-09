#include "pch.h"

#ifdef MEMDATA_TESTS
#include "TMemData.h"
#include <iostream>

TEST(FunctionsForMemData, calculate_capacity) {
    // Создаём объект с ненулевым размером, чтобы проверить ветку size <= _size
    TMemData<double> md = { 1.0, 2.0, 3.0 };   // _size=3, _capacity=3
    // size <= _size и >0 => возвращает size
    EXPECT_EQ(md.calculate_capacity(2), 2);
    EXPECT_EQ(md.calculate_capacity(3), 3);
    // size <= _size, но size=0 не входит (по условию size>0), идём дальше
    EXPECT_EQ(md.calculate_capacity(0), MEM_STEP); // 0 <= MEM_STEP -> MEM_STEP
    // size <= MEM_STEP -> MEM_STEP
    EXPECT_EQ(md.calculate_capacity(10), MEM_STEP);
    // size > MEM_STEP -> size + MEM_STEP
    EXPECT_EQ(md.calculate_capacity(20), 20 + MEM_STEP);
}

TEST(ClassMemData, can_move_assigment) {
    TMemData<double> md1 = { 1.0, 2.0, 3.0 };
    TMemData<double> md2;
    md2 = std::move(md1);

    EXPECT_EQ(md1.data(), nullptr);
    EXPECT_EQ(md1.size(), 0);
    EXPECT_EQ(md1.capacity(), 0);

    EXPECT_EQ(md2.size(), 3);
    EXPECT_EQ(md2.capacity(), 3);
    EXPECT_DOUBLE_EQ(md2.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md2.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md2.data()[2], 3.0);
}

TEST(ClassMemData, can_create_with_default_constructor) {
    TMemData<double> md;
    EXPECT_EQ(md.size(), 0);
    EXPECT_EQ(md.capacity(), 15);

}

TEST(ClassMemData, can_create_with_constructor_by_size) {
    TMemData<double> md(5);
    EXPECT_EQ(md.size(), 0);
    EXPECT_EQ(md.capacity(), 5);
}

TEST(ClassMemData, can_create_with_constructor_by_initializer_list) {
    TMemData<double> md = { 1.0, 3.4, 1.1 };
    EXPECT_EQ(md.size(), 3);
    EXPECT_EQ(md.capacity(), 3);
    EXPECT_DOUBLE_EQ(md.data()[1], 3.4);
}

TEST(ClassMemData, can_create_with_init_constructor) {
    double a[] = { 3.5, 11, 2.2 };
    TMemData<double> md(a, 3);
    EXPECT_EQ(md.size(), 3);
    EXPECT_EQ(md.capacity(), 3);
    EXPECT_DOUBLE_EQ(md.data()[1], 11.0);
}

TEST(ClassMemData, can_create_with_copy_constructor) {
    TMemData<double> md1 = { 1.0, 3.4, 1.1 };
    TMemData<double> md2(md1);
    EXPECT_EQ(md1.data()[1], md2.data()[1]);
    EXPECT_EQ(md2.size(), 3);
    EXPECT_EQ(md2.capacity(), 3);
}

TEST(ClassMemData, can_create_with_move_constructor) {
    TMemData<double> md1 = { 1.0, 3.4, 1.1 };
    TMemData<double> md2(std::move(md1));

    EXPECT_EQ(md1.data(), nullptr);
    EXPECT_DOUBLE_EQ(md2.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md2.data()[1], 3.4);
    EXPECT_DOUBLE_EQ(md2.data()[2], 1.1);
    EXPECT_EQ(md2.size(), 3);
    EXPECT_EQ(md2.capacity(), 3);

    EXPECT_TRUE(md1.is_empty());
}

TEST(ClassMemData, can_is_empty) {
    TMemData<double> md;
    EXPECT_TRUE(md.is_empty());
}

TEST(ClassMemData, can_is_full) {
    TMemData<double> md2 = { 1.0, 2.0, 3.0 };
    md2.clear_memory();
    EXPECT_EQ(md2.size(), 3);
    EXPECT_EQ(md2.capacity(), 3);
    EXPECT_TRUE(md2.is_full());
}

TEST(ClassMemData, can_set_memory_for_empty) {
    TMemData<double> md1;
    EXPECT_NO_THROW(md1.set_memory(md1.size()));
    EXPECT_NE(md1.data(), nullptr);
}

TEST(ClassMemData, can_set_memory_for_not_empty) {
    TMemData<double> md1(5);
    EXPECT_NO_THROW(md1.set_memory(md1.size()));
    EXPECT_NE(md1.data(), nullptr);
}

TEST(ClassMemData, can_set_memory_without_reallocation) {
    TMemData<double> md1 = { 1.0, 2.0, 3.0 };
    EXPECT_NO_THROW(md1.set_memory(md1.size()));
    EXPECT_NE(md1.data(), nullptr);
}

TEST(ClassMemData, can_reset_memory_for_empty) {
    TMemData<double> md;

    md.reset_memory(10, 0);

    EXPECT_EQ(md.capacity(), 15);
    EXPECT_NE(md.data(), nullptr);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_increase) {
    TMemData<double> md = { 1.0, 2.0, 3.0 };
    const double* old_data = md.data();

    md.reset_memory(10, 0);

    EXPECT_EQ(md.capacity(), 15);
    EXPECT_EQ(md.size(), 10);
    EXPECT_NE(md.data(), old_data);
    EXPECT_DOUBLE_EQ(md.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 3.0);
}

TEST(ClassMemData, can_reset_memory_for_not_empty_decrease) {
    TMemData<double> md = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    const double* old_data = md.data();

    md.reset_memory(3);

    EXPECT_EQ(md.capacity(), 3);
    EXPECT_EQ(md.size(), 3);
    EXPECT_NE(md.data(), old_data);
    EXPECT_DOUBLE_EQ(md.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 3.0);
}

TEST(ClassMemData, can_reset_memory_without_reallocation) {
    TMemData<double> md = { 1.0, 2.0, 3.0 };
    const double* old_data = md.data();
    size_t old_capacity = md.capacity();

    md.reset_memory(old_capacity);

    EXPECT_EQ(md.capacity(), old_capacity);
    EXPECT_NE(md.data(), old_data);
    EXPECT_DOUBLE_EQ(md.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 3.0);
}

TEST(ClassMemData, reset_memory_with_cap_calculation_false) {
    TMemData<double> md = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    size_t new_size = 3;
    md.reset_memory(new_size, 0, 0, false);

    EXPECT_EQ(md.size(), new_size);
    EXPECT_EQ(md.capacity(), new_size);
    EXPECT_DOUBLE_EQ(md.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 3.0);
}

TEST(ClassMemData, reset_memory_with_cap_calculation_true) {
    TMemData<double> md = { 1.0, 2.0, 3.0 };

    size_t new_size = 5;
    md.reset_memory(new_size, 0, 0, true);

    EXPECT_EQ(md.size(), new_size);
    EXPECT_GT(md.capacity(), new_size);
    EXPECT_DOUBLE_EQ(md.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 3.0);
}

TEST(ClassMemData, reset_memory_with_start_index_and_cap_false) {
    TMemData<double> md = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    size_t new_size = 3;
    md.reset_memory(new_size, 2, 0, false);

    EXPECT_EQ(md.size(), new_size);
    EXPECT_EQ(md.capacity(), new_size);
    EXPECT_DOUBLE_EQ(md.data()[0], 3.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 4.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 5.0);
}

TEST(ClassMemData, can_reset_memory_with_shift) {
    TMemData<double> md = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    const double* old_data = md.data();

    md.reset_memory(3, 2);  // start_index = 2

    EXPECT_EQ(md.capacity(), 3);//Запас все равно создается
    EXPECT_EQ(md.size(), 3);
    EXPECT_NE(md.data(), old_data);
    EXPECT_DOUBLE_EQ(md.data()[0], 3.0);
    EXPECT_DOUBLE_EQ(md.data()[1], 4.0);
    EXPECT_DOUBLE_EQ(md.data()[2], 5.0);
}

TEST(ClassMemData, can_clear_memory_for_empty) {
    TMemData<double> md1;
    md1.clear_memory();
    EXPECT_EQ(md1.data(), nullptr);
}

TEST(ClassMemData, can_clear_memory_for_not_empty) {
    TMemData<double> md1 = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    md1.clear_memory();
    EXPECT_EQ(md1.data(), nullptr);
}

TEST(ClassMemData, can_assigment) {
    TMemData<double> md1 = { 1.0, 2.0, 3.0 };
    TMemData<double> md2;

    md2 = md1;

    EXPECT_EQ(md2.size(), 3);
    EXPECT_NE(md2.data(), md1.data());
    EXPECT_DOUBLE_EQ(md2.data()[0], 1.0);
    EXPECT_DOUBLE_EQ(md2.data()[1], 2.0);
    EXPECT_DOUBLE_EQ(md2.data()[2], 3.0);
}



#endif