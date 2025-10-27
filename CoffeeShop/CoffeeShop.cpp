#include "CoffeeShop.hpp"

// CoffeeShop
CoffeeShop::CoffeeShop(const std::string& name) : shopName(name) {}
std::string CoffeeShop::getName() const { return shopName;}
void CoffeeShop::setName(const std::string& newName) { shopName = newName; }

void CoffeeShop::addDrink(Drink* drink) { menu.push_back(drink); }
void CoffeeShop::removeDrink(const std::string& drinkName) {
    for (int i = 0; i < menu.size(); ++i) {
        if (menu[i]->getName() == drinkName) {
            delete menu[i]; 
            menu.erase(menu.begin() + i);
            return;
        }
    }
    std::cout << "Drink " << drinkName << " not found."  << std::endl; 
}
void CoffeeShop::showMenu() const {
    std::cout << "| ";
    for (int i = 0; i < menu.size(); ++i) {
        std::cout <<  i + 1 << " " << menu[i]->getName() << " | ";
    }
}

void CoffeeShop::hireBarista(Barista* barista) { baristas.push_back(barista); }
void CoffeeShop::fireBarista(Barista* barista) {
    for (int i = 0; i < baristas.size(); ++i) {
        if (baristas[i]->getName() == barista->getName()) {
            delete baristas[i]; 
            baristas.erase(baristas.begin() + i);
            return;
        }
    }
    std::cout << "Barista not found." << std::endl; 
}
void CoffeeShop::showBaristas() const {
    std::cout << "| ";
    for (int i = 0; i < baristas.size(); ++i) {
        std::cout <<  i + 1 << "." << baristas[i]->getName() << " | ";
    }
}

void CoffeeShop::addCustomerToQueue(Customer* customer) { queue.push_back(customer);}

void CoffeeShop::serveNextCustomer() {
    if (queue.empty()) {
        std::cout << "| No customers in queue. |" << std::endl;
        return;
    }

    Customer* customer = queue.front();
    queue.erase(queue.begin());

    std::cout << "| Serving customer: " << customer->getName() << " |" << std::endl;

    if (!baristas.empty() && customer->hasOrder()) {
        baristas[0]->prepareOrder(customer->getCurrentOrder());
    } else if (baristas.empty()) {
        std::cout << "| No baristas available to prepare order. |" << std::endl;
    }

}

void CoffeeShop::showQueue() const {
    for(Customer* i : queue) i->display();
}

void CoffeeShop::constructMenu() {
    addDrink(new Drink("Espresso", Drink::Size::Small, 2.5));
    addDrink(new Drink("Cappuccino", Drink::Size::Medium, 3.5));
    addDrink(new Drink("Latte", Drink::Size::Big, 4.0));
}

void CoffeeShop::displayShopInfo() const {
    std::cout << "=== Coffee Shop: " << shopName << " ===" << std::endl;

    // Menu
    std::cout << "--- Menu ---" << std::endl;
    if (menu.empty()) {
        std::cout << "No drinks available." << std::endl;
    } else {
        for (int i = 0; i < menu.size(); ++i) {
            std::cout << i + 1 << ". ";
            menu[i]->display();
        }
    }

    // Barista
    std::cout << "--- Baristas ---" << std::endl;
    if (baristas.empty()) {
        std::cout << "No baristas hired." << std::endl;
    } else {
        for (int i = 0; i < baristas.size(); ++i) {
            std::cout << i + 1 << ". " << baristas[i]->getName() << std::endl;
        }
    }

    // Queue
    std::cout << "--- Customer Queue ---" << std::endl;
    if (queue.empty()) {
        std::cout << "No customers in queue." << std::endl;
    } else {
        for (int i = 0; i < queue.size(); ++i) {
            std::cout << i + 1 << ". " << queue[i]->getName() << std::endl;
        }
    }

    std::cout << "========================" << std::endl;
}




    // Drink 
// Constructor
Drink::Drink(const std::string& name, Size volume, double price) : name(name), volume(volume), price(price) {}

// Getters
std::string Drink::getName() const { return name; }
Drink::Size Drink::getVolume() const { return volume; }
double Drink::getPrice() const { return price; }

// Setters
void Drink::setName(const std::string& newName) { name = newName; }
void Drink::setVolume(Size newVolume) { volume = newVolume; }
void Drink::setPrice(double newPrice) { price = newPrice; }

// Display
void Drink::display() const {

    std::cout << "| " << name << " | " << price << " | ";
    switch (volume)
    {
    case Size::Small:
        std::cout << "Small |" << std::endl;
        break;
    case Size::Medium:
        std::cout << "Medium |" << std::endl;
        break;
    case Size::Big:
        std::cout << "Big |" << std::endl;
        break;    
    default:
        break;
    } 
}

// Order

Order::Order(int id) : orderId(id){}

