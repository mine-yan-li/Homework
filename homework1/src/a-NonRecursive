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
