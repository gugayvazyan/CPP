#include "Rational.hpp"

//Constructors
Rational::Rational(int num, int denum) {
    if (denum == 0) std::cout << "Denum cant be 0" <<  std::exit;

    if(denum < 0 && num > 0) {
        denum = -denum;
        num = -num;
    }  
    int gcd = (num < denum) ? num : denum;
    while(num % gcd != 0 && denum % gcd != 0 ) {
        --gcd;
    }
    num /= gcd;
    denum /= gcd;
    this->denum = denum;
    this->num = num;
}
Rational::Rational(Rational&& obj){
        obj.num = 0;
        obj.denum = 0;
}
Rational &Rational :: operator= (const Rational& obj) {
    if(this == &obj) {return *this;}

    this->denum = obj.denum;
    this-> num = obj.num;

    return *this;
}
Rational& Rational::operator=(Rational&& obj) noexcept {
    if (this == &obj) {
        return *this;
    }
    this->num = obj.num;
    this->denum = obj.denum;

    obj.num = 0;
    obj.denum = 1;

    return *this;
}


//Unary operators
Rational &Rational:: operator+ (const Rational& obj){
        Rational obj1(obj.num, obj.denum);
        return obj1; 
}
Rational &Rational:: operator- (const Rational& obj){
    Rational obj1(obj.num, obj.denum);
    obj1.num *= -1;
    return obj1;
}
Rational &Rational::operator++(){
    this->num += this->denum;
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
    
    int gcd = std::gcd(this->num, this->denum);
    this->num /= gcd;
    this->denum /= gcd;

    return *this; 
}
Rational &Rational::operator-= (const Rational& obj){
    int lcm = std::lcm(this->denum, obj.denum);

    int tmp = lcm / this->denum;
    int tmp1 = lcm / obj.denum;

    this->num = this->num * tmp - obj.num * tmp1;
    this->denum = lcm;

    int gcd = std::gcd(this->num, this->denum);
    this->num /= gcd;
    this->denum /= gcd;

    return *this; 
}
Rational &Rational::operator*= (const Rational& obj) {
    if (this->denum == 0 || obj.denum == 0 ) std::cout << "Denum cant be 0" <<  std::exit;
    this->num *= obj.num;
    this->denum *= obj.denum;

    int gcd = std::gcd(this->num, this->denum);
    this->num /= gcd;
    this->denum /= gcd;

      if (this->denum < 0) {
        this->denum =  -this->denum ;
        this->num = -this->num;
    }

    return *this;
}
Rational &Rational::operator/= (const Rational& obj) {
    if (obj.num == 0) std::cout << "Num cant be 0" <<  std::exit;

    
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
Rational operator+(Rational lhs, const Rational& rhs){
    if(lhs.denum == rhs.denum) {
        Rational obj(lhs.num + rhs.num, lhs.denum);
        return obj;
    }

    Rational obj(lhs.num, lhs.denum);
    int lcm = std::lcm(obj.denum, rhs.denum);

    int tmp = lcm / obj.denum;
    int tmp1 = lcm / rhs.denum;

    obj.num = obj.num * tmp + rhs.num * tmp1;
    obj.denum = lcm;
    
    int gcd = std::gcd(obj.num, obj.denum);
    obj.num /= gcd;
    obj.denum /= gcd;

    return obj; 
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
    if (lhs.denum == 0 || rhs.denum == 0 ) std::cout << "Denum cant be 0" <<  std::exit;
    
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
    if (lhs.denum == 0 || rhs.denum == 0 ) std::cout << "Denum cant be 0" <<  std::exit;
    
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
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 == tmp2;
}
bool operator!=(const Rational& lhs, const Rational& rhs){
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 != tmp2;
}
bool operator<(const Rational& lhs, const Rational& rhs){
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 < tmp2;
}
bool operator<=(const Rational& lhs, const Rational& rhs){
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 <= tmp2;
}
bool operator>(const Rational& lhs, const Rational& rhs){
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 > tmp2;
}
bool operator>=(const Rational& lhs, const Rational& rhs){
    int tmp1 = lhs.num / lhs.denum;
    int tmp2 = rhs.num / rhs.denum;
    return tmp1 >= tmp2;
}

//Stream operators
std::ostream& operator<<(std::ostream& os, const Rational& r) {
    os << "Rational: " << r.num << "/" << r.denum;
    return os;
}
std::istream& operator>>(std::istream& is, Rational& r) {
    is >> r.num;
    is >> r.denum;
    return is;
}

//Accessors
int Rational::numerator() const noexcept {
    return this->num;
}
int Rational::denominator() const noexcept {
    return this->denum;
}

//Optional conversions
Rational::operator double() const {
    return this->num / this->denum;
}
