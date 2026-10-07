#pragma once
#include "pch.h"
//#define MEMDATA_TESTS
//#define VECTOR_TESTS
#define TMATHVECTOR_TESTS

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


#ifdef VECTOR_TESTS
#include "TVector.h"

TEST(ClassVector, can_create_with_default_constructor) {
    TVector<double> v1;

    EXPECT_EQ(v1.front_pos(), 0);
    EXPECT_EQ(v1.back_pos(), 0);
    EXPECT_EQ(v1.is_empty(), 1);
}

TEST(ClassVector, can_create_with_constructor_by_size) {
    TVector<double> v1(5);

    EXPECT_EQ(v1.front_pos(), 0);
    EXPECT_EQ(v1.back_pos(), 4);
    EXPECT_EQ(v1.is_empty(), true);
}

TEST(ClassVector, can_create_with_constructor_by_initializer_list) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1.capacity(), 3);
    EXPECT_EQ(v1.front_pos(), 0);
    EXPECT_DOUBLE_EQ(v1.front(), 1.0);
}

TEST(ClassVector, can_create_with_init_constructor) {
    double mass[] = { 1.0, 3.4, 1.1 };
    TVector<double> v1(mass, 3);
    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1.capacity(), 3);
    EXPECT_DOUBLE_EQ(v1.front(), 1.0);
}

TEST(ClassVector, can_create_with_copy_constructor) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    TVector<double> v2(v1);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.capacity(), 3);
    EXPECT_EQ(v1.front_pos(), 0);
    EXPECT_EQ(v2.front_pos(), 0);
    EXPECT_DOUBLE_EQ(v2.front(), 1.0);
}

TEST(ClassVector, can_create_with_move_constructor) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    TVector<double> v2(std::move(v1));
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v2.capacity(), 3);
    EXPECT_DOUBLE_EQ(v2.front(), 1.0);
}

TEST(ClassVector, can_is_empty) {
    TVector<double> v1;
    EXPECT_TRUE(v1.is_empty());
}

TEST(ClassVector, can_is_full) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_TRUE(v1.is_full());
}

TEST(ClassVector, can_get_front) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_EQ(v1.front(), 1.0);
}

TEST(ClassVector, can_get_back) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_EQ(v1.back_pos(), 2);
    EXPECT_EQ(v1.back(), 1.1);
}

TEST(ClassVector, can_set_front) {
    TVector<double> v = { 1.0, 2.0, 3.0 };

    v.set_front() = 10.0;

    EXPECT_DOUBLE_EQ(v.front(), 10.0);
    EXPECT_DOUBLE_EQ(v[0], 10.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);

    TVector<double> v2(5);
    for (int i = 0; i < 5; i++) {
        v2.push_back(i * 1.0);
    }
    v2.set_front() = 99.0;
    EXPECT_DOUBLE_EQ(v2.front(), 99.0);
    EXPECT_DOUBLE_EQ(v2[0], 99.0);
}

TEST(ClassVector, can_set_back) {
    TVector<double> v = { 1.0, 2.0, 3.0 };

    v.set_back() = 30.0;

    EXPECT_DOUBLE_EQ(v.back(), 30.0);
    EXPECT_DOUBLE_EQ(v[2], 30.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);

    TVector<double> v2 = { 5.0 };
    v2.set_back() = 50.0;
    EXPECT_DOUBLE_EQ(v2.front(), 50.0);
    EXPECT_DOUBLE_EQ(v2.back(), 50.0);

    TVector<double> v3(10);
    for (int i = 0; i < 10; i++) {
        v3.push_back(i * 1.0);
    }
    v3.set_back() = 99.0;
    EXPECT_DOUBLE_EQ(v3.back(), 99.0);
    EXPECT_DOUBLE_EQ(v3[v3.size() - 1], 99.0);
    EXPECT_DOUBLE_EQ(v3.back_pos(), 9);
}

TEST(ClassVector, throw_when_try_get_front_in_empty_vector) {
    TVector<double> v1;
    EXPECT_THROW(v1.front(), std::out_of_range);
}

TEST(ClassVector, throw_when_try_get_back_in_empty_vector) {
    TVector<double> v1;
    EXPECT_THROW(v1.back(), std::out_of_range);
}

