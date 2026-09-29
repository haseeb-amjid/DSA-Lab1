#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;
struct BitNode {
    int      bit;
    BitNode* next;
    BitNode* prev;
    BitNode(int b) : bit(b), next(nullptr), prev(nullptr) {}
};

class BinaryDLL {
private:
    BitNode* head;   
    BitNode* tail;   
    int      size; 
    void pushBack(int b) {
        BitNode* n = new BitNode(b);
        if (!tail) head = tail = n;
        else { n->prev = tail; tail->next = n; tail = n; }
        size++;
    }
    void pushFront(int b) {
        BitNode* n = new BitNode(b);
        if (!head) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        size++;
    }
    void popFront() {
        if (!head) return;
        BitNode* old = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete old;
        size--;
    }
    void padToBlock() {
        if (size == 0) pushFront(0);
        while (size % 8 != 0) pushFront(0);
    }
    void normalize() {
        int lz = 0;
        for (BitNode* p = head; p && p->bit == 0; p = p->next) lz++;
        int removable = (lz / 8) * 8;
        if (removable > size - 8) removable = size - 8;
        for (int i = 0; i < removable; i++) popFront();
    }
public:
    BinaryDLL() : head(nullptr), tail(nullptr), size(0) {}
    BinaryDLL(const BinaryDLL& o) : head(nullptr), tail(nullptr), size(0) {
        for (BitNode* p = o.head; p; p = p->next) pushBack(p->bit);
    }
    BinaryDLL& operator=(const BinaryDLL& o) {
        if (this != &o) {
            clear();
            for (BitNode* p = o.head; p; p = p->next) pushBack(p->bit);
        }
        return *this;
    }
    ~BinaryDLL() { clear(); 
    }
    void clear() {
        while (head) popFront();
    }
    bool empty() const { return size == 0; }
    bool store(const string& s) {
        string bits;
        for (char c : s) {
            if (c == ' ') continue;                   
            if (c != '0' && c != '1') return false;
            bits += c;
        }
        if (bits.empty()) return false;
        clear();
        for (char c : bits) pushBack(c - '0');
        padToBlock();
        return true;
    }
    BinaryDLL onesComplement() const {
        BinaryDLL r(*this);
        for (BitNode* p = r.head; p; p = p->next) p->bit ^= 1;
        return r;
    }
    static BinaryDLL add(const BinaryDLL& a, const BinaryDLL& b, bool growCarry = true) {
        BinaryDLL r;
        BitNode* p = a.tail;
        BitNode* q = b.tail;
        int carry = 0;
        while (p || q) {
            int sum = carry + (p ? p->bit : 0) + (q ? q->bit : 0);
            r.pushFront(sum % 2);
            carry = sum / 2;
            if (p) p = p->prev;
            if (q) q = q->prev;
        }
        if (carry && growCarry) r.pushFront(1);
        r.padToBlock();
        return r;
    }
    BinaryDLL twosComplement() const {
        BinaryDLL one;
        for (int i = 0; i < size - 1; i++) one.pushBack(0);
        one.pushBack(1);                            
        return add(onesComplement(), one, false);
    }
    static BinaryDLL multiply(const BinaryDLL& a, const BinaryDLL& b) {
        BinaryDLL product;
        product.pushBack(0);
        product.padToBlock();
        BinaryDLL shifted(a);
        for (BitNode* m = b.tail; m; m = m->prev) {
            if (m->bit == 1) product = add(product, shifted);
            shifted.pushBack(0);                   
            shifted.padToBlock();
        }
        product.normalize();
        return product;
    }
    string toDecimal() const {
        vector<int> digits(1, 0);                     
        for (BitNode* p = head; p; p = p->next) {
            int carry = p->bit;
            for (size_t i = 0; i < digits.size(); i++) {
                int v = digits[i] * 2 + carry;
                digits[i] = v % 10;
                carry = v / 10;
            }
            if (carry) digits.push_back(carry);
        }
        string s;
        for (size_t i = digits.size(); i > 0; i--) s += char('0' + digits[i - 1]);
        return s;
    }
    void display(const string& label) const {
        cout << label << ": ";
        if (!head) { cout << "(not set)\n"; return; }
        int i = 0;
        for (BitNode* p = head; p; p = p->next, i++) {
            if (i > 0 && i % 8 == 0) cout << ' ';
            cout << p->bit;
        }
        cout << "\n";
    }
};
static int readInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); return x; }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input.\n";
    }
}
static void readBinary(BinaryDLL& num, const string& name) {
    string s;
    cout << "Enter binary number for " << name << ": ";
    getline(cin, s);
    if (num.store(s)) num.display(name);
    else cout << "Invalid input: use only 0 and 1.\n";
}
static void showMenu() {
    cout << "\n===== BINARY ARITHMETIC MENU =====\n"
         << "1. Store binary number A\n2. Store binary number B\n"
         << "3. 1's complement of A\n4. 2's complement of A\n"
         << "5. A + B\n6. A x B\n7. Convert A to decimal\n"
         << "8. Convert B to decimal\n0. Exit\n";
}
int main() {
    BinaryDLL A, B;
    int choice;
    do {
        showMenu();
        choice = readInt("Enter choice: ");
        bool needA = (choice >= 3 && choice <= 7);
        bool needB = (choice == 5 || choice == 6 || choice == 8);
        if ((needA && A.empty()) || (needB && B.empty())) {
            cout << "Please store the required number(s) first.\n";
            continue;
        }
        switch (choice) {
        case 1: readBinary(A, "A"); break;
        case 2: readBinary(B, "B"); break;
        case 3: A.onesComplement().display("1's complement of A"); break;
        case 4: A.twosComplement().display("2's complement of A"); break;
        case 5: BinaryDLL::add(A, B).display("A + B"); break;
        case 6: BinaryDLL::multiply(A, B).display("A x B"); break;
        case 7: cout << "A in decimal: " << A.toDecimal() << "\n"; break;
        case 8: cout << "B in decimal: " << B.toDecimal() << "\n"; break;
        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}