#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stack>

using namespace std;

float calc(float &x, float &y, char &z);
void separate_numbers_symbols(string &expression, vector<float> *Copy_Numbers, vector<char> *Copy_Symbols);
void calc_bet_vectors(vector<float> *Copy_Numbers, vector<char> *Copy_Symbols);

int main(void)
{
    string eq;
    vector<char> Symbols;
    vector<float> Numbers;

    cout << "Write an equation that follow the following instructions: \n\t";
    cout << "1. Don\'t two brackets without closing the first one\n\t";
    cout << "2. Only use one of the following (+, -, *, /, ^)\n\t";
    cout << "3.Don\'t expect negative number\n";
    cout << "Start:\n";
    cin >> eq;

    separate_numbers_symbols(eq, &Numbers, &Symbols);

    calc_bet_vectors(&Numbers, &Symbols);

    cout << " = " << Numbers[0] << endl;

    return 0;
}

// This function calculate two numbers.
float calc(float &x, float &y, char &z)
{
    switch (z)
    {
    case '*':
        return x * y;

    case '/':
        if (y == 0)

        {
            throw out_of_range("You can't divide by 0");
        }
        else
            return x / y;

    case '^':
        return pow(x, y);

    case '+':
        return x + y;

    case '-':
        return x - y;

    default:
        throw out_of_range("Your operator is out of the range");
        break;
    }
}

// Separates numbers and symbols from a string expression into separate vectors.
void separate_numbers_symbols(string &expression, vector<float> *Copy_Numbers, vector<char> *Copy_Symbols)
{
    string number_str;

    // Iterate through each character in the expression
    for (char c : expression)

    {
        if (isdigit(c) || c == '.')

        {
            number_str += c;
        }
        else if (!number_str.empty()) // If a number has been accumulated, convert it to float and store it

        {
            (*Copy_Numbers).push_back(stof(number_str));
            number_str.clear(); // Reset the number string for the next number
        }
        if ((c != ' ' && c != '.') && !isdigit(c))

        {
            (*Copy_Symbols).push_back(c); // Ignore spaces
        }
    }

    // Handle the last number if any
    if (!number_str.empty())

    {
        (*Copy_Numbers).push_back(stof(number_str));
        number_str.clear();
    }
}

// Evaluates an expression represented by vectors of numbers and symbols,
// handling basic parenthesis nesting and operator precedence.
void calc_bet_vectors(vector<float> *Copy_Numbers, vector<char> *Copy_Symbols)
{
    stack<int> Brackets_Dedector;
    int Start_Index;

    for (int i = 0; i < (*Copy_Symbols).size(); i++)

    {

        if ((*Copy_Symbols)[i] == '(')

        {

            Brackets_Dedector.push(i);
        }
        else if ((*Copy_Symbols)[i] == ')')

        {
            Start_Index = Brackets_Dedector.top();
            Brackets_Dedector.pop();
            // Handle sub-expressions within parentheses recursively
            vector<float> SubNumbers((*Copy_Numbers).begin() + Start_Index, (*Copy_Numbers).begin() + i);
            vector<char> SubSymbols((*Copy_Symbols).begin() + Start_Index + 1, (*Copy_Symbols).begin() + i);

            calc_bet_vectors(&SubNumbers, &SubSymbols);

            // Replace sub-expression with its result
            (*Copy_Numbers)[Start_Index] = SubNumbers[0];
            (*Copy_Numbers).erase((*Copy_Numbers).begin() + Start_Index + 1, (*Copy_Numbers).begin() + i);
            (*Copy_Symbols).erase((*Copy_Symbols).begin() + Start_Index, (*Copy_Symbols).begin() + i + 1);

            i = Start_Index; // Adjust index for next iteration
        }
    }
    for (int i = 0; i < (*Copy_Symbols).size(); i++)

    {
        if ((*Copy_Symbols)[i] == '*' || (*Copy_Symbols)[i] == '/' || (*Copy_Symbols)[i] == '^')

        {
            (*Copy_Numbers)[i] = calc(((*Copy_Numbers)[i]), ((*Copy_Numbers)[i + 1]), (*Copy_Symbols)[i]);
            (*Copy_Numbers).erase((*Copy_Numbers).begin() + i + 1);
            (*Copy_Symbols).erase((*Copy_Symbols).begin() + i);
            i--;
        }
    }
    // Handle remaining operators in left-to-right order
    for (int i = 0; i < (*Copy_Symbols).size(); i++)

    {
        if ((*Copy_Symbols)[i] == '-' || (*Copy_Symbols)[i] == '+')

        {
            (*Copy_Numbers)[i] = calc(((*Copy_Numbers)[i]), ((*Copy_Numbers)[i + 1]), (*Copy_Symbols)[i]);
            (*Copy_Numbers).erase((*Copy_Numbers).begin() + i + 1);
            (*Copy_Symbols).erase((*Copy_Symbols).begin() + i);
            i--;
        }
        else
            break;
    }
}
