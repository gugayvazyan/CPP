// main.cpp
#include <iostream>
#include <random>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include "SingleList.hpp"

// Helper: convert SingleList -> vector<int> using explicit operator
static std::vector<int> to_vector(const SingleList& lst) {
    return static_cast<std::vector<int>>(lst); // header declared explicit
}

// Helper: compare list against vector, print diagnostic on mismatch
static void assert_list_equals_vec(const SingleList& lst, const std::vector<int>& v, const std::string& context = "") {
    std::vector<int> lv = to_vector(lst);
    if (lv != v) {
        std::cerr << "Mismatch! " << context << "\n";
        std::cerr << "Expected vector: [";
        for (size_t i = 0; i < v.size(); ++i) {
            if (i) std::cerr << ", ";
            std::cerr << v[i];
        }
        std::cerr << "]\n";

        std::cerr << "List -> vector: [";
        for (size_t i = 0; i < lv.size(); ++i) {
            if (i) std::cerr << ", ";
            std::cerr << lv[i];
        }
        std::cerr << "]\n";

        // also print size() and operator! and operator==
        std::cerr << "list.size() = " << lst.size() << ", vector.size() = " << v.size() << "\n";
        std::cerr << "list.operator!() = " << (!lst ? "true" : "false") << "\n";
        // try operator==
        bool eq = (lst == lst); // sanity
        std::cerr << "abort due to mismatch\n";
        std::exit(1);
    }
    // sanity: check size() matches
    if (static_cast<int>(v.size()) != lst.size()) {
        std::cerr << "Size mismatch (vector vs list.size()). context: " << context << "\n";
        std::cerr << "vector.size()=" << v.size() << " list.size()=" << lst.size() << "\n";
        std::exit(1);
    }
}

// Random helpers
static int rand_val(std::mt19937 &rng) {
    std::uniform_int_distribution<int> d(-1000, 1000);
    return d(rng);
}

