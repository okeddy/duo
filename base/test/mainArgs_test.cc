#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    string str;
    cout<<"argc:"<<argc<<endl;
    for (int i = 0; i < argc; ++i)
    {
        cout << "arg[" << i << "]" << argv[i] << endl;
    }
    return 0;
}
