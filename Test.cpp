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

const int number_of_tests = 10;
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
        // CHECK((number != number2));
        CHECK((number == number_copy));
        CHECK((number == number_copy2));
    }

    // istringstream units("1 km = 1000 m \n 1 m = 100 cm \n 1 min = 60 sec");

    unsigned int rand_y = (unsigned int)rand();
    unsigned int rand_x = (unsigned int)rand();
}

TEST_CASE("TEST pass string") {
    CreateFakeUnits("1 im = 300 rl \n 1 rl = 50 ff \n 1 pi = 180 deg");

    for (size_t i = 0; i < number_of_tests; i++) {
        double v1 = NextDouble() + 1;

        NumberWithUnits n1{v1, "ff"};

        string str_n1 = v1 + string{"        [         ff]"};

        NumberWithUnits temp{v1 - 1, "rl"};

        CHECK((temp != n1));

        stringstream sample_input;
        sample_input << "1231 [ ww ]";
        CHECK_THROWS((sample_input >> temp));
        sample_input = stringstream{};

        sample_input << "1231 [ ff ";
        //cout << sample_input.str() << endl;
        CHECK_THROWS((sample_input >> temp));
        sample_input = stringstream{};

        sample_input << "1231  ff ]";
        CHECK_THROWS((sample_input >> temp));
        sample_input = stringstream{};

        sample_input << "1231 [ ff ]";
        CHECK((sample_input >> temp));
        sample_input = stringstream{};

        sample_input << n1;
        //cout << " str : " << sample_input.str() << endl;
        sample_input >> temp;
        CHECK((n1 == temp));

        temp += temp;
        sample_input = stringstream{str_n1};
        sample_input >> temp;
        CHECK((n1 == temp));
    }
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
            double m = (int)NextDouble() * 5 + 2;
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
    CreateFakeUnits("1 im =    2 rl \n 1 rl =    5 ff \n");

    double convert_im_rl = 2;
    double convert_rl_ff = 5;

    for (size_t i = 0; i < number_of_tests; i++) {
        double vff = (int)NextDouble() * 10 + 1; // ff
        double vrl = (int)NextDouble() * 10 + 1; // rl
        double vim = (int)NextDouble() * 10 + 1; //

        NumberWithUnits nff{vff, "ff"};
        NumberWithUnits nrl{vrl, "rl"};
        NumberWithUnits nim{vim, "im"};

        double vff_rl = vff / convert_rl_ff;
        double vrl_ff = vrl * convert_rl_ff;
        double vim_ff = vim * convert_im_rl * convert_rl_ff;
        double vff_im = vff / (convert_im_rl * convert_rl_ff);

        NumberWithUnits nff_rl = {vff_rl, "rl"};
        NumberWithUnits nrl_ff = {vrl_ff, "ff"};
        NumberWithUnits nim_ff = {vim_ff, "ff"};
        NumberWithUnits nff_im = {vff_im, "im"};

        string check_ff_add_rl = vff + vrl_ff + string{"[ff]"};
        string check_rl_add_ff = vrl + vff_rl + string{"[rl]"};
        string check_im_add_ff = vim + vff_im + string{"[im]"};
        string check_ff_add_im = vff + vim_ff + string{"[ff]"};

        string check_ff_minus_rl = vff - vrl_ff + string{"[ff]"};
        string check_rl_minus_ff = vrl - vff_rl + string{"[rl]"};
        string check_im_minus_ff = vim - vff_im + string{"[im]"};
        string check_ff_minus_im = vff - vim_ff + string{"[ff]"};

        CHECK((check_ff_add_rl == nff + nrl));
        CHECK((check_rl_add_ff == nrl + nff));
        CHECK((check_im_add_ff == nim + nff));
        CHECK((check_ff_add_im == nff + nim));

        CHECK((check_ff_minus_rl == nff - nrl));
        CHECK((check_rl_minus_ff == nrl - nff));

        CHECK((check_im_minus_ff == (nim - nff)));
        CHECK((check_ff_minus_im == nff - nim));

        // // CHECK EQUALS AFTER CONVERSION
        CHECK((nff_rl == nff));
        CHECK((nff_im == nff));
        CHECK((nrl_ff == nrl));
        CHECK((nim_ff == nim));
        CHECK((nff_rl <= nff));
        CHECK((nff_im >= nff));
        CHECK((nrl_ff <= nrl));
        CHECK((nim_ff >= nim));

        CHECK((nff == +nff));
        CHECK((+nff == nff));
    }
}

