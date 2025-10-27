#include <vector>
#include <iostream>

class Drink;
class Customer;
class Order;
class Barista;

class CoffeeShop {
    private:
    std::string shopName;
    std::vector<Drink*> menu;
    std::vector<Barista*> baristas; // Aggregation
    std::vector<Customer*> queue;   // Association
    
    public:
    // ==For gpt test ==
    const std::vector<Drink*>& getMenu() const { return menu; }
    const std::vector<Barista*>& getBaristas() const { return baristas; }
    const std::vector<Customer*>& getQueue() const { return queue; }
    // ==================
    CoffeeShop() = default;
    CoffeeShop(const std::string& name);
    std::string getName() const;
    void setName(const std::string& newName);
    
    void addDrink(Drink* drink);
    void removeDrink(const std::string& drinkName);
    void showMenu() const;
    
    void hireBarista(Barista* barista);
    void fireBarista(Barista* barista);
    void showBaristas() const;
    
    void addCustomerToQueue(Customer* customer);
    void serveNextCustomer();
    void showQueue() const;
    void constructMenu();
    
    void displayShopInfo() const;
};

class Order {
private:
    int orderId{0};
    std::vector<Drink> drinks;
    double totalPrice{0.0};
    bool isPrepared{false};
    bool isPaid{false};
    
public:
    Order() =default;
    Order(int id);
    
    int getOrderId() const;
    double getTotalPrice() const;
    bool getIsPrepared() const;
    bool getIsPaid() const;
    
    void addDrink(const Drink& drink);
    void removeDrink(size_t index);
    void calculateTotalPrice();
    
    void markPrepared();
    void markPaid();
    
    void displayOrder() const;
};

class Drink {
public:
    enum class Size { Small, Medium, Big };

private:
    std::string name;
    Size volume;
    double price;

public:
    // Constructors
    Drink() = default;
    Drink(const std::string& name, Size volume, double price);

    // Getters
    std::string getName() const;
    Size getVolume() const;
    double getPrice() const;

    // Setters
    void setName(const std::string& newName);
    void setVolume(Size newVolume);
    void setPrice(double newPrice);

    // Display
    void display() const;
};

class Customer {
private:
    std::string name;
    double balance;
    Order* currentOrder;

public:
    Customer() = default;
    Customer(const std::string& name, double balance);


    std::string getName() const;
    double getBalance() const;
    void setBalance(double newBalance);

    void placeOrder(Order* order);
    void payOrder();
    void viewOrder() const;

    Order* getCurrentOrder() const;
    bool hasOrder() const;

    void display() const;
};

class Barista {
private:
    std::string name;
    
public:
    // Constructors
    Barista() = default;
    Barista(const std::string& name);
    
    // Getters/Setters
    std::string getName() const;
    void setName(const std::string& newName);
    
    // Behavior
    void prepareOrder(Order* order); // dependency
    void greetCustomer() const;
};
