#include <iostream>

template<int N>
class Error {
public:
    explicit Error(const std::string& s) : message{ s } { }

    std::string get_message() const { return message; }
    void put(std::ostream& os) const { os << message; }

private:
    std::string message;
};

using LexicalError = Error<1>;
using SyntaxError = Error<2>;
using RuntimeError = Error<3>;

template<int N>
std::ostream& operator<<(std::ostream& os, const Error<N>& x)
{
    x.put(os);
    return os;
}