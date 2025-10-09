#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    char name[50];
    int age;
    double height;
} Person;

// Constructors
void Person_ctor(Person* const this){
	this->age = 0;
	this->height = 0.0;
    strcpy(this->name, "Unknown");
}

void Person_ctor_i(Person* const this, char* n, int a, double h){
	this->age = a;
	this->height = h;
    strcpy(this->name, n);
}

// Getters
const char* getName(const Person* const this) { return this->name; }
const int getAge(const Person* const this) { return this->age; }
const double getHeight(const Person* const this) { return this->height; }

void setName(Person* const this,const char* n) { strcpy(this->name, n); }
void setAge(Person* const this,int a) { this->age = a; }
void setHeight(Person* const this,double h) { this->height = h; }

// Overloaded member functions
void printInfo(const Person* const this) { printf("Name: %s, Age: %d, Height: %0.2f\n", this->name, this->age, this->height); }

void printInfo_b(const Person* const this,bool showHeight) {
    printf("Name: %s, Age: %d", this->name, this->age);
    if(showHeight) printf(", Height: %0.2f", this->height);
    printf("\n");
}

void Person_dtor(Person* const this) {}

int main() {
    Person p1;
    Person_ctor_i(&p1,"Bob", 19, 1.75);

    printInfo(&p1);
    printInfo_b(&p1,false);

    setAge(&p1,20);
    
    printf("Updates age: %d\n", getAge(&p1));
    return 0;
    Person_dtor(&p1);
}
