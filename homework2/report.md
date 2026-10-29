# 41443116
作業二


---

# 一、解題說明

## 1.1 題目說明

這次作業要使用 C++ 的類別與動態陣列來實作多項式，並完成多項式的輸入、輸出、相加、相乘及代入數值計算。

例如，以下是一個多項式：

\[
P(x)=2x^3+5x+1
\]

這個多項式可以拆成三個項目：

| 項目 | 係數（Coefficient） | 指數（Exponent） |
|---|---:|---:|
| \(2x^3\) | 2 | 3 |
| \(5x\) | 5 | 1 |
| \(1\) | 1 | 0 |

因此，我使用 `coef` 儲存每一項的係數，使用 `exp` 儲存每一項的指數，再透過 `Polynomial` 類別管理整個多項式。

本次作業需要完成以下功能：

1. 初始化多項式。
2. 新增多項式項目。
3. 輸入與輸出多項式。
4. 計算兩個多項式的相加結果。
5. 計算兩個多項式的相乘結果。
6. 將指定的數值代入多項式並計算結果。
7. 分析各項功能的時間複雜度與空間複雜度。
8. 使用測試案例驗證程式結果。
9. 實際量測不同輸入規模下的執行時間。

## 1.2 解題想法

我的想法是先把每一項多項式需要的資料分開儲存，再使用一個類別管理所有項目。

例如：

\[
P(x)=2x^3+5x
\]

可以將它儲存成：

```text
coef = 2, exp = 3
coef = 5, exp = 1
```

程式使用 `termArray` 存放這些項目，並使用 `terms` 記錄目前實際存放的項數。

由於不同多項式的項數可能不一樣，因此使用動態陣列，並在空間不足時擴充容量。

在運算方面，相加需要比較兩個項目的指數；相乘則需要將兩個多項式的每一項互相相乘，再把指數相同的項目合併。

透過這種方式，就可以使用同一個 `Polynomial` 類別處理不同的多項式。

---

# 二、Algorithm Design & Programming（演算法設計與程式實作）

## 2.1 類別設計

本次使用 `Term` 與 `Polynomial` 兩個類別。

### Term 類別

`Term` 用來儲存多項式中的單一項目。

```cpp
class Term {
    friend class Polynomial;

private:
    float coef;
    int exp;
};
```

其中：

- `coef`：儲存係數。
- `exp`：儲存指數。

例如，\(3x^2\) 的係數為 3，指數為 2。

### Polynomial 類別

`Polynomial` 用來管理整個多項式。

```cpp
class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;
};
```

各變數的用途如下：

| 變數 | 用途 |
|---|---|
| `termArray` | 儲存多項式各項的動態陣列 |
| `capacity` | 目前陣列可以存放的項數 |
| `terms` | 目前實際存放的項數 |

我一開始比較容易把 `capacity` 和 `terms` 搞混，後來將它們想成一個罐子：

`capacity` 是罐子最多可以裝幾項，`terms` 則是目前已經裝了幾項。

例如，`capacity = 4`、`terms = 2`，代表目前可以存放四項，但實際上只有兩項資料。

## 2.2 建構子與動態陣列

建立多項式物件時，先將容量設定為 1，並將項數設為 0。

```cpp
Polynomial::Polynomial()
{
    capacity = 1;
    terms = 0;
    termArray = new Term[capacity];
}
```

這樣建立物件後，就會先配置一個可以存放項目的空間。

當新增的項目超過目前容量時，就需要擴充陣列。

```cpp
if (terms == capacity) {
    capacity *= 2;

    Term* temp = new Term[capacity];

    copy(termArray, termArray + terms, temp);

    delete[] termArray;
    termArray = temp;
}
```

這段程式會將容量加倍，再把原本的資料複製到新的陣列，最後釋放舊陣列。

我使用這種方式，讓多項式不必一開始就決定固定的最大項數。

## 2.3 NewTerm() 新增項目

`NewTerm()` 負責將一個項目的係數與指數存進陣列。

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

如果陣列還有空間，就直接新增；如果空間不足，就先擴充陣列。

