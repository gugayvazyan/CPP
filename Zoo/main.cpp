#include "Zoo.hpp"
#include <iostream>
#include <vector>

int main() {
    Lion* simba = new Lion("Simba");
    Tiger* tony = new Tiger("Tony");
    Eagle* aquila = new Eagle("Aquila");
    Snake* sly = new Snake("Sly");

    std::vector<Animal*> animals;

    animals.push_back(simba);
    animals.push_back(tony);
    animals.push_back(aquila);
    animals.push_back(sly);

    std::cout << "=== ANIMAL INFO ===" << std::endl;
    for (Animal* a : animals) {
        a->PrintInfo();
        std::cout << "---" << std::endl;
    }
    std::cout << "\n=== FEEDING TIME ===" << std::endl;
    for (Animal* a : animals) { a->Feed(); }

    std::cout << "\n=== SPECIAL ABILITIES ===" << std::endl;
    simba->Roar();
    tony->Jump();
    aquila->Soar();
    sly->Hiss();

    return 0;


}