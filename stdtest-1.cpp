#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<string> v;
    string s;

    while (cin >> s)
        v.push_back(s);

    cout << "====" << endl;
    for (int i = 0; i != v.size(); ++i)
        cout << v[i] << endl;

    return 0;
}
