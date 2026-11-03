#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    // 陣列滿了就增加空間。
    void newTerm(float coefficient, int exponent) {
        if (coefficient == 0) return;
        if (terms == capacity) {
            int newCapacity = capacity * 2;
            Term* temp = new Term[newCapacity];
            for (int i = 0; i < terms; i++) temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
            capacity = newCapacity;
        }
        termArray[terms].coef = coefficient;
        termArray[terms].exp = exponent;
        terms++;
    }

    // 找到同次方就相加，否則依指數大小插入。
    void insertTerm(float coefficient, int exponent) {
        if (coefficient == 0) return;
        int position = 0;
        while (position < terms && termArray[position].exp > exponent)
            position++;

        if (position < terms && termArray[position].exp == exponent) {
            termArray[position].coef += coefficient;
            if (termArray[position].coef == 0) {
                for (int i = position; i < terms - 1; i++)
                    termArray[i] = termArray[i + 1];
                terms--;
            }
        } else {
            newTerm(coefficient, exponent);
            for (int i = terms - 1; i > position; i--)
                termArray[i] = termArray[i - 1];
            termArray[position].coef = coefficient;
            termArray[position].exp = exponent;
        }
    }

public:
    Polynomial() {
        capacity = 2;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 複製時建立自己的陣列，不能只複製指標。
    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) termArray[i] = other.termArray[i];
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        Term* temp = new Term[other.capacity];
        for (int i = 0; i < other.terms; i++) temp[i] = other.termArray[i];
        delete[] termArray;
        termArray = temp;
        capacity = other.capacity;
        terms = other.terms;
        return *this;
    }

    ~Polynomial() {
        delete[] termArray;
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result;
        int i = 0;
        int j = 0;
        // 每次取出指數比較大的項目。
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp > poly.termArray[j].exp) {
                result.newTerm(termArray[i].coef, termArray[i].exp);
                i++;
            } else if (termArray[i].exp < poly.termArray[j].exp) {
                result.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                j++;
            } else {
                result.newTerm(termArray[i].coef + poly.termArray[j].coef,
                               termArray[i].exp);
                i++;
                j++;
            }
        }
        while (i < terms) {
            result.newTerm(termArray[i].coef, termArray[i].exp);
            i++;
        }
        while (j < poly.terms) {
            result.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
            j++;
        }
        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float coefficient = termArray[i].coef * poly.termArray[j].coef;
                int exponent = termArray[i].exp + poly.termArray[j].exp;
                result.insertTerm(coefficient, exponent);
            }
        }
        return result;
    }

    float Eval(float f) {
        float answer = 0;
        for (int i = 0; i < terms; i++) {
            float power = 1;
            // 例如 x 的三次方，就是連續乘三次 x。
            for (int j = 0; j < termArray[i].exp; j++) power *= f;
            answer += termArray[i].coef * power;
        }
        return answer;
    }

    friend istream& operator>>(istream& in, Polynomial& poly) {
        int count;
        if (!(in >> count)) return in;
        if (count < 0) {
            in.setstate(ios::failbit);
            return in;
        }
        // 先讀入另一個物件，全部成功才更新 poly。
        Polynomial temp;
        for (int i = 0; i < count; i++) {
            float coefficient;
            int exponent;
            if (!(in >> coefficient >> exponent)) return in;
            if (exponent < 0 || !isfinite(coefficient)) {
                in.setstate(ios::failbit);
                return in;
            }
            temp.insertTerm(coefficient, exponent);
        }
        poly = temp;
        return in;
    }

    // 由成員函式讀取 Term 的私有資料，再交給 << 使用。
    void print(ostream& out) const {
        if (terms == 0) {
            out << 0;
            return;
        }
        for (int i = 0; i < terms; i++) {
            float coefficient = termArray[i].coef;
            int exponent = termArray[i].exp;
            if (i == 0) {
                if (coefficient < 0) out << '-';
            } else {
                if (coefficient < 0) out << " - ";
                else out << " + ";
            }
            float value = fabs(coefficient);
            if (value != 1 || exponent == 0) out << value;
            if (exponent > 0) {
                out << 'x';
                if (exponent > 1) out << '^' << exponent;
            }
        }
    }

    friend ostream& operator<<(ostream& out, const Polynomial& poly) {
        poly.print(out);
        return out;
    }
};

int main() {
    Polynomial p, q;
    float x;
    if (!(cin >> p >> q >> x) || !isfinite(x)) {
        cerr << "Invalid input.\n";
        return 1;
    }
    cout << "P(x) = " << p << '\n';
    cout << "Q(x) = " << q << '\n';
    cout << "P + Q = " << p.Add(q) << '\n';
    cout << "P * Q = " << p.Mult(q) << '\n';
    cout << "P(" << x << ") = " << p.Eval(x) << '\n';
    cout << "Q(" << x << ") = " << q.Eval(x) << '\n';
    return 0;
}