新增完成後，`terms` 會增加，表示多項式多了一項。

## 2.4 Add() 多項式相加

### 一開始的想法

一開始，我的想法比較簡單，先嘗試讓兩個多項式相同位置的項目直接相加。

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

這個版本的想法是將兩個係數相加，再把結果存進新的多項式 `c`。

但是後來發現，這樣寫沒有考慮到兩個多項式的指數可能不同，也沒有處理兩個多項式項數不一樣的情況。

例如：

\[
A(x)=2x^3+5x
\]

\[
B(x)=3x^2+4x+1
\]

如果直接將相同位置的係數相加，就可能把不同指數的項目加在一起。

### 修改後的想法

後來我改用兩個索引 `i` 和 `j`，分別走訪兩個多項式，並比較目前項目的指數。

處理方式如下：

1. 如果兩個指數相同，就將係數相加。
2. 如果指數不同，就先將指數較大的項目放進結果。
3. 如果其中一個多項式已經走訪完畢，就把另一個多項式剩下的項目加入結果。
4. 如果相同指數的係數相加後為 0，就不需要新增這一項。

這個方法假設兩個輸入多項式都按照指數由大到小排列。

## 2.5 Mult() 多項式相乘

多項式相乘時，需要將第一個多項式的每一項，分別乘上第二個多項式的每一項。

例如：

\[
(2x+1)(x+3)
\]

展開後為：

\[
2x^2+6x+x+3
\]

合併同類項後：

\[
2x^2+7x+3
\]

從這個例子可以看出，多項式相乘後，可能會出現指數相同的項目，因此需要合併同類項。

我的作法是使用兩層 `for` 迴圈，讓兩個多項式的每一項互相相乘。

每次相乘時：

- 係數相乘，得到新的係數。
- 指數相加，得到新的指數。
- 搜尋結果中是否已經存在相同指數。
- 如果找到相同指數，就將係數加到原本的項目。
- 如果沒有找到，就使用 `NewTerm()` 新增項目。

這樣就可以將相同指數的項目合併在一起。

## 2.6 Eval() 多項式代入計算

`Eval()` 用來計算將指定數值代入多項式後的結果。

例如：

\[
P(x)=2x^2+3x+1
\]

當 \(x=2\) 時：

\[
P(2)=2(2^2)+3(2)+1=15
\]

因此，程式會逐項計算係數乘上指定數值的指數次方，再將所有結果加總。

我使用 `pow()` 計算次方，並使用 `sum` 累加每一項的結果。

## 2.7 operator>> 與 operator<<

### operator>>

`operator>>` 負責輸入多項式。

使用者先輸入項數，再依序輸入每一項的係數與指數。

例如：

```text
3
2 3
5 1
1 0
```

代表：

\[
P(x)=2x^3+5x+1
\]

我使用 `istream& in` 接收輸入串流，再透過 `in >> coef >> exp` 讀取資料。

這樣不只能使用 `cin`，也能讓其他輸入串流使用同一個運算子。

### operator<<

`operator<<` 負責輸出多項式。

程式會逐項輸出係數，並根據指數決定是否輸出 `x` 與 `^`。

另外，我也調整了正負號的判斷，避免負係數前面又多出一個加號。

例如：

```text
-5x^2+2x+1
```

這樣就能以比較接近數學式的方式顯示結果。

---

# 三、完整程式碼（Source Code）

以下是本次作業的完整 `main.cpp`，包含多項式運算、輸入輸出、動態記憶體管理，以及效能量測功能。

