# 41443116

作業一

# 題目一：Ackermann Function

## 解題說明

本題要求實現 Ackermann Function（阿克曼函數），並分別使用遞迴與非遞迴方式進行實作。

Ackermann 函數的定義如下：

`A(m,n) = n + 1，當 m = 0`

`A(m,n) = A(m-1,1)，當 m > 0 且 n = 0`

`A(m,n) = A(m-1,A(m,n-1))，當 m > 0 且 n > 0`

遞迴版本直接依照 Ackermann Function 的數學定義進行實作。

非遞迴版本則不直接使用函式的遞迴呼叫，而是使用 `Node` 結構與陣列 `s[1000]` 自行建立 Stack，模擬遞迴函式執行時的 Call Stack。

程式最後會同時輸出遞迴版本與非遞迴版本的計算結果，方便比較兩種方法是否得到相同結果。

## 解題策略

### 遞迴版本

1. 使用遞迴函式 `ac()` 計算 Ackermann Function。
2. 當 `m == 0` 時，返回 `n + 1` 作為遞迴的結束條件。
3. 當 `m > 0` 且 `n == 0` 時，呼叫 `ac(m - 1, 1)`。
4. 當 `m > 0` 且 `n > 0` 時，使用巢狀遞迴計算 `ac(m - 1, ac(m, n - 1))`。
5. 主程式讀取 `m` 與 `n`，並輸出計算結果。

### 非遞迴版本

1. 使用 `Node` 結構儲存目前 Ackermann 函式中的 `m`、`n` 以及目前的執行狀態 `state`。
2. 使用陣列 `s[1000]` 作為 Stack，並使用 `top` 記錄目前 Stack 的位置。
3. 將原本的遞迴呼叫轉換成 Stack 的 `push` 與 `pop` 概念。
4. `state` 用來記錄目前的函式是否已經完成內層遞迴。
5. 當 `m == 0` 時，計算 `n + 1`，並將目前的 Stack 移除。
6. 當 `n == 0` 時，將 `m` 減少 1，並將 `n` 設為 1。
7. 當 `m > 0` 且 `n > 0` 時，先保存目前狀態，再建立下一個 Stack 節點處理 `ac(m, n - 1)`。
8. 最後得到的 `result` 就是 Ackermann Function 的結果。

## 程式實作

以下為完整程式碼：

```cpp
#include <iostream>
using namespace std;

int ac(int m, int n)
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ac(m - 1, 1);
    else
        return ac(m - 1, ac(m, n - 1));
}

struct Node
{
    int m;
    int n;
    int state;
};

int ac_nonrecursive(int m, int n)
{
    Node s[1000];
    int top = -1;
    int result = 0;

    top++;
    s[top].m = m;
    s[top].n = n;
    s[top].state = 0;

    while (top >= 0)
    {
        if (s[top].state == 1)
        {
            s[top].n = result;
            s[top].state = 0;
        }

        else if (s[top].m == 0)
        {
            result = s[top].n + 1;
            top--;
        }

        // n == 0
        else if (s[top].n == 0)
        {
            s[top].m--;
            s[top].n = 1;
        }

        // m > 0 && n > 0
        else
        {
            int old_m = s[top].m;
            int old_n = s[top].n;

            s[top].m = old_m - 1;
            s[top].n = 0;
            s[top].state = 1;

            top++;
            s[top].m = old_m;
            s[top].n = old_n - 1;
            s[top].state = 0;
        }
    }

    return result;
}

int main()
{
    int a, b;

    cin >> a >> b;

    cout << "Recursive: "
         << ac(a, b) << endl;

    cout << "Nonrecursive: "
         << ac_nonrecursive(a, b) << endl;

    return 0;
}
```

## 程式說明

### 遞迴函式 `ac()`

```cpp
int ac(int m, int n)
{
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return ac(m - 1, 1);
    else
        return ac(m - 1, ac(m, n - 1));
}
```

