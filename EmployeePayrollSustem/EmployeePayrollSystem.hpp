#pragma once
#include <iostream>
#include <vector>
#include <string>

class Employee;

// ---------- Counter ----------
class Counter {
    inline static int id = 0;
    friend class Employee;
};

// ---------- Role ----------
enum class Role { Intern, Junior, Middle, Senior };

// ---------- Departament ----------
class Departament {
    std::vector<Employee*> employees;
public:
    Departament() = default;
    void add_employee(Employee* e);
    int calculate_total();
    ~Departament() = default;
};

// ---------- Employee ----------
class Employee {
protected:
    std::string name;
    int id;
    int projects;
    int exp;
    int salary;
    Role role;
public:
    Employee(std::string _name, int _projects, int _exp, Role _role);
    virtual void calculate_salary();
    virtual void print_info();
    std::string get_name() const;
    int get_id() const;
    int get_projects() const;
    int get_exp() const;
    int get_salary() const;
    Role get_role() const;
    virtual ~Employee() = default;
};

// ---------- Intern ----------
class Intern : public Employee {
    Employee* mentor;
public:
    Intern(std::string _name, int _projects, int _exp, Role _role, Employee* _mentor);
    void calculate_salary() override;
    void print_info() override;
};

// ---------- Junior ----------
class Junior : public Employee {
    Employee* team_lead;
public:
    Junior(std::string _name, int _projects, int _exp, Role _role, Employee* _team_lead);
    void print_info() override;
    void calculate_salary() override;
    virtual ~Junior() = default;
};

// ---------- Middle ----------
class Middle : public Employee {
    Employee* team_lead;
public:
    Middle(std::string _name, int _projects, int _exp, Role _role, Employee* _team_lead);
    void calculate_salary() override;
    void print_info() override;
    virtual ~Middle() = default;
};

// ---------- Senior ----------
class Senior : public Employee {
    std::vector<Employee*> subord;
public:
    Senior(std::string _name, int _projects, int _exp, Role _role, std::vector<Employee*> _subord);
    void calculate_salary() override;
    void print_info() override;
    virtual ~Senior() = default;
};
