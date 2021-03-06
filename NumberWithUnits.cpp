#include "NumberWithUnits.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

const double epsilone = 0.00001;

// doubles comparisons is tricky, becuase they are not really "equal",
// then we would say they are, so we need to check how "close" they are to be equal.
bool definetly_equals(double a, double b) {
    return fabs(a - b) <= epsilone;
}

bool definetly_greater(double a, double b) {
    //cout << " a < b" a << " " << b << endl;
    return (a - b) > epsilone;
}

bool definetly_smaller(double a, double b) {
    return definetly_greater(b, a);
}

namespace ariel {
bool add_line(ifstream &file, map<pair<string, string>, double> &exchange_rate, vector<string> &types);
//map<pair<string, string>, double> NumberWithUnits::exchange_rate;
std::map<std::pair<std::string, std::string>, double> NumberWithUnits::exchange_rate;
std::vector<std::string> NumberWithUnits::types;
void NumberWithUnits::read_units(std::ifstream &file) {
    // exchange_rate.clear();
    // types.clear();
    while (!file.eof() && add_line(file, exchange_rate, types)) {
    }
}

/**
 * this function is responsible for adding all the logic from a  new unit line.
 * and integrading all the background stuff necessary to make it work with all the other units.
 * */
bool add_line(ifstream &file, map<pair<string, string>, double> &exchange_rate, vector<string> &types) {
    double v1 = 0;
    double exchange_rate_val = 0;
    string unit_from;
    string unit_to;
    file >> v1;
    if (v1 != 1) {
        // if the format fails, i will consider it as endof file.
        // end of file
        return false;
    }
    file >> unit_from;
    file >> unit_to;
    if (unit_to != "=") {
        //cout << v1 << unit_from << " : " << unit_to << endl;
        //throw invalid_argument{"Wrong format, format should be \"1 something = x something_else\""};

        // if the format fails, i will consider it as endof file.
        return false;
    }
    file >> exchange_rate_val;
    if (exchange_rate_val == 0) {
        throw invalid_argument{"exchange rate cannot be 0"};
    }
    file >> unit_to;

    if (exchange_rate.count({unit_from, unit_to}) > 0) {
        if (exchange_rate_val != exchange_rate[{unit_from, unit_to}]) {
            throw invalid_argument{"Cannot have duplicate exchange rates!"};
        }
        return true;
    }

    if (unit_to == unit_from) {
        throw invalid_argument{"exchange cannot be made to the same unit type!"};
    }

    exchange_rate[{unit_from, unit_to}] = exchange_rate_val;
    exchange_rate[{unit_to, unit_from}] = 1 / exchange_rate_val;

    // END OF READ NEW LINE.
    //////////////////////////////////
    // START INEGRADING WITH PREV LINES

    bool contains_u1 = false;
    bool contains_u2 = false;

    bool has_changed = true;

    // cout << v1 << unit_from << " = " << exchange_rate_val << unit_to << endl;

    // CHECK and link prev units with the new given
    // for example if 1 km = 1000 m
    // and we just got :  1 m = 100 cm
    // we need to calculate that 1 km = 100000 cm;
    vector<string> connect_from;
    vector<string> connect_to;
    for (size_t i = 0; i < types.size(); i++) {
        if (types[i] == unit_from) {
            contains_u1 = true;
            continue;
        }
        if (types[i] == unit_to) {
            contains_u2 = true;
            continue;
        }

        // if there is a conversion from other type to this unit form
        // and no conversion from that type to the converted type,
        // add a conversion from that type to the new converted type
        if (exchange_rate.count({types[i], unit_from}) > 0 && exchange_rate.count({types[i], unit_to}) == 0) {
            double rate = exchange_rate[{types[i], unit_from}] * exchange_rate_val;

            exchange_rate[{types[i], unit_to}] = rate;
            exchange_rate[{unit_to, types[i]}] = 1 / rate;

            connect_from.push_back(types[i]);
            // cout << "added exchange from :" << types[i] << " to " << unit_to << " rate :" << rate << endl;
        }

        // if there is a conversion from the converted type to some type
        // and no conversion from the converting unit to that same type
        // add a conversion from that type to the new converted type
        if (exchange_rate.count({unit_to, types[i]}) > 0 && exchange_rate.count({unit_from, types[i]}) == 0) {
            double rate = exchange_rate[{unit_to, types[i]}] * exchange_rate_val;

            exchange_rate[{unit_from, types[i]}] = rate;
            exchange_rate[{types[i], unit_from}] = 1 / rate;

            connect_to.push_back(types[i]);

            // cout << "added exchange from :" << unit_from << " to " << types[i] << " rate :" << rate << endl;
        }
    }

    // Integrade with PREV lines part 2.
    // EXAMPLE
    // we know
    // 1 km = 1000 m
    // 1 cm  = 10 mm
    // we get : 1 m = 100 cm
    // prev result will calculate:
    // 1 km = 100000 cm and 1 m = 1000 mm.
    // but this step will calculate that
    // 1 km = 1000000 mm

    for (size_t i = 0; i < connect_from.size(); i++) {
        for (size_t j = 0; j < connect_to.size(); j++) {
            // cout << connect_from[i] << " << " << connect_to[j] << endl;
            double rate = exchange_rate[{connect_from[i], unit_from}] * exchange_rate[{unit_from, connect_to[j]}];
            exchange_rate[{connect_from[i], connect_to[j]}] = rate;
            exchange_rate[{connect_to[j], connect_from[i]}] = 1 / rate;

            // cout << "added exchange from :" << connect_from[i] << " to " << connect_to[i] << " rate :" << rate << endl;
        }
    }

    // add to type list.
    if (!contains_u1) {
        types.push_back(unit_from);
    }
    // add to type list

    if (!contains_u2) {
        types.push_back(unit_to);
    }
    return true;
}

bool NumberWithUnits::contains_type(const string &type) {
    for (size_t i = 0; i < types.size(); i++) {
        if (type == types[i]) {
            return true;
        }
    }
    return false;
}

NumberWithUnits::NumberWithUnits(double val, const string &unit) {
    this->val = val;
    this->unit = unit;

    if (!contains_type(unit)) {
        throw invalid_argument{"unit " + unit + " doesnt exist!"};
    }
}

/**
 * convert given number to a different number unit
 * Throws error if convertion is imposible.
 * */
NumberWithUnits NumberWithUnits::convert_to_type(const string &unit) const {
    if (unit == this->unit) {
        return *this;
    }

    auto key = make_pair(this->unit, unit);
    if (exchange_rate.count(key) == 0) {
        throw invalid_argument{"cannot convert from " + this->unit + " to " + unit};
    }
    double ex_rate = exchange_rate[key];
    return NumberWithUnits{ex_rate * this->val, unit};
}

NumberWithUnits NumberWithUnits::operator*(double val) const {
    return NumberWithUnits{this->val * val, this->unit};
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &number) {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    this->val += nconverted.val;
    return *this;
}

NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &number) {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    this->val -= nconverted.val;
    return *this;
}