TEST(ClassVector, throw_when_try_set_front_in_empty_vector) {
    TVector<double> v;

    EXPECT_THROW(v.set_front() = 1.0, std::out_of_range);
}

TEST(ClassVector, throw_when_try_set_back_in_empty_vector) {
    TVector<double> v;
    EXPECT_THROW(v.set_back() = 1.0, std::out_of_range);
}
TEST(ClassVector, can_push_front) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    v1.push_front(2.5);
    EXPECT_EQ(v1.front(), 2.5);
}

TEST(ClassVector, can_push_front_with_reallocation) {
    TVector<double> v = { 1.0, 2.0, 3.0 };      // capacity=3, full
    v.push_front(0.0);               // должна произойти реаллокация с FRONT_BUFFER

    EXPECT_EQ(v.size(), 4);
    EXPECT_GE(v.capacity(), 4);
    EXPECT_EQ(v.front_pos(), FRONT_BUFFER - 1); // front сместился на буфер
    EXPECT_DOUBLE_EQ(v.front(), 0.0);
    EXPECT_DOUBLE_EQ(v[0], 0.0);
    EXPECT_DOUBLE_EQ(v[1], 1.0);
    EXPECT_DOUBLE_EQ(v[2], 2.0);
    EXPECT_DOUBLE_EQ(v[3], 3.0);
}

TEST(ClassVector, can_push_front_in_empty_vector) {
    TVector<double> v1; // capacity = 0
    v1.push_front(2.5);
    EXPECT_EQ(v1.front(), 2.5);
    EXPECT_EQ(v1.front_pos(), 0);
    EXPECT_EQ(v1.capacity(), MEM_STEP); // в set_memory выделяется буфер - создается массив с позиции FRONT_BUFFER
}


TEST(ClassVector, can_push_back) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    v1.push_back(2.5);
    EXPECT_EQ(v1.back(), 2.5);
    EXPECT_EQ(v1.back_pos(), 3 + FRONT_BUFFER);
}

TEST(ClassVector, can_push_back_in_empty_vector) {
    TVector<double> v1;
    v1.push_back(2.5);
    EXPECT_EQ(v1.back(), 2.5);
    EXPECT_EQ(v1.back_pos(), 0);
    EXPECT_EQ(v1.capacity(), MEM_STEP);
}
TEST(ClassVector, can_push_back_with_reallocation) {
    TVector<double> v = { 1.0, 2.0, 3.0 };      // capacity=3, full
    v.push_back(4.0);                 // должна произойти реаллокация с FRONT_BUFFER

    EXPECT_EQ(v.size(), 4);
    EXPECT_GE(v.capacity(), 4);
    EXPECT_EQ(v.front_pos(), FRONT_BUFFER); // начало сдвинуто на буфер
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 4.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
}
TEST(ClassVector, can_insert) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    v1.insert(2.5, 2);
    EXPECT_EQ(v1.capacity(), 15);
    EXPECT_EQ(v1.size(), 4);
}

TEST(ClassVector, can_insert_to_front) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_EQ(v1.front_pos(), 0);
    v1.insert(2.5, v1.front_pos());
    EXPECT_EQ(v1.front_pos(), FRONT_BUFFER);
    EXPECT_EQ(v1.capacity(), 15);
    EXPECT_EQ(v1.size(), 4);
}
TEST(ClassVector, can_insert_with_reallocation) {
    TVector<double> v = { 1.0, 2.0, 3.0 };      // capacity=3, full
    v.insert(99.0, 1);                // вставка в середину требует реаллокации

    EXPECT_EQ(v.size(), 4);
    EXPECT_GE(v.capacity(), 4);
    EXPECT_EQ(v.front_pos(), FRONT_BUFFER); // данные выровнены с буфером
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 99.0);
    EXPECT_DOUBLE_EQ(v[2], 2.0);
    EXPECT_DOUBLE_EQ(v[3], 3.0);
}
TEST(ClassVector, throw_when_try_insert_with_wrong_position) {
    TVector<double> v1 = { 1.0, 3.4, 1.1 };
    EXPECT_THROW(v1.insert(2.5, 4), std::out_of_range);
    TVector<double> v2;
    EXPECT_THROW(v2.insert(2.5, 1), std::out_of_range);
    TVector<double> v3 = { 1.0, 3.4, 1.1 };
    EXPECT_THROW(v3.insert(2.5, -1), std::out_of_range);

}