int main() {
    constexpr int LEVELS = 200;
    std::cout << "Running " << LEVELS << " randomized test levels...\n";

    for (int level = 1; level <= LEVELS; ++level) {
        // deterministic per-level seed for reproducibility
        std::mt19937 rng(level * 0x9e3779b1u);

        // number of ops this level
        std::uniform_int_distribution<int> ops_dist(10, 80);
        int ops = ops_dist(rng);

        // our oracle vector and tested list
        std::vector<int> oracle;
        SingleList s;

        // small helper lambdas capturing rng by ref
        auto push_back = [&](int x) {
            oracle.push_back(x);
            s.push_back(x);
            assert_list_equals_vec(s, oracle, "after push_back");
        };
        auto push_front = [&](int x) {
            oracle.insert(oracle.begin(), x);
            s.push_front(x);
            assert_list_equals_vec(s, oracle, "after push_front");
        };
        auto pop_back = [&]() {
            if (!oracle.empty()) {
                oracle.pop_back();
            }
            s.pop_back();
            assert_list_equals_vec(s, oracle, "after pop_back");
        };
        auto pop_front = [&]() {
            if (!oracle.empty()) {
                oracle.erase(oracle.begin());
            }
            s.pop_front();
            assert_list_equals_vec(s, oracle, "after pop_front");
        };

        // perform sequence of random operations
        std::uniform_int_distribution<int> op_choice(0, 12);
        for (int step = 0; step < ops; ++step) {
            int op = op_choice(rng);
            switch (op) {
                case 0: { // push_back
                    int v = rand_val(rng);
                    push_back(v);
                    break;
                }
                case 1: { // push_front
                    int v = rand_val(rng);
                    push_front(v);
                    break;
                }
                case 2: { // pop_back
                    pop_back();
                    break;
                }
                case 3: { // pop_front
                    pop_front();
                    break;
                }
                case 4: { // operator[] write (if non-empty) or push_back if empty
                    if (oracle.empty()) {
                        int v = rand_val(rng);
                        push_back(v);
                    } else {
                        std::uniform_int_distribution<size_t> idxd(0, oracle.size()-1);
                        size_t idx = idxd(rng);
                        int v = rand_val(rng);
                        oracle[idx] = v;
                        try {
                            s[idx] = v; // uses operator[]
                        } catch (const std::out_of_range&) {
                            std::cerr << "Unexpected out_of_range on operator[] write. level=" << level << " step=" << step << "\n";
                            std::exit(1);
                        }
                        assert_list_equals_vec(s, oracle, "after operator[] write");
                    }
                    break;
                }
                case 5: { // operator[] read (verify)
                    if (!oracle.empty()) {
                        std::uniform_int_distribution<size_t> idxd(0, oracle.size()-1);
                        size_t idx = idxd(rng);
                        int fromVec = oracle[idx];
                        int fromList = 0;
                        try {
                            fromList = s[idx];
                        } catch (const std::out_of_range&) {
                            std::cerr << "Unexpected out_of_range on operator[] read. level=" << level << " step=" << step << "\n";
                            std::exit(1);
                        }
                        if (fromVec != fromList) {
                            std::cerr << "operator[] read mismatch. level=" << level << " step=" << step << "\n";
                            std::exit(1);
                        }
                    }
                    break;
                }
                case 6: { // prefix ++
                    s.operator++();
                    oracle.push_back(0);
                    assert_list_equals_vec(s, oracle, "after prefix ++");
                    break;
                }
                case 7: { // postfix ++
                    // get old copy and ensure original+added
                    SingleList old = s++;
                    // old should match oracle before change
                    std::vector<int> oldVec = to_vector(old);
                    if (oldVec != oracle) {
                        std::cerr << "postfix++ returned wrong old copy. level=" << level << " step=" << step << "\n";
                        std::exit(1);
                    }
                    oracle.push_back(0);
                    assert_list_equals_vec(s, oracle, "after postfix ++");
                    break;
                }
                case 8: { // prefix -- (pop_back)
                    // pop_back may do nothing if empty
                    s.operator--();
                    if (!oracle.empty()) oracle.pop_back();
                    assert_list_equals_vec(s, oracle, "after prefix -- (pop_back)");
                    break;
                }
                case 9: { // postfix -- (pop_back)
                    SingleList old = s--;
                    std::vector<int> oldVec = to_vector(old);
                    if (oldVec != oracle) {
                        std::cerr << "postfix-- returned wrong old copy. level=" << level << " step=" << step << "\n";
                        std::exit(1);
                    }
                    if (!oracle.empty()) oracle.pop_back();
                    assert_list_equals_vec(s, oracle, "after postfix --");
                    break;
                }
                case 10: { // operator+ (concat, produces new)
                    // create another random small list
                    SingleList other;
                    std::vector<int> otherv;
                    std::uniform_int_distribution<int> lenDist(0, 6);
                    int len = lenDist(rng);
                    for (int i = 0; i < len; ++i) {
                        int vv = rand_val(rng);
                        other.push_back(vv);
                        otherv.push_back(vv);
                    }
                    // result should be concatenation
                    SingleList res = s + other;
                    std::vector<int> expect = oracle;
                    expect.insert(expect.end(), otherv.begin(), otherv.end());
                    assert_list_equals_vec(res, expect, "after operator+");
                    // original unchanged
                    assert_list_equals_vec(s, oracle, "after operator+ original unchanged");
                    break;
                }
                case 11: { // operator+=(const SingleList&)
                    // copy-append: make small list
                    SingleList other;
                    std::vector<int> otherv;
                    std::uniform_int_distribution<int> lenDist(0, 6);
                    int len = lenDist(rng);
                    for (int i = 0; i < len; ++i) {
                        int vv = rand_val(rng);
                        other.push_back(vv);
                        otherv.push_back(vv);
                    }
                    s += other; // copy-append
                    oracle.insert(oracle.end(), otherv.begin(), otherv.end());
                    assert_list_equals_vec(s, oracle, "after operator+=(const&)");
                    break;
                }
                case 12: { // operator+=(SingleList& ) move-append
                    // create temp list and move-append
                    SingleList other;
                    std::vector<int> otherv;
                    std::uniform_int_distribution<int> lenDist(0, 6);
                    int len = lenDist(rng);
                    for (int i = 0; i < len; ++i) {
                        int vv = rand_val(rng);
                        other.push_back(vv);
                        otherv.push_back(vv);
                    }
                    // call move-append; header defines operator+=(SingleList&), so pass non-const
                    s += other;
                    // After move-append header implementation sets other.head=nullptr; so other must be empty
                    // For oracle we append values
                    oracle.insert(oracle.end(), otherv.begin(), otherv.end());
                    assert_list_equals_vec(s, oracle, "after operator+=(move-like)");
                    break;
                }
                default:
                    break;
            }

            // sanity checks after each step
            // operator! should reflect emptiness
            bool list_bool = static_cast<bool>(s);
            bool expect_bool = !oracle.empty();
            if (list_bool != expect_bool) {
                std::cerr << "operator bool mismatch. level=" << level << " step=" << step << "\n";
                std::exit(1);
            }
            // operator== with a freshly built list from vector
            SingleList rebuilt;
            for (int x : oracle) rebuilt.push_back(x);
            if (!(rebuilt == s)) {
                std::cerr << "operator== mismatch with rebuilt list. level=" << level << " step=" << step << "\n";
                std::exit(1);
            }
            if (!(s == rebuilt)) {
                std::cerr << "operator== asymmetric? level=" << level << " step=" << step << "\n";
                std::exit(1);
            }
        } // end ops

        // At end of level also test copy and move ctors/operators
        {
            SingleList copy_constructed(s);
            assert_list_equals_vec(copy_constructed, oracle, "after copy ctor");

            SingleList assigned;
            assigned = s;
            assert_list_equals_vec(assigned, oracle, "after copy assignment");

            // move constructor
            SingleList tmp_for_move;
            for (int x = 0; x < 3; ++x) tmp_for_move.push_back(rand_val(rng));
            SingleList moved(std::move(tmp_for_move));
            // moved has 3 elements, tmp_for_move should be empty
            if (static_cast<bool>(tmp_for_move)) {
                std::cerr << "move ctor did not null source. level=" << level << "\n";
                std::exit(1);
            }

            // move assignment
            SingleList a;
            a = std::move(moved);
            // moved (source) should be empty after move assignment
            // (we don't assume anything about moved's content now)
            if (static_cast<bool>(moved)) {
                std::cerr << "move assignment did not null source. level=" << level << "\n";
                std::exit(1);
            }
        }

        // small progress
        if (level % 50 == 0) {
            std::cout << "Passed level " << level << "\n";
        }
    } // end levels

    std::cout << "All " << LEVELS << " levels passed successfully.\n";
    return 0;
}
