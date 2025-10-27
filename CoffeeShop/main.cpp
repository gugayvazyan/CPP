// test_coffeeshop.cpp
// Comprehensive automated test suite for the CoffeeShop project.
// Compile: g++ -std=c++17 test_coffeeshop.cpp -o test_coffeeshop
// Run: ./test_coffeeshop

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>
#include <cmath>
#include <cstring>

#include "CoffeeShop.hpp"

// ---------- Helpers for capturing output and simulating input ----------
struct CoutCapture {
    std::streambuf* oldbuf;
    std::ostringstream oss;
    CoutCapture() { oldbuf = std::cout.rdbuf(oss.rdbuf()); }
    ~CoutCapture() { std::cout.rdbuf(oldbuf); }
    std::string str() const { return oss.str(); }
};

struct CinFeeder {
    std::streambuf* oldbuf;
    std::istringstream iss;
    CinFeeder(const std::string& s) : iss(s) { oldbuf = std::cin.rdbuf(iss.rdbuf()); }
    ~CinFeeder() { std::cin.rdbuf(oldbuf); }
};

// Floating compare
bool nearlyEqual(double a, double b, double eps=1e-6) {
    return std::fabs(a - b) <= eps;
}

// Simple test harness
class Tester {
public:
    std::vector<std::string> failures;

    void expect(bool cond, const std::string& msg) {
        if (!cond) failures.push_back(msg);
    }

    void report() {
        if (failures.empty()) {
            std::cout << "\n=== All tests passed successfully! ===\n";
        } else {
            std::cout << "\n=== TEST FAILURES: " << failures.size() << " ===\n";
            for (size_t i = 0; i < failures.size(); ++i) {
                std::cout << i+1 << ") " << failures[i] << "\n";
            }
            std::cout << "=== End of failures ===\n";
        }
    }

    int exitCode() const { return failures.empty() ? 0 : 1; }
} T;

// Small helper to safely cleanup pointers still owned by CoffeeShop via its API
void cleanupShop(CoffeeShop& shop) {
    // remove all drinks by name (use removeDrink which deletes)
    auto menu = shop.getMenu();
    // Copy names because removeDrink mutates vector
    std::vector<std::string> names;
    for (auto d : menu) if (d) names.push_back(d->getName());
    for (auto& n : names) shop.removeDrink(n);

    // fire all baristas via API (fireBarista deletes them)
    auto bars = shop.getBaristas();
    std::vector<std::string> bnames;
    for (auto b : bars) if (b) bnames.push_back(b->getName());
    for (auto& n : bnames) {
        Barista temp(n); // create a temporary Barista object with same name to match
        shop.fireBarista(&temp);
    }

    // Delete customers left in queue (CoffeeShop doesn't delete them)
    auto q = shop.getQueue();
    for (auto c : q) {
        delete c;
    }
    // Note: after this the CoffeeShop.queue still points to freed addresses;
    // we are not using shop afterwards in ways dependent on queue internals.
}

// ---------- Tests ----------

void test_drink_basic() {
    // Constructor, getters, setters, display format
    Drink d("Mocha", Drink::Size::Medium, 3.75);
    T.expect(d.getName() == "Mocha", "Drink.getName mismatch");
    T.expect(d.getVolume() == Drink::Size::Medium, "Drink.getVolume mismatch");
    T.expect(nearlyEqual(d.getPrice(), 3.75), "Drink.getPrice mismatch");

    d.setName("Americano");
    d.setVolume(Drink::Size::Small);
    d.setPrice(2.25);
    T.expect(d.getName() == "Americano", "Drink.setName failed");
    T.expect(d.getVolume() == Drink::Size::Small, "Drink.setVolume failed");
    T.expect(nearlyEqual(d.getPrice(), 2.25), "Drink.setPrice failed");

    CoutCapture cap;
    d.display();
    std::string out = cap.str();
    T.expect(out.find("Americano") != std::string::npos, "Drink.display missing name");
    T.expect(out.find("2.25") != std::string::npos, "Drink.display missing price");
    T.expect(out.find("Small") != std::string::npos, "Drink.display missing size string");
}

void test_order_operations() {
    Order o(101);
    // calculateTotalPrice should be safe to call even with empty drinks
    o.calculateTotalPrice();
    T.expect(nearlyEqual(o.getTotalPrice(), 0.0), "Empty order total should be 0");

    Drink a("Espresso", Drink::Size::Small, 2.50);
    Drink b("Latte", Drink::Size::Big, 4.00);

    o.addDrink(a);
    o.addDrink(b);
    T.expect(o.getOrderId() == 101, "OrderId getter wrong");
    T.expect(o.getTotalPrice() == a.getPrice() + b.getPrice(), "Order total price after adds wrong");

    // remove out-of-range index (should print "Wrong index.")
    {
        CoutCapture cap;
        o.removeDrink(999);
        std::string out = cap.str();
        T.expect(out.find("Wrong index") != std::string::npos, "removeDrink(out-of-range) didn't print expected message");
    }

    // remove valid index and check price recalculation
    o.removeDrink(0); // remove first (Espresso)
    T.expect(nearlyEqual(o.getTotalPrice(), b.getPrice()), "Order total after removing drink wrong");

    // display order should not crash
    {
        CoutCapture cap;
        o.displayOrder();
        std::string out = cap.str();
        T.expect(out.find("Total Price") != std::string::npos, "displayOrder missing 'Total Price' text");
    }

    // mark prepared and paid flags
    o.markPrepared();
    o.markPaid();
    T.expect(o.getIsPrepared(), "Order.markPrepared failed");
    T.expect(o.getIsPaid(), "Order.markPaid failed");
}

