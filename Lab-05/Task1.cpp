#include <iostream>
#include <string>
using namespace std;
struct Tab {
    int id;
    string title, url;
    Tab *next, *prev;
    Tab(int i, const string& t, const string& u)
        : id(i), title(t), url(u), next(this), prev(this) {}
};
class TabManager {
    Tab* current = nullptr;

    static void print(const Tab* t) {
        cout << "  [ID " << t->id << "] " << t->title << " - " << t->url << "\n";
    }
public:
    ~TabManager() {
        if (!current) return;
        current->prev->next = nullptr;          
        while (current) { Tab* n = current->next; delete current; current = n; }
    }
    void openTab(int id, const string& title, const string& url) {
        Tab* t = new Tab(id, title, url);
        if (!current) { current = t; return; }  
        t->next = current->next;
        t->prev = current;
        current->next->prev = t;
        current->next = t;
        current = t;                           
    }
    void closeCurrent() {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* victim = current;
        if (victim->next == victim) {          
            current = nullptr;
        } else {
            victim->prev->next = victim->next;
            victim->next->prev = victim->prev;
            current = victim->next;            
        }
        delete victim;
    }
    void moveNext() { if (current) current = current->next; else cout << "No tabs open.\n"; }
    void movePrev() { if (current) current = current->prev; else cout << "No tabs open.\n"; }
    void displayCurrent() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        print(current);
    }
    void displayForward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* p = current;
        do { print(p); p = p->next; } while (p != current);
    }
    void displayBackward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* p = current;
        do { print(p); p = p->prev; } while (p != current);
    }
    void search(int id) const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* p = current;
        do {
            if (p->id == id) { cout << "Found:\n"; print(p); return; }
            p = p->next;
        } while (p != current);
        cout << "Tab " << id << " not found.\n";
    }
};

int main() {
    TabManager tm;
    int choice;
    do {
        cout << "\n--- Browser Tab Manager ---\n"
                "1. Open New Tab\n2. Close Current Tab\n3. Move Next\n4. Move Previous\n"
                "5. Display Current Tab\n6. Display All Forward\n7. Display All Backward\n"
                "8. Search Tab\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        switch (choice) {
            case 1: {
                int id; string title, url;
                cout << "Tab ID: "; cin >> id; cin.ignore();
                cout << "Title: "; getline(cin, title);
                cout << "URL: "; getline(cin, url);
                tm.openTab(id, title, url);
                break;
            }
            case 2: tm.closeCurrent(); break;
            case 3: tm.moveNext(); break;
            case 4: tm.movePrev(); break;
            case 5: tm.displayCurrent(); break;
            case 6: tm.displayForward(); break;
            case 7: tm.displayBackward(); break;
            case 8: { int id; cout << "Tab ID: "; cin >> id; tm.search(id); break; }
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}