TEST(ClassVector, can_pop_front) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    EXPECT_EQ(v.size(), 5);
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 5.0);

    v.pop_front();

    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v.front(), 2.0);
    EXPECT_DOUBLE_EQ(v.back(), 5.0);


    v.pop_front();

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v.front(), 3.0);
    EXPECT_DOUBLE_EQ(v.back(), 5.0);
}

TEST(ClassVector, can_pop_front_with_reallocation) {
    TVector<double> v(5);
    EXPECT_EQ(v.size(), 0);

    for (int i = 0; i < 5; i++) {
        v.push_back(i * 1.0);
    }
    EXPECT_EQ(v.size(), 5);

    size_t old_capacity = v.capacity();


    v.pop_front();
    v.pop_front();
    v.pop_front();

    EXPECT_EQ(v.size(), 2);
    EXPECT_DOUBLE_EQ(v.front(), 3.0);
    EXPECT_DOUBLE_EQ(v.back(), 4.0);


    EXPECT_GE(v.capacity(), v.size());
}

TEST(ClassVector, throw_when_try_pop_front_from_empty_vector) {
    TVector<double> v;

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_THROW(v.pop_front(), std::out_of_range);
}

TEST(ClassVector, can_pop_back) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    EXPECT_EQ(v.size(), 5);
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 5.0);


    v.pop_back();

    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 4.0);


    v.pop_back();

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 3.0);
}

TEST(ClassVector, can_pop_back_with_reallocation) {

    TVector<double> v(10);

    for (int i = 0; i < 10; i++) {
        v.push_back(i * 1.0);
    }

    EXPECT_EQ(v.size(), 10);
    size_t old_capacity = v.capacity();


    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();

    EXPECT_EQ(v.size(), 5);
    EXPECT_DOUBLE_EQ(v.front(), 0.0);
    EXPECT_DOUBLE_EQ(v.back(), 4.0);


    EXPECT_GE(v.capacity(), v.size());
}

TEST(ClassVector, throw_when_try_pop_back_from_empty_vector) {
    TVector<double> v;

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);


    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

TEST(ClassVector, pop_front_back_to_empty) {
    TVector<double> v = { 1.0, 2.0, 3.0 };

    v.pop_front();
    v.pop_back();
    v.pop_front();

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(ClassVector, pop_single_element) {
    TVector<double> v = { 42.0 };

    EXPECT_EQ(v.size(), 1);
    EXPECT_DOUBLE_EQ(v.front(), 42.0);
    EXPECT_DOUBLE_EQ(v.back(), 42.0);

    v.pop_front();

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(ClassVector, pop_front_back_alternating) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    v.pop_front();  // {2, 3, 4, 5}
    v.pop_back();   // {2, 3, 4}
    v.pop_front();  // {3, 4}
    v.pop_back();   // {3}
    v.pop_front();  // {}

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(ClassVector, can_correctly_recalc_back_in_area_of_zero) {
    TVector<double> vec;
    for (size_t i = 0; i < 15; i++) {
        vec.push_back(i + 1);
    }
    EXPECT_EQ(15, vec.capacity());
    vec.pop_front();
    vec.push_back(16);

    EXPECT_EQ(15, vec.size());
    EXPECT_EQ(15, vec.capacity());
    EXPECT_DOUBLE_EQ(16.0, vec.back());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }

    vec.pop_back();

    EXPECT_EQ(14, vec.size());
    EXPECT_EQ(15, vec.capacity());
    EXPECT_DOUBLE_EQ(15.0, vec.back());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 2);
    }
}
TEST(ClassVector, can_correctly_recalc_front_in_area_of_zero) {
    TVector<double> v(10);
    for (int i = 0; i < 10; i++) v.push_back(i * 1.0);

    for (int i = 0; i < 10; i++) v.pop_front();

    EXPECT_TRUE(v.is_empty());

    for (int i = 10; i < 20; i++) v.push_back(i * 1.0);

    EXPECT_EQ(v.size(), 10);
    EXPECT_DOUBLE_EQ(v.front(), 10.0);
    EXPECT_DOUBLE_EQ(v.back(), 19.0);
}

