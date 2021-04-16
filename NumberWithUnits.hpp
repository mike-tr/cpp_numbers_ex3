#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ariel {
class NumberWithUnits {
private:
    std::string unit;
    double val;

public:
    static void read_units(std::ifstream &file);
    NumberWithUnits(double val, std::string unit);

    const NumberWithUnits operator*(double val) const;

    NumberWithUnits &operator+=(const NumberWithUnits &number);

    const bool
    operator<(const NumberWithUnits &number) const;
    const bool operator>(const NumberWithUnits &number) const;
    const bool operator==(const NumberWithUnits &number) const;
    const bool operator!=(const NumberWithUnits &number) const;
    const bool operator<=(const NumberWithUnits &number) const;
    const bool operator>=(const NumberWithUnits &number) const;
    const NumberWithUnits operator-(NumberWithUnits &number) const;
    const NumberWithUnits operator+(NumberWithUnits &number) const;

    friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &c);
    friend std::istream &operator>>(std::istream &input, NumberWithUnits &c);
    const friend NumberWithUnits operator*(double val, const NumberWithUnits &number);
    const friend NumberWithUnits operator-(const NumberWithUnits &number);
    const friend NumberWithUnits operator+(const NumberWithUnits &number);
};
}; // namespace ariel