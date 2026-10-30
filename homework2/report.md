# 41443116

## 一、解題說明

用類別與動態陣列來實作多項式，完成多項式的輸入、輸出、相加、相乘，以及代入數值計算。

### 解題策略

多項式可以拆成係數（Coefficient）與指數（Exponent）兩個部分，再利用陣列存放每一項的資料。

例如：

| 項目 | 係數（Coefficient） | 指數（Exponent） |
|---|---:|---:|
| `2x^3` | 2 | 3 |
| `5x` | 5 | 1 |
| `1` | 1 | 0 |

本次實作的步驟如下：

1. 使用建構子初始化多項式。
2. 使用 `NewTerm()` 新增多項式項目，並在空間不足時擴充動態陣列。
3. 使用運算子多載完成多項式的輸入與輸出。
4. 使用 `Add()` 計算兩個多項式的相加結果。
5. 使用 `Mult()` 計算兩個多項式的相乘結果。
6. 使用 `Eval()` 將指定的數值代入多項式並計算結果。

### 多項式相加的發想過程

一開始想到的是讓兩個多項式相同位置的係數直接相加，並把結果存進新的多項式 `c`。

```cpp
Polynomial Polynomial::Add(Polynomial poly)
{
    Polynomial c;

    for (int i = 0; i < terms; i++) {
        float sum = termArray[i].coef + poly.termArray[i].coef;
        c.NewTerm(sum, termArray[i].exp);
    }

    return c;
}
```

這個版本的想法比較簡單，就是將兩個係數相加，再把結果存進新的多項式。不過，這樣沒有考慮到兩個多項式的指數可能不同，也沒有處理兩個多項式項數不一樣的情況。

後面看題目發現差很多我才開始思考不同輸入情況下應該如何處理，才改成使用兩個索引 `i` 跟 `j` 比較指數。

修改後的處理方式如下：

1. 如果兩個指數相同，就將係數相加。
2. 如果指數不同，就先將指數較大的項目放進結果。
3. 如果其中一個多項式已經走訪完畢，就將另一個多項式剩下的項目加入結果。
4. 如果相同指數的係數相加後為 0，就不新增這一項。

這個方法假設兩個輸入多項式都按照指數由大到小排列，所以有(特別打請由大到小輸入指數!)，這算是程式的不完整。

### 多項式相乘的想法

多項式相乘比較接近數學上的運算方式，就是將第一個多項式的每一項與第二個多項式的每一項相乘。

例如：

\[
(2x+2)(2x^2+4x)
\]

展開後為：

\[
4x^3+8x^2+4x^2+8x
\]

將相同指數的項目合併後，得到：

\[
4x^3+12x^2+8x
\]

在程式中，係數相乘，指數相加，再使用 `bool found` 判斷結果中是否已經存在相同指數的項目。如果存在，就將係數累加；如果不存在，就新增項目。

### 多項式代入計算的想法

代入計算的方式比較直觀，就是將每一項的係數乘上指定數值的指數次方，再將所有結果加起來。

```cpp
sum += termArray[i].coef * pow(f, termArray[i].exp);
```

例如，多項式為：

\[
5x^2
\]

當 \(x=2\) 時：

\[
5\times 2^2=20
\]

程式使用 `pow()` 計算指數次方，再將每一項的結果累加。

### 動態陣列的實作

這次使用老師上課教的 `NewTerm()` 作為基礎。當陣列空間不足時，就將容量加倍，建立新的陣列，再把原本的資料複製過去。

```cpp
void Polynomial::NewTerm(const float thecoeff, const int theexp)
{
    if (terms == capacity) {
        capacity *= 2;

        Term* temp = new Term[capacity];

        copy(termArray, termArray + terms, temp);

        delete[] termArray;
        termArray = temp;
    }

    termArray[terms].coef = thecoeff;
    termArray[terms++].exp = theexp;
}
```

這個方法讓陣列不需要一開始就預留很大的空間，而是等到容量不足時再擴充，這就我腦袋中的罐子。

## 二、Algorithm Design & Programming