TEST(ClassVector, can_erase) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v.capacity(), 5);
    v.erase(2);

    EXPECT_EQ(v.size(), 4);
    for (size_t i = -2; i < v.size(); i++) {
        EXPECT_DOUBLE_EQ(v[i + v.front()], 0);
    }
}

TEST(ClassVector, can_erase_front) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    v.erase(0);

    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v.front(), 2.0);
    EXPECT_DOUBLE_EQ(v.back(), 5.0);
    EXPECT_DOUBLE_EQ(v[0], 2.0);
    EXPECT_DOUBLE_EQ(v[v.size() - 1], 5.0);
}

TEST(ClassVector, can_erase_back) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    EXPECT_EQ(v.size(), 5);
    v.erase(4);
    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v.front(), 1.0);
    EXPECT_DOUBLE_EQ(v.back(), 4.0);


}

TEST(ClassVector, can_erase_with_reallocation) {
    TVector<double> v(5);
    for (int i = 0; i < 5; i++) {
        v.push_back(i * 1.0);
    }
    v.erase(0);
    v.erase(0);
    v.erase(0);
    //for (size_t i = 0; i < v.size();i++) {
    //    EXPECT_DOUBLE_EQ(v[i],0);
    //}
    EXPECT_DOUBLE_EQ(v.back_pos(), 1);
    EXPECT_EQ(v.size(), 2);
    EXPECT_DOUBLE_EQ(v.front(), 3);
    EXPECT_DOUBLE_EQ(v.back(), 4);
    EXPECT_GE(v.capacity(), v.size());
}

TEST(ClassVector, throw_when_try_erase_from_empty_vector) {
    TVector<double> v;

    EXPECT_THROW(v.erase(0), std::out_of_range);
}

TEST(ClassVector, throw_when_try_erase_with_wrong_position) {
    TVector<double> v = { 1.0, 2.0, 3.0 };

    EXPECT_THROW(v.erase(3), std::out_of_range);
    EXPECT_THROW(v.erase(100), std::out_of_range);
}

TEST(ClassVector, can_assigment) {
    TVector<double> v1 = { 1.0, 2.0, 3.0 };
    TVector<double> v2;

    v2 = v1;

    EXPECT_EQ(v2.size(), v1.size());
    EXPECT_EQ(v2.capacity(), v1.capacity());
    EXPECT_DOUBLE_EQ(v2[0], 1.0);
    EXPECT_DOUBLE_EQ(v2[1], 2.0);
    EXPECT_DOUBLE_EQ(v2[2], 3.0);
    EXPECT_DOUBLE_EQ(v2.front(), 1.0);
    EXPECT_DOUBLE_EQ(v2.back(), 3.0);

    v1.push_back(4.0);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1.size(), 4);

    TVector<double> v3 = { 10.0, 20.0 };
    v2 = v3;
    EXPECT_EQ(v2.size(), 2);
    EXPECT_DOUBLE_EQ(v2[0], 10.0);
    EXPECT_DOUBLE_EQ(v2[1], 20.0);

    TVector<double> v4;
    v4 = v4;
    EXPECT_TRUE(v4.is_empty());

    TVector<double> v5 = { 5.0, 6.0, 7.0 };
    const TVector<double>& v5_ref = v5;
    TVector<double> v6;
    v6 = v5_ref;
    EXPECT_EQ(v6.size(), 3);
    EXPECT_DOUBLE_EQ(v6[0], 5.0);
}


TEST(ClassVector, can_move_assigment) {
    TVector<double> vec_1;
    TVector<double> vec_2;

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_back(5 + i);
    }

    for (size_t i = 0; i < 4; i++) {
        vec_1.push_front(4 - i);
    }

    vec_2 = std::move(vec_1);

    EXPECT_EQ(0, vec_1.size());
    EXPECT_EQ(0, vec_1.capacity());

    EXPECT_EQ(8, vec_2.size());
    EXPECT_EQ(15, vec_2.capacity());
    EXPECT_EQ(1, vec_2.front_pos());
    for (size_t i = 0; i < vec_2.size(); i++) {
        EXPECT_EQ(vec_2[i], i + 1);
    }
}

TEST(ClassVector, can_output_with_operator_cout) {
    TVector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }", out.str());
}

