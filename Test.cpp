/**
 * An example of how to write unit tests.
 * Use this as a basis to build a more complete Test.cpp file.
 * 
 * IMPORTANT: Please write more tests - the tests here are only for example and are not complete.
 *
 * AUTHORS: Michael trushkin
 * 
 * Date: 2021-04
 */

#include "NumberWithUnits.hpp"
#include "doctest.h"
using namespace ariel;

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
using namespace std;

const int number_of_tests = 20;
const int number_of_inner_test = 5;
const int max_rand = 1000;
const int div_factor = 100;

double NextDouble() {
    return (double)((unsigned int)rand() % max_rand) / div_factor;
}

void CreateFakeUnits(string unitsStr) {
    ofstream MyFile("testDemo.txt");
    MyFile << unitsStr;
    MyFile.close();

    ifstream units("testDemo.txt");

    NumberWithUnits::read_units(units);
    units.close();
}

bool operator==(const NumberWithUnits &number, string expected) {
    stringstream stream;
    stream << number;
    return expected == stream.str();
}

bool operator==(string expected, const NumberWithUnits &number) {
    stringstream stream;
    stream << number;
    return expected == stream.str();
}

const string operator+(double val, const string &str) {
    string num = to_string(val);
    num.erase(num.find_last_not_of('0') + 1, std::string::npos);
    num.erase(num.find_last_not_of('.') + 1, std::string::npos);
    return num + str;
}

string combine_num(double val, string unit) {
    return val + unit;
}

TEST_CASE("Test multiplication ,assignment and equals") {
    srand(time(nullptr));

    CreateFakeUnits("1 im = 300 rl \n 1 rl = 50 ff \n 1 pi = 180 deg");

    for (size_t j = 0; j < number_of_tests; j++) {
        double start_val = NextDouble();
        string check_str = combine_num(start_val, "[im]");
        string check_str2 = combine_num(start_val, "[deg]");

        NumberWithUnits number = {start_val, "im"};
        NumberWithUnits number2 = {start_val, "deg"};
        NumberWithUnits number_copy = number;
        NumberWithUnits number_copy2 = {start_val, "im"};

        CHECK((number == check_str));
        for (size_t i = 0; i < number_of_inner_test; i++) {
            unsigned int rval = NextDouble();
            string check_str = combine_num(start_val * rval, "[im]");
            string check_str2 = combine_num(start_val * rval, "[deg]");
            CHECK((number * rval == check_str));
            CHECK((number2 * rval == check_str2));
            // cout << number * rval << " :: " << check_str << endl;
        }

        // cout << number << " :: " << check_str << endl;

        CHECK((number == check_str));
        CHECK((number2 == check_str2));
        CHECK((number != number2));
        CHECK((number == number_copy));
        CHECK((number == number_copy2));
    }

    // istringstream units("1 km = 1000 m \n 1 m = 100 cm \n 1 min = 60 sec");

    unsigned int rand_y = (unsigned int)rand();
    unsigned int rand_x = (unsigned int)rand();
}

TEST_CASE("Test unit arithmetics similar Units") {
    CreateFakeUnits("1 im = 300 rl \n 1 rl = 50 ff \n 1 pi = 180 deg");

    for (size_t i = 0; i < number_of_tests; i++) {
        double v1 = NextDouble() + 1;
        double v2 = NextDouble() + 1;

        NumberWithUnits n1{v1, "ff"};
        NumberWithUnits n2{v2, "ff"};

        string check_str = (v1 + v2) + string{"[ff]"};
        CHECK((n1 + n2 == check_str));
        CHECK((n1 + n2 > n1));
        CHECK((n1 < n1 + n1));
        CHECK((n1 + n2 == n2 + n1));
        //cout << n1 + n2 << " :: " << check_str << endl;
        for (size_t j = 0; j < number_of_inner_test; j++) {
            double m = NextDouble() + 1;
            string check_str = (v1 * m + v2) + string{"[ff]"};
            auto nm = n1 * m;

            CHECK(((nm + n2) == (m * n1 + n2)));
            CHECK(((nm + n2) == check_str));
            CHECK((nm > n1));
            CHECK((n1 < nm));
            CHECK((m * n1 == nm));
        }

        /* code */
    }
}

TEST_CASE("Test unit arithmetics different Units") {
    CreateFakeUnits("1 im =    10.5 rl \n 1 rl =    5 ff \n   1    pi = 180 deg");

    double convert_im_rl = 10.5;
    double convert_rl_ff = 5;

    for (size_t i = 0; i < number_of_tests; i++) {
        double v1 = NextDouble() + 1;
        double v2 = NextDouble() + 1;
        double v3 = NextDouble() + 1;

        NumberWithUnits n1{v1, "ff"};
        NumberWithUnits n2{v2, "rl"};
        NumberWithUnits n2{v2, "im"};

        NumberWithUnits n1_rl = { v1 }
    }
}