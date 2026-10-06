# 41241131

作業一

## 第一題：Ackermann's Function

### 解題說明

#### 問題描述

本題要求計算 Ackermann 函數 $A(m, n)$，定義如下：

$$
A(m, n) =
\begin{cases}
n + 1 & \text{if } m = 0 \\
A(m - 1, 1) & \text{if } n = 0 \\
A(m - 1, A(m, n - 1)) & \text{otherwise}
\end{cases}
$$

需要寫出**遞迴**與**非遞迴**兩種版本。

#### 解題策略

1. 遞迴版：直接把上面三個公式翻成 `if / else if / else`。
2. 非遞迴版：用陣列自己做 stack，存放還沒算完的 $m$，在迴圈裡依公式更新 $m$、$n$。
3. 因為這個函數成長很快，所以只測比較小的 $m$、$n$。

### 程式實作

```cpp
#include <iostream>
using namespace std;

// 遞迴版
int ackermann_r(int m, int n) {
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ackermann_r(m - 1, 1);
    else
        return ackermann_r(m - 1, ackermann_r(m, n - 1));
}

// 非遞迴版：用陣列自己做 stack
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
```

### 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

Ackermann 實際成長比一般指數還快，所以時間用 $O(2^n)$ 表示它不是 $O(n)$ 或 $O(n^2)$ 這種多項式時間。空間寫 $O(n)$ 是因為即使 $m$ 固定，$n$ 變大時堆疊深度還是會增加，不能寫成 $O(1)$。

### 測試與驗證

| 測試案例 | 輸入 | 預期輸出 | 實際輸出 |
|----------|------|----------|----------|
| 測試一   | $A(0,0)$ | 1 | 1 |
| 測試二   | $A(1,2)$ | 4 | 4 |
| 測試三   | $A(2,2)$ | 7 | 7 |
| 測試四   | $A(3,2)$ | 29 | 29 |
| 測試五   | $A(3,3)$ | 61 | 61 |

### 編譯與執行指令

```shell
$ g++ ackermann.cpp --std=c++17 -o ackermann.exe
$ .\ackermann.exe
A(0, 0) = 1, 1
A(1, 2) = 4, 4
A(2, 2) = 7, 7
A(3, 2) = 29, 29
A(3, 3) = 61, 61
```

### 結論

1. 遞迴版和非遞迴版算出來的結果一樣，測試都可以對上已知答案，例如 $A(2,2)=7$、$A(3,2)=29$、$A(3,3)=61$。
2. $m=0$ 這種邊界情況也有測到，$A(0,0)=1$，代表第一個公式有寫對。
3. 這題不太適合測太大的數字，所以作業裡只放比較小的數字。
4. 遞迴版雖然好寫，但呼叫層數一多就有系統堆疊爆掉的風險；非遞迴版用陣列自己做 stack，比較不會受系統遞迴深度限制。

---

## 第二題：Powerset

### 解題說明

#### 問題描述

若 $S$ 有 $n$ 個元素，powerset 就是 $S$ 的所有子集。

例如 $S = \{a, b, c\}$，則：

$$
powerset(S) = \{(), (a), (b), (c), (a,b), (a,c), (b,c), (a,b,c)\}
$$

本題要求用**遞迴**計算 powerset。

#### 解題策略

1. 對每個元素，都有「選」或「不選」兩種選擇。
2. 用 `index` 記錄現在處理到第幾個元素。
3. 當 `index` 走到最後，就把目前選到的子集印出來。

### 程式實作

```cpp
#include <iostream>
#include <string>
using namespace std;

void powerset(string s, string current, int index) {
    if (index == (int)s.length()) {
        cout << "(" << current << ")" << endl;
        return;
    }

    // 不選這個元素
    powerset(s, current, index + 1);

    // 選這個元素
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
```

### 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

### 測試與驗證

| 測試案例 | 輸入 | 預期輸出個數 | 實際輸出個數 |
|----------|------|--------------|--------------|
| 測試一   | $\{a,b,c\}$ | 8 | 8 |

### 編譯與執行指令

```shell
$ g++ powerset.cpp --std=c++17 -o powerset.exe
$ .\powerset.exe
powerset({a, b, c}):
()
(c)
(b)
(b,c)
(a)
(a,c)
(a,b)
(a,b,c)
```

### 結論

1. 以 $S=\{a,b,c\}$ 測試時，總共印出 8 個子集，數量等於 $2^3$，空集合也有印出來。
2. 輸出順序是先「不選」再「選」，所以看起來會跟題目範例的排列不太一樣，但內容是同一組子集。
3. 空集合有包含在結果裡，代表遞迴結束條件有寫對，不是只會印有元素的子集。
4. 元素再多一點，子集數量會變成兩倍兩倍往上加，印出來會很長，所以這題也是用小集合比較合適。

---

## 申論及開發報告

### 第一題為什麼用遞迴

Ackermann 本身就是用遞迴定義的，所以遞迴演算法最直接。程式幾乎可以照公式寫：

```cpp
if (m == 0)
    return n + 1;
else if (n == 0)
    return ackermann_r(m - 1, 1);
else
    return ackermann_r(m - 1, ackermann_r(m, n - 1));
```

用遞迴的原因是：每一層都在算更小的 $A(m,n)$，不用自己記中間過程，對答案也比較容易。缺點是呼叫次數會很快變多，而且會一直佔用系統堆疊，輸入稍大就可能當掉，所以題目才會再要非遞迴版。

### 第一題為什麼用堆疊

非遞迴不能只改成一個 for 迴圈，因為這題不是「從 1 加到 n」那種順序計算。算 $A(m, A(m,n-1))$ 時，外層的 $m$ 還要先記住，等內層算出來才能繼續。這種「後放進去的先拿出來」剛好就是堆疊。

作業不能 include `<stack>`，所以我用陣列 `s[]` 和變數 `top` 自己做堆疊，把還沒處理完的 $m$ 存進去：

- $m=0$：直接 $n+1$
- $n=0$：變成算 $A(m-1,1)$
- 其他：先把 $m-1$ 和 $m$ 壓回去，然後 $n$ 減 1

### 第二題為什麼用遞迴

powerset 用的也是遞迴。原因是每個元素只有兩種選擇：放進子集，或不放進去。這很適合一層一層往下走：

```cpp
// 不選這個元素
powerset(s, current, index + 1);

// 選這個元素
powerset(s, current + s[index], index + 1);
```

`index` 走到最後，代表每個元素都決定過了，就可以把 `current` 印出來。空集合也不用另外處理，一開始 `current` 是空的，全部都不選就會印出 `()`。

如果改成迴圈，就要自己用二進位或很多層 for 去生子集，程式會比較長，也不太符合這次在練遞迴。

這兩題用到的就是遞迴演算法，以及第一題非遞迴需要的堆疊。遞迴比較好讀，但不一定比較省；需要自己控制計算順序時，堆疊比較合適。