以下是完整程式:
```cpp
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

class Polynomial;//類別

class Term {
    friend class Polynomial;// 讓 Polynomial 可以存取 Term 的 private 成員
    friend ostream& operator<<(ostream& out, const Polynomial& poly); //讓輸出能讀取term coef exp
    friend istream& operator>>(istream& in, Polynomial& poly);
private:
    float coef;//係數
    int exp;//指數
};

class Polynomial {
private:
    Term* termArray; //多項式 存係數指數
    int capacity; //罐子空間
    int terms;//目前罐子用了多少

public:
    Polynomial();

    void NewTerm(const float thecoeff, const int theexp);

    Polynomial Add(Polynomial poly);

    Polynomial Mult(Polynomial poly);

    float Eval(float f);

    friend istream& operator>>(istream& in, Polynomial& poly);// 讓 cin >> p 可以讀取多項式

    friend ostream& operator<<(ostream& out, const Polynomial& poly);// 讓 cout << p 可以輸出多項式
};

Polynomial::Polynomial()//初始化罐子
{
    capacity = 1; //是0的話會乘不了
    terms = 0;
    termArray = new Term[capacity];//動態記憶體
}

void Polynomial::NewTerm(const float thecoeff, const int theexp) //考試有教的
{
    if (terms == capacity) {
        capacity *= 2;

        Term* temp = new Term[capacity]; // 建立一個容量更大的罐子

        copy(termArray, termArray + terms, temp);//把舊資料丟進新罐子

        delete[] termArray; //刪除舊罐子
        termArray = temp;//指向新罐子
    }

    termArray[terms].coef = thecoeff;// 將新係數存入目前空位置
    termArray[terms++].exp = theexp;//指數存入 讓項數加1

}

// 多項式相加
Polynomial Polynomial::Add(Polynomial poly) //poly是第二個多項式
{
    Polynomial c; //建立物件c 初始化成員 上面有的初始化罐子 ex c.terms=0

    int i = 0;//拿來跑while的
    int j = 0;

    while (i < terms && j < poly.terms) { //i跟j跑到罐子內目前存多少ex 5x^2+2x terms=2 

        if (termArray[i].exp == poly.termArray[j].exp) { //判斷兩個指數是否相同
            float sum = termArray[i].coef + poly.termArray[j].coef; //是的話就相加

            if (sum != 0) {  //相加不為0就把結果丟罐子
                c.NewTerm(sum, termArray[i].exp);
            }
            i++;
            j++;//下一格
        }
        else if (termArray[i].exp > poly.termArray[j].exp) { //如果當前I的指數大於J
            c.NewTerm(termArray[i].coef, termArray[i].exp);//直接丟到罐子
            i++; //下一格
        }
        else {
            c.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp); //同上
            j++;
        }
    }

    while (i < terms) {//如果J處理結束 I繼續跑
        c.NewTerm(termArray[i].coef, termArray[i].exp);
        i++;
    }

    while (j < poly.terms) { //同上
        c.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);
        j++;
    }

    return c; //回傳結果
}

// 多項式相乘
Polynomial Polynomial::Mult(Polynomial poly)
{
    Polynomial c;

    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < poly.terms; j++) { //每個都要相乘

            float sumC = termArray[i].coef * poly.termArray[j].coef; //係數相乘

            int sumE = termArray[i].exp + poly.termArray[j].exp; //指數是相加

            bool found = false; //預設沒找到相同的

            for (int k = 0; k < c.terms; k++) {
                if (c.termArray[k].exp == sumE) {  //判斷c裡面的指數是否相同

                    c.termArray[k].coef += sumC; //指數相同則同項相加

                    found = true;
                    break;
                }
            }

            if (found == false) { //沒有就額外新增
                c.NewTerm(sumC, sumE);
            }
        }
    }

    return c;
}

// 計算多項式的值
float Polynomial::Eval(float f)
{
    float sum = 0;
    for (int i = 0; i < terms; i++) {
        sum += termArray[i].coef * pow(f, termArray[i].exp); //就相乘  ex 5x^2 x=2 4*5=20
    }
    return sum;
}

// 多項式輸入
istream& operator>>(istream& in, Polynomial& poly)
{
    int n;
    in >> n;

    float coef;
    int exp;

    for (int i = 0; i < n; i++) {
        in >> coef >> exp; // 輸入係數與指數

        if (exp < 0) {
            cout << "指數不能是負數，程式結束！" << endl;
            exit(1);
        }

        poly.NewTerm(coef, exp); // 指數合法才存入
    }

    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly)
{
    if (poly.terms == 0) {
        out << '0';
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {

        if (i > 0 && poly.termArray[i].coef > 0) {
            out << '+';
        }

        out << poly.termArray[i].coef;

        if (poly.termArray[i].exp > 0) {
            out << 'x';
        }

        if (poly.termArray[i].exp > 1) {
            out << '^' << poly.termArray[i].exp;
        }
    }

    return out;
}


int main()
{
    Polynomial p1, p2, result;

    cout << "請由大到小輸入指數!\n";

    cout << "請輸入第一個多項式的項數、係數與指數：";
    cin >> p1;

    cout << "請輸入第二個多項式的項數、係數與指數：";
    cin >> p2;

    cout << "第一個多項式：" << p1 << '\n';
    cout << "第二個多項式：" << p2 << '\n';

    result = p1.Add(p2);
    cout << "相加結果：" << result << '\n';

    result = p1.Mult(p2);
    cout << "相乘結果：" << result << '\n';

    float x;
    cout << "請輸入要代入的 x：";
    cin >> x;

    cout << "第一個多項式的計算結果：" << p1.Eval(x) << '\n';
    cout << "第一個多項式的計算結果：" << p2.Eval(x) << '\n';

    return 0;
}
```

