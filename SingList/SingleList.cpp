#include "SingleList.hpp"

//Helpers (private)
void SingleList::clear() {
    Node* tmp;
    while (head) {
        tmp = head;    
        head = head->next;  
        delete tmp;    
    }
    head = nullptr;    
}
void SingleList::copy(const SingleList& list) {
    Node* tmp = list.head;
    Node dummy;
    dummy.next = nullptr;
    Node* tmp2 = &dummy;

    while (tmp) {
        tmp2->next = new Node(tmp->val);
        tmp2 = tmp2->next;
        tmp = tmp->next;
    }

    this->head = dummy.next;
}

// Constructors & Destructor
SingleList::SingleList() : head{nullptr} {}
SingleList::SingleList(size_t count, int val = 0) {
    Node dummy;
    Node* tmp = &dummy;
    for(int i = 0; i < count; ++i) {
        tmp->next = new Node(val);
        tmp = tmp->next;
    }
    this->head = dummy.next;
}
SingleList::SingleList(std::initializer_list<int> init) {
    Node dummy;
    Node* tmp = &dummy;
    for(int val : init) {
        tmp->next = new Node(val);
        tmp = tmp->next;
    }
    this->head = dummy.next;
}
SingleList::SingleList(const SingleList& list) : head(nullptr) {
    copy(list);
    
}
SingleList::SingleList(SingleList&& list) {
    head = list.head;
    list.head = nullptr;
}
SingleList::~SingleList() {
    Node* tmp = head;
    while(tmp) {
        Node* tmp2 = tmp->next;
        delete tmp;
        tmp = tmp2;
    }
}


// Prefix/Postfix
SingleList& SingleList::operator++ () {
    push_back(0);
    return *this;
}
SingleList SingleList::operator++ (int) {
    SingleList old(*this);
    push_back(0); 
    return old; 
}
SingleList& SingleList::operator-- () {
    pop_back();
    return *this;
}
SingleList SingleList::operator-- (int) {
    SingleList old(*this);
    pop_back();
    return old;
}

// Assignment Operators
SingleList& SingleList::operator=(const SingleList& list) {
    if (this == &list) return *this;

    clear();

    Node dummy;
    dummy.next = nullptr;
    Node* tmp = &dummy;
    Node* tmp2 = list.head;

    while (tmp2) {
        tmp->next = new Node(tmp2->val);
        tmp = tmp->next;
        tmp2 = tmp2->next;
    }

    this->head = dummy.next;
    return *this;
}
SingleList& SingleList::operator=(SingleList&& list) {
    if (this == &list) return *this;

    clear();
    
    head = list.head;
    list.head = nullptr;
    return *this;
}

// Operator Overloading
SingleList SingleList::operator+(const SingleList& list) {
    SingleList temp;

    Node* tmp = this->head;
    while (tmp) {
        temp.push_back(tmp->val);
        tmp = tmp->next;
    }

    tmp = list.head;
    while (tmp) {
        temp.push_back(tmp->val);
        tmp = tmp->next;
    }

    return temp;
}
SingleList SingleList::operator+=(SingleList& list) {
    if (!list.head) return *this;

    if (!head) head = list.head;
    else {
        Node* tmp = head;
        while(tmp->next) tmp = tmp->next;
        tmp->next = list.head;
    }
    list.head = nullptr;
    return *this;

}
SingleList& SingleList::operator+=(const SingleList& list) {
    Node* tmp = list.head;
    while (tmp) {
        push_back(tmp->val);
        tmp = tmp->next;
    }
    return *this;
}

// Bools
bool SingleList::operator==(const SingleList& list) const {
    Node* tmp1 = this->head;
    Node* tmp2 = list.head;

    while (tmp1 && tmp2) {
        if (tmp1->val != tmp2->val) return false;
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }

    
    return tmp1 == nullptr && tmp2 == nullptr;
}
bool SingleList::operator!=(const SingleList& list) const {
    Node* tmp1 = this->head;
    Node* tmp2 = list.head;

    while (tmp1 && tmp2) {
        if (tmp1->val != tmp2->val) return true;
        tmp1 = tmp1->next;
        tmp2 = tmp2->next;
    }

    return tmp1 != tmp2;
}
bool SingleList::operator!() const { return head == nullptr; }

// Read/Write
int& SingleList::operator[](size_t index) {
    Node* tmp = head;
    size_t i = 0;

    while (tmp) {
        if (i == index) return tmp->val;
        tmp = tmp->next;
        ++i;
    }

    std::cout << "Invalid index" << std::endl;
    std::exit;
    
}

//Stream operators
std::ostream& operator<<(std::ostream& ost, const SingleList& list) {
    SingleList::Node* tmp = list.head;
    while(tmp) {
        ost << tmp->val << " ";
        tmp = tmp->next;
    }
    return ost;
}
std::istream& operator>>(std::istream& is, SingleList& list) {
    int val;
    list.clear();

    while (is >> val) {
        list.push_back(val);
    }

    return is;
}

// Push/Pop
void SingleList::push_back(int val) {
    if (!head) {
        head = new Node(val);
    } else {
        Node* tmp = head;
        while(tmp->next) tmp = tmp->next;
        tmp->next = new Node(val);
    }
}
void SingleList::push_front(int val) {
    if (!head) {
        head = new Node(val);
    } else {
        Node* tmp = head;
        head = new Node(val);
        head->next = tmp;
    }
}
void SingleList::pop_back() {
    if (!head) return;  

    if (!head->next) {
        delete head;
        head = nullptr;
        return;
    }

    if (head) {
        Node* tmp = head;
        while (tmp->next->next) tmp = tmp->next;
        
        delete tmp->next;
        tmp->next = nullptr;   
    }
}
void SingleList::pop_front() {
    if (!head) return;

    Node* tmp = head;
    head = head->next;
    delete tmp;
}

// Helpers
int SingleList::size() const {
    if (!head) return 0;

    Node* tmp = head;
    int count = 0;

    while (tmp) {
        tmp = tmp->next;
        ++count;
    }

    return count;
}

// Type conversion
SingleList::operator std::vector<int>() const {
    std::vector<int> vec;
    Node* tmp = head;
    while (tmp) {
        vec.push_back(tmp->val);
        tmp = tmp->next;
    }
    return vec;
}
SingleList::operator bool() const {
    return head != nullptr;
}
