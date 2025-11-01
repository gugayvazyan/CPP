#ifndef TYPE_HPP
#define TYPE_HPP

#include "Animal.hpp"

class Mammal : public Animal{
protected:
    bool warmBlooded = true;
public:
    Mammal();
    Mammal(std::string aname);
    void makesound();
};

class Bird : public Animal {
protected:
    double wingSpan;
public:
    Bird();
    Bird(std::string aname, double wS = 75.4);
    void Fly();    
};

class Reptile : public Animal{
protected:
    bool coldBlooded = true;
public:
    Reptile();
    Reptile(std::string name);
    void Sunbathe(); 
};

#endif