```cpp
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& out, const Polynomial& poly); //讓輸出能讀取term coef exp

private:
    float coef;//係數
    int exp;//指數
};

class Polynomial {
private:
    Term* termArray; //多項式
    int capacity; //罐子空間
    int terms;//目前罐子用了多少

public:
    Polynomial();

    void NewTerm(const float thecoeff, const int theexp);

    Polynomial Add(Polynomial poly);

    Polynomial Mult(Polynomial poly);

    float Eval(float f);

    friend istream& operator>>(istream& in, Polynomial& poly);

    friend ostream& operator<<(ostream& out, const Polynomial& poly);
};

Polynomial::Polynomial()//初始化罐子
{
    capacity = 1; //是0的話會乘不了
    terms = 0;
    termArray = new Term[capacity];
}

void Polynomial::NewTerm(const float thecoeff, const int theexp) //考試有教的
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

// 多項式相加
Polynomial Polynomial::Add(Polynomial poly) 
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
    float sum=0;
    for (int i = 0; i < terms; i++) {
        sum += termArray[i].coef*pow(f, termArray[i].exp); //就相乘
    }
    return sum;
}

// 多項式輸入
istream& operator>>(istream& in, Polynomial& poly) //istream&是宣告函式格式 Polynomial& poly是把讀進來的資料存入
{
    int n;
    in >> n; //in代表傳入函式的cin 這個函式被呼叫時所輸入的會進到in
    float coef;
    int exp;

    for (int i = 0; i < n; i++) {
        in >> coef >> exp; //給值
        poly.NewTerm(coef, exp); //存入
    }
    return in; //回傳 不然跑不動
}

// 多項式輸出
ostream& operator<<(ostream& out, const Polynomial& poly)
{
    for (int i = 0; i < poly.terms; i++) {

        // 處理正負號
        if (i > 0 && poly.termArray[i].coef > 0) {
            out << '+';
        }
        // 輸出係數
        out << poly.termArray[i].coef;
        // 大於 0 才輸出 x
        if (poly.termArray[i].exp > 0) {
            out << 'x';
        }
        // 大於 1 才輸出 ^指數
        if (poly.termArray[i].exp > 1) {
            out << '^' << poly.termArray[i].exp;
        }
    }

    return out;
}

int main()
{
    Polynomial p1, p2, result;

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

    return 0;
}
```

### 程式補充說明

完整程式比最初版本多了三個與動態記憶體管理有關的函式：

- 複製建構子：建立新物件時，複製多項式資料。
- 指定運算子：將一個多項式指定給另一個物件時，複製資料而不是直接共用指標。
- 解構子：物件生命週期結束時，釋放動態陣列。

另外，`MeasurePerformance()` 會使用 `steady_clock` 量測 `Add()` 與 `Mult()` 的執行時間。測試項數為 10、20、30、40，每種規模各執行五次，再計算平均時間。

量測結果會依照電腦硬體、編譯設定與執行環境而有所不同。

---

# 四、效能分析（Time Complexity & Space Complexity）

本次使用以下符號進行分析：

- \(m\)：第一個多項式的項數。
- \(n\)：第二個多項式的項數。
- \(r\)：乘法結果中的不同指數數量，最多不超過 \(mn\)。

## 4.1 建構子

建構子只需要初始化少量成員變數並配置初始陣列。

時間複雜度：

\[
O(1)
\]

初始陣列的額外空間複雜度：

\[
O(1)
\]

## 4.2 NewTerm()

一般情況下，新增一項只需要將係數與指數存入陣列。

單次新增的時間複雜度為：

\[
O(1)
\]

當容量不足時，需要配置新陣列並複製原本的資料，因此單次擴充可能需要線性時間。

不過，由於每次容量都加倍，連續新增多項時，`NewTerm()` 的攤銷時間複雜度為：

\[
O(1)
\]

## 4.3 Add()

`Add()` 使用兩個索引走訪兩個多項式。

在輸入項目依照指數遞減排列的前提下，每個項目最多被處理一次。

因此時間複雜度為：

\[
O(m+n)
\]

結果最多需要儲存 \(m+n\) 項，因此額外空間複雜度為：

\[
O(m+n)
\]

## 4.4 Mult()

`Mult()` 使用兩層迴圈，將兩個多項式的所有項目配對。

配對次數最多為：

\[
mn
\]

每次配對後，程式還會搜尋目前的結果，判斷是否有相同指數。若結果中有 \(r\) 個不同指數，搜尋最壞需要 \(O(r)\) 時間。

因此，更精確的時間複雜度可以寫成：

\[
O(mnr)
\]

由於：

\[
r\leq mn
\]

所以最壞情況下：

