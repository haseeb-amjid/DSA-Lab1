
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;
struct Song {
    int    id;
    string name;
    int    minutes;
    int    seconds;
    Song*  prev;
    Song*  next;
    Song(int i, const string& n, int m, int s)
        : id(i), name(n), minutes(m), seconds(s), prev(nullptr), next(nullptr) {}
};
class Playlist {
private:
    Song* head;      
    Song* tail;    
    Song* current;   
    int   count;      
    static void printSong(const Song* s) {
        cout << "  ID: " << s->id << " | Name: " << s->name << " | Duration: "
             << s->minutes << ":" << (s->seconds < 10 ? "0" : "") << s->seconds << "\n";
    }
    Song* find(int id) const {
        for (Song* p = head; p != nullptr; p = p->next)
            if (p->id == id) return p;
        return nullptr;
    }
public:
    Playlist() : head(nullptr), tail(nullptr), current(nullptr), count(0) {}
    ~Playlist() {
        Song* p = head;
        while (p != nullptr) {
            Song* nxt = p->next;
            delete p;
            p = nxt;
        }
    }    
    Playlist(const Playlist&) = delete;
    Playlist& operator=(const Playlist&) = delete;
    bool addSong(int id, const string& name, int m, int s) {
        if (find(id) != nullptr) {
            cout << "A song with ID " << id << " already exists.\n";
            return false;
        }
        if (m < 0 || s < 0 || s > 59) {
            cout << "Invalid duration (minutes >= 0, seconds 0-59).\n";
            return false;
        }
        Song* node = new Song(id, name, m, s);
        if (head == nullptr) {
            head = tail = node;
        } else {
            node->prev = tail;
            tail->next = node;
            tail = node;
        }
        count++;
        cout << "Song added.\n";
        return true;
    }
    bool deleteSong(int id) {
        Song* node = find(id);
        if (node == nullptr) {
            cout << "Song with ID " << id << " not found.\n";
            return false;
        }
        if (current == node)
            current = (node->next != nullptr) ? node->next : node->prev;
        if (node->prev != nullptr) node->prev->next = node->next;
        else                       head = node->next;
        if (node->next != nullptr) node->next->prev = node->prev;
        else                       tail = node->prev;

        delete node;
        count--;
        cout << "Song deleted.\n";
        return true;
    }
    void displayForward() const {
        if (head == nullptr) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (first to last):\n";
        for (Song* p = head; p != nullptr; p = p->next) printSong(p);
    }
    void displayBackward() const {
        if (tail == nullptr) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (last to first):\n";
        for (Song* p = tail; p != nullptr; p = p->prev) printSong(p);
    }
    void searchSong(int id) const {
        Song* s = find(id);
        if (s == nullptr) cout << "Song with ID " << id << " not found.\n";
        else { cout << "Song found:\n"; printSong(s); }
    }
    void playNext() {
        if (head == nullptr) { cout << "Playlist is empty.\n"; return; }
        if (current == nullptr)            current = head;      // first play
        else if (current->next != nullptr) current = current->next;
        else { cout << "Already at the last song.\n"; }
        cout << "Now playing:\n"; printSong(current);
    }
    void playPrevious() {
        if (tail == nullptr) { cout << "Playlist is empty.\n"; return; }
        if (current == nullptr)            current = tail;      // first play
        else if (current->prev != nullptr) current = current->prev;
        else { cout << "Already at the first song.\n"; }
        cout << "Now playing:\n"; printSong(current);
    }
    void reverse() {
        if (head == nullptr || head == tail) {
            cout << "Playlist reversed.\n";
            return;
        }
        Song* p = head;
        while (p != nullptr) {
            Song* nxt = p->next;     
            p->next = p->prev;      
            p->prev = nxt;
            p = nxt;                  
        }
        Song* tmp = head;            
        head = tail;
        tail = tmp;
        cout << "Playlist reversed.\n";
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
        cout << "Invalid input, enter a number.\n";
    }
}
static bool readDuration(int& m, int& s) {
    string line;
    cout << "Enter duration (mm:ss): ";
    getline(cin, line);
    char colon = 0;
    istringstream in(line);
    if (in >> m >> colon >> s && colon == ':') return true;
    cout << "Invalid duration format. Use mm:ss.\n";
    return false;
}
static void showMenu() {
    cout << "\n===== PLAYLIST MENU =====\n"
         << "1. Add Song\n2. Delete Song\n3. Display Playlist Forward\n"
         << "4. Display Playlist Backward\n5. Search Song\n"
         << "6. Play Next Song\n7. Play Previous Song\n"
         << "8. Reverse Playlist\n0. Exit\n";
}
int main() {
    Playlist playlist;
    int choice;
    do {
        showMenu();
        choice = readInt("Enter choice: ");
        switch (choice) {
        case 1: {
            int id = readInt("Enter song ID: ");
            string name;
            cout << "Enter song name: ";
            getline(cin, name);
            int m, s;
            if (readDuration(m, s)) playlist.addSong(id, name, m, s);
            break;
        }
        case 2: playlist.deleteSong(readInt("Enter song ID to delete: ")); break;
        case 3: playlist.displayForward();  break;
        case 4: playlist.displayBackward(); break;
        case 5: playlist.searchSong(readInt("Enter song ID to search: ")); break;
        case 6: playlist.playNext();     break;
        case 7: playlist.playPrevious(); break;
        case 8: playlist.reverse();      break;
        case 0: cout << "Goodbye!\n";    break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;   
}