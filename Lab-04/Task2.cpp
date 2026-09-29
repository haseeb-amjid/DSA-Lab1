#include <iostream>
#include <vector>
#include <limits>
using namespace std;
struct Person {
    int     id;
    Person* next;
    Person(int i) : id(i), next(nullptr) {}
};
Person* createCircle(int n) {
    if (n < 1) return nullptr;
    Person* head = new Person(1);
    Person* tail = head;
    for (int i = 2; i <= n; i++) {
        tail->next = new Person(i);
        tail = tail->next;
    }
    tail->next = head;            
    return head;
}
void displayCircle(Person* head) {
    if (head == nullptr) return;
    Person* p = head;
    do {
        cout << p->id << " ";
        p = p->next;
    } while (p != head);
    cout << "\n";
}
int eliminate(Person* head, int n, int k, vector<int>& order) {
    Person* prev = head;
    while (prev->next != head) prev = prev->next;
    Person* cur = head;
    int remaining = n;
    while (remaining > 1) {
        int steps = (k - 1) % remaining;
        for (int i = 0; i < steps; i++) {
            prev = cur;
            cur = cur->next;
        }
        order.push_back(cur->id);      
        prev->next = cur->next;        
        Person* victim = cur;
        cur = cur->next;              
        delete victim;              
        remaining--;
    }
    int survivor = cur->id;
    delete cur;                      
    return survivor;
}
void displayEliminated(const vector<int>& order) {
    cout << "Order of elimination: ";
    for (size_t i = 0; i < order.size(); i++)
        cout << order[i] << (i + 1 < order.size() ? " -> " : "");
    cout << "\n";
}
void displaySurvivor(int id) {
    cout << "Survivor: Person " << id << "\n";
}
static int readPositive(const char* prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= 1) return x;
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter an integer >= 1.\n";
    }
}
int main() {
    int n = readPositive("Enter number of people (N): ");
    int k = readPositive("Enter step count (k): ");
    Person* head = createCircle(n);
    cout << "\nInitial circle: ";
    displayCircle(head);
    vector<int> order;
    int survivor = eliminate(head, n, k, order);
    cout << "\n";
    displayEliminated(order);
    displaySurvivor(survivor);
    return 0;
}