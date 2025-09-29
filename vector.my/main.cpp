#include <iostream>
#include <vector>
#include <string>
#include "vector.hpp"

// Simple test harness
static int tests_run = 0;
static int tests_failed = 0;

void check(bool condition, const std::string &name) {
    ++tests_run;
    if (condition) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << "\n";
        ++tests_failed;
    }
}

void print_summary() {
    std::cout << "\n==== TEST SUMMARY ====" << std::endl;
    std::cout << "Tests run: " << tests_run << std::endl;
    std::cout << "Tests failed: " << tests_failed << std::endl;
    if (tests_failed == 0)
        std::cout << "\n🎉 All tests passed successfully!" << std::endl;
    else
        std::cout << "\n❌ Some tests failed." << std::endl;
}

bool compare_contents(const MyVector *v, const std::vector<int> &expected) {
    if (myvector_size(v) != expected.size()) return false;
    for (size_t i = 0; i < expected.size(); ++i) {
        if (v->data[i] != expected[i]) return false;
    }
    return true;
}

void print_meta(const char *label, const MyVector *v) {
    std::cout << label << " | size=" << myvector_size(v) << " capacity=" << myvector_capacity(v) << "\n";
}

int main() {
    std::cout << "=== MyVector test suite ===\n";

    // 1) init with capacity and value
    {
        MyVector v;
        myvector_init(&v, 5, 7);
        check(myvector_size(&v) == 5, "init(size, val) sets size correctly");
        check(myvector_capacity(&v) == 5, "init(size, val) sets capacity correctly");
        std::vector<int> expected(5, 7);
        check(compare_contents(&v, expected), "init(size, val) fills values correctly");
        myvector_destroy(&v);
    }

    // 2) init with capacity 0 then push_back (test realloc)
    {
        MyVector v;
        myvector_init(&v, 0);
        check(myvector_size(&v) == 0, "init(0) -> size 0");
        size_t old_cap = myvector_capacity(&v);
        myvector_push_back(&v, 1);
        check(myvector_size(&v) == 1, "push_back on empty increases size to 1");
        check(myvector_capacity(&v) >= 1, "capacity >= 1 after first push");
        myvector_destroy(&v);
    }

    // 3) push_back and pop_back
    {
        MyVector v;
        myvector_init(&v, 1);
        myvector_push_back(&v, 10);
        myvector_push_back(&v, 20); // triggers realloc probably
        check(myvector_size(&v) == 2, "size after two pushes is 2");
        check(v.data[0] == 10 && v.data[1] == 20, "values preserved after push/realloc");
        size_t cap_after = myvector_capacity(&v);
        myvector_pop_back(&v);
        check(myvector_size(&v) == 1, "pop_back decreases size");
        check(myvector_capacity(&v) == cap_after, "pop_back does not change capacity");
        myvector_pop_back(&v);
        check(myvector_size(&v) == 0, "pop_back to empty yields size 0");
        // pop on empty should not crash and should keep size zero
        myvector_pop_back(&v);
        check(myvector_size(&v) == 0, "pop_back on empty keeps size 0 (no underflow)");
        myvector_destroy(&v);
    }

    // 4) insert at beginning, middle, end
    {
        MyVector v;
        myvector_init(&v, 2);
        myvector_push_back(&v, 1);
        myvector_push_back(&v, 3);
        // insert at beginning
        myvector_insert(&v, 0, 0); // [0,1,3]
        check(compare_contents(&v, std::vector<int>{0,1,3}), "insert at beginning places element correctly");
        // insert in middle
        myvector_insert(&v, 2, 2); // [0,1,2,3]
        check(compare_contents(&v, std::vector<int>{0,1,2,3}), "insert in middle places element correctly");
        // insert at end (index == size)
        myvector_insert(&v, myvector_size(&v), 4); // [0,1,2,3,4]
        check(compare_contents(&v, std::vector<int>{0,1,2,3,4}), "insert at end (index==size) works");
        myvector_destroy(&v);
    }

    // 5) erase valid and invalid indices
    {
        MyVector v;
        myvector_init(&v, 5);
        for (int i = 0; i < 5; ++i) myvector_push_back(&v, i); // [0,1,2,3,4]
        myvector_erase(&v, 2); // remove '2' -> [0,1,3,4]
        check(compare_contents(&v, std::vector<int>{0,1,3,4}), "erase middle element shifts correctly");
        size_t sz_before = myvector_size(&v);
        // erase invalid index (>= size) should not change size
        myvector_erase(&v, 100);
        check(myvector_size(&v) == sz_before, "erase with invalid index does not change size (safe)");
        myvector_destroy(&v);
    }

    // 6) clear keeps capacity but sets size to zero
    {
        MyVector v;
        myvector_init(&v, 10);
        for (int i = 0; i < 7; ++i) myvector_push_back(&v, i);
        size_t cap_before = myvector_capacity(&v);
        myvector_clear(&v);
        check(myvector_size(&v) == 0, "clear resets size to 0");
        check(myvector_capacity(&v) == cap_before, "clear does not change capacity");
        myvector_destroy(&v);
    }

    // 7) destroy sets data to nullptr and size/capacity to 0
    {
        MyVector v;
        myvector_init(&v, 3);
        myvector_push_back(&v, 1);
        myvector_destroy(&v);
        check(v.data == nullptr, "destroy sets data to nullptr");
        check(myvector_size(&v) == 0 && myvector_capacity(&v) == 0, "destroy sets size and capacity to 0");
    }

    // 8) stress test: 100 elements push, then erase some, insert some
    {
        MyVector v;
        myvector_init(&v, 0);
        const int N = 100;
        for (int i = 0; i < N; ++i) myvector_push_back(&v, i);
        check(myvector_size(&v) == (size_t)N, "stress: pushed 100 elements -> size==100");
        bool ok_contents = true;
        for (int i = 0; i < N; ++i) {
            if (v.data[i] != i) { ok_contents = false; break; }
        }
        check(ok_contents, "stress: pushed 100 elements -> contents correct");

        // erase every 10th element starting from end to avoid shifting complexity
        for (int i = N-1; i >= 0; i -= 10) {
            myvector_erase(&v, i);
        }
        // now expected size: 100 - 10 = 90 (because we removed indices 99,89,...,9 => 10 elements)
        check(myvector_size(&v) == (size_t)(N - 10), "stress: after erasing 10 elements size correct");

        // insert 5 elements at various positions
        myvector_insert(&v, 0, 555);
        myvector_insert(&v, myvector_size(&v)/2, 666);
        myvector_insert(&v, myvector_size(&v), 777);
        check(myvector_size(&v) == (size_t)(N - 10 + 3), "stress: after 3 inserts size correct");

        // quick sanity check: ensure no out-of-bounds for first and last
        check(v.data[0] == 555, "stress: insert at head placed 555");
        check(v.data[myvector_size(&v)-1] == 777, "stress: insert at tail placed 777");

        myvector_destroy(&v);
    }

    // 9) more edge-case checks
    {
        MyVector v;
        myvector_init(&v, 2);
        // insert with index > size should be ignored and not crash
        myvector_insert(&v, 1000, 1);
        check(myvector_size(&v) == 0, "insert with invalid index does not change vector (safe)");

        // erase on empty should be safe
        myvector_erase(&v, 0);
        check(myvector_size(&v) == 0, "erase on empty vector keeps size 0 (safe)");

        myvector_destroy(&v);
    }

    print_summary();
    return (tests_failed == 0) ? 0 : 1;
}
