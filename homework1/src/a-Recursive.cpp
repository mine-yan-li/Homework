#include <iostream>
using namespace std;

int ac(int m, int n) {
	if (m == 0) {
		return n + 1;
	}
	else if (m > 0 && n == 0) {
		return ac(m - 1, 1);
	}
	else if (m > 0 && n > 0) {
		return ac(m - 1, ac(m, n - 1));
	}
}

int main() {
	int a, b;
	while (cin >> a >> b) {
		cout << ac(a, b) << endl;
	}
	return 0;
}