這個函式直接按照 Ackermann Function 的三種條件進行判斷。

當：

```cpp
m == 0
```

直接回傳：

`n + 1`

當：

```cpp
m > 0 && n == 0
```

計算：

`A(m,0) = A(m-1,1)`

當：

```cpp
m > 0 && n > 0
```

計算：

`A(m,n) = A(m-1,A(m,n-1))`

因此遞迴版本的程式碼與 Ackermann Function 的定義非常接近。

### `Node` 結構

```cpp
struct Node
{
    int m;
    int n;
    int state;
};
```

非遞迴版本需要自行保存原本遞迴函式中的資訊，因此使用 `Node` 結構保存三個資料。

- `m`：目前的 `m` 值。
- `n`：目前的 `n` 值。
- `state`：記錄目前遞迴狀態。

其中 `state` 主要用來判斷目前的內層計算是否已經完成。

### Stack 的建立

```cpp
Node s[1000];
int top = -1;
int result = 0;
```

使用 `s[1000]` 建立一個可以儲存 `Node` 的陣列，並利用 `top` 記錄目前 Stack 的位置。

這個 Stack 的作用就是模擬原本遞迴函式使用的 Call Stack。

### `state == 1`

```cpp
if (s[top].state == 1)
{
    s[top].n = result;
    s[top].state = 0;
}
```

當目前的狀態為 `1` 時，表示前面的內層計算已經完成。

此時將計算結果 `result` 放回目前節點的 `n`，再繼續處理外層的計算。

### `m == 0`

```cpp
else if (s[top].m == 0)
{
    result = s[top].n + 1;
    top--;
}
```

這裡對應 Ackermann Function 的：

`A(0,n) = n + 1`

計算完成後將目前的 Stack 節點移除。

### `n == 0`

```cpp
else if (s[top].n == 0)
{
    s[top].m--;
    s[top].n = 1;
}
```

這裡對應：

`A(m,0) = A(m-1,1)`

因此將 `m` 減少 1，並將 `n` 設定為 1。

### `m > 0 && n > 0`

```cpp
else
{
    int old_m = s[top].m;
    int old_n = s[top].n;

    s[top].m = old_m - 1;
    s[top].n = 0;
    s[top].state = 1;

    top++;
    s[top].m = old_m;
    s[top].n = old_n - 1;
    s[top].state = 0;
}
```

這一部分是非遞迴版本最重要的地方。

原本遞迴版本：

```cpp
return ac(m - 1, ac(m, n - 1));
```

會先計算：

```cpp
ac(m, n - 1)
```

再將結果帶入：

```cpp
ac(m - 1, result)
```

非遞迴版本沒有直接呼叫自己，因此使用 Stack 保存原本的狀態，再建立新的 Stack 節點處理內層計算。

## 效能分析

### 遞迴版本

1. 時間複雜度：Ackermann 函數的成長速度非常快，遞迴呼叫數量會隨著 `m` 與 `n` 增加而快速增加，因此無法用一般的 `O(n)`、`O(n²)` 或 `O(log n)` 簡單表示。
2. 空間複雜度：主要來自遞迴呼叫所使用的 Call Stack，因此為 `O(recursion depth)`。
3. 當輸入值過大時，可能因為遞迴層數過深而發生 Stack Overflow。

### 非遞迴版本

1. 時間複雜度：非遞迴版本雖然沒有直接使用函式遞迴，但仍然需要模擬 Ackermann Function 原本的遞迴計算，因此計算量仍會隨著輸入快速增加。
2. 空間複雜度：使用 `Node s[1000]` 建立固定大小的 Stack，因此會使用額外的陣列空間。
3. 非遞迴版本不會直接使用系統的 Call Stack，而是自行使用陣列模擬 Stack。
4. 當實際需要的 Stack 深度超過 `1000` 時，陣列大小可能不足，因此這個版本仍然有 Stack 容量限制。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 `m n` | 預期輸出 | Recursive | Nonrecursive |
|----------|----------------|----------|-----------|--------------|
| 測試一 | `0 0` | 1 | 1 | 1 |
| 測試二 | `0 1` | 2 | 2 | 2 |
| 測試三 | `1 0` | 2 | 2 | 2 |
| 測試四 | `1 1` | 3 | 3 | 3 |
| 測試五 | `2 2` | 7 | 7 | 7 |
| 測試六 | `3 2` | 29 | 29 | 29 |

