#include <iostream>
#include <numeric>


class Rational {
   private:
    int num;
    int denum;

    void reduce_helper();

    public:
        //Constructors
        Rational() : num(0), denum(1) {}
        Rational(int num): num(num), denum(1) {}
        Rational(int num, int denum);
        Rational(const Rational& obj) : num(obj.num), denum(obj.denum) {}
        Rational(Rational&& obj);
        
        Rational& operator=(const Rational& obj);
        Rational& operator=(Rational&& obj);

        ~Rational() =default;

        //Unary operators
        Rational operator+();
        Rational operator-();
        Rational& operator++ ();
        Rational operator++ (int);
        Rational& operator-- ();
        Rational operator-- (int);
        bool operator!() const;

        //Binary arithmetic operators
            //Member
        Rational& operator+=(const Rational& obj);
        Rational& operator-=(const Rational& obj);
        Rational& operator*=(const Rational& obj);
        Rational& operator/=(const Rational& obj);

            //Non-member
        friend Rational operator+(Rational lhs, const Rational& rhs);
        friend Rational operator-(Rational lhs, const Rational& rhs);
        friend Rational operator*(Rational lhs, const Rational& rhs);
        friend Rational operator/(Rational lhs, const Rational& rhs);
        // Comparison operators
        friend bool operator==(const Rational& lhs, const Rational& rhs);
        friend bool operator!=(const Rational& lhs, const Rational& rhs);
        friend bool operator<(const Rational& lhs, const Rational& rhs);
        friend bool operator<=(const Rational& lhs, const Rational& rhs);
        friend bool operator>(const Rational& lhs, const Rational& rhs);
        friend bool operator>=(const Rational& lhs, const Rational& rhs);

        //Stream operators
        friend std::ostream& operator<<(std::ostream& os, const Rational& r);
        friend std::istream& operator>>(std::istream& is, Rational& r);

        // Accessors
        int numerator() const;
        int denominator() const;

        explicit operator double() const;

};