# 41241131

張廷幃
作業二：Polynomial

## 解題說明

### 問題描述

這次要完成 `Polynomial` 類別，用來儲存多項式，並完成加法 `Add`、乘法 `Mult` 和代入數字計算的 `Eval`。另外要讓 `>>` 可以輸入多項式，`<<` 可以輸出多項式。

依照題目，每一項使用 `Term` 記錄係數 `coef` 和指數 `exp`。`Polynomial` 裡面用 `termArray` 動態陣列儲存各項，`capacity` 是陣列容量，`terms` 是目前的項數。建立物件時，預設多項式是 0。

### 解題策略

各項依指數從大到小存放。輸入一項時，就用迴圈找它的位置。如果已有相同指數，直接把係數相加；加完是 0，就移除該項。沒有相同指數時，將後面的項目往後移，再插入新的一項。

加法用兩個索引讀取兩個多項式。指數大的項先放進答案，相同指數的係數就相加。例如：

```text
(3x² + 2x + 1) + (x - 1) = 3x² + 3x
```

加法中，如果其中一個多項式已經處理完，就把另一個剩下的項目依序放入答案。因為原本已按照指數排列，剩下的項目不必重新找位置。

乘法使用兩層迴圈，讓兩個多項式的每一項互相相乘。每算出一項，就用前面的插入方法合併同類項。例如：

```text
(3x² + 2x + 1)(x - 1) = 3x³ - x² - x - 1
```

求值使用連乘計算次方。例如 x³ 就是把 1 連續乘三次 x，再乘上該項係數。指數是 0 時不進入連乘迴圈，保留 1，所以常數項也能正確計算。

陣列容量不足時，建立兩倍大的新陣列，將原本的項目複製過去，再釋放舊陣列。`capacity` 記錄能放幾項，`terms` 記錄實際存了幾項，兩者不一定相同。

輸出時依序印出各項，第一項只在係數為負時印出負號，後面的項目依係數印出加號或減號。係數為 1 的非常數項省略 1，一次項省略 `^1`，常數項只印係數。如果沒有任何項目，就印出 `0`。

輸入時，先給 P 的項數，再給每一項的「係數、指數」，Q 也一樣，最後輸入 x。例如：

```text
3 3 2 2 1 1 0
2 1 1 -1 0
2
```

第一行輸入 P：開頭的 3 表示有三項，後面的 `3 2`、`2 1`、`1 0` 分別代表 3x²、2x、1。第二行輸入 Q：開頭的 2 表示有兩項，後面的 `1 1`、`-1 0` 代表 x 和 -1。第三行的 2 是代入的 x。

## 程式實作

程式使用一般迴圈處理陣列，求次方也用連乘。陣列滿了就建立兩倍大的陣列，複製原本資料後釋放舊陣列。

因為類別中有動態陣列，複製物件時要另外建立陣列，不能只複製位址。程式因此保留複製建構函式、指定運算子和解構函式，分別處理複製、指定及釋放空間。輸入時先放進暫存物件，全部成功才指定給原物件；中途讀取失敗就保留原來的內容。

完整程式碼如下：

```cpp
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
```

## 效能分析

令 P 有 m 項、Q 有 n 項，最大指數為 E，輸入的項數為 k。以下空間包含運算產生的答案，不包含原本的 P、Q。

### 時間複雜度

1. 輸入：每次插入可能要找位置、搬動前面的資料。輸入 k 項，最差需要 $O(k^2)$。
2. 加法：兩個索引只往後移，每一項處理一次。包含參數複製和陣列擴充，共 $O(m+n)$。
3. 乘法：兩層迴圈先產生 mn 個乘積，每個乘積插入時還要搜尋、移動答案中的項目。如果暫存答案最多有 r 項，這部分需要 $O(mnr)$，且 r 最多為 mn，所以可用 $O(m^2n^2)$ 作為上界。這個寫法容易理解，但不能只看外面的兩層迴圈就寫成 $O(mn)$。
4. 求值：每項最多連乘 E 次，共 $O(m(E+1))$。加 1 是因為常數項即使不連乘，仍需要讀取並加進答案。

上面的加法和乘法分析，假設兩個多項式都不是零多項式。零多項式不會產生乘積，但 `Mult` 仍會複製傳入的 Q，需要 $O(n+1)$；空輸入和零多項式求值則是 $O(1)$。