由測試結果可以確認，遞迴版本與非遞迴版本在相同輸入下都可以得到相同的 Ackermann Function 結果。

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o a a.cpp
$ ./a
```

輸入：

```text
2 2
```

輸出：

```text
Recursive: 7
Nonrecursive: 7
```

輸入：

```text
3 2
```

輸出：

```text
Recursive: 29
Nonrecursive: 29
```

## 結論

1. 遞迴版本可以依照 Ackermann Function 的數學定義直接完成計算。
2. 非遞迴版本利用 `Node` 結構與陣列 Stack 模擬原本的 Call Stack。
3. 兩種版本使用不同的方法處理 Ackermann Function，但最後可以得到相同的結果。
4. 遞迴版本的程式碼比較簡單，也比較容易直接對照數學公式。
5. 非遞迴版本需要自行管理 Stack 與 `state`，程式碼比較複雜。
6. 非遞迴版本可以避免直接使用系統 Call Stack，但仍然需要自行建立 Stack，因此並不是完全不需要 Stack。
7. Ackermann Function 的數值成長非常快，因此實際測試時不適合使用太大的輸入值。

## 申論及開發報告

### 選擇遞迴與非遞迴的原因

本題分別使用遞迴與非遞迴兩種方式實作 Ackermann Function，主要是為了比較兩種方法在程式設計上的差異。

1. **遞迴版本較接近數學定義**

   Ackermann Function 本身就是使用遞迴方式定義，因此直接使用遞迴實作會比較簡單。

   例如：

   `A(0,n) = n + 1`

   在程式中：

   ```cpp
   if (m == 0)
       return n + 1;
   ```

   兩者可以直接對照，因此比較容易理解。

2. **非遞迴版本可以了解 Stack 的作用**

   遞迴函式執行時，系統會使用 Call Stack 保存函式呼叫的資訊。

   非遞迴版本則是自己建立：

   ```cpp
   Node s[1000];
   ```

   再利用 `top` 管理 Stack，因此可以模擬原本遞迴函式的執行方式。

3. **`state` 用來保存遞迴狀態**

   在 Ackermann Function 中：

   ```cpp
   ac(m - 1, ac(m, n - 1))
   ```

   需要先完成內層的 `ac(m, n - 1)`，才能繼續計算外層的 `ac(m - 1, result)`。

   因此非遞迴版本需要使用 `state` 記錄目前的計算進度。

4. **兩種方法可以互相比較**

   遞迴版本的優點是程式碼比較簡單，並且容易對照數學公式。

   非遞迴版本雖然程式碼比較複雜，但是可以了解遞迴背後的 Stack 運作方式，也可以知道遞迴其實可以透過自行管理 Stack 的方式進行模擬。

透過這次實作，可以比較清楚地了解遞迴函式在執行時的運作方式。遞迴版本比較容易撰寫，而非遞迴版本需要自行處理 Stack 與執行狀態，因此實作難度比較高。不過非遞迴版本可以幫助理解遞迴與 Stack 之間的關係。

# 題目二：冪集（Power Set）

## 解題說明

本題要求實現一個遞迴函式，計算輸入字串的所有子集合，也就是 Power Set（冪集）。

假設輸入字串為：

```text
ABC
```

每一個字元都有選擇或不選擇兩種情況，因此可以產生：

```text
空集合
B
C
BC
A
AC
AB
ABC
```

一個包含 `n` 個元素的集合，其冪集共有：

`2^n`

個子集合。

本程式使用遞迴的方式，讓每個字元分別進行「不選擇」與「選擇」兩種情況，最後產生所有可能的子集合。

## 解題策略

1. 使用 `powerset()` 函式進行遞迴。
2. 使用 `index` 表示目前正在處理字串中的位置。
3. 使用 `current` 儲存目前已經選擇的字元。
4. 當 `index == s.length()` 時，表示所有字元都已經處理完成，直接輸出 `current`。
5. 每一個字元都有兩種情況，一種是不選擇，一種是選擇。
6. 不選擇目前字元時，直接進入下一個位置。
7. 選擇目前字元時，將 `s[index]` 加到 `current` 後，再進入下一個位置。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <string>
using namespace std;

void powerset(string s, int index, string current)
{
    if (index == s.length()) {
        cout << current << endl;
        return;
    }

    powerset(s, index + 1, current);
    powerset(s, index + 1, current + s[index]);
}

int main() {
    string n;
    cin >> n;
    powerset(n, 0, "");
}
```