TEST(ClassVector, can_input_with_operator_cin) {
    TVector<double> vec({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    std::stringstream in("9 1 2 3 4 5 6 7 8 9");
    in >> vec;

    EXPECT_EQ(9, vec.size());
    EXPECT_EQ(15, vec.capacity());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}
TEST(ClassVector, combination_push_pop_insert_erase) {
    TVector<double> vec({ 3, 44, 5, 7, 8 });

    std::stringstream out;
    out << vec;
    EXPECT_EQ("{ 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_front();
    out << vec;
    EXPECT_EQ("{ 44, 5, 7, 8 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_front(3 - i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8 }", out.str());
    out.str("");

    vec.pop_back();
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7 }", out.str());
    out.str("");

    for (size_t i = 0; i < 4; i++) {
        vec.push_back(8 + i);
    }
    out << vec;
    EXPECT_EQ("{ 0, 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(0);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 44, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.erase(3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    vec.insert(6, 4);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11 }", out.str());
    out.str("");

    for (size_t i = 0; i < 5; i++) {
        vec.push_back(12 + i);
    }
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");
    vec.insert(4, 3);
    out << vec;
    EXPECT_EQ("{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }", out.str());
    out.str("");

    EXPECT_EQ(16, vec.size());
    EXPECT_EQ(31, vec.capacity());

    for (size_t i = 0; i < vec.size(); i++) {
        EXPECT_DOUBLE_EQ(vec[i], i + 1);
    }
}
TEST(ClassVector, can_push_back_n_empty) {
    TVector<double> v;
    double vals[] = { 1.0, 2.0, 3.0 };
    v.push_back_n(vals, 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), MEM_STEP);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_EQ(v.front_pos(), 0);
    EXPECT_EQ(v.back_pos(), 2);
}

TEST(ClassVector, can_push_back_n_no_realloc) {
    TVector<double> v(10);                     // capacity = 15
    v.push_back(10.0);                // size=1, back=0
    double vals[] = { 20.0, 30.0 };
    v.push_back_n(vals, 2);           // должно хватить места без реаллокации

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 15);
    EXPECT_DOUBLE_EQ(v[0], 10.0);
    EXPECT_DOUBLE_EQ(v[1], 20.0);
    EXPECT_DOUBLE_EQ(v[2], 30.0);
    EXPECT_EQ(v.front_pos(), 0);
    EXPECT_EQ(v.back_pos(), 2);
}

TEST(ClassVector, can_push_back_n_with_reallocation) {
    TVector<double> v = { 1.0, 2.0, 3.0 };       // capacity = 3, full
    double vals[] = { 4.0, 5.0, 6.0 };
    size_t old_cap = v.capacity();
    v.push_back_n(vals, 3);

    EXPECT_EQ(v.size(), 6);
    EXPECT_GE(v.capacity(), 6);
    EXPECT_EQ(v.front_pos(), FRONT_BUFFER); // после реаллокации с буфером
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
    EXPECT_DOUBLE_EQ(v[4], 5.0);
    EXPECT_DOUBLE_EQ(v[5], 6.0);
}

// push_front_n
TEST(ClassVector, can_push_front_n_empty) {
    TVector<double> v;
    double vals[] = { 3.0, 2.0, 1.0 };
    v.push_front_n(vals, 3);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), MEM_STEP);
    EXPECT_DOUBLE_EQ(v[0], 3.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 1.0);
    EXPECT_EQ(v.front_pos(), 0);
    EXPECT_EQ(v.back_pos(), 2);
}

TEST(ClassVector, can_push_front_n_no_realloc) {
    TVector<double> v(10);
    v.push_front(10.0);               // front=9, back=9 (после push_front для size=0 вызывает push_when_empty, затем front=0)
    // Сбросим в нормальное состояние: v = {10} с front=0
    // Легче создать готовый вектор с местом спереди:
    TVector<double> v2(10);
    v2.push_back(1.0);
    v2.push_back(2.0);
    v2.push_back(3.0);                // сейчас front=0, back=2
    double vals[] = { -2.0, -1.0, 0.0 };
    v2.push_front_n(vals, 3);         // front сдвинется на 3 назад

    EXPECT_EQ(v2.size(), 6);
    EXPECT_DOUBLE_EQ(v2[0], -2.0);
    EXPECT_DOUBLE_EQ(v2[1], -1.0);
    EXPECT_DOUBLE_EQ(v2[2], 0.0);
    EXPECT_DOUBLE_EQ(v2[3], 1.0);
    EXPECT_DOUBLE_EQ(v2[4], 2.0);
    EXPECT_DOUBLE_EQ(v2[5], 3.0);
    EXPECT_EQ(v2.front_pos(), 4);
}

