#include <cstring>
#include <cstdio>
#include <algorithm>

// build with:          g++ main.cpp
// execture with:       ./a.out "(1+2)-1+1"


/*
from the video: https://youtu.be/dDtZLm7HIJs
expr = term + expr | term
term = factor * term | factor
factor = (expr) | int

but if - and /, then we need to loop with one lower level
expr = while (term - term) | term

noticed a few people lifted his code, but just added minus and divide, which won't work
and they created github repos of this broken lib too...  :(

with the original code from the video, adding minus:
(1+2)-1+1 = 1
the minus causes the expresion to be 1+1, so it evals as:
1+2-(1+1)

The correct Haskell code is elsewhere online, with a loop like I describe above

Below is my C/C++ version of his Haskell code.

*/

// code that matches the video, and the code from many repos
namespace broken
{
        std::pair<float, const char*> expr(const char *s);

        std::pair<float, const char*> factor(const char *s)
        {
                float value{0};
                if (s[0] == '(') {
                        auto x = expr(s+1);
                        value = x.first;
                        s = x.second;
                } else {
                        value = s[0] - '0';
                        ++s;
                }
                return std::make_pair(value, s);
        }

        std::pair<float, const char*> term(const char *s)
        {
                auto x = factor(s);
                float value = x.first;
                s = x.second;
                switch (s[0]) {
                        case '*': {
                                auto x = term(s+1);
                                return std::make_pair(value * x.first, x.second);
                        }
                        case '/': {
                                auto x = term(s+1);
                                return std::make_pair(value / x.first, x.second);
                        }
                }
                return std::make_pair(value, s);
        }


        std::pair<float, const char*> expr(const char *s)
        {
                auto x = term(s);
                float value = x.first;
                s = x.second;
                switch (s[0]) {
                        case 0:
                                return std::make_pair(value, s);
                        case ')':
                                return std::make_pair(value, s+1);
                        case '+': {
                                auto x = expr(s+1);
                                return std::make_pair(value + x.first, x.second);
                        }
                        case '-': {
                                auto x = expr(s+1);
                                return std::make_pair(value - x.first, x.second);
                        }
                        default:
                                return std::make_pair(value, s);
                }
                return std::make_pair(value, s);
        }

        float parse(const char *s)
        {
                return expr(s).first;
        }

}


// code that corrects above, and also matches repos that have correctly added new functions
namespace correct
{
        std::pair<float, const char*> expr(const char *s);

        std::pair<float, const char*> factor(const char *s)
        {
                float value{0};
                if (s[0] == '(') {
                        auto x = expr(s+1);
                        value = x.first;
                        s = x.second;
                } else {
                        value = s[0] - '0';
                        ++s;
                }
                return std::make_pair(value, s);
        }

        std::pair<float, const char*> term(const char *s)
        {
                auto x = factor(s);
                float value = x.first;
                s = x.second;
                while (true) {
                        switch (s[0]) {
                                case '*': {
                                        auto x = factor(s+1);
                                        value *= x.first;
                                        s = x.second;
                                        break;
                                }
                                case '/': {
                                        auto x = factor(s+1);
                                        value /= x.first;
                                        s = x.second;
                                        break;
                                }
                                default:
                                        return std::make_pair(value, s);
                        }
                }
                return std::make_pair(value, s);
        }


        std::pair<float, const char*> expr(const char *s)
        {
                auto x = term(s);
                float value = x.first;
                s = x.second;
                while (true) {
                        switch (s[0]) {
                                case 0:
                                        return std::make_pair(value, s);
                                case ')':
                                        return std::make_pair(value, s+1);
                                case '+': {
                                        auto x = term(s+1);
                                        value += x.first;
                                        s = x.second;
                                        break;
                                }
                                case '-': {
                                        auto x = term(s+1);
                                        value -= x.first;
                                        s = x.second;
                                        break;
                                }
                                default:
                                        return std::make_pair(value, s);
                        }
                }
                return std::make_pair(value, s);
        }

        float parse(const char *s)
        {
                return expr(s).first;
        }
}


int main(int argc, char** argv)
{
        // (1+2)-1+1 should be tested

        printf("answer = %f\n", broken::parse(argv[1]));
        printf("answer = %f\n", correct::parse(argv[1]));
}