#include <iostream>
#include <string>
#include <sstream>

using namespace std;

long long interchange_prices(long long const aggregated_prices[], int const array_right[], int const array_left[],
                             int index, int array_size)
{
    int index_left = array_size - index - 1;
    if(array_right[index_left] == -1 || array_left[index_left] == -1) return -1;
    return aggregated_prices[index] - array_right[index_left] + array_left[index_left];
}

void print_result(int number_of_products, long long const aggregated_prices[], int const even_right[], int const even_left[],
                  int const odd_right[], int const odd_left[], int array_size)
{
    if(aggregated_prices[number_of_products - 1] % 2 == 1) //
        printf("%lld\n", aggregated_prices[number_of_products - 1]);
    else if(number_of_products == array_size) //if number of products equals array size and the sum is even
        printf("-1\n");
    else
    {
        long long price_one = interchange_prices(aggregated_prices, even_right, odd_left, number_of_products - 1,
                                                 array_size);
        long long price_two = interchange_prices(aggregated_prices, odd_right, even_left, number_of_products - 1,
                                                 array_size);
        printf("%lld\n", price_one > price_two ? price_one : price_two);
    }
}

int main()
{
    string input;
    int array_size;
    //getting the size of an array
    cin >> array_size;

    int array_of_prices[array_size];

    // filling array with prices
    for(int i = 0; i < array_size; i++)
        cin >> array_of_prices[i];

    //constructing array with aggregated prices (counting in reverse)

    long long aggregated_prices[array_size];
    long long previous = 0;

    for(int i = 0; i < array_size; i++)
    {
        aggregated_prices[i] = array_of_prices[array_size - 1 - i] + previous;
        previous = aggregated_prices[i];
    }

    //constructing arrays with even prices (starting from right side)

    int even_right[array_size];
    int smallest_even_number = -1;
    int odd_right[array_size];
    int smallest_odd_number = -1;
    for(int i = array_size - 1; i >= 0; i--)
    {
        if(array_of_prices[i] % 2 == 0)
        {
            even_right[i] = array_of_prices[i];
            smallest_even_number = array_of_prices[i];
            odd_right[i] = smallest_odd_number;
        }
        else
        {
            even_right[i] = smallest_even_number;
            odd_right[i] = array_of_prices[i];
            smallest_odd_number = array_of_prices[i];
        }
    }

    //constructing arrays with odd prices (starting from right side)

    int even_left[array_size];
    int biggest_even_number = -1;
    int odd_left[array_size];
    int biggest_odd_number = -1;
    for(int i = 0; i < array_size; i++)
    {
        if(array_of_prices[i] % 2 == 0)
        {
            even_left[i] = biggest_even_number;
            biggest_even_number = array_of_prices[i];
            odd_left[i] = biggest_odd_number;
        }
        else
        {
            even_left[i] = biggest_even_number;
            odd_left[i] = biggest_odd_number;
            biggest_odd_number = array_of_prices[i];
        }
    }

    //reading number of days
    int days;
    cin >> days;

    //reading prices for a given day and printing the result
    int number_of_products;
    int i = 0;
    while(i < days)
    {
        cin >> number_of_products;
        //getline(cin, input);
        //number_of_products = stoi(input);
        print_result(number_of_products, aggregated_prices, even_right, even_left, odd_right, odd_left, array_size);
        i++;
    }

    return 0;
}