TEST_CASE("Test ++, --, +=") {
    CreateFakeUnits("1 im =    10.5 rl \n 1 rl =    5 ff \n   1    pi = 180 deg");
    for (size_t i = 0; i < number_of_tests; i++) {
        double v1 = NextDouble();
        double v2 = NextDouble();
        double v3 = NextDouble();
        double v_add = NextDouble();
        NumberWithUnits n_pe{v1, "im"};
        NumberWithUnits n_add{v_add, "im"};
        NumberWithUnits n_pp{v2, "im"};
        NumberWithUnits n_mm{v3, "rl"};
        NumberWithUnits pp_n{v2, "im"};
        NumberWithUnits mm_n{v3, "rl"};

        for (size_t j = 0; j < number_of_inner_test; j++) {
            v1 += v_add;
            string check_str = v1 + string{"[im]"};

            n_pe += n_add;
            CHECK((n_pe == check_str));

            check_str = v2 + string{"[im]"};
            CHECK((pp_n++ == check_str));
            v2++;
            check_str = v2 + string{"[im]"};
            CHECK((++n_pp == check_str));

            check_str = v3 + string{"[rl]"};
            CHECK((n_mm-- == check_str));
            v3--;
            check_str = v3 + string{"[rl]"};
            CHECK((--mm_n == check_str));
        }
    }
}

TEST_CASE("Test diff units bolean ") {
    CreateFakeUnits("1 im =    10.5 rl \n 1 rl =    5 ff \n   1    pi = 180 deg");

    double convert_im_rl = 10.5;
    double convert_rl_ff = 5;

    for (size_t i = 0; i < number_of_tests; i++) {
        double v = NextDouble() + 1; // ff

        NumberWithUnits nff{v, "ff"};
        NumberWithUnits nrl{v, "rl"};
        NumberWithUnits nim{v, "im"};

        CHECK((nff < nrl));
        CHECK((nff <= nrl));
        CHECK((nff < nim));
        CHECK((nff <= nim));
        CHECK((nrl < nim));
        CHECK((nrl <= nim));

        CHECK((nrl > nff));
        CHECK((nrl >= nff));
        CHECK((nim > nff));
        CHECK((nim >= nff));
        CHECK((nim > nrl));
        CHECK((nim >= nrl));

        CHECK((nff != nrl));
        CHECK((nff != nrl));
        CHECK((nff != nim));
        CHECK((nff != nim));
        CHECK((nrl != nim));
        CHECK((nrl != nim));

        CHECK((nrl != nff));
        CHECK((nrl != nff));
        CHECK((nim != nff));
        CHECK((nim != nff));
        CHECK((nim != nrl));
        CHECK((nim != nrl));

        CHECK((nff + nff > nff));
        CHECK((nff + nrl > nrl));
        CHECK((nff + nim > nim));
        CHECK((nff + nrl == nrl + nff));
        CHECK((nff + nim == nim + nff));
        CHECK((nff + nim - nrl < nim));

        CHECK((nff - (0.5 * nff) < nff));
        CHECK((nff - (0.5 * nff) == (0.5 * nff)));
    }
}

TEST_CASE("CHECK THROWS") {
    CreateFakeUnits("1 im =    10.5 rl \n 1 rl =    5 PI \n   1    pi = 180 deg");
    for (size_t i = 0; i < number_of_tests; i++) {
        double v1 = NextDouble();
        double v2 = NextDouble();

        NumberWithUnits nim{v1, "im"};
        NumberWithUnits npi{v2, "pi"};
        NumberWithUnits nrl{v1, "rl"};
        NumberWithUnits ndeg{v2, "deg"};
        NumberWithUnits nPI{v1, "PI"};

        CHECK_THROWS((nim + npi));
        CHECK_THROWS((nim - ndeg));
        CHECK_THROWS((nrl - npi));
        CHECK_THROWS((nrl + ndeg));
        CHECK_THROWS((nPI - npi));
        CHECK_THROWS((nPI + ndeg));

        CHECK_THROWS((npi - nPI));
        CHECK_THROWS((ndeg + nPI));
        CHECK_THROWS((npi - nrl));
        CHECK_THROWS((ndeg + nrl));
        CHECK_THROWS((npi + nim));
        CHECK_THROWS((ndeg - nim));

        CHECK_THROWS((NumberWithUnits{v1, "Im"}));
        CHECK_THROWS((NumberWithUnits{v2, "xx"}));
        CHECK_THROWS((NumberWithUnits{v1 + v2, "iP"}));
    }
}