## 程式說明

### `powerset()` 函式

```cpp
void powerset(string s, int index, string current)
```

其中：

- `s`：輸入的字串。
- `index`：目前正在處理的字元位置。
- `current`：目前已經選擇的字元。

當：

```cpp
if (index == s.length()) {
    cout << current << endl;
    return;
}
```

表示所有字元都已經處理完成，此時 `current` 就是一個完整的子集合，所以直接輸出。

### 不選擇目前的字元

```cpp
powerset(s, index + 1, current);
```

這一行代表目前的字元不加入子集合，只將 `index` 往下一個位置移動。

### 選擇目前的字元

```cpp
powerset(s, index + 1, current + s[index]);
```

這一行代表選擇目前的字元，將 `s[index]` 加到 `current` 裡，再繼續處理下一個字元。

因此每一個字元都會產生兩種結果。

例如輸入：

```text
AB
```

第一個字元 `A` 可以選擇或不選擇。

如果不選擇 `A`，再處理 `B`：

```text
""
"B"
```

如果選擇 `A`，再處理 `B`：

```text
"A"
"AB"
```

所以最後會產生：

```text
""
"B"
"A"
"AB"
```

空字串代表空集合。

## 效能分析

假設輸入字串長度為 `n`。

1. 時間複雜度：每個字元都有選擇與不選擇兩種情況，因此總共會產生 `2^n` 個子集合。如果將每個子集合輸出的字元數也計算進去，時間複雜度約為：

   `O(n × 2^n)`

2. 空間複雜度：遞迴深度最多為 `n`，因此 Call Stack 的空間複雜度為：

   `O(n)`

3. `current` 會隨著遞迴過程儲存目前選擇的字元，因此額外空間也與 `n` 有關。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入字串 | 預期子集合數量 | 實際子集合數量 |
|----------|----------|----------------|----------------|
| 測試一 | `A` | 2 | 2 |
| 測試二 | `AB` | 4 | 4 |
| 測試三 | `ABC` | 8 | 8 |
| 測試四 | `ABCD` | 16 | 16 |
| 測試五 | `ABCDE` | 32 | 32 |

冪集的數量可以使用：

`2^n`

進行驗證。

例如輸入：

```text
ABC
```

因為字串長度為 3：

`2^3 = 8`

所以應該產生 8 個子集合。

### 測試案例一

輸入：

```text
A
```

輸出：

```text

A
```

第一行為空集合，第二行為 `A`。

### 測試案例二

輸入：

```text
AB
```

輸出：

```text

B
A
AB
```

第一行為空集合，之後依序產生 `B`、`A`、`AB`，總共 4 個子集合。

### 測試案例三

輸入：

```text
ABC
```

輸出：

```text

C
B
BC
A
AC
AB
ABC
```

第一行為空集合，總共產生 8 個子集合。

### 編譯與執行指令

