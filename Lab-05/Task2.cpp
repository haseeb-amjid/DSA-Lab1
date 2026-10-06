#include <iostream>
#include <string>
using namespace std;
struct Photo {
    int id;
    string name, date, location;
    Photo *next, *prev;
    Photo(int i, const string& n, const string& d, const string& l)
        : id(i), name(n), date(d), location(l), next(this), prev(this) {}
};
class Album {
    Photo* head = nullptr;
    Photo* current = nullptr;

    static void print(const Photo* p) {
        cout << "  [ID " << p->id << "] " << p->name << " | " << p->date
             << " | " << p->location << "\n";
    }
    void unlink(Photo* p) {                   
        if (p->next == p) {
            head = current = nullptr;
        } else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            if (head == p) head = p->next;
            if (current == p) current = p->next;
        }
        delete p;
    }
public:
    ~Album() {
        if (!head) return;
        head->prev->next = nullptr;
        while (head) { Photo* n = head->next; delete head; head = n; }
    }
    void addPhoto(int id, const string& n, const string& d, const string& l) {
        Photo* p = new Photo(id, n, d, l);
        if (!head) { head = current = p; return; }
        Photo* tail = head->prev;
        p->next = head;  p->prev = tail;
        tail->next = p;  head->prev = p;
    }
    void insertAfterCurrent(int id, const string& n, const string& d, const string& l) {
        if (!current) { addPhoto(id, n, d, l); return; }
        Photo* p = new Photo(id, n, d, l);
        p->next = current->next;
        p->prev = current;
        current->next->prev = p;
        current->next = p;
    }
    void removeById(int id) {
        if (!head) { cout << "Album is empty.\n"; return; }
        Photo* p = head;
        do {
            if (p->id == id) { unlink(p); cout << "Photo " << id << " removed.\n"; return; }
            p = p->next;
        } while (p != head);
        cout << "Photo " << id << " not found.\n";
    }
    void removeCurrent() {
        if (!current) { cout << "Album is empty.\n"; return; }
        unlink(current);
    }
    void moveNext() { if (current) current = current->next; else cout << "Album is empty.\n"; }
    void movePrev() { if (current) current = current->prev; else cout << "Album is empty.\n"; }
    void displayForward() const {
        if (!current) { cout << "Album is empty.\n"; return; }
        Photo* p = current;
        do { print(p); p = p->next; } while (p != current);
    }
    void displayBackward() const {
        if (!current) { cout << "Album is empty.\n"; return; }
        Photo* p = current;
        do { print(p); p = p->prev; } while (p != current);
    }
    void search(int id) const {
        if (!head) { cout << "Album is empty.\n"; return; }
        Photo* p = head;
        do {
            if (p->id == id) { cout << "Found:\n"; print(p); return; }
            p = p->next;
        } while (p != head);
        cout << "Photo " << id << " not found.\n";
    }
    int count() const {
        if (!head) return 0;
        int c = 0; Photo* p = head;
        do { c++; p = p->next; } while (p != head);
        return c;
    }
};
static void readPhoto(int& id, string& n, string& d, string& l) {
    cout << "Photo ID: "; cin >> id; cin.ignore();
    cout << "Name: "; getline(cin, n);
    cout << "Date Taken: "; getline(cin, d);
    cout << "Location: "; getline(cin, l);
}
int main() {
    Album album;
    int n;
    cout << "Number of photos: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int id; string nm, d, l;
        cout << "Photo " << i + 1 << ":\n";
        readPhoto(id, nm, d, l);
        album.addPhoto(id, nm, d, l);
    }
    int choice;
    do {
        cout << "\n--- Circular Photo Album ---\n"
                "1. Add Photo (end)\n2. Insert After Current\n3. Remove Photo by ID\n"
                "4. Remove Current Photo\n5. Move Next\n6. Move Previous\n"
                "7. Display Forward\n8. Display Backward\n9. Search Photo\n"
                "10. Count Photos\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        int id; string nm, d, l;
        switch (choice) {
            case 1: readPhoto(id, nm, d, l); album.addPhoto(id, nm, d, l); break;
            case 2: readPhoto(id, nm, d, l); album.insertAfterCurrent(id, nm, d, l); break;
            case 3: cout << "Photo ID: "; cin >> id; album.removeById(id); break;
            case 4: album.removeCurrent(); break;
            case 5: album.moveNext(); break;
            case 6: album.movePrev(); break;
            case 7: album.displayForward(); break;
            case 8: album.displayBackward(); break;
            case 9: cout << "Photo ID: "; cin >> id; album.search(id); break;
            case 10: cout << "Total photos: " << album.count() << "\n"; break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}