#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <type_traits>

#include "TMatrix.h"
#include "TTriangleMatrix.h"


inline void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline void printInputHint(bool isTriangular) {
    std::cout << "Формат ввода: <строки> <столбцы>, затем все элементы построчно.\n";
    if (isTriangular) {
        std::cout << "(Для треугольной матрицы вводятся ВСЕ элементы, но хранится\n"
            " только верхний треугольник — при столбец >= строка.)\n";
    }
}

template <typename MatrixType>
void inputMatrix(MatrixType& mat, const std::string& name, bool isTriangular) {
    std::cout << "\n--- Ввод матрицы " << name << " ---\n";
    printInputHint(isTriangular);
    std::cout << "> ";
    if (!(std::cin >> mat)) {
        clearInput();
        throw std::runtime_error("Ошибка чтения матрицы " + name);
    }
}

template <typename MatrixType>
void runCalculator(const std::string& matrixName) {
    constexpr bool isTriangular = std::is_same_v<MatrixType, TTriangleMatrix<double>>;
    int choice;

    while (true) {
        std::cout << "\n=== МАТРИЧНЫЙ КАЛЬКУЛЯТОР (" << matrixName << ") ===\n";
        std::cout << "1. Сложить две матрицы (A + B)\n";
        std::cout << "2. Вычесть две матрицы (A - B)\n";
        std::cout << "3. Умножить матрицу на скаляр (A * k)\n";
        std::cout << "4. Умножить две матрицы (A * B)\n";
        if constexpr (!isTriangular) {
            std::cout << "5. Транспонировать матрицу (A^T)\n";
        }
        std::cout << "0. Назад в главное меню\n";
        std::cout << "Выберите действие: ";

        if (!(std::cin >> choice)) {
            clearInput();
            std::cout << "Ошибка ввода. Введите число.\n";
            continue;
        }

        if (choice == 0) { 
            system("cls");
            break;
        }

        try {
            if (choice == 1 || choice == 2 || choice == 4) {
                MatrixType A, B;
                inputMatrix(A, "A", isTriangular);
                inputMatrix(B, "B", isTriangular);

                std::cout << "\nМатрица A:\n" << A;
                std::cout << "Матрица B:\n" << B;

                if (choice == 1) {
                    std::cout << "\nРезультат A + B:\n" << (A + B);
                }
                else if (choice == 2) {
                    std::cout << "\nРезультат A - B:\n" << (A - B);
                }
                else {
                    std::cout << "\nРезультат A * B:\n" << (A * B);
                }
            }
            else if (choice == 3) {
                MatrixType A;
                double k;
                inputMatrix(A, "A", isTriangular);
                std::cout << "Введите скаляр k: ";
                if (!(std::cin >> k)) {
                    clearInput();
                    std::cout << "Ошибка ввода. Введите число.\n";
                    continue;
                }
                std::cout << "\nРезультат A * " << k << ":\n" << (A * k);
            }
            else if (choice == 5) {
                if constexpr (isTriangular) {
                    std::cout << "Транспонирование недоступно для треугольной матрицы.\n";
                }
                else {
                    MatrixType A;
                    inputMatrix(A, "A", isTriangular);
                    std::cout << "\nИсходная матрица A:\n" << A;
                    A.transpose();
                    std::cout << "\nТранспонированная матрица:\n" << A;
                }
            }
            else {
                std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "\n[ОШИБКА]: " << e.what() << "\n";
        }
    }
}