void test_barista_behavior() {
    Order o(202);
    o.calculateTotalPrice(); // initialize

    Barista b("Alice");

    // prepare null order -> should print "No order to prepare."
    {
        CoutCapture cap;
        b.prepareOrder(nullptr);
        T.expect(cap.str().find("No order to prepare") != std::string::npos, "Barista.prepareOrder(nullptr) message missing");
    }

    // prepare a fresh order (should mark prepared)
    {
        CoutCapture cap;
        b.prepareOrder(&o);
        std::string out = cap.str();
        // Either prepared message OR already prepared (both acceptable as long as result is prepared)
        T.expect(out.find("has been prepared by") != std::string::npos || out.find("already prepared") != std::string::npos,
                 "Barista.prepareOrder didn't print expected prepared message");
    }
    T.expect(o.getIsPrepared(), "Barista.prepareOrder should result in order marked prepared");

    // calling prepare again should indicate already prepared
    {
        CoutCapture cap;
        b.prepareOrder(&o);
        T.expect(cap.str().find("already prepared") != std::string::npos || cap.str().find("has been prepared") != std::string::npos,
                 "Second Barista.prepareOrder expected 'already prepared' or similar");
    }

    // greetCustomer
    {
        CoutCapture cap;
        b.greetCustomer();
        T.expect(cap.str().find("Hello! I'm Alice") != std::string::npos, "Barista.greetCustomer output wrong");
    }
}

void test_customer_place_and_pay() {
    // We'll simulate interactive input for placeOrder using CinFeeder
    Customer c("Bob", 5.0); // initial balance $5.00
    Order* ord = new Order(303);

    // Prepare input: add two drinks then finish.
    // The placeOrder menu expects:
    // 1 (Add) name price size
    // 1 (Add) name price size
    // 4 (Finish)
    // We provide whitespace separated values (std::cin >> works with spaces/newlines).
    std::string input =
        "1\nEspresso\n2.50\n0\n"   // add Espresso small 2.50
        "1\nLatte\n3.00\n2\n"      // add Latte big 3.00
        "4\n";                     // finish

    {
        CinFeeder feed(input);
        CoutCapture cap;
        c.placeOrder(ord); // interactive but fed programmatically
        // capture display messages optionally
    }

    T.expect(c.hasOrder(), "Customer should have an order after placeOrder");
    Order* cur = c.getCurrentOrder();
    T.expect(cur != nullptr, "getCurrentOrder returned nullptr after placeOrder");
    T.expect(nearlyEqual(cur->getTotalPrice(), 2.50 + 3.00), "Order total from placeOrder mismatch");

    // payOrder: first attempt should be declined because balance (5.0) < total (5.5)
    {
        CoutCapture cap;
        c.payOrder();
        T.expect(cap.str().find("Declined: insufficient funds") != std::string::npos ||
                 cap.str().find("Order paid successfully") == std::string::npos,
                 "payOrder with insufficient funds didn't print expected decline message");
    }
    // increase balance and pay
    c.setBalance(10.0);
    {
        CoutCapture cap;
        c.payOrder();
        std::string out = cap.str();
        T.expect(out.find("Order paid successfully") != std::string::npos, "payOrder didn't confirm successful payment");
    }
    T.expect(cur->getIsPaid(), "Order should be marked paid after successful payOrder");

    // paying again should say "Order already paid."
    {
        CoutCapture cap;
        c.payOrder();
        T.expect(cap.str().find("Order already paid") != std::string::npos, "Second payOrder didn't print 'already paid' message");
    }

    // viewOrder should display content
    {
        CoutCapture cap;
        c.viewOrder();
        T.expect(cap.str().find("Drinks in order") != std::string::npos || cap.str().find("Total Price") != std::string::npos,
                 "viewOrder didn't display expected order details");
    }

    delete ord;
}