程式使用 `Polynomial` 類別管理多項式，並使用 `Term` 儲存每一項的係數與指數。

主要函式如下：

| 函式 | 功能 |
|---|---|
| `Polynomial()` | 初始化容量、項數及動態陣列 |
| `NewTerm()` | 新增一項，必要時擴充陣列 |
| `Add()` | 計算兩個多項式的相加結果 |
| `Mult()` | 計算兩個多項式的相乘結果，並合併相同指數 |
| `Eval()` | 將指定數值代入多項式計算 |
| `operator>>` | 輸入多項式的項數、係數及指數 |
| `operator<<` | 輸出多項式 |

## 三、效能分析

令第一個多項式有 \(m\) 項，第二個多項式有 \(n\) 項。

| 函式 | 時間複雜度 | 額外空間複雜度 | 原因 |
|---|---|---|---|
| `NewTerm()` | 平均攤銷 \(O(1)\)，擴充時為 \(O(k)\) | \(O(k)\) | 容量不足時複製原有的 \(k\) 項 |
| `Add()` | \(O(m+n)\) | \(O(m+n)\) | 使用兩個索引依序比較指數 |
| `Mult()` | 最壞情況 \(O(m^2n^2)\) | \(O(mn)\) | 每一組乘積都可能搜尋目前的結果 |
| `Eval()` | \(O(m)\) | \(O(1)\) | 逐項計算並累加結果 |
| 輸入與輸出 | \(O(m)\) | \(O(1)\) | 逐項讀取或輸出多項式 |

其中，`Add()` 的時間複雜度成立的前提是兩個輸入多項式都依照指數由大到小排列。

`Mult()` 會先將每一項互相相乘，再搜尋結果中是否有相同指數，因此最壞情況的時間複雜度為 \(O(m^2n^2)\)。

以上為根據目前程式推導出的複雜度，與實際執行時間的量測結果不同。

## 四、測試與驗證

### 測試一：正常多項式

第一個多項式輸入：

```text
2
5 2
5 1
```

第二個多項式輸入：

```text
2
5 3
5 2
```

預期結果：

```text
第一個多項式：5x^2+5x
第二個多項式：5x^3+5x^2
相加結果：5x^3+10x^2+5x
相乘結果：25x^5+50x^4+25x^3
```

測試結果符合多項式的基本運算規則。

### 測試二：輸入負指數

輸入：

```text
2
5 -1
```

程式輸出：

```text
指數不能是負數，程式結束！
```

程式會在讀取到負指數時呼叫 `exit(1)`，直接結束執行，避免將不符合本次作業限制的資料存入多項式。

### 測試三：零多項式

原本在輸入零係數項目時，輸出出現不必要的 `+0`。