\[
O(mn\cdot mn)=O(m^2n^2)
\]

結果最多需要儲存 \(mn\) 項，因此額外空間複雜度為：

\[
O(mn)
\]

## 4.5 Eval()

`Eval()` 逐項計算多項式的值。

若多項式有 \(m\) 項，則時間複雜度為：

\[
O(m)
\]

額外空間複雜度為：

\[
O(1)
\]

這裡將每次 `pow()` 視為一次基本運算；實際計算時間仍會受到指數大小與數學函式實作影響。

## 4.6 operator>> 與 operator<<

輸入運算子會逐項讀取資料，因此輸入 \(m\) 項時，時間複雜度為：

\[
O(m)
\]

輸出運算子也會逐項走訪多項式，因此時間複雜度為：

\[
O(m)
\]

輸出本身使用固定數量的額外變數，所以額外空間複雜度為：

\[
O(1)
\]

輸入時需要將資料存入多項式，因此所需空間為：

\[
O(m)
\]

## 4.7 複雜度整理

| 功能 | 時間複雜度 | 額外空間複雜度 |
|---|---|---|
| 建構子 | \(O(1)\) | \(O(1)\) |
| `NewTerm()` | 攤銷 \(O(1)\) | 擴充時需要新陣列 |
| `Add()` | \(O(m+n)\) | \(O(m+n)\) |
| `Mult()` | 最壞 \(O(m^2n^2)\) | \(O(mn)\) |
| `Eval()` | \(O(m)\) | \(O(1)\) |
| `operator>>` | \(O(m)\) | \(O(m)\) |
| `operator<<` | \(O(m)\) | \(O(1)\) |

---

# 五、測試與驗證（Testing and Proving）

本次測試使用 Visual Studio 執行程式，並將程式結果與手動計算結果進行比對。

## 5.1 測試案例一：多項式相加、相乘與代入

第一個多項式：

\[
P(x)=2x^3+5x
\]

第二個多項式：

\[
Q(x)=2x^3+5x^2+5x
\]

### 相加驗證

\[
\begin{aligned}
P(x)+Q(x)
&=(2x^3+5x)+(2x^3+5x^2+5x)\\
&=4x^3+5x^2+10x
\end{aligned}
\]

預期結果：

```text
4x^3+5x^2+10x
```

### 相乘驗證

\[
\begin{aligned}
P(x)Q(x)
&=(2x^3+5x)(2x^3+5x^2+5x)\\
&=4x^6+10x^5+10x^4\\
&\quad+10x^4+25x^3+25x^2\\
&=4x^6+10x^5+20x^4+25x^3+25x^2
\end{aligned}
\]

預期結果：

```text
4x^6+10x^5+20x^4+25x^3+25x^2
```

### 代入驗證

當 \(x=2\) 時：

\[
P(2)=2(2^3)+5(2)
\]

\[
P(2)=16+10=26
\]

預期結果：

```text
26
```

以上結果與實際測試畫面一致。

## 5.2 測試案例二：負係數

第一個多項式：

\[
P(x)=-5x^2+2x+1
\]

第二個多項式：

\[
Q(x)=3x^2+5x+9
\]

### 相加驗證

\[
\begin{aligned}
P(x)+Q(x)
&=(-5x^2+2x+1)+(3x^2+5x+9)\\
&=-2x^2+7x+10
\end{aligned}
\]

預期結果：

```text
-2x^2+7x+10
```

### 相乘驗證

\[
\begin{aligned}
P(x)Q(x)
&=(-5x^2+2x+1)(3x^2+5x+9)\\
&=-15x^4-25x^3-45x^2\\
&\quad+6x^3+10x^2+18x\\
&\quad+3x^2+5x+9\\
&=-15x^4-19x^3-32x^2+23x+9
\end{aligned}
\]

預期結果：

```text
-15x^4-19x^3-32x^2+23x+9
```

### 代入驗證

當 \(x=3\) 時：

\[
P(3)=-5(3^2)+2(3)+1
\]

\[
P(3)=-45+6+1=-38
\]

預期結果：

```text
-38
```