TEST(ClassVector, can_push_front_n_with_reallocation) {
    TVector<double> v = { 1.0, 2.0, 3.0 };       // capacity=3, full
    double vals[] = { -1.0, 0.0 };
    v.push_front_n(vals, 2);

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v.front_pos(), 4);
    EXPECT_DOUBLE_EQ(v[0], -1.0);
    EXPECT_DOUBLE_EQ(v[1], 0.0);
    EXPECT_DOUBLE_EQ(v[2], 1.0);
    EXPECT_DOUBLE_EQ(v[3], 2.0);
    EXPECT_DOUBLE_EQ(v[4], 3.0);
}

// insert_n
TEST(ClassVector, can_insert_n_middle) {
    TVector<double> v = { 1.0, 5.0, 6.0 };
    double vals[] = { 2.0, 3.0, 4.0 };
    v.insert_n(1, vals, 3);           // вставляем после 1.0

    EXPECT_EQ(v.size(), 6);
    EXPECT_GE(v.capacity(), 6);
    EXPECT_EQ(v.front_pos(), FRONT_BUFFER);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
    EXPECT_DOUBLE_EQ(v[4], 5.0);
    EXPECT_DOUBLE_EQ(v[5], 6.0);
}

TEST(ClassVector, can_insert_n_at_front) {
    TVector<double> v = { 3.0, 4.0 };
    double vals[] = { 1.0, 2.0 };
    v.insert_n(0, vals, 2);           // то же, что push_front_n

    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
}

TEST(ClassVector, can_insert_n_at_back) {
    TVector<double> v = { 1.0, 2.0 };
    double vals[] = { 3.0, 4.0 };
    v.insert_n(2, vals, 2);           // то же, что push_back_n

    EXPECT_EQ(v.size(), 4);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
}

TEST(ClassVector, throw_when_insert_n_wrong_position) {
    TVector<double> v = { 1.0, 2.0, 3.0 };
    double vals[] = { 0.0 };

    EXPECT_THROW(v.insert_n(4, vals, 1), std::out_of_range);   // pos > size()
    EXPECT_THROW(v.insert_n(5, vals, 1), std::out_of_range);
    TVector<double> empty_v;
    EXPECT_THROW(empty_v.insert_n(1, vals, 1), std::out_of_range); // пустой, pos > 0
}


// erase_n
TEST(ClassVector, can_erase_n_middle) {
    TVector<double> v = { 1.0, 99.0, 100.0, 2.0, 3.0 };
    v.erase_n(1, 2);                 // удаляем два элемента начиная с позиции 1

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(ClassVector, can_erase_n_front) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    v.erase_n(0, 2);                 // удаляем первые два

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v[0], 3.0);
    EXPECT_DOUBLE_EQ(v[1], 4.0);
    EXPECT_DOUBLE_EQ(v[2], 5.0);
}

TEST(ClassVector, can_erase_n_back) {
    TVector<double> v = { 1.0, 2.0, 3.0, 4.0, 5.0 };
    v.erase_n(3, 2);                 // удаляем последние два

    EXPECT_EQ(v.size(), 3);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}

TEST(ClassVector, can_erase_n_all) {
    TVector<double> v = { 1.0, 2.0, 3.0 };
    v.erase_n(0, 3);                 // удалить все

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);
}

TEST(ClassVector, throw_when_erase_n_wrong_position) {
    TVector<double> v = { 1.0, 2.0, 3.0 };

    EXPECT_THROW(v.erase_n(3, 1), std::out_of_range);   // pos == size()
    EXPECT_THROW(v.erase_n(1, 3), std::out_of_range);   // pos + n > size()
    EXPECT_THROW(v.erase_n(4, 1), std::out_of_range);   // pos > size()
    TVector<double> empty_v;
    EXPECT_THROW(empty_v.erase_n(0, 1), std::out_of_range); // из пустого
}