後來我在輸出函式加入：

```cpp
if (poly.terms == 0) {
    out << '0';
    return out;
}
```

這段程式的作用是檢查多項式目前是否沒有任何項目。如果 `terms == 0`，代表沒有可以輸出的項目，就直接輸出 `0` 並回傳，避免空多項式輸出成空白。

不過，這段程式只能處理 `terms == 0` 的情況。如果輸入的零係數項目仍被存入陣列，`terms` 仍會增加，因此仍可能輸出不必要的零項目，這也是程式的不完整性。

### 測試四：負係數

第一個多項式輸入：

```text
2
-5 1
-5 0
```

第二個多項式輸入：

```text
2
3 1
2 0
```

預期結果：

```text
第一個多項式：-5x-5
第二個多項式：3x+2
相加結果：-2x-3
相乘結果：-15x^2-25x-10
```

為了正確處理正負號，輸出函式使用：

```cpp
if (i > 0 && poly.termArray[i].coef > 0) {
    out << '+';
}
```

只有在不是第一項，而且目前係數大於零時，才輸出加號。負係數本身已經帶有負號，因此不需要再額外輸出加號。

這段程式必須放在輸出係數之前，才能正確顯示正負號。

### 測試限制

目前 `Add()` 假設輸入項目已經依照指數由大到小排列。如果輸入順序不一致，相同指數的項目可能無法正確合併。因此，輸入時需要遵守程式提示，後續也可以加入自動排序功能改善這個問題。

## 效能量測（Measuring）

### 測試資料

第一個多項式：

\[
5x^2+5x
\]

第二個多項式：

\[
5x^3+5x^2
\]

代入值：

\[
x=1
\]

### 測試結果

| 測試項目 | 執行結果 |
|---|---|
| 多項式相加 | \(5x^3+10x^2+5x\) |
| 多項式相乘 | \(25x^5+50x^4+25x^3\) |
| 第一個多項式代入 \(x=1\) | 10 |
| 第二個多項式代入 \(x=1\) | 10 |
| 整體程式執行時間 | 20.6663 毫秒 |

## 六、心得討論

## 心得討論

這次真的很難。第一次寫的時候老師還沒有教到 `capacity`、`terms`、`termArray` 這些概念，我完全沒有頭緒。後來老師畫出那張圖，我用罐子的方式去理解這題，才能慢慢把程式寫出來。

寫 `Add()` 時，我一開始只想到讓兩個多項式相同位置的係數相加。這種寫法雖然容易理解，但沒有考慮到兩個多項式的指數可能不同。後來我才開始想到，要考慮不同的輸入情況，跟比較指數的大小，才能正確地將多項式相加。

寫 `Mult()` 倒是輕鬆一點，因為比較直觀，可以用 `for` 迴圈寫出來，跟數學式的概念比較接近。這次也學到平常比較少用的 `bool found = false;`，可以利用布林值幫助我判斷是否找到相同的指數，再決定要不要合併係數。

處理負係數時，學長跟我說到指數是負數的情況時，我直覺很簡單，但當我真的在寫時完全沒想法。後來查資料，知道能用 `exit()` 強制結束整個程式。不過，對於負指數的處理，其實也可以有其他方法，例如依照題目的要求拒絕不合法的輸入，或是另外設計錯誤處理方式，這是我程式的不完整。

動態陣列真的滿有趣的，當原本的空間不足時，程式會擴充陣列，讓多項式可以存放更多項目。透過老師用罐子來解釋的方式，我也更容易理解 `capacity`、`terms` 和 `termArray` 之間的關係。

寫這份作業真的花了我很多時間，過程中不斷修改程式、理解概念和查詢資料，真的滿痛苦的。但是，這次也學到了不少東西，還重新複習了以前學過的類別和建構子。

尤其是運算子多載，之前幾乎沒有用過，這次實際使用後，讓我更了解它的用途。像是 `istream& operator>>` 和 `ostream& operator<<`，一開始看不太懂，後來查資料才慢慢理解。透過這次的實作，我也更了解 `cin` 和 `cout` 的使用方式，以及如何透過自己設計的輸入與輸出運算子，讓多項式的資料讀取和輸出更符合自己的需求。
