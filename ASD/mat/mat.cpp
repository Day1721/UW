#include <iostream>
using namespace std;

int main()
{
    string data;
    cin >> data;
    char character = data[0];
    int n = (int) data.length();
    int minimum = n - 1;
    int counter = 0;
    //TODO gwiazdki bieżące + pierwsza litera
    int k = 0;
    while(data[k] == '*')
        k++;
    character = data[k];
    k++;

    //TODO
    for(int i = k; i < n; i++)
    {
        if(data[i] == '*')
            counter++;
        else if(data[i] == character)
            counter = 0;
        else
        {
            minimum = minimum < counter ? minimum : counter;
            counter = 0;
            character = data[i];
        }
    }
    cout << n - minimum;
}