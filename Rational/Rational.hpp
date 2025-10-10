#include <iostream>
#include <numeric>


class Rational {
    private:
        int num;
        int denum;

    public:
        //Constructors
        Rational():num(0),denum(1) {}
        Rational(int num): num(num), denum(1) {}
        Rational(int num, int denum): num(num), denum(denum) {}
        Rational(Rational& obj): num(obj.num), denum(obj.denum) {}
        Rational(Rational&& obj): num(obj.num), denum(obj.denum) {}
        
        Rational& operator=(const Rational& obj);
        
        Rational& operator=(Rational&& obj) noexcept;
        ~Rational() =default;
    public:
        //Unary operators
        Rational& operator+ (const Rational& obj);
        Rational& operator- (const Rational& obj);
        Rational& operator++ ();
        Rational operator++ (int);
        Rational& operator-- ();
        Rational operator-- (int);
        bool operator!() const;
    public:
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
    public: 
        // Comparison operators
        friend bool operator==(const Rational& lhs, const Rational& rhs);
        friend bool operator!=(const Rational& lhs, const Rational& rhs);
        friend bool operator<(const Rational& lhs, const Rational& rhs);
        friend bool operator<=(const Rational& lhs, const Rational& rhs);
        friend bool operator>(const Rational& lhs, const Rational& rhs);
        friend bool operator>=(const Rational& lhs, const Rational& rhs);
    public:
        //Stream operators
        friend std::ostream& operator<<(std::ostream& os, const Rational& r);
        friend std::istream& operator>>(std::istream& is, Rational& r);
    public: 
        // Accessors
        int numerator() const noexcept;
        int denominator() const noexcept;
    public:
        explicit operator double() const;




};