以上結果與實際測試畫面一致。

## 5.3 測試案例三：相同指數的係數相消

第一個多項式：

\[
A(x)=3x^2+2x+1
\]

第二個多項式：

\[
B(x)=-3x^2+4x+5
\]

相加後：

\[
A(x)+B(x)=6x+6
\]

因為：

\[
3x^2+(-3x^2)=0
\]

所以 \(x^2\) 項會消失。

這個案例可以用來驗證 `Add()` 是否能正確處理係數相加後等於 0 的情況。

預期結果：

```text
6x+6
```

注意：這個案例是額外設計的測試案例，正式報告應在實際執行並確認結果後，再將它標記為已通過。

## 5.4 測試案例四：常數多項式

第一個多項式：

\[
A(x)=5
\]

第二個多項式：

\[
B(x)=3
\]

相加：

\[
A(x)+B(x)=8
\]

相乘：

\[
A(x)B(x)=15
\]

這個案例可以確認指數為 0 的項目能否正確輸入、輸出與運算。

預期結果：

```text
相加結果：8
相乘結果：15
```

這個案例也需要在實際執行後確認。

## 5.5 測試結果整理

| 測試項目 | 預期結果 | 狀態 |
|---|---|---|
| 正係數輸入與輸出 | 正確顯示多項式 | 已測試 |
| 多項式相加 | 與手動計算一致 | 已測試 |
| 多項式相乘 | 與手動展開結果一致 | 已測試 |
| 負係數輸出 | 正確顯示負號 | 已測試 |
| `Eval()` 代入計算 | 與手動計算一致 | 已測試 |
| 相同指數的係數相消 | 相消後不輸出該項 | 待執行 |
| 常數多項式 | 正確處理指數 0 | 待執行 |

---

# 六、效能量測（Performance Measuring）

## 6.1 量測目的

效能分析是透過演算法結構推導時間複雜度與空間複雜度；效能量測則是實際執行程式，觀察不同輸入規模下的執行時間。

因此，本次作業加入 `MeasurePerformance()`，讓程式自動產生測試用多項式，並量測 `Add()` 和 `Mult()` 的執行時間。

## 6.2 量測方式

本次量測設定如下：

- 測試項數：10、20、30、40。
- 每種項數重複執行 5 次。
- 使用 `steady_clock` 記錄開始與結束時間。
- 將執行時間換算成微秒（us）。
- 計算五次執行時間的平均值。
- 分別記錄相加與相乘的平均執行時間。

程式會自動輸出類似以下格式的表格：

```text
========== 效能量測 ==========
時間單位：微秒 (us)

項數        Add 平均時間(us)    Mult 平均時間(us)
10          由程式實際輸出       由程式實際輸出
20          由程式實際輸出       由程式實際輸出
30          由程式實際輸出       由程式實際輸出
40          由程式實際輸出       由程式實際輸出
```

以上僅為輸出格式示意，並非實際量測數據。

## 6.3 實際量測結果

將程式編譯並執行後，請把終端機實際輸出的數字填入下表。

| 項數 | Add 平均時間（us） | Mult 平均時間（us） |
|---:|---:|---:|
| 10 | 執行後填入 | 執行後填入 |
| 20 | 執行後填入 | 執行後填入 |
| 30 | 執行後填入 | 執行後填入 |
| 40 | 執行後填入 | 執行後填入 |

目前尚未在實際開發環境執行這份量測程式，因此不預先填入數值。

## 6.4 量測結果討論

完成量測後，可以比較項數增加時，相加與相乘的執行時間變化。

依照演算法分析，`Add()` 的時間複雜度為：

\[
O(m+n)
\]

因此，在輸入規模增加時，相加的工作量預期會隨項數大致線性增加。

`Mult()` 的時間複雜度最壞為：

\[
O(m^2n^2)
\]

因為每一組項目乘積都可能需要搜尋目前的結果，所以相乘的執行時間預期會比相加更容易隨輸入規模增加而上升。

不過，實際量測結果也會受到電腦效能、編譯器最佳化、作業系統背景工作及測試規模影響，因此需要以實際數據進行討論，不能只根據理論複雜度判斷實際執行時間。