### 空間複雜度

1. 輸入：新增 k 項最多需要 $O(k+1)$ 的陣列空間，若重複讀入同一物件，原本較大的容量會保留。
2. 加法：答案最多有 m+n 項，因此一般情況為 $O(m+n)$。
3. 乘法：答案最多存 mn 項，因此一般情況為 $O(mn)$。
4. 求值：只使用幾個變數，額外空間為 $O(1)$。

加法、乘法的參數照題目使用傳值，也會建立 Q 的副本。若 Q 原本保留的容量是 C，而不是接近 n，完整的額外空間分別要寫成 $O(C+m+n+1)$ 和 $O(C+mn+1)$。合併或相消後，項數可能變少，但陣列不會自動縮小。
## 測試與驗證

### 編譯與執行指令

測試環境：Visual Studio 2022、C++17。

```shell
$ cl /nologo /std:c++17 /EHsc /W4 /O2 /utf-8 src/polynomial.cpp /Fe:polynomial.exe
$ .\polynomial.exe
3 3 2 2 1 1 0
2 1 1 -1 0
2
P(x) = 3x^2 + 2x + 1
Q(x) = x - 1
P + Q = 3x^2 + 3x
P * Q = 3x^3 - x^2 - x - 1
P(2) = 17
Q(2) = 1
```

三行輸入依序為 P、Q 和代入的 x。
### 測試案例

| 測試案例 | 輸入 | 預期輸出 | 實際輸出 |
| --- | --- | --- | --- |
| 測試一：一般運算 | P = 3x² + 2x + 1，Q = x − 1，x = 2 | 和為 3x² + 3x，積為 3x³ − x² − x − 1，P(2) = 17，Q(2) = 1 | 與預期相同 |
| 測試二：零多項式 | P = 0，Q = 0，x = 0 | 和、積、P(0)、Q(0) 都是 0 | 和、積、P(0)、Q(0) 都是 0 |
| 測試三：完全相消 | P = x + 1，Q = −x − 1，x = 1 | 和為 0，積為 −x² − 2x − 1，P(1) = 2，Q(1) = −2 | 與預期相同 |
| 測試四：合併同類項 | P = 2x + 3x⁴ − 2x − 1，另輸入零係數項，Q = 0，x = 1 | P 整理為 3x⁴ − 1，和為 3x⁴ − 1，積為 0，P(1) = 2 | 與預期相同 |
| 測試五：小數與常數 | P = 2.5，Q = −1，x = 0 | 和為 1.5，積為 −2.5，P(0) = 2.5，Q(0) = −1 | 與預期相同 |
| 測試六：負指數 | P 輸入係數 2、指數 −1 | 顯示輸入錯誤，結束代碼為 1 | 顯示輸入錯誤，結束代碼為 1 |



每組輸入分成三行，依序是 P 的資料、Q 的資料，以及代入的 x。

**測試一：一般運算**

輸入：

```text
3 3 2 2 1 1 0
2 1 1 -1 0
2
```

預期輸出與實際輸出相同：

```text
P(x) = 3x^2 + 2x + 1
Q(x) = x - 1
P + Q = 3x^2 + 3x
P * Q = 3x^3 - x^2 - x - 1
P(2) = 17
Q(2) = 1
```

**測試二：零多項式**

輸入：

```text
0
0
0
```

預期輸出與實際輸出相同：

```text
P(x) = 0
Q(x) = 0
P + Q = 0
P * Q = 0
P(0) = 0
Q(0) = 0
```

**測試三：完全相消**

輸入：

```text
2 1 1 1 0
2 -1 1 -1 0
1
```

預期輸出與實際輸出相同：

```text
P(x) = x + 1
Q(x) = -x - 1
P + Q = 0
P * Q = -x^2 - 2x - 1
P(1) = 2
Q(1) = -2
```

**測試四：亂序、重複指數與零係數**

輸入：

```text
5 2 1 3 4 -2 1 0 8 -1 0
0
1
```

預期輸出與實際輸出相同：

```text
P(x) = 3x^4 - 1
Q(x) = 0
P + Q = 3x^4 - 1
P * Q = 0
P(1) = 2
Q(1) = 0
```

**測試五：小數與常數**

輸入：

```text
1 2.5 0
1 -1 0
0
```

預期輸出與實際輸出相同：

