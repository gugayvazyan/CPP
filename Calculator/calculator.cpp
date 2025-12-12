#include <cmath>
#include <iostream>
#include <limits>
#

#define MAX std::numeric_limits<double>::max()
#define MIN std::numeric_limits<double>::min()

double add(double a, double b) {
    try {
        if ((MAX == a && b > 0) || (MAX == b && a > 0))
            throw std::overflow_error("Double cant contain your number.");
        if ((MAX - a < b) || (MIN - a > b))
            throw std::overflow_error("Double cant contain your sum.");
        return a + b;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double sub(double a, double b) {
    try {
        if ((MAX == a && b < 0) || (MAX == b && a < 0))
            throw std::overflow_error("Double cant contain your number.");
        if ((a - b) > MAX || (a - b) < MIN)
            throw std::overflow_error("Double cant contain your result.");
        return a - b;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double mul(double a, double b) {
    try {
        if (b != 0 && (a > MAX / b || a < MIN / b))
            throw std::overflow_error("Double cant contain your result.");
        return a * b;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double div(double a, double b) {
    try {
        if (b == 0)
            throw std::logic_error("Divisor cannot be zero.");
        if (a > MAX / b || a < MIN / b)
            throw std::overflow_error("Double cant contain your result.");
        return a / b;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double ppow(double a, double b) {
    try {
        double result = pow(a, b);

        if (result > MAX || result < MIN)
            throw std::overflow_error("Double cant contain your result.");

        return result;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double llog(double x, double base = 2.7) {
    try {
        if (x <= 0)
            throw std::domain_error("Logarithm undefined for x <= 0.");
        if (base <= 0 || base == 1)
            throw std::domain_error("Invalid logarithm base.");

        double result = log(x) / log(base);

        if (result > MAX || result < MIN)
            throw std::overflow_error("Double cant contain your result.");

        return result;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

double ssqrt(double x, double y = 2) {
    try {

        if (y == 0)
            throw std::logic_error("Root degree cannot be zero.");
        double result = pow(x, 1.0 / y);

        if (result > MAX || result < MIN)
            throw std::overflow_error("Double cant contain your result.");

        return result;
    }
    catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}


int main() {
    int choice;
    double a, b;

    std::cout << "Select operation:\n";
    std::cout << "1) Addition\n2) Subtraction\n3) Multiplication\n4) Division\n";
    std::cout << "5) Power\n6) Logarithm\n7) Root\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            std::cout << "Enter first number: ";
            std::cin >> a;
            std::cout << "Enter second number: ";
            std::cin >> b;
            
            std::cout << "Result: " << add(a, b) << std::endl;
            break;
            
            case 2:
            std::cout << "Enter first number: ";
            std::cin >> a;
            std::cout << "Enter second number: ";
            std::cin >> b;
            std::cout << "Result: " << sub(a, b) << std::endl;
            break;
            
            case 3:
            std::cout << "Enter first number: ";
            std::cin >> a;
            std::cout << "Enter second number: ";
            std::cin >> b;
            std::cout << "Result: " << mul(a, b) << std::endl;
            break;

        case 4:
            std::cout << "Enter first number: ";
            std::cin >> a;
            std::cout << "Enter second number: ";
            std::cin >> b;
            std::cout << "Result: " << div(a, b) << std::endl;
            break;

        case 5:
            std::cout << "Enter base: ";
            std::cin >> a;
            std::cout << "Enter exponent: ";
            std::cin >> b;
            std::cout << "Result: " << ppow(a, b) << std::endl;
            break;

        case 6:
            std::cout << "Enter number: ";
            std::cin >> a;
            std::cout << "Enter base (default 2.7)";
            if (!(std::cin >> b)) b = 2.7;
            std::cout << "Result: " << llog(a, b) << std::endl;
            break;

        case 7:
            std::cout << "Enter number: ";
            std::cin >> a;
            std::cout << "Enter root degree (default 2):";
            if (!(std::cin >> b)) b = 2;
            std::cout << "Result: " << ssqrt(a, b) << std::endl;
            break;

        default:
            std::cout << "Invalid choice." << std::endl;
    }

    return 0;
}

