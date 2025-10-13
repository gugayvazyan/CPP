#include "Rational.hpp"


void Rational::reduce_helper() {
    int gcd = std::gcd(num, denum);
    num /= gcd;
    denum /= gcd;
}
//Constructors
Rational::Rational(int _num, int _denum) : num{_num}, denum{_denum} {
    if (denum == 0) {
        std::cout << "Denum cant be 0" << std::endl;
        std::exit;
    }

    if(denum < 0) {
        denum = -denum;
        num = -num;
    }  
    reduce_helper();
}

Rational::Rational(Rational&& obj) : num{obj.num}, denum{obj.denum} {
        obj.num = 0;
        obj.denum = 0;
}

Rational& Rational::operator=(const Rational& obj) {
    if(this == &obj) return *this;

    this->denum = obj.denum;
    this->num = obj.num;

    return *this;
}

Rational& Rational::operator=(Rational&& obj) {
    if (this == &obj) return *this;
    
    this->num = obj.num;
    this->denum = obj.denum;

    obj.num = 0;
    obj.denum = 1;

    return *this;
}


//Unary operators
Rational Rational::operator+(){
    return *this;
}

Rational Rational:: operator-(){
    return Rational(-num, denum);
}

Rational& Rational::operator++(){
    this->num += this->denum;
    reduce_helper();
    return *this;
}

Rational Rational::operator++(int) {
    Rational obj1(num, denum);
    this->num += this->denum;
    return obj1;
}

Rational &Rational::operator--(){
    this->num -= this->denum;
    return *this;
}

Rational Rational::operator--(int) {
    Rational obj1(num, denum);
    this->num -= this->denum;
    return obj1;
}

bool Rational::operator!() const {
    return num == 0;
}


//Binary arithmetic operators
    //Member
Rational &Rational::operator+= (const Rational& obj){
    int lcm = std::lcm(this->denum, obj.denum);

    int tmp = lcm / this->denum;
    int tmp1 = lcm / obj.denum;

    this->num = this->num * tmp + obj.num * tmp1;
    this->denum = lcm;

    reduce_helper();
    return *this; 
}

Rational &Rational::operator-= (const Rational& obj){
    int lcm = std::lcm(this->denum, obj.denum);

    int tmp = lcm / this->denum;
    int tmp1 = lcm / obj.denum;

    this->num = this->num * tmp - obj.num * tmp1;
    this->denum = lcm;

    reduce_helper();
    return *this; 
}

Rational &Rational::operator*= (const Rational& obj) {
    this->num *= obj.num;
    this->denum *= obj.denum;

    reduce_helper();

    return *this;
}
Rational &Rational::operator/= (const Rational& obj) {
     if (num == 0) {
        std::cout << "Num cant be 0" << std::endl;
        std::exit;
    }
    
    this->num *= obj.denum;
    this->denum *= obj.num;

    
    int gcd = std::gcd(this->num, this->denum);
    this->num /= gcd;
    this->denum /= gcd;

    
    if (this->denum < 0) {
        this->denum = -this->denum;
        this->num = -this->num;
    }

    return *this;
}

//Non-member
Rational operator+(Rational lhs, const Rational& rhs) {
    lhs += rhs;
    return lhs;
    
}

Rational operator-(Rational lhs, const Rational& rhs){
    if(lhs.denum == rhs.denum) {
        Rational obj(lhs.num - rhs.num, lhs.denum);
        return obj;
    }

    Rational obj(lhs.num, lhs.denum);
    int lcm = std::lcm(obj.denum, rhs.denum);

    int tmp = lcm / obj.denum;
    int tmp1 = lcm / rhs.denum;

    obj.num = obj.num * tmp - rhs.num * tmp1;
    obj.denum = lcm;
    
    int gcd = std::gcd(obj.num, obj.denum);
    obj.num /= gcd;
    obj.denum /= gcd;

    return obj; 
}

Rational operator*(Rational lhs, const Rational& rhs){
    Rational tmp(lhs.num, lhs.denum);
    tmp.num *= rhs.num;
    tmp.denum *= rhs.denum;

    int gcd = std::gcd(tmp.num, tmp.denum);
    tmp.num /= gcd;
    tmp.denum /= gcd;

      if (tmp.denum < 0) {
        tmp.denum =  -tmp.denum ;
        tmp.num = -tmp.num;
    }

    return tmp;
}
Rational operator/(Rational lhs, const Rational& rhs){
    Rational tmp(lhs.num, lhs.denum);
    tmp.num *= rhs.denum;
    tmp.denum *= rhs.num;

    int gcd = std::gcd(tmp.num, tmp.denum);
    tmp.num /= gcd;
    tmp.denum /= gcd;

      if (tmp.denum < 0) {
        tmp.denum =  -tmp.denum ;
        tmp.num = -tmp.num;
    }

    return tmp;
}

// Comparison operators
bool operator==(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 == tmp2;
}

bool operator!=(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 != tmp2;
}
bool operator<(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 < tmp2;
}
bool operator<=(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 <= tmp2;
}
bool operator>(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 > tmp2;
}
bool operator>=(const Rational& lhs, const Rational& rhs){
    float tmp1 = lhs.num / lhs.denum;
    float tmp2 = rhs.num / rhs.denum;
    return tmp1 >= tmp2;
}

//Stream operators
std::ostream& operator<<(std::ostream& ost, const Rational& r) {
    ost << "Rational: " << r.num << "/" << r.denum << std::endl;
    return ost;
}

std::istream& operator>>(std::istream& is, Rational& r) {
    is >> r.num;
    is >> r.denum;
    return is;
}

//Accessors
int Rational::numerator() const {
    return this->num;
}
int Rational::denominator() const {
    return this->denum;
}

//Optional conversions
Rational::operator double() const {
    return (double)this->num / (double)this->denum;

}
