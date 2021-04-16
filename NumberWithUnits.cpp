#include "NumberWithUnits.hpp"

using namespace std;

namespace ariel {
void NumberWithUnits::read_units(std::ifstream &file) {
}

NumberWithUnits::NumberWithUnits(double val, string unit) {
    this->val = val;
    this->unit = unit;
}

const NumberWithUnits NumberWithUnits::operator*(double val) const {
    return NumberWithUnits{this->val * val, this->unit};
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &number) {
    return *this;
}

const bool NumberWithUnits::operator<(const NumberWithUnits &number) const {
    return true;
}
const bool NumberWithUnits::operator>(const NumberWithUnits &number) const {
    return true;
}
const bool NumberWithUnits::operator==(const NumberWithUnits &number) const {
    return true;
}
const bool NumberWithUnits::operator!=(const NumberWithUnits &number) const {
    return true;
}
const bool NumberWithUnits::operator<=(const NumberWithUnits &number) const {
    return true;
}
const bool NumberWithUnits::operator>=(const NumberWithUnits &number) const {
    return true;
}
const NumberWithUnits NumberWithUnits::operator-(NumberWithUnits &number) const {
    return number;
}
const NumberWithUnits NumberWithUnits::operator+(NumberWithUnits &number) const {
    return number;
}

ostream &operator<<(ostream &os, const NumberWithUnits &c) {
    os << c.val << "[" << c.unit << "]";
    //os << "kek";
    return os;
}
istream &operator>>(istream &input, NumberWithUnits &c) {
    return input;
}
const NumberWithUnits operator*(double val, const NumberWithUnits &number) {
    return number;
}
const NumberWithUnits operator-(const NumberWithUnits &number) {
    return number;
}
const NumberWithUnits operator+(const NumberWithUnits &number) {
    return number;
}
} // namespace ariel