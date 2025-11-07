
#include "EmployeePayrollSystem.hpp"
#include <iostream>

// ---------------- Departament ----------------

int Departament::calculate_total() {
    int total = 0;
    for (Employee* e : employees) {
        total += e->get_salary();
    }
    return total;
}

void Departament::add_employee(Employee* e) {
    employees.push_back(e);
}


// ---------------- Employee ----------------

Employee::Employee(std::string _name, int _projects, int _exp, Role _role)
    : name(_name), projects(_projects), exp(_exp), role(_role), salary(0) {}

void Employee::print_info() {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Years worked: " << exp << std::endl;
    std::cout << "Projects: " << projects << std::endl;

    switch (role) {
        case Role::Intern: std::cout << "Role: Intern." << std::endl; break;
        case Role::Junior: std::cout << "Role: Junior." << std::endl; break;
        case Role::Middle: std::cout << "Role: Middle." << std::endl; break;
        case Role::Senior: std::cout << "Role: Senior." << std::endl; break;
        default: std::cout << "Unknown role." << std::endl; break;
    }
}
void Employee::calculate_salary() {
    salary = 0;
}
std::string Employee::get_name() const { return name; }
int Employee::get_id() const { return id; }
int Employee::get_projects() const { return projects; }
int Employee::get_exp() const { return exp; }
int Employee::get_salary() const { return salary; }
Role Employee::get_role() const { return role; }



// ---------------- Intern ----------------

Intern::Intern(std::string _name, int _projects, int _exp, Role _role, Employee* _mentor)
    : Employee(_name, _projects, _exp, _role), mentor(_mentor) {}

void Intern::print_info() {
    Employee::print_info();
    std::cout << "Mentor: " << mentor->get_name() << std::endl;
    std::cout << "Salary: " << salary << std::endl;
}

void Intern::calculate_salary() {
    salary = 250000;
}


// ---------------- Junior ----------------

Junior::Junior(std::string _name, int _projects, int _exp, Role _role, Employee* _team_lead)
    : Employee(_name, _projects, _exp, _role), team_lead(_team_lead) {}

void Junior::print_info() {
    Employee::print_info();
    std::cout << "Team lead: " << team_lead->get_name() << std::endl;
    std::cout << "Salary: " << salary << std::endl;
}

void Junior::calculate_salary() {
    salary = projects * 100000;
}


// ---------------- Middle ----------------

Middle::Middle(std::string _name, int _projects, int _exp, Role _role, Employee* _team_lead)
    : Employee(_name, _projects, _exp, _role), team_lead(_team_lead) {}

void Middle::print_info() {
    Employee::print_info();
    std::cout << "Team lead: " << team_lead->get_name() << std::endl;
    std::cout << "Salary: " << salary << std::endl;
}

void Middle::calculate_salary() {
    salary = (exp * 50000) + (projects * 150000);
}


// ---------------- Senior ----------------

Senior::Senior(std::string _name, int _projects, int _exp, Role _role, std::vector<Employee*> _subord)
    : Employee(_name, _projects, _exp, _role), subord(_subord) {}

void Senior::print_info() {
    Employee::print_info();
    std::cout << "Subordinates: " << std::endl;
    for (Employee* e : subord) {
        std::cout << " - " << e->get_name() << std::endl;
    }
    std::cout << "Salary: " << salary << std::endl;
}

void Senior::calculate_salary() {
    salary = (subord.size() * 100000) + (exp * 150000) + (projects * 500000);
}
