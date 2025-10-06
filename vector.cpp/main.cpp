// main.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>      
#include "VecInt.hpp"      

static std::string capture_stdout(const std::function<void()> &fn) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    fn();
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    bool all_ok = true;

    // 1) default ctor
    VecInt v1;
    if (v1.size() != 0) {
        std::cout << "[FAIL] default ctor: size != 0\n";
        all_ok = false;
    }
    if (v1.capacity() != 0) {
        std::cout << "[FAIL] default ctor: capacity != 0\n";
        all_ok = false;
    }
    std::string out = capture_stdout([&](){ v1.print(); });
    if (out != "\n") {
        std::cout << "[FAIL] default ctor: print != empty line (got: \"" << out << "\")\n";
        all_ok = false;
    }

    // 2) ctor (n, val)
    VecInt v2(5, 7);
    if (v2.size() != 5) { std::cout << "[FAIL] ctor(n,val): size\n"; all_ok = false; }
    out = capture_stdout([&](){ v2.print(); });
    if (out != "7 7 7 7 7 \n") { std::cout << "[FAIL] ctor(n,val): print got: \"" << out << "\"\n"; all_ok = false; }

    // 3) initializer_list
    VecInt v3 = {1,2,3,4};
    if (v3.size() != 4) { std::cout << "[FAIL] init-list: size\n"; all_ok = false; }
    out = capture_stdout([&](){ v3.print(); });
    if (out != "1 2 3 4 \n") { std::cout << "[FAIL] init-list: print got: \"" << out << "\"\n"; all_ok = false; }

    // 4) push_back and capacity growth behavior
    VecInt v4;
    std::vector<size_t> caps;
    for (int i = 0; i < 16; ++i) {
        v4.push_back(i);
        caps.push_back(v4.capacity());
        if (v4.size() != (size_t)(i+1)) {
            std::cout << "[FAIL] push_back: wrong size after push " << i << "\n";
            all_ok = false;
        }
        if (v4.capacity() < v4.size()) {
            std::cout << "[FAIL] push_back: capacity < size after push " << i << "\n";
            all_ok = false;
        }
    }
    for (size_t i = 1; i < caps.size(); ++i) {
        if (caps[i] < caps[i-1]) {
            std::cout << "[FAIL] capacity decreased at step " << i << " (" << caps[i-1] << " -> " << caps[i] << ")\n";
            all_ok = false;
        }
    }
    out = capture_stdout([&](){ v4.print(); });
    std::string expected="";
    for (int i=0;i<16;++i){ expected += std::to_string(i) + " "; }
    expected += "\n";
    if (out != expected) {
        std::cout << "[FAIL] push_back: contents mismatch\nGot: \"" << out << "\"\nExp: \"" << expected << "\"\n";
        all_ok = false;
    }

    // 5) pop_back
    VecInt v5 = {10,20,30};
    v5.pop_back();
    out = capture_stdout([&](){ v5.print(); });
    if (out != "10 20 \n") { std::cout << "[FAIL] pop_back: after pop contents: " << out << "\n"; all_ok = false; }
    v5.pop_back();
    v5.pop_back();
    std::string msg = capture_stdout([&](){ v5.pop_back(); });
    if (msg.find("Vector is empty") == std::string::npos &&
        msg.find("Vector is empty:") == std::string::npos) {
        std::cout << "[FAIL] pop_back on empty: expected message containing 'Vector is empty', got: \"" << msg << "\"\n";
        all_ok = false;
    }

    // 6) insert
    VecInt v6 = {1,3,4};
    v6.insert(1, 2);
    out = capture_stdout([&](){ v6.print(); });
    if (out != "1 2 3 4 \n") { std::cout << "[FAIL] insert middle: got: \"" << out << "\"\n"; all_ok = false; }
    v6.insert(0, 0);
    out = capture_stdout([&](){ v6.print(); });
    if (out != "0 1 2 3 4 \n") { std::cout << "[FAIL] insert begin: got: \"" << out << "\"\n"; all_ok = false; }
    v6.insert(v6.size(), 5);
    out = capture_stdout([&](){ v6.print(); });
    if (out != "0 1 2 3 4 5 \n") { std::cout << "[FAIL] insert end: got: \"" << out << "\"\n"; all_ok = false; }

    // 7) insert invalid
    std::string inv_msg = capture_stdout([&](){ v6.insert(v6.size() + 5, 99); });
    if (inv_msg.find("Invalid index") == std::string::npos &&
        inv_msg.find("You can't use index") == std::string::npos) {
        std::cout << "[FAIL] insert invalid: expected error message, got: \"" << inv_msg << "\"\n";
        all_ok = false;
    }

    // 8) copy ctor deep copy
    VecInt original = {7,8,9};
    VecInt copy = original;
    original.push_back(10);
    out = capture_stdout([&](){ copy.print(); });
    if (out != "7 8 9 \n") { std::cout << "[FAIL] copy ctor: copy changed after original modification, got: \"" << out << "\"\n"; all_ok = false; }

    // 9) clear
    VecInt v9(4, 1);
    size_t cap_before = v9.capacity();
    v9.myvector_clear();
    if (v9.size() != 0) {
        std::cout << "[FAIL] clear: size not zero after clear\n"; all_ok = false;
    }
    if (v9.capacity() < cap_before) {
        std::cout << "[FAIL] clear: capacity decreased after clear (should not) \n"; all_ok = false;
    }

    // 10) stress
    VecInt stress;
    for (int i = 0; i < 1000; ++i) stress.push_back(i);
    if (stress.size() != 1000) { std::cout << "[FAIL] stress push: size != 1000\n"; all_ok = false; }
    for (int i = 0; i < 1000; ++i) stress.pop_back();
    if (stress.size() != 0) { std::cout << "[FAIL] stress pop: size not zero\n"; all_ok = false; }

    // Итог
    if (all_ok) {
        std::cout << "Тесты пройдены: УСПЕШНО\n";
        return 0;
    } else {
        std::cout << "Тесты пройдены: НЕ УСПЕШНО\n";
        return 1;
    }
}
