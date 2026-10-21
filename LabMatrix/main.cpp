#include "calc.h"

int main() {
    system("chcp 65001 > null");
    system("cls");                

    int mainChoice;
    while (true) {
        std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
        std::cout << "1. Работа с обычной матрицей (TMatrix)\n";
        std::cout << "2. Работа с треугольной матрицей (TTriangleMatrix)\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите тип матрицы: ";

        if (!(std::cin >> mainChoice)) {
            clearInput();
            std::cout << "Ошибка ввода. Введите число.\n";
            continue;
        }

        if (mainChoice == 0) {
            break;
        }
        else if (mainChoice == 1) {
            runCalculator<TMatrix<double>>("Обычная матрица");
        }
        else if (mainChoice == 2) {
            runCalculator<TTriangleMatrix<double>>("Треугольная матрица");
        }
        else {
            std::cout << "Неверный выбор. Попробуйте снова.\n";
        }
    }

    std::cout << "Выход из программы.\n";
    return 0;
}