int Order::getOrderId() const { return orderId; }
double Order::getTotalPrice() const { return totalPrice; }
bool Order::getIsPrepared() const { return isPrepared; }
bool Order::getIsPaid() const { return isPaid; }

void Order::addDrink(const Drink& drink) {
    drinks.push_back(drink);
    calculateTotalPrice();
}
void Order::removeDrink(size_t index) {
    if(index >= drinks.size()) {
        std::cout << "Wrong index." << std::endl;
        return; 
    }
    drinks.erase(drinks.begin() + index);
    std::cout << "Drink successfully deleted." << std::endl;
    calculateTotalPrice();
}
void Order::calculateTotalPrice(){
    totalPrice = 0;
    for(const Drink& i : drinks){
        totalPrice += i.getPrice(); 
    }
}

void Order::markPrepared() { isPrepared = true;}
void Order::markPaid() { isPaid = true; }

void Order::displayOrder() const {
    for(int i = 0; i < drinks.size(); ++i){
        std::cout <<"| " << i + 1;
        drinks[i].display();
    }
    std::cout << "| Total Price : " << totalPrice << " |"; 
    std::cout << "| Prepared: " << (isPrepared ? "Yes" : "No");
    std::cout << " | Paid: " << (isPaid ? "Yes" : "No") << " |" << std::endl;
}



// Customer 

Customer::Customer(const std::string& name, double balance) : name(name), balance(balance) {}

std::string Customer::getName() const { return name; }
double Customer::getBalance() const { return balance; }
void Customer::setBalance(double newBalance) { balance = newBalance; }


void Customer::placeOrder(Order* order) {
    if (!order) return;

    int choice;
    bool done = false;

    while (!done) {
        std::cout << "\n1. Add drink\n2. Remove drink\n3. Display order\n4. Finish\nChoice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string name;
                double price;
                int sizeChoice;

                std::cout << "Enter drink name: ";
                std::cin >> name;

                std::cout << "Enter price: ";
                std::cin >> price;

                std::cout << "Select size (0-Small, 1-Medium, 2-Big): ";
                std::cin >> sizeChoice;

                Drink::Size size;
                switch (sizeChoice) {
                    case 0:
                        size = Drink::Size::Small;
                        break;
                    case 1:
                        size = Drink::Size::Medium;
                        break;
                    case 2:
                        size = Drink::Size::Big;
                        break;
                    default:
                        std::cout << "Wrong size.\n";
                        return;
                }
                
                Drink drink(name, size, price);
                order->addDrink(drink);
                order->calculateTotalPrice();
                break;
            }

            case 2: {
                size_t idx;
                std::cout << "Enter index to remove: ";
                std::cin >> idx;
                order->removeDrink(idx);
                break;
            }

            case 3:
                order->displayOrder();
                break;

            case 4:
                done = true;
                break;

            default:
                std::cout << "Invalid choice.\n";
        }
    }
    currentOrder = order;
}

void Customer::payOrder () {
    if (!currentOrder) {
        std::cout << "| No active order to pay. |" << std::endl;
        return;
    }

    if (currentOrder->getIsPaid()) {
        std::cout << "| Order already paid. |" << std::endl;
        return;
    }

    if (balance < currentOrder->getTotalPrice()) {
        std::cout << "| Declined: insufficient funds. |" << std::endl;
        return;
    }

    balance -= currentOrder->getTotalPrice();
    currentOrder->markPaid();
    std::cout << "| Order paid successfully! |" << std::endl;
}

void Customer::viewOrder() const {
    if (!currentOrder) {
        std::cout << "| No active order to display. |" << std::endl;
        return;
    }

    std::cout << "| Drinks in order: |" << std::endl;
    currentOrder->displayOrder();
}

Order* Customer::getCurrentOrder() const { return currentOrder; }
bool Customer::hasOrder() const { return currentOrder != nullptr; }

void Customer::display() const {
    std::cout << "| Customer: " << name << " | Balance: $" << balance << " |" << std::endl;
    if (currentOrder) {
        std::cout << "| Current order ID: " << currentOrder->getOrderId() << " |" << std::endl;
    } else {
        std::cout << "| No current order |" << std::endl;
    }
}


// Barista
Barista::Barista(const std::string& name) : name(name) {}

// Getters/Setters
std::string Barista::getName() const { return name; }

void Barista::setName(const std::string& newName) { name = newName; }

// Behavior
void Barista::prepareOrder(Order* order) {
    if (!order) {
        std::cout << "| No order to prepare. |" << std::endl;
        return;
    }

    if (order->getIsPrepared()) {
        std::cout << "| Order already prepared. |" << std::endl;
        return;
    }

    order->markPrepared();
    std::cout << "| Order ID " << order->getOrderId() << " has been prepared by "  << name << ". |" << std::endl;
}

void Barista::greetCustomer() const {
    std::cout << "| Hello! I'm " << name << ", your barista today. |" << std::endl;
}
