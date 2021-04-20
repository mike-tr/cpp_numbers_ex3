/**
 * Demo file for the exercise on numbers with units
 *
 * @author Erel Segal-Halevi
 * @since 2019-02
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

void test(ifstream mfile) {
}

int main() {
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);

    NumberWithUnits a{2, "km"};            // 2 kilometers
    cout << "prit a : " << a << endl;      // Prints "2[km]".
    cout << "print -a : " << (-a) << endl; // Prints "-2[km]"
    cout << "print a in meters : " << a.convert_to_type("m") << endl;
    cout << "print a in mm : " << a.convert_to_type("mm") << endl;
    cout << "prnt a conver to m and then back to km : " << a.convert_to_type("m").convert_to_type("km") << endl;
    cout << "print a * 3 : " << (3 * a) << endl; // Prints "6[km]"

    NumberWithUnits b{300, "m"}; // 300 meters
    cout << "print b : " << b << endl;
    cout << "print a + b : " << (a + b) << endl; // Prints "2.3[km]". Note: units are determined by first number (a).
    cout << "print b - a : " << (b - a) << endl; // Prints "-1700[m]". Note: units are determined by first number (b).

    cout << boolalpha;                                                         // print booleans as strings from now on:
    cout << "print a > b : " << (a > b) << endl;                               // Prints "true"
    cout << "print a <= b : " << (a <= b) << endl;                             // Prints "false"
    cout << "print a == 2000[m]" << (a == NumberWithUnits{2000, "m"}) << endl; // Prints "true"

    istringstream sample_input{"700 [ kg ]"};
    sample_input >> a;
    cout << "print a after loading 700 [ km ] : " << a << endl;                 // Prints "700[kg]"
    cout << "print a += 1[ton] : " << (a += NumberWithUnits{1, "ton"}) << endl; // prints "1700[kg]"
    cout << "print a : " << a << endl;                                          // Prints "1700[kg]". Note that a has changed.

    cout << "try : " << a << " + " << b << endl
         << " output : ";
    try {
        cout << (a + b) << endl;
    } catch (const std::exception &ex) {
        cout << ex.what() << endl; // Prints "Units do not match - [m] cannot be converted to [kg]"
    }
    cout << "End of demo!" << endl;
    return 0;
}
