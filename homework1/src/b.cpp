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
