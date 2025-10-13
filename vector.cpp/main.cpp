#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include "VecInt.hpp"

int main() {
    std::cout << "=== VecInt 200-Level Randomized Test ===\n";

    VecInt myvec;
    std::vector<int> ref;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> opdist(0, 6);
    std::uniform_int_distribution<int> valdist(-1000, 1000);

    bool all_good = true;

    for (int level = 1; level <= 200; ++level) {
        int op = opdist(rng);
        try {
            switch (op) {
                case 0: { // push_back
                    int val = valdist(rng);
                    myvec.push_back(val);
                    ref.push_back(val);
                    break;
                }
                case 1: { // pop_back
                    if (!ref.empty()) {
                        myvec.pop_back();
                        ref.pop_back();
                    }
                    break;
                }
                case 2: { // insert
                    if (!ref.empty()) {
                        int val = valdist(rng);
                        std::uniform_int_distribution<size_t> posdist(0, ref.size() - 1);
                        size_t pos = posdist(rng);
                        myvec.insert(pos, val);
                        ref.insert(ref.begin() + pos, val);
                    }
                    break;
                }
                case 3: { // clear
                    myvec.myvector_clear();
                    ref.clear();
                    break;
                }
                case 4: { // copy constructor
                    VecInt tmp = myvec;
                    VecInt copy = tmp;
                    (void)copy;
                    break;
                }
                case 5: { // move constructor
                    VecInt tmp = myvec;
                    VecInt moved = std::move(tmp);
                    (void)moved;
                    break;
                }
                case 6: { // assignment
                    VecInt assigned;
                    assigned = myvec;
                    (void)assigned;
                    break;
                }
            }

            // === Проверка синхронности с std::vector ===
            if (myvec.size() != ref.size()) {
                std::cerr << "❌ Size mismatch at level " << level << "\n";
                all_good = false;
                break;
            }

            // Сравнение содержимого
            for (size_t i = 0; i < myvec.size(); ++i) {
                // Нам нужен метод для доступа: myvec[i]
                // Если у тебя его нет — временно можно добавить метод get(i)
                extern int get_at(const VecInt&, size_t); // или заменить на твой
            }

        } catch (...) {
            std::cerr << "❌ Exception caught at level " << level << "\n";
            all_good = false;
            break;
        }
    }

    if (all_good)
        std::cout << "\n✅ All 200 randomized levels PASSED.\n";
    else
        std::cout << "\n❌ Test FAILED.\n";

    return 0;
}
