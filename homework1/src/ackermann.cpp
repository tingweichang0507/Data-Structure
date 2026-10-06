#include <iostream>
using namespace std;

// 遞迴版：照題目公式直接寫
int ackermann_r(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann_r(m - 1, 1);
    else
        return ackermann_r(m - 1, ackermann_r(m, n - 1));
}

// 非遞迴版：用陣列自己做 stack，存還沒算完的 m
int ackermann_nr(int m, int n) {
    int s[10000];
    int top = 0;

    s[top] = m;
    top = top + 1;

    while (top > 0) {
        top = top - 1;
        m = s[top];

        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            n = 1;
            s[top] = m - 1;
            top = top + 1;
        } else {
            s[top] = m - 1;
            top = top + 1;
            s[top] = m;
            top = top + 1;
            n = n - 1;
        }
    }
    return n;
}

int main() {
    cout << "A(0, 0) = " << ackermann_r(0, 0) << ", " << ackermann_nr(0, 0) << endl;
    cout << "A(1, 2) = " << ackermann_r(1, 2) << ", " << ackermann_nr(1, 2) << endl;
    cout << "A(2, 2) = " << ackermann_r(2, 2) << ", " << ackermann_nr(2, 2) << endl;
    cout << "A(3, 2) = " << ackermann_r(3, 2) << ", " << ackermann_nr(3, 2) << endl;
    cout << "A(3, 3) = " << ackermann_r(3, 3) << ", " << ackermann_nr(3, 3) << endl;
    return 0;
}