bool NumberWithUnits::operator<(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_smaller(this->val, nconverted.val);
}

bool NumberWithUnits::operator>(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_greater(this->val, nconverted.val);
}

bool NumberWithUnits::operator==(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_equals(this->val, nconverted.val);
}

bool NumberWithUnits::operator!=(const NumberWithUnits &number) const {
    return !(*this == number);
}

bool NumberWithUnits::operator<=(const NumberWithUnits &number) const {
    return (*this < number) || (*this == number);
}

bool NumberWithUnits::operator>=(const NumberWithUnits &number) const {
    return (*this > number) || (*this == number);
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return NumberWithUnits{this->val - nconverted.val, this->unit};
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return NumberWithUnits{this->val + nconverted.val, this->unit};
}

NumberWithUnits &NumberWithUnits::operator++() {
    this->val++;
    return *this;
}
NumberWithUnits &NumberWithUnits::operator--() {
    this->val--;
    return *this;
}
NumberWithUnits NumberWithUnits::operator++(int) {
    NumberWithUnits temp = *this;
    this->val++;
    return temp;
}
NumberWithUnits NumberWithUnits::operator--(int) {
    NumberWithUnits temp = *this;
    this->val--;
    return temp;
}

NumberWithUnits &NumberWithUnits::operator=(const NumberWithUnits &number) {
    if (&number == this) {
        return *this;
    }
    this->unit = number.unit;
    this->val = number.val;
    return *this;
}

ostream &operator<<(ostream &os, const NumberWithUnits &c) {
    os << c.val << "[" << c.unit << "]";
    return os;
}

istream &operator>>(istream &input, NumberWithUnits &c) {
    double val = 0;
    string unit;

    input >> val;
    input >> unit;

    // make sure the format is:
    // number [ unit ]
    // with or without spaces.

    // check starting bracket
    if (unit == "[") {
        input >> unit;
    } else if (unit[0] == '[') {
        unit = unit.erase(0, 1);
    } else {
        throw invalid_argument{"wrong format should be of format val [ unit ]"};
    }

    // check ending bracket/
    if (unit[unit.size() - 1] == ']') {
        unit = unit.erase(unit.size() - 1, 1);
    } else {
        string next;
        input >> next;
        if (next[0] != ']') {
            throw invalid_argument{"wrong format should be of format val [ unit ]"};
        }
    }

    // check if unit is viable.
    if (!NumberWithUnits::contains_type(unit)) {
        throw invalid_argument{"unit " + unit + " doesnt exist!"};
    }

    c.val = val;
    c.unit = unit;

    return input;
}
NumberWithUnits operator*(double val, const NumberWithUnits &number) {
    return NumberWithUnits{number.val * val, number.unit};
}
NumberWithUnits operator-(const NumberWithUnits &number) {
    return NumberWithUnits{-number.val, number.unit};
}
NumberWithUnits operator+(const NumberWithUnits &number) {
    return number;
}
} // namespace ariel