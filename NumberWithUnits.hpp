#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace ariel {
class NumberWithUnits {
private:
    std::string unit;
    double val;
    static std::map<std::pair<std::string, std::string>, double> exchange_rate;
    static std::vector<std::string> types;

public:
    static bool contains_type(const std::string &type);
    static void read_units(std::ifstream &file);

    NumberWithUnits(double val, const std::string &unit);

    NumberWithUnits convert_to_type(const std::string &unit) const;
    NumberWithUnits operator*(double val) const;

    NumberWithUnits &operator+=(const NumberWithUnits &number);
    NumberWithUnits &operator-=(const NumberWithUnits &number);

    bool operator<(const NumberWithUnits &number) const;
    bool operator>(const NumberWithUnits &number) const;
    bool operator==(const NumberWithUnits &number) const;
    bool operator!=(const NumberWithUnits &number) const;
    bool operator<=(const NumberWithUnits &number) const;
    bool operator>=(const NumberWithUnits &number) const;
    NumberWithUnits operator-(const NumberWithUnits &number) const;
    NumberWithUnits operator+(const NumberWithUnits &number) const;
    NumberWithUnits &operator=(const NumberWithUnits &number);

    NumberWithUnits &operator++();
    NumberWithUnits &operator--();
    NumberWithUnits operator++(int);
    NumberWithUnits operator--(int);

    friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &c);
    friend std::istream &operator>>(std::istream &input, NumberWithUnits &c);
    friend NumberWithUnits operator*(double val, const NumberWithUnits &number);
    friend NumberWithUnits operator-(const NumberWithUnits &number);
    friend NumberWithUnits operator+(const NumberWithUnits &number);
};
}; // namespace ariel