#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// удаляет ведущие нули из строки
string stripLeadingZeros(string num) {
    while (num.length() > 1 && num[0] == '0')
        num.erase(0, 1);
    return num;
}

// сравнение двух чисел в виде строки 
int compare(const string& a, const string& b) {
    string A = stripLeadingZeros(a);
    string B = stripLeadingZeros(b);
    if (A.size() != B.size()) return A.size() < B.size() ? -1 : 1;
    return A.compare(B);
}

// сложение двух больших чисел, представленных строками
string add(const string& num1, const string& num2) {
    string result;
    int carry = 0;
    int i = num1.size() - 1, j = num2.size() - 1;

    while (i >= 0 || j >= 0 || carry) {
        int digitSum = carry;
        if (i >= 0) digitSum += num1[i--] - '0';
        if (j >= 0) digitSum += num2[j--] - '0';
        result += digitSum % 10 + '0';
        carry = digitSum / 10;
    }

    reverse(result.begin(), result.end());
    return result;
}

// (num1 >= num2)
string subtract(const string& num1, const string& num2) {
    string result;
    int borrow = 0;
    int i = num1.size() - 1, j = num2.size() - 1;

    while (i >= 0) {
        int diff = (num1[i] - '0') - borrow;
        if (j >= 0) diff -= (num2[j--] - '0');

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }

        result += diff + '0';
        i--;
    }

    reverse(result.begin(), result.end());
    return stripLeadingZeros(result);
}

//умножение большого числа на 10^n
string shift(const string& num, int n) {
    return num + string(n, '0');
}

// умножение Карацубе
string karatsuba(const string& x, const string& y) {
    string a = stripLeadingZeros(x);
    string b = stripLeadingZeros(y);

    if (a.size() == 0 || b.size() == 0) return "0";
    if (a.size() == 1 && b.size() == 1)
        return to_string((a[0] - '0') * (b[0] - '0'));

    int n = max(a.size(), b.size());
    int half = n / 2;

    while (a.size() < n) a = '0' + a;
    while (b.size() < n) b = '0' + b;

    string a1 = a.substr(0, n - half);
    string a0 = a.substr(n - half);
    string b1 = b.substr(0, n - half);
    string b0 = b.substr(n - half);

    string z2 = karatsuba(a1, b1);
    string z0 = karatsuba(a0, b0);
    string z1 = karatsuba(add(a1, a0), add(b1, b0));
    z1 = subtract(subtract(z1, z2), z0);

    return stripLeadingZeros(add(add(shift(z2, 2 * half), shift(z1, half)), z0));
}

// деление длинных чисел: целочисленное деление 
string divide(const string& dividend, const string& divisor) {
    if (stripLeadingZeros(divisor) == "0") {
        throw invalid_argument("Division by zero");
    }

    string result;
    string current;

    for (size_t i = 0; i < dividend.size(); ++i) {
        current += dividend[i];
        current = stripLeadingZeros(current);

        int quotientDigit = 0;
        while (compare(current, divisor) >= 0) {
            current = subtract(current, divisor);
            ++quotientDigit;
        }
        result += quotientDigit + '0';
    }

    return stripLeadingZeros(result);
}

int main() {
    string num1, num2;
    setlocale(LC_ALL, "");
    cout << "Введите первое длинное число: ";
    cin >> num1;
    cout << "Введите второе длинное число: ";
    cin >> num2;

    cout << "Умножение Карацубы: " << karatsuba(num1, num2) << endl;
    try {
        cout << "Результат алгоритма деления: " << divide(num1, num2) << endl;
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