假設程式檔案名稱為 `b.cpp`：

```shell
$ g++ -std=c++17 -o b b.cpp
$ ./b
```

輸入：

```text
ABC
```

輸出：

```text

C
B
BC
A
AC
AB
ABC
```

## 結論

1. 程式可以利用遞迴方式產生輸入字串的所有子集合。
2. 每個字元都有選擇與不選擇兩種情況。
3. `current` 用來記錄目前已經選擇的字元。
4. 當所有字元處理完成後，就輸出目前的 `current`。
5. 一個有 `n` 個元素的集合，其冪集共有 `2^n` 個子集合。
6. 測試不同長度的字串後，實際產生的數量符合 `2^n`。
7. 當字串長度增加時，子集合數量會快速增加，因此程式需要處理的資料也會變多。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算字串的冪集，主要原因如下：

1. **程式邏輯簡單直觀**  
   每一個字元都只有兩種選擇，一種是放入目前的子集合，另一種是不放入目前的子集合。

   例如輸入 `ABC`，處理 `A` 時可以分成：

   ```text
   不選擇 A
   選擇 A
   ```

   之後再繼續處理 `B` 和 `C`。

2. **容易理解與實現**  
   程式只需要使用兩次遞迴呼叫，就可以處理每一個字元：

   ```cpp
   powerset(s, index + 1, current);
   powerset(s, index + 1, current + s[index]);
   ```

   第一個函式代表不選擇目前字元，第二個函式代表選擇目前字元。

3. **遞迴的語意清楚**  
   `index` 負責記錄目前處理到哪一個字元，`current` 則記錄目前已經選擇的內容。

   當：

   ```cpp
   if (index == s.length()) {
       cout << current << endl;
       return;
   }
   ```

   代表所有字元都已經處理完成，因此可以將目前的結果輸出。

透過遞迴實作 Power Set，可以清楚了解每個元素的選擇與不選擇。這種方式程式碼比較簡單，也容易理解。不過當字串長度增加時，因為每個字元都有兩種可能，所以產生的子集合數量會快速增加。

# 作業總結

本次作業主要練習遞迴與非遞迴的使用方式。

第一題是 Ackermann Function，分別使用遞迴與非遞迴兩種方式進行實作。遞迴版本直接依照 Ackermann Function 的數學定義進行計算，程式碼比較簡單。非遞迴版本則使用 `Node` 結構和陣列 Stack 模擬遞迴過程，可以了解遞迴函式背後的 Call Stack 如何運作。

第二題是 Power Set，利用每個字元的「選擇」和「不選擇」兩種情況產生所有子集合。這題可以練習如何使用兩個遞迴分支處理不同的可能結果。

透過這次作業，可以比較遞迴與非遞迴兩種方法的差異。遞迴方式比較接近數學定義，程式碼也比較簡單；非遞迴方式雖然需要自行管理 Stack 和狀態，但是可以更了解遞迴執行時的實際運作方式。

# 兩題複雜度比較

| 題目 | 實作方式 | 時間複雜度 | 空間複雜度 |
|------|----------|------------|------------|
| 題目一 Ackermann | 遞迴 | 隨 Ackermann 函數快速成長 | `O(recursion depth)` |
| 題目一 Ackermann | 非遞迴 | 隨 Ackermann 函數快速成長 | `O(recursion depth)` |
| 題目二 Power Set | 遞迴 | `O(n × 2^n)` | `O(n)` |

題目一的遞迴與非遞迴版本雖然使用不同的實作方式，但是兩者都需要處理 Ackermann Function 本身非常快速增加的計算量。

遞迴版本使用系統的 Call Stack，而非遞迴版本則使用自行建立的 Stack 來模擬，因此兩者的主要差異在於 Stack 的管理方式。

題目二則是利用遞迴處理每個元素的選擇與不選擇，因此當輸入長度增加時，產生的子集合數量會以 `2^n` 快速增加。