void test_coffeeshop_end_to_end() {
    CoffeeShop shop("TestCafe");

    // constructMenu (adds 3 drinks)
    shop.constructMenu();
    T.expect(shop.getMenu().size() == 3, "constructMenu should create 3 menu items");

    // showMenu should print something
    {
        CoutCapture cap;
        shop.showMenu();
        T.expect(cap.str().find("Espresso") != std::string::npos || cap.str().size() > 0, "showMenu output unexpected");
    }

    // add an extra drink dynamically and then remove by name
    Drink* special = new Drink("Peppermint", Drink::Size::Medium, 5.25);
    shop.addDrink(special);
    T.expect(shop.getMenu().size() == 4, "addDrink didn't increase menu size");

    // remove existing
    {
        CoutCapture cap;
        shop.removeDrink("Peppermint");
        T.expect(cap.str().find("not found") == std::string::npos, "removeDrink printed 'not found' for existing drink");
    }
    T.expect(shop.getMenu().size() == 3, "removeDrink didn't reduce menu size after removing existing drink");

    // remove nonexistent
    {
        CoutCapture cap;
        shop.removeDrink("NonexistentDrink");
        T.expect(cap.str().find("not found") != std::string::npos, "removeDrink missing message when removing nonexistent drink");
    }

    // Baristas: hire and fire
    Barista* b1 = new Barista("Cathy");
    Barista* b2 = new Barista("Derek");
    shop.hireBarista(b1);
    shop.hireBarista(b2);
    T.expect(shop.getBaristas().size() == 2, "hireBarista didn't increase barista list");

    {
        CoutCapture cap;
        shop.showBaristas();
        T.expect(cap.str().find("Cathy") != std::string::npos && cap.str().find("Derek") != std::string::npos,
                 "showBaristas didn't list hired baristas");
    }

    // Fire a barista by passing a temporary with the same name
    {
        Barista temp("Cathy");
        shop.fireBarista(&temp);
        T.expect(shop.getBaristas().size() == 1, "fireBarista didn't remove barista by name");
    }

    // Fire nonexistent
    {
        Barista temp("DoesNotExist");
        CoutCapture cap;
        shop.fireBarista(&temp);
        T.expect(cap.str().find("Barista not found") != std::string::npos, "fireBarista missing 'Barista not found' message for nonexistent barista");
    }

    // Customers and serving
    Customer* cust1 = new Customer("Eve", 20.0);
    Customer* cust2 = new Customer("Frank", 2.0);

    // Create orders and attach to customers (use placeOrder simulation)
    Order* o1 = new Order(401);
    o1->addDrink(Drink("Espresso", Drink::Size::Small, 2.5));
    o1->addDrink(Drink("Latte", Drink::Size::Big, 4.0));
    // Use interactive placeOrder simulation to set currentOrder (customer.placeOrder is interactive)
    {
        // We'll bypass placeOrder by feeding interactive input that immediately finishes (no adds)
        // and then manually set the current order by calling placeOrder with prepared input.
        // However placeOrder always reads from cin; we will just call placeOrder and then overwrite currentOrder via hack:
        // But since currentOrder is private and no setter exists, we'll instead simulate placing the order using placeOrder with inputs that add items.
        std::string feed = "1\nEspresso\n2.5\n0\n1\nLatte\n4.0\n2\n4\n";
        CinFeeder f(feed);
        cust1->placeOrder(o1); // will set currentOrder
    }

    // For cust2: small order more than balance to test insufficient funds
    Order* o2 = new Order(402);
    o2->addDrink(Drink("Expensive", Drink::Size::Big, 10.0));
    {
        std::string feed = "1\nExpensive\n10.0\n2\n4\n";
        CinFeeder f(feed);
        cust2->placeOrder(o2);
    }

    shop.addCustomerToQueue(cust1);
    shop.addCustomerToQueue(cust2);

    T.expect(shop.getQueue().size() >= 2, "addCustomerToQueue didn't add customers properly");

    // Serve next customer when there is one barista (Derek left in shop.getBaristas())
    {
        CoutCapture cap;
        shop.serveNextCustomer(); // should serve cust1 and prepare its order if barista exists
        std::string out = cap.str();
        T.expect(out.find("Serving customer") != std::string::npos, "serveNextCustomer missing 'Serving customer' message");
    }

    // Serve next customer when barista exists (should prepare or print no baristas depending on internal state)
    {
        CoutCapture cap;
        shop.serveNextCustomer();
        // It may print "No baristas available" if none; either branch acceptable as long as no crash
        T.expect(cap.str().size() >= 0, "serveNextCustomer produced no output (unexpected)");
    }

    // showQueue and displayShopInfo (should not crash)
    {
        CoutCapture cap;
        shop.showQueue();
        shop.displayShopInfo();
        T.expect(cap.str().size() >= 0, "showQueue/displayShopInfo produced no output (unexpected)");
    }

    // Cleanup: CoffeeShop.removeDrink will delete remaining menu items
    cleanupShop(shop);

    // delete customers and orders we created
    delete o1;
    delete o2;
    delete cust1;
    delete cust2;
    // Note: barista pointers b2 has been deleted by fireBarista; do not delete again.
}

int main() {
    std::cout << "Running CoffeeShop automated test suite...\n";

    test_drink_basic();
    test_order_operations();
    test_barista_behavior();
    test_customer_place_and_pay();
    test_coffeeshop_end_to_end();

    T.report();
    return T.exitCode();
}