```text
P(x) = 2.5
Q(x) = -1
P + Q = 1.5
P * Q = -2.5
P(0) = 2.5
Q(0) = -1
```

**測試六：負指數**

輸入：

```text
1 2 -1
0
0
```

預期結果：顯示輸入錯誤並結束，結束代碼為 1。

實際輸出：

```text
Invalid input.
```

實際結束代碼為 1，與預期相同。

測試時直接執行主程式，逐組輸入資料，再將輸出與手算結果比較。六組測試的結果都與預期相同。

### 結論

加法每次處理剩餘項中指數最大的項，指數相同就相加。兩個索引只往後走，所以每一項都會被處理，且不會重複。

乘法依照分配律列出全部項目的配對。每算出一項就找相同指數，有的話把係數相加，沒有就插入新項目。係數是 0 的項移除後不影響答案。

求值時，每一項按照指數決定要乘幾次 x，再乘上係數，最後把各項結果相加。因為使用 `float` 存小數，計算時仍可能有小數誤差。

## 效能量測

使用 Visual Studio 2022、C++17、`/O2` 編譯，再用 PowerShell 的 `Measure-Command` 計時。P、Q 分別各有 10、100、500 項，每一項的係數都是 1，指數從 0 到項數減 1，代入 x=1。每組執行 5 次後取平均。

量測指令如下：

```powershell
foreach ($n in 10, 100, 500) {
    $poly = "$n"
    for ($j = 0; $j -lt $n; $j++) {
        $poly += " 1 $j"
    }

    $data = @(
        $poly
        $poly
        "1"
    )

    Write-Output "P、Q 各 $n 項"
    $total = 0
    for ($i = 0; $i -lt 5; $i++) {
        $time = Measure-Command {
            $data | .\polynomial.exe | Out-Null
        }
        $ms = $time.TotalMilliseconds
        Write-Output ("第 {0} 次：{1:N2} 毫秒" -f ($i + 1), $ms)
        $total += $ms
    }
    Write-Output ("平均：{0:N2} 毫秒" -f ($total / 5))
    Write-Output ""
}
```

實測結果如下，單位為毫秒：

```text
P、Q 項數     第1次  第2次  第3次  第4次  第5次  平均
各10項        21.04   7.78   7.72   7.95   8.08  10.51
各100項        8.86   8.53   8.73   8.89   9.67   8.93
各500項       44.13  44.17  44.98  45.06  44.62  44.59
```

平均值由程式使用未四捨五入的時間計算，再顯示至小數第二位。10 項的第一次較久，仍保留在平均中。

10 項與 100 項的時間差異不大，100 項的平均反而較小，不能據此認為項數增加會更快。500 項的五次結果都約為 44 到 45 毫秒，比前兩組明顯增加。

這裡量到的是資料傳入、程式啟動、加法、乘法、求值及輸出處理的總時間，不能把增加的時間全部算在乘法上。`Out-Null` 只是不把輸出印在畫面上，程式仍會產生輸出。時間量測用來觀察這三組資料的執行情況，不能直接證明 Big-O。
## 心得討論

這題先把每一項存好，再處理加法、乘法和求值。各項按照指數排列後，加法可以從前面一路往後比，不用一直重新搜尋。

乘法原本以為只需要兩層迴圈，但算完一項還要處理同類項，所以實際花的時間也要算進搜尋和移動資料。這次用迴圈處理，雖然速度不是最快，但比較容易看懂每一步。

動態陣列要注意複製與釋放。兩個物件不能共用同一個陣列位址，否則修改資料或釋放時會出問題。這也是除了題目四個函式外，還需要補上複製相關函式的原因。

開發時發現，如果直接把輸入寫進原本的物件，中途少打一組資料，原本的內容可能已經被改動。因此改成先讀進暫存物件，全部讀取成功後才指定給原物件，輸入失敗時就能保留原資料。

一開始量測只用了幾項資料，時間大多受到程式啟動影響，看不出項數增加後的差別。後來改測 P、Q 各 10、100、500 項，每組執行五次取平均。500 項的執行時間明顯增加，但這仍是整個程式的時間，不能直接當成乘法的時間。

程式使用小範圍的資料測試，假設項數、指數相加和計算結果都在型別可以表示的範圍內。輸入時會檢查負項數、負指數和資料格式錯誤，但沒有處理所有超大數值的情況。係數使用 `float`，小數計算仍可能有誤差。













