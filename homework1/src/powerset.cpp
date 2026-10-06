#include <iostream>
#include <string>
using namespace std;

// 遞迴算 powerset
// s：原本的集合（用字串表示，例如 "abc"）
// current：目前已經選到的元素
// index：現在要處理第幾個元素
void powerset(string s, string current, int index) {
    // 所有元素都處理完了，印出目前這個子集
    if (index == (int)s.length()) {
        cout << "(" << current << ")" << endl;
        return;
    }

    // 情況 1：不選 s[index]
    powerset(s, current, index + 1);

    // 情況 2：選 s[index]
    if (current == "")
        powerset(s, current + s[index], index + 1);
    else
        powerset(s, current + "," + s[index], index + 1);
}

int main() {
    string s = "abc";
    cout << "powerset({a, b, c}):" << endl;
    powerset(s, "", 0);
    return 0;
}
