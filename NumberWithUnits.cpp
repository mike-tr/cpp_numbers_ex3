#include "NumberWithUnits.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

double epsilone = 0.00001;

bool definetly_equals(double a, double b) {
    // cout << a << " : " << b << endl;
    // cout << fabs(a - b) << endl;
    return fabs(a - b) <= epsilone;
}

bool definetly_greater(double a, double b) {
    return (a - b) > epsilone;
}

bool definetly_smaller(double a, double b) {
    return fabs(b - a) > epsilone;
}

namespace ariel {
bool add_line(ifstream &file, map<pair<string, string>, double> &exchange_rate, vector<string> &types);
//map<pair<string, string>, double> NumberWithUnits::exchange_rate;
std::map<std::pair<std::string, std::string>, double> NumberWithUnits::exchange_rate;
std::vector<std::string> NumberWithUnits::types;
void NumberWithUnits::read_units(std::ifstream &file) {
    exchange_rate.clear();
    while (!file.eof() && add_line(file, exchange_rate, types)) {
    }
}

/**
 * this function is responsible for adding all the logic from a  new unit line.
 * and integrading all the background stuff necessary to make it work with all the other units.
 * */
bool add_line(ifstream &file, map<pair<string, string>, double> &exchange_rate, vector<string> &types) {
    double v1;
    double exchange_rate_val;
    string unit_from, unit_to;
    file >> v1;
    if (v1 != 1) {
        // end of file
        return false;
    }
    file >> unit_from;
    file >> unit_to;
    if (unit_to != "=") {
        //cout << v1 << unit_from << " : " << unit_to << endl;
        //throw invalid_argument{"Wrong format, format should be \"1 something = x something_else\""};
        return false;
    }
    file >> exchange_rate_val;
    file >> unit_to;

    if (exchange_rate.count({unit_from, unit_to})) {
        throw invalid_argument{"Cannot have duplicate exchange rates!"};
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
        } else if (types[i] == unit_to) {
            contains_u2 = true;
            continue;
        }

        if (exchange_rate.count({types[i], unit_from}) && !exchange_rate.count({types[i], unit_to})) {
            double rate = exchange_rate[{types[i], unit_from}] * exchange_rate_val;

            exchange_rate[{types[i], unit_to}] = rate;
            exchange_rate[{unit_to, types[i]}] = 1 / rate;

            connect_from.push_back(types[i]);
            // cout << "added exchange from :" << types[i] << " to " << unit_to << " rate :" << rate << endl;
        }

        if (exchange_rate.count({unit_to, types[i]}) && !exchange_rate.count({unit_from, types[i]})) {
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
            double rate = exchange_rate[{connect_from[i], unit_from}] * exchange_rate[{unit_from, connect_to[i]}];
            exchange_rate[{connect_from[i], connect_to[i]}] = rate;
            exchange_rate[{connect_to[i], connect_from[i]}] = 1 / rate;

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

bool NumberWithUnits::contains_type(string type) {
    for (size_t i = 0; i < types.size(); i++) {
        if (type == types[i]) {
            return true;
        }
    }
    return false;
}

NumberWithUnits::NumberWithUnits(double val, string unit) {
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
const NumberWithUnits NumberWithUnits::convert_to_type(string unit) const {
    if (unit == this->unit) {
        return *this;
    }

    if (!exchange_rate[{this->unit, unit}]) {
        throw invalid_argument{"cannot convert from " + this->unit + " to " + unit};
    }
    double ex_rate = exchange_rate[{this->unit, unit}];
    return NumberWithUnits{ex_rate * this->val, unit};
}

const NumberWithUnits NumberWithUnits::operator*(double val) const {
    return NumberWithUnits{this->val * val, this->unit};
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &number) {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    this->val += nconverted.val;
    return *this;
}

const bool NumberWithUnits::operator<(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_smaller(this->val, nconverted.val);
}
const bool NumberWithUnits::operator>(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_greater(this->val, nconverted.val);
}
const bool NumberWithUnits::operator==(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return definetly_equals(this->val, nconverted.val);
}
const bool NumberWithUnits::operator!=(const NumberWithUnits &number) const {
    return !(*this == number);
}
const bool NumberWithUnits::operator<=(const NumberWithUnits &number) const {
    return (*this < number) || (*this == number);
}
const bool NumberWithUnits::operator>=(const NumberWithUnits &number) const {
    return (*this > number) || (*this == number);
}
const NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &number) const {
    const NumberWithUnits &nconverted = number.convert_to_type(this->unit);
    return NumberWithUnits{this->val - nconverted.val, this->unit};
}
const NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &number) const {
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
const NumberWithUnits NumberWithUnits::operator++(int) {
    NumberWithUnits temp = *this;
    this->val++;
    return temp;
}
const NumberWithUnits NumberWithUnits::operator--(int) {
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
    //os << "kek";
    return os;
}
istream &operator>>(istream &input, NumberWithUnits &c) {
    double val;
    string unit;

    input >> val;
    if (input.peek() == ' ') {
        input.ignore(256, ' ');
    }
    input.ignore(256, '[');
    input >> unit;

    int l = unit.size();
    unit.erase(unit.find_last_not_of(']') + 1, std::string::npos);
    if (l == unit.size()) {
        throw invalid_argument{"wrong format should be of format val [ unit ]"};
    }

    if (!NumberWithUnits::contains_type(unit)) {
        throw invalid_argument{"unit " + unit + " doesnt exist!"};
    }

    c.val = val;
    c.unit = unit;

    return input;
}
const NumberWithUnits operator*(double val, const NumberWithUnits &number) {
    return NumberWithUnits{number.val * val, number.unit};
}
const NumberWithUnits operator-(const NumberWithUnits &number) {
    return NumberWithUnits{-number.val, number.unit};
}
const NumberWithUnits operator+(const NumberWithUnits &number) {
    return number;
}
} // namespace ariel