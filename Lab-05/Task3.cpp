#include <iostream>
#include <string>
using namespace std;
struct Coach {
    int number;
    string type;
    int capacity, passengers;
    Coach *next, *prev;
    Coach(int n, const string& t, int c, int p)
        : number(n), type(t), capacity(c), passengers(p), next(this), prev(this) {}
};
class Train {
    Coach* head = nullptr;      
    Coach* current = nullptr;  

    static void print(const Coach* c) {
        cout << "  Coach " << c->number << " | " << c->type
             << " | Capacity: " << c->capacity
             << " | Passengers: " << c->passengers
             << " | Free: " << c->capacity - c->passengers << "\n";
    }
    Coach* find(int num) const {
        if (!head) return nullptr;
        Coach* p = head;
        do {
            if (p->number == num) return p;
            p = p->next;
        } while (p != head);
        return nullptr;
    }
public:
    ~Train() {
        if (!head) return;
        head->prev->next = nullptr;            
        while (head) { Coach* n = head->next; delete head; head = n; }
    }
    bool addCoach(int num, const string& type, int cap, int pass) {
        if (find(num)) { cout << "Coach " << num << " already exists.\n"; return false; }
        Coach* c = new Coach(num, type, cap, pass);
        if (!head) { head = current = c; return true; }
        Coach* tail = head->prev;
        c->next = head;  c->prev = tail;
        tail->next = c;  head->prev = c;
        return true;
    }
    bool insertAfter(int afterNum, int num, const string& type, int cap, int pass) {
        Coach* a = find(afterNum);
        if (!a) { cout << "Coach " << afterNum << " not found.\n"; return false; }
        if (find(num)) { cout << "Coach " << num << " already exists.\n"; return false; }
        Coach* c = new Coach(num, type, cap, pass);
        c->next = a->next;
        c->prev = a;
        a->next->prev = c;
        a->next = c;
        return true;
    }
    void removeCoach(int num) {
        Coach* c = find(num);
        if (!c) { cout << "Coach " << num << " not found.\n"; return; }
        if (c->next == c) {                     // only coach
            head = current = nullptr;
        } else {
            c->prev->next = c->next;
            c->next->prev = c->prev;
            if (head == c) head = c->next;
            if (current == c) current = c->next; // next valid coach becomes current
        }
        delete c;
        cout << "Coach " << num << " removed.\n";
    }
    void moveForward()  { if (current) current = current->next; else cout << "Train is empty.\n"; }
    void moveBackward() { if (current) current = current->prev; else cout << "Train is empty.\n"; }
    void displayClockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* p = head;
        do { print(p); p = p->next; } while (p != head);
    }
    void displayAntiClockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* p = head->prev;                  // start from last coach
        do { print(p); p = p->prev; } while (p != head->prev);
    }
    void search(int num) const {
        Coach* c = find(num);
        if (c) { cout << "Found:\n"; print(c); }
        else cout << "Coach " << num << " not found.\n";
    }
    void maxAvailable() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* best = head;
        Coach* p = head->next;
        while (p != head) {
            if (p->capacity - p->passengers > best->capacity - best->passengers) best = p;
            p = p->next;
        }
        cout << "Coach with most empty seats (" << best->capacity - best->passengers << "):\n";
        print(best);
    }
    void displayCurrent() const {
        if (!current) { cout << "Train is empty.\n"; return; }
        print(current);
    }
    void reverse() {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* oldTail = head->prev;
        Coach* p = head;
        do {
            Coach* nxt = p->next;           
            p->next = p->prev;
            p->prev = nxt;
            p = nxt;
        } while (p != head);
        head = oldTail;
        cout << "Train direction reversed.\n";
    }
};
static void readCoach(int& num, string& type, int& cap, int& pass) {
    while (true) {
        cout << "Coach Number: "; cin >> num; cin.ignore();
        cout << "Coach Type: "; getline(cin, type);
        cout << "Passenger Capacity: "; cin >> cap;
        cout << "Current Passengers: "; cin >> pass;
        if (cap >= 0 && pass >= 0 && pass <= cap) return;
        cout << "Invalid: passengers must be between 0 and capacity. Re-enter.\n";
    }
}
int main() {
    Train train;
    int n;
    cout << "Number of coaches: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int num, cap, pass; string type;
        cout << "Coach " << i + 1 << ":\n";
        readCoach(num, type, cap, pass);
        train.addCoach(num, type, cap, pass);
    }
    int choice;
    do {
        cout << "\n--- Train Coach Navigation ---\n"
                "1. Add Coach\n2. Insert Coach After\n3. Remove Coach\n4. Move Forward\n"
                "5. Move Backward\n6. Display Clockwise\n7. Display Anti-clockwise\n"
                "8. Search Coach\n9. Max Available Capacity\n10. Display Current Coach\n"
                "11. Reverse Train Direction\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        int num, cap, pass, after; string type;
        switch (choice) {
            case 1: readCoach(num, type, cap, pass); train.addCoach(num, type, cap, pass); break;
            case 2:
                cout << "Insert after coach number: "; cin >> after;
                readCoach(num, type, cap, pass);
                train.insertAfter(after, num, type, cap, pass);
                break;
            case 3: cout << "Coach Number: "; cin >> num; train.removeCoach(num); break;
            case 4: train.moveForward(); break;
            case 5: train.moveBackward(); break;
            case 6: train.displayClockwise(); break;
            case 7: train.displayAntiClockwise(); break;
            case 8: cout << "Coach Number: "; cin >> num; train.search(num); break;
            case 9: train.maxAvailable(); break;
            case 10: train.displayCurrent(); break;
            case 11: train.reverse(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}