---

# 七、編譯與執行指令（Compile and Run）

## 7.1 使用 Visual Studio

1. 開啟 Visual Studio。
2. 開啟 C++ 專案。
3. 將完整程式碼放入 `main.cpp`。
4. 建置專案。
5. 執行程式。
6. 依照畫面輸入兩個多項式。
7. 查看相加、相乘與代入計算結果。
8. 查看程式最後輸出的效能量測表格。

## 7.2 使用 g++

如果環境已經安裝 g++，也可以透過命令列編譯：

```bash
g++ main.cpp -std=c++11 -O2 -o polynomial
```

Windows 命令提示字元可以執行：

```bash
polynomial.exe
```

Linux 或 macOS 可以執行：

```bash
./polynomial
```

---

# 八、結論（Conclusion）

這次作業讓我練習使用 C++ 類別與動態陣列處理多項式。

一開始我對多項式相加的想法比較簡單，只想到將兩個多項式相同位置的係數直接相加。但是實際思考後發現，不同多項式的項數與指數可能不一樣，因此不能只依照陣列位置計算。

後來我改用兩個索引比較指數，才逐漸理解為什麼相加時需要判斷指數大小，以及為什麼相乘時還需要合併同類項。

在實作 `Mult()` 時，我使用兩層迴圈讓兩個多項式的每一項互相相乘，再利用另一層迴圈搜尋是否存在相同指數。雖然程式比原本的相加版本複雜，但透過實際測試，我能更清楚地理解每個項目如何產生，以及同類項如何合併。

除此之外，這次也練習了 `operator>>` 和 `operator<<`。一開始我不太熟悉運算子多載，後來透過 `istream`、`ostream` 與回傳串流的寫法，了解如何讓自訂類別也能使用 `cin >> p1` 和 `cout << p1`。

我也發現，程式能夠成功編譯與執行，不代表結果一定正確，因此需要使用實際案例和手動計算互相比對。

最後，透過時間複雜度分析與效能量測，可以從理論和實際執行兩個角度觀察不同演算法的差異。

整體來說，這次作業讓我對動態陣列、類別、迴圈、多項式運算與複雜度分析有更完整的理解。

---

# 九、心得討論及開發報告（Reflection and Development Report）

這次作業不是一開始就能直接完成的，而是從比較簡單的程式開始，再逐步修改。

最初在撰寫 `Add()` 時，我只想到讓兩個多項式相同位置的係數相加。這種寫法容易理解，但沒有考慮到兩個多項式的指數可能不同。

後來，我開始思考如何判斷兩個項目是否屬於同類項，並加入指數比較的邏輯。這個過程讓我理解到，寫程式時不能只考慮最簡單的情況，還需要思考不同輸入可能造成的問題。

在實作 `Mult()` 時，我使用兩層迴圈將每一項互相相乘，再透過另一層迴圈搜尋是否有相同指數。這讓我更了解多項式乘法的原理，也讓我開始注意程式在資料量增加時的執行效率。

另外，輸出負係數也是這次遇到的問題。原本我在每一項後面都固定輸出加號，結果遇到負係數時就會出現 `+-`。後來我改成判斷係數的正負，才讓輸出結果更符合一般多項式的寫法。

在動態陣列的部分，我也更清楚地理解 `capacity` 和 `terms` 的差別。`capacity` 是目前可用的容量，`terms` 則是實際存放的項數。當空間不足時，程式會擴充陣列，讓多項式可以存放更多項目。

此外，因為程式使用動態記憶體，也需要考慮物件複製與記憶體釋放的問題。因此，我進一步加入複製建構子、指定運算子與解構子，避免不同物件直接共用同一個動態陣列。

我認為這次作業最大的收穫，是開始理解程式不只是把語法寫出來，還需要考慮資料如何儲存、演算法如何處理不同情況，以及最後如何驗證結果。

之後如果有機會繼續改進，我會再加強輸入資料檢查、零係數處理、乘法結果排序與效能量測，讓整個程式更加完整。

---
