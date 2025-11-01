#ifndef ZOO_HPP
#define ZOO_HPP

#include "Type.hpp"

class Lion : public Mammal {
    int roarPower;
public:
    Lion();
    Lion(std::string aname, int power = 5);
    void Roar();
    void PrintInfo() const;
};

class Tiger : public Mammal {
    double jumpHeight;
public:
    Tiger();
    Tiger(std::string aname, double jmp = 3.5);
    void Jump();
    void PrintInfo() const;
};

class Elephant : public Mammal {    
    double trunkLength;
public:
    Elephant();
    Elephant(std::string aname);
    void UseTrunk();
    void PrintInfo() const;
};

class Eagle : public Bird {
    double visionRange;
public:
    Eagle();
    Eagle(std::string aname, double vision = 100.0);
    void Soar();
    void PrintInfo() const;
};

class Parrot : public Bird {
    std::vector<std::string> vocabulary;
public:
    Parrot();
    Parrot(std::string aname, std::vector<std::string> words);
    void Speak();
    void PrintInfo() const;
};

class Snake : public Reptile {
    bool poisonous;
public:
    Snake();
    Snake(std::string aname, bool poison = false);
    void Hiss();
    void PrintInfo() const;
};

class Crocodile : public Reptile {
    int biteForce;
public:
    Crocodile();
    Crocodile(std::string aname, int force = 5);
    void Snap();
    void PrintInfo() const;
};

#endif