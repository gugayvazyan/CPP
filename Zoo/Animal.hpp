#include <iostream>
#include <vector>

enum class Kind {
    Animal, Mammal, Bird, Reptile,
    Lion, Tiger, Elephant,
    Eagle, Parrot,
    Snake, Crocodile
};

class Animal;

class Counter{ 
    inline static int id; 
    friend class Animal;
};

class Animal{
protected:
    std::string name;
    size_t health;     // 0..100
    size_t hunger;     // 0..100
    Kind _kind;
    int id;
public:
    Animal();
    Animal(std::string aname);
    void PrintInfo() const;
    void Feed();
    Kind KindOf() const;
    int Id() const;
};