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
