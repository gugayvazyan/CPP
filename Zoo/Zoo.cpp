#include "Zoo.hpp"

// Animal
Animal::Animal() : health(100), hunger(0), name("Unknown"), _kind(Kind::Animal) { 
    this->id = ++Counter::id; 
} 

Animal::Animal(std::string aname) : health(100), hunger(0), name(aname), _kind(Kind::Animal) { 
    this->id = ++Counter::id; 
}

void Lion::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Mamal" << std::endl;
    std::cout << "Animal species: Lion" << std::endl;
    std::cout << "Roar" << roarPower << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Tiger::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Mamal" << std::endl;
    std::cout << "Animal species: Tiger" << std::endl;
    std::cout << "Jump Height: " << jumpHeight << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Elephant::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Mamal" << std::endl;
    std::cout << "Animal species: Elephant" << std::endl;
    std::cout << "Trunk length: " << trunkLength << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Snake::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Reptile" << std::endl;
    std::cout << "Animal species: Snake" << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Crocodile::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Reptile" << std::endl;
    std::cout << "Animal species: Crocodile" << std::endl;
    std::cout << "Bite force " << biteForce << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Eagle::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Bird" << std::endl;
    std::cout << "Animal species: Eagle" << std::endl;
    std::cout << "VisionRange " << visionRange << std::endl;
    std::cout << "Wingspan" << wingSpan << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Parrot::PrintInfo() const{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Animal class: Bird" << std::endl;
    std::cout << "Animal species: Parrot" << std::endl;
    std::cout << "Wingspan" << wingSpan << std::endl;
    std::cout << "Hunger: " << hunger << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "id: " << id << std::endl;
}

void Animal::PrintInfo() const {
    switch (KindOf()) {
        case Kind::Lion:
            static_cast<const Lion* const>(this)->PrintInfo();
            break;

        case Kind::Elephant:
            static_cast<const Elephant* const>(this)->PrintInfo();
            break;

        case Kind::Tiger:
            static_cast<const Tiger* const>(this)->PrintInfo();
            break;

        case Kind::Eagle:
            static_cast<const Eagle* const>(this)->PrintInfo();
            break;

        case Kind::Parrot:
            static_cast<const Parrot* const>(this)->PrintInfo();
            break;

        case Kind::Snake:
            static_cast<const Snake* const>(this)->PrintInfo();
            break;

        case Kind::Crocodile:
            static_cast<const Crocodile* const>(this)->PrintInfo();
            break;
        default:
            std::cout << "Unknown kind" << std::endl;
        break;
    }
}

void Animal::Feed() {
    if(hunger == 0 || health == 100) {
        std::cout << "Animal is not hungry!" << std::endl;
        return;
    }
    std::cout << "Nom. Nom. Nom..." << std::endl;
    hunger = 0;
    health = 100;
}

Kind Animal::KindOf() const { return _kind; }
int Animal::Id() const { return this->id; }

// Mammal
Mammal::Mammal() : Animal() { 
    _kind = Kind::Mammal;
}

Mammal::Mammal(std::string aname) : Animal(aname) { 
    _kind = Kind::Mammal;
}

void Mammal::makesound() { std::cout << "Brrr" << std::endl; }

// Bird
Bird::Bird() : Animal() { 
    _kind = Kind::Bird;
    wingSpan = 75.4;
}

Bird::Bird(std::string aname, double wS) : Animal(aname) { 
    _kind = Kind::Bird;
    wingSpan = wS;
}

void Bird::Fly() { std::cout << "Whoosh. Swoosh. Whoosh... " << std::endl; }

// Reptile
Reptile::Reptile() : Animal() { 
    _kind = Kind::Reptile;
}

Reptile::Reptile(std::string aname) : Animal(aname) { 
    _kind = Kind::Reptile;
}

void Reptile::Sunbathe() { std::cout << "Nice. Nice. Nice...." << std::endl; }

// Lion 
Lion::Lion() : Mammal() { 
    _kind = Kind::Lion;
    roarPower = 5;
}

Lion::Lion(std::string aname, int power) : Mammal(aname) {
    _kind = Kind::Lion;
    roarPower = power;
}

void Lion::Roar() { 
    for(int i = 0; i < roarPower; ++i) {
        std::cout << "Roar. ";
    }
    std::cout << std::endl;
}

// Tiger 
Tiger::Tiger() : Mammal() { 
    _kind = Kind::Tiger;
    jumpHeight = 3.5;
}

Tiger::Tiger(std::string aname, double jmp) : Mammal(aname) {
    _kind = Kind::Tiger;
    jumpHeight = jmp;
}

void Tiger::Jump() { std::cout << "Juuuuump..." << std::endl; }

// Elephant
Elephant::Elephant() : Mammal() { 
    _kind = Kind::Elephant;
    trunkLength = 2.4;
}

Elephant::Elephant(std::string aname) : Mammal(aname) { 
    _kind = Kind::Elephant;
    trunkLength = 2.4;
}

void Elephant::UseTrunk() { std::cout << "Frrrrnnnn" << std::endl; }

// Eagle
Eagle::Eagle() : Bird() { 
    _kind = Kind::Eagle;
    visionRange = 100.0;
}

Eagle::Eagle(std::string aname, double vision) : Bird(aname) { 
    _kind = Kind::Eagle;
    visionRange = vision;
}

void Eagle::Soar() { std::cout << "Soaring..." << std::endl; }

// Parrot
Parrot::Parrot() : Bird() { 
    _kind = Kind::Parrot;
    vocabulary = {"Hello", "Pretty bird"};
}

Parrot::Parrot(std::string aname, std::vector<std::string> words) : Bird(aname) { 
    _kind = Kind::Parrot;
    vocabulary = words;
}

void Parrot::Speak() {
    if (vocabulary.empty()) {
        std::cout << name << " mmmmm..." << std::endl;
        return;
    }
    std::cout << name << " says: ";
    for (const std::string& word : vocabulary) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}

// Snake
Snake::Snake() : Reptile() { 
    _kind = Kind::Snake;
    poisonous = false;
}

Snake::Snake(std::string aname, bool poison) : Reptile(aname) { 
    _kind = Kind::Snake;
    poisonous = poison;
}

void Snake::Hiss() {
    if (poisonous) std::cout << "Fshhhhhh...";
    else std::cout << "Sssssss...";
    std::cout << std::endl;
}

// Crocodile
Crocodile::Crocodile() : Reptile() { 
    _kind = Kind::Crocodile;
    biteForce = 5;
}

Crocodile::Crocodile(std::string aname, int force) : Reptile(aname) {
    _kind = Kind::Crocodile;
    biteForce = force;
}

void Crocodile::Snap() {
    for(int i = 0; i < biteForce; ++i) {
        std::cout << "SNAP! ";
    }
    std::cout << std::endl;
}