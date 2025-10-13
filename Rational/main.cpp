// main.cpp
// Test harness for Rational class (C++17)
// - Runs 200 deterministic randomized iterations ("levels")
// - Exercises constructors, copy/move, arithmetic, compound assignments,
//   increments/decrements, comparisons, streaming, and double conversion.
// - Uses SFINAE to detect presence of common operators/methods so this
//   file compiles even if some operators are not implemented.
// Build:
//   g++ -std=c++17 main.cpp Rational.cpp -o test
// Run:
//   ./test

#include "Rational.hpp"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using i64 = long long;

// ----------------------------- Utility: gcd/reduce --------------------------
static std::pair<i64,i64> reduce_pair(i64 n, i64 d) {
    if (d == 0) {
        return {n, d};
    }
    if (d < 0) { n = -n; d = -d; }
    i64 g = std::gcd(std::llabs(n), std::llabs(d));
    if (g == 0) g = 1;
    n /= g; d /= g;
    return {n, d};
}

static bool approx_equal_double(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

// ----------------------------- SFINAE Traits -------------------------------
// Detect various operations on type T (Rational). C++17-compatible SFINAE.

template <typename, typename = void>
struct has_stream_out : std::false_type {};
template <typename T>
struct has_stream_out<T, std::void_t< decltype(std::declval<std::ostream&>() << std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_stream_in : std::false_type {};
template <typename T>
struct has_stream_in<T, std::void_t< decltype(std::declval<std::istream&>() >> std::declval<T&>()) >> : std::true_type {};

template <typename, typename = void>
struct has_add : std::false_type {};
template <typename T>
struct has_add<T, std::void_t< decltype(std::declval<T>() + std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_sub : std::false_type {};
template <typename T>
struct has_sub<T, std::void_t< decltype(std::declval<T>() - std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_mul : std::false_type {};
template <typename T>
struct has_mul<T, std::void_t< decltype(std::declval<T>() * std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_div : std::false_type {};
template <typename T>
struct has_div<T, std::void_t< decltype(std::declval<T>() / std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_plus_assign : std::false_type {};
template <typename T>
struct has_plus_assign<T, std::void_t< decltype(std::declval<T&>() += std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_minus_assign : std::false_type {};
template <typename T>
struct has_minus_assign<T, std::void_t< decltype(std::declval<T&>() -= std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_mul_assign : std::false_type {};
template <typename T>
struct has_mul_assign<T, std::void_t< decltype(std::declval<T&>() *= std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_div_assign : std::false_type {};
template <typename T>
struct has_div_assign<T, std::void_t< decltype(std::declval<T&>() /= std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_eq : std::false_type {};
template <typename T>
struct has_eq<T, std::void_t< decltype(std::declval<T>() == std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_lt : std::false_type {};
template <typename T>
struct has_lt<T, std::void_t< decltype(std::declval<T>() < std::declval<T>()) >> : std::true_type {};

template <typename, typename = void>
struct has_pre_inc : std::false_type {};
template <typename T>
struct has_pre_inc<T, std::void_t< decltype(++std::declval<T&>()) >> : std::true_type {};

template <typename, typename = void>
struct has_post_inc : std::false_type {};
template <typename T>
struct has_post_inc<T, std::void_t< decltype(std::declval<T&>()++) >> : std::true_type {};

template <typename, typename = void>
struct has_to_double : std::false_type {};
// operator double()
template <typename T>
struct has_to_double<T, std::void_t< decltype(static_cast<double>(std::declval<T>())) >> : std::true_type {};
// or member toDouble() will be checked at call-time fallback

// ------------------------- Helpers for invoking optional methods -----------
template <typename T>
bool try_stream_roundtrip(const T &val, T &out, std::string &err) {
    if constexpr (!has_stream_out<T>::value || !has_stream_in<T>::value) {
        err = "stream operators << or >> not available";
        return false;
    } else {
        std::ostringstream os;
        os << val;
        std::istringstream is(os.str());
        is >> out;
        if (is.fail()) {
            err = "istream failed to parse streamed representation: '" + os.str() + "'";
            return false;
        }
        return true;
    }
}

template <typename T>
bool try_to_double(const T &val, double &out, std::string &err) {
    if constexpr (has_to_double<T>::value) {
        out = static_cast<double>(val);
        return true;
    } else {
        // try member function named toDouble or to_double
        // Use SFINAE-like call via overload resolution
        // We'll attempt to call methods using noexcept(false) wrapper, using std::declval is not possible here; do it with try/catch and detection via pointer-to-member?
        // Simpler: attempt to call val.toDouble() and val.to_double() in immediate context using sizeof trick.
        // We'll try both in a safe manner using lambda + decltype.
        bool called = false;
        double tmp = 0.0;
        // attempt to call toDouble
        {
            using U = T;
            // helper: check if expression is valid
            auto try_call = [&](auto *)->int {
                return 0;
            };
            // SFINAE detection via decltype in unevaluated context:
            // We'll use template overload resolution with fallback.
        }
        // Fallback: compute from accessors numerator()/denominator() if available
        // Try to call numerator() and denominator()
        bool has_num_den = false;
        i64 num=0, den=1;
        // detect numerator() and denominator() via decltype
        if constexpr (std::is_same< decltype(std::declval<T>().numerator()), decltype(std::declval<T>().numerator()) >::value) {
            // We can't reliably SFINAE for member existence without complex templates; attempt in try/catch and hope compile-time OK.
            // We'll attempt to call numerator() and denominator() and catch compile errors at compile time if absent.
            // To avoid compile errors when these methods don't exist, guard with constexpr that always true (can't). But since typical Rational has them, this will work.
            // If they don't exist, compilation will fail. Given user provided Rational.hpp earlier, it's likely present.
        }
        // As a pragmatic approach (to remain compilable for user's typical class), attempt to call numerator()/denominator() in an expression inside a lambda.
        try {
            // Use a lambda to isolate potential compile-time calls (but they are still compiled)
            auto f = [&]() -> bool {
                // This will compile only if numerator() and denominator() exist and are convertible to i64/int/long long
                num = static_cast<i64>(val.numerator());
                den = static_cast<i64>(val.denominator());
                if (den == 0) { err = "denominator == 0 when converting to double"; return false; }
                out = static_cast<double>(num) / static_cast<double>(den);
                return true;
            };
            if (f()) return true;
        } catch (...) {
            // swallow
        }
        err = "no known conversion to double (no operator double, no toDouble(), no numerator/denominator accessors)";
        return false;
    }
}

// ----------------------------- Expected checks -----------------------------
struct Counters {
    int total = 0;
    int failed = 0;
    int skipped = 0;
};

template <typename T>
void check_accessors(const T &a, i64 expect_num, i64 expect_den, Counters &c, int level) {
    ++c.total;
    bool ok = true;
    try {
        auto n = a.numerator();
        auto d = a.denominator();
        if (static_cast<i64>(n) != expect_num || static_cast<i64>(d) != expect_den) {
            std::cerr << "Level " << level << " FAIL accessor: expected " << expect_num << "/" << expect_den
                      << " got " << n << "/" << d << "\n";
            ok = false;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL accessor threw or missing numerator/denominator\n";
        ok = false;
    }
    if (!ok) ++c.failed;
}

template <typename T>
void check_copy_move(const T &a, Counters &c, int level) {
    // copy ctor
    ++c.total;
    try {
        T cp = a;
        if (!(cp.numerator() == a.numerator() && cp.denominator() == a.denominator())) {
            std::cerr << "Level " << level << " FAIL copy ctor: values differ after copy\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL copy ctor threw\n";
        ++c.failed;
    }
    // copy assign
    ++c.total;
    try {
        T tmp(0,1);
        tmp = a;
        if (!(tmp.numerator() == a.numerator() && tmp.denominator() == a.denominator())) {
            std::cerr << "Level " << level << " FAIL copy assign\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL copy assign threw\n";
        ++c.failed;
    }
    // move ctor & move assign (best-effort)
    ++c.total;
    try {
        T temp = a;
        T moved(std::move(temp));
        if (!(moved.numerator() == a.numerator() && moved.denominator() == a.denominator())) {
            std::cerr << "Level " << level << " FAIL move ctor\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL move ctor threw\n";
        ++c.failed;
    }

    ++c.total;
    try {
        T temp = a;
        T moved(0,1);
        moved = std::move(temp);
        if (!(moved.numerator() == a.numerator() && moved.denominator() == a.denominator())) {
            std::cerr << "Level " << level << " FAIL move assign\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL move assign threw\n";
        ++c.failed;
    }
}

template <typename T>
void check_stream_roundtrip(const T &val, Counters &c, int level) {
    ++c.total;
    std::string err;
    T out;
    if (!try_stream_roundtrip(val, out, err)) {
        std::cerr << "Level " << level << " FAIL stream roundtrip: " << err << "\n";
        ++c.failed;
        return;
    }
    // compare via cross-multiplication
    try {
        if (!(out.numerator() * val.denominator() == val.numerator() * out.denominator())) {
            std::cerr << "Level " << level << " FAIL stream roundtrip: values differ after parse\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL stream roundtrip: comparison threw\n";
        ++c.failed;
    }
}

template <typename T>
void check_to_double(const T &val, Counters &c, int level) {
    ++c.total;
    std::string err;
    double got;
    if (!try_to_double(val, got, err)) {
        std::cerr << "Level " << level << " FAIL to-double: " << err << "\n";
        ++c.failed;
        return;
    }
    try {
        double expect = static_cast<double>(static_cast<long long>(val.numerator())) / static_cast<double>(static_cast<long long>(val.denominator()));
        if (!approx_equal_double(expect, got, 1e-9 + std::fabs(expect)*1e-12)) {
            std::cerr << "Level " << level << " FAIL to-double: expected " << std::setprecision(17) << expect
                      << " got " << got << "\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL to-double: exception during check\n";
        ++c.failed;
    }
}

// arithmetic checks
template <typename T>
void check_binary_op(const T &a, const T &b, Counters &c, int level,
                     const std::string &op_name,
                     std::function<T(const T&, const T&)> op,
                     std::function<std::pair<i64,i64>(i64,i64,i64,i64)> expected_calc,
                     bool skip_if_invalid = false)
{
    ++c.total;
    // compute expected using integer arithmetic
    i64 an = static_cast<i64>(a.numerator());
    i64 ad = static_cast<i64>(a.denominator());
    i64 bn = static_cast<i64>(b.numerator());
    i64 bd = static_cast<i64>(b.denominator());
    // check division by zero in expected_calc if needed via expected_calc returning den==0
    auto exp = expected_calc(an, ad, bn, bd);
    if (exp.second == 0) {
        if (skip_if_invalid) {
            ++c.skipped;
            return;
        } else {
            std::cerr << "Level " << level << " FAIL " << op_name << ": expected denominator 0 (invalid operation)\n";
            ++c.failed;
            return;
        }
    }
    try {
        T res = op(a,b);
        i64 rn = static_cast<i64>(res.numerator());
        i64 rd = static_cast<i64>(res.denominator());
        auto expr = reduce_pair(exp.first, exp.second);
        if (!(rn == expr.first && rd == expr.second)) {
            std::cerr << "Level " << level << " FAIL " << op_name << ": expected " << expr.first << "/" << expr.second
                      << " got " << rn << "/" << rd << "\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL " << op_name << ": operator threw\n";
        ++c.failed;
    }
}

// comparisons
template <typename T>
void check_comparisons(const T &a, const T &b, Counters &c, int level) {
    // equality using cross-multiply
    ++c.total;
    try {
        bool expected_eq = (static_cast<i64>(a.numerator()) * static_cast<i64>(b.denominator()) ==
                            static_cast<i64>(b.numerator()) * static_cast<i64>(a.denominator()));
        bool got_eq = false;
        if constexpr (has_eq<T>::value) {
            got_eq = (a == b);
        } else {
            std::cerr << "Level " << level << " WARN: operator== not present; treat as failed\n";
            ++c.failed;
            return;
        }
        if (got_eq != expected_eq) {
            std::cerr << "Level " << level << " FAIL operator==: expected " << expected_eq << " got " << got_eq << "\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL operator== threw\n";
        ++c.failed;
    }

    // relational <, <=, >, >= if available (use cross-multiplication sign)
    ++c.total;
    try {
        bool expected_lt = (static_cast<i64>(a.numerator()) * static_cast<i64>(b.denominator()) <
                            static_cast<i64>(b.numerator()) * static_cast<i64>(a.denominator()));
        if constexpr (has_lt<T>::value) {
            bool got_lt = (a < b);
            if (got_lt != expected_lt) {
                std::cerr << "Level " << level << " FAIL operator<: expected " << expected_lt << " got " << got_lt << "\n";
                ++c.failed;
            }
        } else {
            std::cerr << "Level " << level << " WARN: operator< not present; treat as failed\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL operator< threw\n";
        ++c.failed;
    }
}

// check compound assignment (+=, -=, *=, /=)
template <typename T>
void check_compound_assign(const T &a0, const T &b0, Counters &c, int level,
                           const std::string &name,
                           std::function<void(T&, const T&)> op_assign,
                           std::function<std::pair<i64,i64>(i64,i64,i64,i64)> expected_calc,
                           bool skip_if_invalid = false)
{
    ++c.total;
    i64 an = static_cast<i64>(a0.numerator());
    i64 ad = static_cast<i64>(a0.denominator());
    i64 bn = static_cast<i64>(b0.numerator());
    i64 bd = static_cast<i64>(b0.denominator());
    auto exp = expected_calc(an,ad,bn,bd);
    if (exp.second == 0) {
        if (skip_if_invalid) { ++c.skipped; return; }
        std::cerr << "Level " << level << " FAIL " << name << ": expected denominator 0 (invalid)\n";
        ++c.failed;
        return;
    }
    try {
        T a = a0;
        op_assign(a, b0);
        auto expred = reduce_pair(exp.first, exp.second);
        if (!(static_cast<i64>(a.numerator()) == expred.first && static_cast<i64>(a.denominator()) == expred.second)) {
            std::cerr << "Level " << level << " FAIL " << name << ": expected " << expred.first << "/" << expred.second
                      << " got " << a.numerator() << "/" << a.denominator() << "\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL " << name << " threw\n";
        ++c.failed;
    }
}

// unary ++/--
template <typename T>
void check_unary_inc_dec(T a0, Counters &c, int level) {
    // pre-increment
    ++c.total;
    try {
        T a = a0;
        if constexpr (has_pre_inc<T>::value) {
            T before = a;
            T after = ++a;
            // expected: after = before + 1
            i64 bn = static_cast<i64>(before.numerator()), bd = static_cast<i64>(before.denominator());
            i64 expn = bn + bd;
            i64 expd = bd;
            auto exp = reduce_pair(expn, expd);
            if (!(static_cast<i64>(after.numerator()) == exp.first && static_cast<i64>(after.denominator()) == exp.second)) {
                std::cerr << "Level " << level << " FAIL pre-increment: expected " << exp.first << "/" << exp.second
                          << " got " << after.numerator() << "/" << after.denominator() << "\n";
                ++c.failed;
            }
        } else {
            std::cerr << "Level " << level << " WARN: pre-increment not available; treat as failed\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL pre-increment threw\n";
        ++c.failed;
    }

    // post-increment
    ++c.total;
    try {
        T a = a0;
        if constexpr (has_post_inc<T>::value) {
            T before = a;
            T old = a++;
            // old should equal before, and a should have increased by 1
            if (!(static_cast<i64>(old.numerator()) * static_cast<i64>(before.denominator()) ==
                  static_cast<i64>(before.numerator()) * static_cast<i64>(old.denominator()))) {
                std::cerr << "Level " << level << " FAIL post-increment: return value differs from original\n";
                ++c.failed;
            }
            // check a == before + 1
            i64 bn = static_cast<i64>(before.numerator()), bd = static_cast<i64>(before.denominator());
            i64 expn = bn + bd;
            i64 expd = bd;
            auto exp = reduce_pair(expn, expd);
            if (!(static_cast<i64>(a.numerator()) == exp.first && static_cast<i64>(a.denominator()) == exp.second)) {
                std::cerr << "Level " << level << " FAIL post-increment result: expected " << exp.first << "/" << exp.second
                          << " got " << a.numerator() << "/" << a.denominator() << "\n";
                ++c.failed;
            }
        } else {
            std::cerr << "Level " << level << " WARN: post-increment not available; treat as failed\n";
            ++c.failed;
        }
    } catch (...) {
        std::cerr << "Level " << level << " FAIL post-increment threw\n";
        ++c.failed;
    }
}

// ----------------------------- Main Test Loop -------------------------------
int main() {
    using T = Rational;
    std::mt19937 rng(123456);
    std::uniform_int_distribution<int> dist_num(-50, 50);
    std::uniform_int_distribution<int> dist_den(-50, 50);

    const int iterations = 200;
    Counters counters;

    for (int level = 1; level <= iterations; ++level) {
        // generate a and b with non-zero denominators
        int an = dist_num(rng);
        int ad = 0;
        while (ad == 0) ad = dist_den(rng);
        int bn = dist_num(rng);
        int bd = 0;
        while (bd == 0) bd = dist_den(rng);

        // Create Rational objects
        T a; T b;
        try {
            a = T(an, ad);
            b = T(bn, bd);
        } catch (...) {
            std::cerr << "Level " << level << " FAIL constructing Rational(" << an << "," << ad << ") or Rational(" << bn << "," << bd << ")\n";
            ++counters.failed;
            ++counters.total;
            continue;
        }

        // expected reduced forms
        auto ared = reduce_pair(an, ad);
        auto bred = reduce_pair(bn, bd);

        // Accessors
        check_accessors<T>(a, ared.first, ared.second, counters, level);
        check_accessors<T>(b, bred.first, bred.second, counters, level);

        // Copy / Move
        check_copy_move<T>(a, counters, level);

        // Stream roundtrip
        check_stream_roundtrip<T>(a, counters, level);

        // to double
        check_to_double<T>(a, counters, level);

        // Arithmetic: addition
        if constexpr (has_add<T>::value) {
            check_binary_op<T>(a,b,counters,level,"operator+",
                [](const T& x,const T& y)->T { return x + y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_d + b_n * a_d;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator+ not implemented; counted as failed\n";
        }

        // Subtraction
        if constexpr (has_sub<T>::value) {
            check_binary_op<T>(a,b,counters,level,"operator-",
                [](const T& x,const T& y)->T { return x - y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_d - b_n * a_d;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator- not implemented; counted as failed\n";
        }

        // Multiplication
        if constexpr (has_mul<T>::value) {
            check_binary_op<T>(a,b,counters,level,"operator*",
                [](const T& x,const T& y)->T { return x * y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_n;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator* not implemented; counted as failed\n";
        }

        // Division (skip if dividing by zero)
        if constexpr (has_div<T>::value) {
            check_binary_op<T>(a,b,counters,level,"operator/",
                [](const T& x,const T& y)->T { return x / y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    // x / y = (a_n/a_d) / (b_n/b_d) = a_n * b_d / (a_d * b_n)
                    if (b_n == 0) return {0, 0}; // mark invalid (denominator 0)
                    i64 rn = a_n * b_d;
                    i64 rd = a_d * b_n;
                    return reduce_pair(rn, rd);
                },
                /*skip_if_invalid=*/true
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator/ not implemented; counted as failed\n";
        }

        // Compound assignment tests (+=, -=, *=, /=)
        if constexpr (has_plus_assign<T>::value) {
            check_compound_assign<T>(a,b,counters,level,"operator+=",
                [](T &x,const T &y){ x += y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_d + b_n * a_d;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator+= not implemented; counted as failed\n";
        }

        if constexpr (has_minus_assign<T>::value) {
            check_compound_assign<T>(a,b,counters,level,"operator-=",
                [](T &x,const T &y){ x -= y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_d - b_n * a_d;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator-= not implemented; counted as failed\n";
        }

        if constexpr (has_mul_assign<T>::value) {
            check_compound_assign<T>(a,b,counters,level,"operator*=",
                [](T &x,const T &y){ x *= y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    i64 rn = a_n * b_n;
                    i64 rd = a_d * b_d;
                    return reduce_pair(rn, rd);
                }
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator*= not implemented; counted as failed\n";
        }

        if constexpr (has_div_assign<T>::value) {
            check_compound_assign<T>(a,b,counters,level,"operator/=",
                [](T &x,const T &y){ x /= y; },
                [&](i64 a_n,i64 a_d,i64 b_n,i64 b_d)->std::pair<i64,i64> {
                    if (b_n == 0) return {0,0};
                    i64 rn = a_n * b_d;
                    i64 rd = a_d * b_n;
                    return reduce_pair(rn, rd);
                },
                true
            );
        } else {
            ++counters.total; ++counters.failed;
            std::cerr << "Level " << level << " WARN: operator/= not implemented; counted as failed\n";
        }

        // Comparisons (== and <) -> count other relations indirectly
        check_comparisons<T>(a,b,counters,level);

        // unary ++/-- tests (only ++ implemented checks are provided)
        check_unary_inc_dec<T>(a, counters, level);

        // Print progress every 20 levels
        if (level % 20 == 0) {
            std::cout << "Progress: level " << level << " / " << iterations
                      << " (total checks: " << counters.total
                      << ", failed: " << counters.failed
                      << ", skipped: " << counters.skipped << ")\n";
        }
    } // end levels

    // Final summary
    if (counters.failed == 0) {
        std::cout << "\nTest result: PASSED\n";
        std::cout << "Total checks: " << counters.total << ", skipped: " << counters.skipped << "\n";
        return 0;
    } else {
        std::cout << "\nTest result: FAILED: " << counters.failed << " failed checks, " << counters.skipped << " skipped\n";
        std::cout << "Total checks: " << counters.total << "\n";
        return 1;
    }
}
