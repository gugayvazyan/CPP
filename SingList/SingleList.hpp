#include <iostream>
#include <vector>

class SingleList {
private:
    struct Node {
        int val;
        Node* next;
        Node() =default;
        Node(int _val): val{_val}, next{nullptr} {}
        Node(int _val,Node* _next): val{_val}, next{_next} {}
    };
    Node* head;
    //helpers
    void clear();
    void copy(const SingleList& list);
public:
    // Constructors & Destructor
    SingleList();
    SingleList(size_t count, int val);
    SingleList(std::initializer_list<int> init);
    SingleList(const SingleList& list);
    SingleList(SingleList&& list);
    ~SingleList();

    // Prefix
    SingleList& operator++ ();
    SingleList operator++ (int);
    SingleList& operator-- ();
    SingleList operator-- (int);

    // Assignment Operators
    SingleList& operator=(const SingleList& list);
    SingleList& operator=(SingleList&& list);

    // Operator Overloading
    SingleList operator+(const SingleList& list);
    SingleList operator+=(SingleList& list);
    SingleList& operator+=(const SingleList& list);
    
    // Bools
    bool operator==(const SingleList& list) const;
    bool operator!=(const SingleList& list) const;
    bool operator!() const;
    
    // Read/Write
    int& operator[](size_t index);

    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const SingleList& r);
    friend std::istream& operator>>(std::istream& is, SingleList& r);

    // Push/Pop
    void push_back(int val);
    void push_front(int val);
    void pop_back();
    void pop_front();

    // Helpers
    int size() const;
    
    explicit operator std::vector<int>() const;
    explicit operator bool() const;
};