// комбинация разных операций (аналог существующего combination_push_pop_insert_erase)
TEST(ClassVector, combination_push_pop_insert_erase_n) {
    TVector<double> v = { 5.0, 6.0, 7.0 };
    double front_vals[] = { 3.0, 4.0 };
    v.push_front_n(front_vals, 2);        // {3,4,5,6,7}
    double back_vals[] = { 8.0, 9.0 };
    v.push_back_n(back_vals, 2);          // {3,4,5,6,7,8,9}

    double insert_vals[] = { 10.0, 11.0 };
    v.insert_n(2, insert_vals, 2);        // {3,4,10,11,5,6,7,8,9}

    v.erase_n(0, 2);                      // {10,11,5,6,7,8,9}
    v.erase_n(3, 2);                      // {10,11,5,8,9}

    EXPECT_EQ(v.size(), 5);
    EXPECT_DOUBLE_EQ(v[0], 10.0);
    EXPECT_DOUBLE_EQ(v[1], 11.0);
    EXPECT_DOUBLE_EQ(v[2], 5.0);
    EXPECT_DOUBLE_EQ(v[3], 8.0);
    EXPECT_DOUBLE_EQ(v[4], 9.0);
}
TEST(VectorTest, ShuffleChangesOrder) {
    TVector<double> v = { 1, 2, 3, 4, 5 };
    TVector<double> original = v;

    std::cout << "Original: ";
    for (size_t i = 0; i < original.size(); ++i)
        std::cout << original[i] << " ";
    std::cout << std::endl;

    v.shuffle();

    std::cout << "Shuffled: ";
    for (size_t i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << std::endl;
    EXPECT_EQ(v.size(), original.size());
}

TEST(VectorTest, QuickSort) {
    TVector<double> v1 = { 1, 2, 3, 4, 5 };
    TVector<double> v2 = { 2, 3, 1 , 5, 4 };

    v2.quicksort(0, v1.size() - 1);
    EXPECT_EQ(v1.size(), v2.size());
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(v1[i], v2[i]);
    }
}

TEST(ClassVector, shrink_to_fit_reduces_capacity_to_size) {
    TVector<double> v;
    for (int i = 0; i < 5; i++) {
        v.push_back(i * 10.0);
    }

    size_t old_capacity = v.capacity();
    EXPECT_GT(old_capacity, v.size());

    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), v.size());
    EXPECT_EQ(v.size(), 5);

    for (int i = 0; i < 5; i++) {
        EXPECT_DOUBLE_EQ(v[i], i * 10.0);
    }
}

TEST(ClassVector, shrink_to_fit_on_empty_vector) {
    TVector<double> v;
    v.push_back(1.0);
    v.push_back(2.0);
    v.pop_back();
    v.pop_back();

    EXPECT_TRUE(v.is_empty());
    EXPECT_EQ(v.size(), 0);

    EXPECT_NO_THROW(v.shrink_to_fit());

    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
}

TEST(ClassVector, shrink_to_fit_idempotent_when_already_fitted) {
    TVector<double> v;
    v.push_back(42.0);

    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), v.size());

    size_t current_cap = v.capacity();

    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), current_cap);
    EXPECT_EQ(v.size(), 1);
    EXPECT_DOUBLE_EQ(v[0], 42.0);
}

TEST(ClassVector, shrink_to_fit_single_element) {
    TVector<double> v;
    v.push_back(99.0);
    v.push_back(100.0);
    v.pop_back();

    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v.capacity(), 1);

    v.shrink_to_fit();

    EXPECT_EQ(v.capacity(), 1);
    EXPECT_EQ(v.size(), 1);
    EXPECT_DOUBLE_EQ(v[0], 99.0);
    EXPECT_EQ(v.front_pos(), 0);
    EXPECT_EQ(v.back_pos(), 0);
}
#endif

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
    EXPECT_EQ(out.str(), "{ 1.5, 2.5, 3.5 } (start_index: 10)");
}

TEST(TMathVector, arithmetic_operations_maintain_invariant_after_reallocation) {
    TMathVector<double> v1 = { 1.0, 2.0 };
    TMathVector<double> v2 = { 3.0, 4.0, 5.0 };

    TMathVector<double> v3(5);
    v3 = { 1.0, 2.0, 3.0 };

    EXPECT_THROW(v1 += v3, std::invalid_argument);
}

#endif