// test_main.cpp
#include "EmployeePayrollSystem.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

struct Result {
    int total_checks = 0;
    int failed = 0;
    vector<string> messages;
    void add_check(bool ok, const string &msg) {
        ++total_checks;
        if (!ok) {
            ++failed;
            messages.push_back(msg);
        }
    }
};

string role_to_short(Role r) {
    switch (r) {
        case Role::Intern: return "Intern";
        case Role::Junior: return "Junior";
        case Role::Middle: return "Middle";
        case Role::Senior: return "Senior";
    }
    return "Unknown";
}

void capture_print_info(function<void()> f, string &out) {
    // capture cout
    ostringstream ss;
    auto *oldbuf = cout.rdbuf(ss.rdbuf());
    f();
    cout.rdbuf(oldbuf);
    out = ss.str();
}

int expected_salary_for(Role role, int projects, int exp, size_t subord_size) {
    switch (role) {
        case Role::Intern: return 250000; // expected convention
        case Role::Junior: return projects * 100000;
        case Role::Middle: return (exp * 50000) + (projects * 150000);
        case Role::Senior: return (int)(subord_size * 100000) + (exp * 150000) + (projects * 500000);
    }
    return 0;
}

int main() {
    Result res;

    // We'll create multiple combinations to exceed 400 checks.
    // For Junior/Middle/Intern we vary projects 0..4 and exp 0..4.
    // For Senior we add subord sizes 0..3.
    vector<Role> roles = {Role::Intern, Role::Junior, Role::Middle, Role::Senior};

    // We'll create a small pool of base employees to use as mentors/teams/subords.
    vector<Employee*> pool;
    pool.push_back(new Junior("TL_A", 1, 3, Role::Junior, nullptr));
    pool.push_back(new Middle("TL_B", 2, 4, Role::Middle, pool.size() ? pool[0] : nullptr));
    pool.push_back(new Intern("Mentor_X", 0, 0, Role::Intern, pool.size() ? pool[0] : nullptr));

    // Ensure calculate_salary exists for the ones in the pool:
    for (Employee* e : pool) {
        e->calculate_salary();
    }

    // test combinations
    for (Role role : roles) {
        for (int projects = 0; projects <= 4; ++projects) {
            for (int exp = 0; exp <= 4; ++exp) {

                // For seniors, test several subordinate sizes; for others just one variant (mentor/teamlead).
                int max_subord = (role == Role::Senior) ? 3 : 0;
                for (int subsz = 0; subsz <= max_subord; ++subsz) {

                    // create appropriate object dynamically
                    Employee* e = nullptr;
                    string name = role_to_short(role) + "_P" + to_string(projects) + "_E" + to_string(exp) + (subsz ? ("_S" + to_string(subsz)) : "");
                    if (role == Role::Intern) {
                        e = new Intern(name, projects, exp, role, pool[0]);
                    } else if (role == Role::Junior) {
                        e = new Junior(name, projects, exp, role, pool[0]);
                    } else if (role == Role::Middle) {
                        e = new Middle(name, projects, exp, role, pool[1]);
                    } else { // Senior
                        vector<Employee*> sub;
                        // fill subordinates from pool (repeat as needed)
                        for (int k = 0; k < subsz; ++k) sub.push_back(pool[k % pool.size()]);
                        e = new Senior(name, projects, exp, role, sub);
                    }

                    // Basic getters checks
                    res.add_check(e->get_name() == name, "name mismatch: expected '" + name + "' got '" + e->get_name() + "'");
                    res.add_check(e->get_projects() == projects, "projects getter mismatch for " + name);
                    res.add_check(e->get_exp() == exp, "exp getter mismatch for " + name);

                    // initial salary -- by header we set salary initially 0 in base constructor
                    int initial_salary = e->get_salary();
                    res.add_check(initial_salary == 0, "initial salary not zero for " + name + " (got " + to_string(initial_salary) + ")");

                    // print_info should contain name and role word (we'll capture)
                    string out;
                    capture_print_info([&](){ e->print_info(); }, out);
                    res.add_check(out.find(name) != string::npos, "print_info missing name for " + name);
                    // role may be printed as "Role: X." or "Role: X" or as separate derived printing — check contains role string
                    string rshort = role_to_short(role);
                    res.add_check(out.find(rshort) != string::npos || out.find(("Role: " + rshort)) != string::npos,
                                  "print_info missing role string (" + rshort + ") for " + name + " -- output: " + out);

                    // calculate_salary and salary correctness
                    size_t subord_size = 0;
                    if (role == Role::Senior) {
                        // we can find subordinate count by searching "Subordinates" lines or compute from type-cast
                        // but we already know subsz
                        subord_size = subsz;
                    }
                    // apply
                    e->calculate_salary();
                    int expected = expected_salary_for(role, projects, exp, subord_size);
                    int salar = e->get_salary();
                    res.add_check(salar == expected,
                                  "salary mismatch for " + name + ": expected " + to_string(expected) + ", got " + to_string(salar));

                    // print_info should contain "Salary" or the numeric value
                    capture_print_info([&](){ e->print_info(); }, out);
                    // we accept either printed numeric or "Salary"
                    bool foundSalary = (out.find("Salary") != string::npos) || (out.find(to_string(salar)) != string::npos);
                    res.add_check(foundSalary, "print_info did not show salary for " + name + " -- output: " + out);

                    // free
                    delete e;
                } // subsz
            } // exp
        } // projects
    } // role

    // Now test Departament aggregation with a larger set to ensure calculate_total works.
    Departament dept;
    vector<Employee*> created;
    // create 30 mixed employees
    for (int i = 0; i < 30; ++i) {
        Role r = roles[i % roles.size()];
        int p = i % 5;
        int ex = (i / 5) % 5;
        Employee* emp = nullptr;
        if (r == Role::Intern) emp = new Intern("Dep_In_" + to_string(i), p, ex, r, pool[0]);
        else if (r == Role::Junior) emp = new Junior("Dep_Jn_" + to_string(i), p, ex, r, pool[0]);
        else if (r == Role::Middle) emp = new Middle("Dep_Md_" + to_string(i), p, ex, r, pool[1]);
        else {
            vector<Employee*> s;
            s.push_back(pool[0]);
            if (i % 2 == 0) s.push_back(pool[1]);
            emp = new Senior("Dep_Sr_" + to_string(i), p, ex, r, s);
        }
        emp->calculate_salary();
        created.push_back(emp);
        dept.add_employee(emp);
    }

    // compute expected total
    int expected_total = 0;
    for (Employee* e : created) expected_total += e->get_salary();

    int dept_total = dept.calculate_total();
    res.add_check(dept_total == expected_total,
                  "Departament total mismatch: expected " + to_string(expected_total) + ", got " + to_string(dept_total));

    // Cleanup created employees and pool
    for (Employee* e : created) delete e;
    for (Employee* e : pool) delete e;

    // Summary
    cout << "Total checks performed: " << res.total_checks << endl;
    if (res.failed == 0) {
        cout << "✅ All tests passed" << endl;
        return 0;
    } else {
        cout << "❌ Failed checks: " << res.failed << endl;
        // print up to first 200 failures for brevity
        size_t print_count = min(res.messages.size(), (size_t)200);
        for (size_t i = 0; i < print_count; ++i) {
            cout << i+1 << ") " << res.messages[i] << endl;
        }
        if (res.messages.size() > print_count) {
            cout << "... and " << (res.messages.size() - print_count) << " more failures." << endl;
        }
        return 2;
    }
}
