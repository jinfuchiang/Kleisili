#include <iostream>
#include <cmath>
#include <functional>

using namespace std;
template<class A> class optional {
    bool _isValid;
    A    _value;
public:
    optional()    : _isValid(false) {}
    explicit optional(A v) : _isValid(true), _value(v) {}
    bool isValid() const { return _isValid; }
    A value() const { return _value; }
    bool operator == (const optional<A> rhs) const {
        if(rhs.isValid() && isValid()) return rhs._value == rhs._value;
        if(!rhs.isValid() && !isValid()) return true;
        return false;
    }
};

template<class A>
ostream & operator << (ostream& out, optional<A> opt) {
    if (opt.isValid()) {
        out << opt.value();
    } else {
        out << "Nothing";
    }
    return out;
}

template<class A>
optional<A> identity(A x) {
    return optional<A>{x};
}

optional<double> safe_root(double x) {
    if (x >= 0) return optional<double>{sqrt(x)};
    else return optional<double>{};
}

optional<double> safe_reciprocal(double x) {
    if (x == 0) return optional<double>{};
    return optional<double>{1/x};
}

template<class A, class B, class C>
function<optional<C>(A)> compose(function<optional<B>(A)> m1,
                               function<optional<C>(B)> m2)
{
    return [m1, m2](A x) {
        auto p1 = m1(x);
        if(!p1.isValid()) return optional<C>();
        auto p2 = m2(p1.value());
        return p2;
    };
}

int main() {
    for(int i = -10; i <= 10; ++i) {
        cout << safe_root(i) << " ";
    }
    cout << endl;

    auto const compose = [](auto m1, auto m2) {
        return [m1, m2](auto x) {
            auto p1 = m1(x);
            if(!p1.isValid()) return optional<double>();
            auto p2 = m2(p1.value());
            return p2;
        };
    };

    auto process = compose(safe_reciprocal, safe_root);
    auto process2 = ::compose<double, double, double>(safe_reciprocal, safe_root);
    for(int i = -10; i <= 10; ++i) {
        cout << (process(i) == process2(i)) << " ";
    }
    cout << endl;
    return 0;
}
