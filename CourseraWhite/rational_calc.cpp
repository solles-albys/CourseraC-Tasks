#import <string>
#import <iostream>
#import <sstream>

using namespace std;

class Rational {
public:

    Rational(int numerator, int denominator) {
        if (denominator == 0) {
            throw invalid_argument("Denominator shouldn't be equal 0");
        }
        this->numerator = numerator;
        this->denominator = denominator;
        optimizeNumbers();
    }

    Rational() {
        numerator = 0;
        denominator = 1;
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return denominator;
    }

    Rational operator/(const Rational& rhs) {
        if (rhs.Numerator() == 0) {
            throw domain_error("Divider == 0");
        }
        return Rational(numerator * rhs.Denominator(), denominator * rhs.Numerator());
    }

    Rational operator*(const Rational& rhs) {
        return Rational(numerator * rhs.Numerator(), denominator * rhs.Denominator());
    }
    Rational operator+(const Rational& rhs) {
        int nok = NOK(denominator, rhs.Denominator());
        int for_lhs = nok / denominator;
        int for_rhs = nok / rhs.Denominator();
        return Rational(numerator * for_lhs + rhs.Numerator() * for_rhs, nok);
    }
    Rational operator-(const Rational& rhs) {
        int nok = NOK(denominator, rhs.Denominator());
        int for_lhs = nok / denominator;
        int for_rhs = nok / rhs.Denominator();
        return Rational(numerator * for_lhs - rhs.Numerator() * for_rhs, nok);
    }
private:
    int numerator;
    int denominator;

    int NOD(int one, int two) {
        int a = abs(one), b = abs(two);
        if (a == b and a == 0) {
            return 1;
        }
        if (a == 0 || a == b) {
            return b;
        }
        if (b == 0) {
            return a;
        }

        if (a > b) {
            return NOD(a % b, b);
        } else {
            return NOD(b % a, a);
        }
    }

    int NOK(int one, int two) {
        int nod = NOD(one, two);
        return abs(one * two) / nod;
    }

    void optimizeNumbers() {
        int nod = NOD(numerator, denominator);
        numerator = numerator / nod;
        denominator = denominator / nod;
    }
};

istream& operator>>(istream& in, Rational& rhs) {
    int num = 0, den = 0;
    in >> num;
    if (in.peek() != '/')
        throw invalid_argument("invalid input");
    in.ignore(1);
    in >> den;
    rhs = Rational(num, den);
    return in;
}

ostream& operator<<(ostream& out, Rational& rhs) {
    out << rhs.Numerator() << '/' << rhs.Denominator();
    return out;
}

int main() {

    Rational lhs, rhs;
    char op;
    try {
        cin >> lhs >> op >> rhs;
    } catch (invalid_argument&) {
        cout << "Invalid argument\n";
        return 0;
    }

    Rational result;
    try {
        if (op == '+') {
            result = lhs + rhs;
        } else if (op == '-') {
            result = lhs - rhs;
        } else if (op == '*') {
            result = lhs * rhs;
        } else if (op == '/') {
            result = lhs / rhs;
        }
    } catch (domain_error&) {
        cout << "Division by zero\n";
        return 0;
    }

    cout << result;
    cout << endl;

    return 0;
}