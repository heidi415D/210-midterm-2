#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

const int MIN_NR = 10, MAX_NR = 99, MIN_LS = 5, MAX_LS = 20;

class DoublyLinkedList {
private:
    struct Node {
        int data;
        Node* prev;
        Node* next;
        Node(int val, Node* p = nullptr, Node* n = nullptr) {
            data = val; 
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(int value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    void delete_val(int value) {
        if (!head) return;

        Node* temp = head;
        
        while (temp && temp->data != value)
            temp = temp->next;

        if (!temp) return; 

        if (temp->prev)
            temp->prev->next = temp->next;
        else
            head = temp->next; 

        if (temp->next)
            temp->next->prev = temp->prev;
        else
            tail = temp->prev; 

        delete temp;
    }

    void delete_pos(int pos) {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            pop_front();
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            pop_back();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }

    void push_back(int v) {
        Node* newNode = new Node(v);
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    
    void push_front(int v) {
        Node* newNode = new Node(v);
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    
    void pop_front() {

        if (!head) {
            cout << "List is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        delete temp;
    }

    void pop_back() {
        if (!tail) {
            cout << "List is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            head = tail = nullptr;
        }
        delete temp;
    }
    
        ~DoublyLinkedList() {
        // clean up all nodes
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // node-walking helpers 
    bool empty() const { return head == nullptr; }

    int size() const {
        int count = 0;
        for (Node* current = head; current != nullptr; current = current->next)
            count++;
        return count;
    }

    void print_names(const vector<string>& names) const {
        if (!head) {
            cout << "List is empty." << endl;
            return;
        }
        Node* cur = head;
        while (cur) {
            int idx = cur->data;
            if (idx >= 0 && idx < (int)names.size())
                cout << names[idx];
            else
                cout << "(?)";
            if (cur->next) cout << " ";
            cur = cur->next;
        }
        cout << endl;
    }

    void print() {
        Node* current = head;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void print_reverse() {
        Node* current = tail;
        if (!current) {
            cout << "List is empty." << endl;
            return;
        }
        while (current) {
            cout << current->data << " ";
            current = current->prev;
        }
        cout << endl;
    }
}; // class close




int main() {
    srand(time(0)); // seed random number generator

    // read all names from file into vector
    ifstream fin("names.txt");
    vector<string> names;
    string nm;
    while (fin >> nm)
        names.push_back(nm);
    fin.close();

        // quick sample so we know file loaded
    cout << "Loaded " << names.size() << " names.\n";
    for (int i = 0; i < 5 && i < (int)names.size(); ++i)
        cout << names[i] << (i+1 < 5 ? " " : "");
    cout << "\n";

    // guard, if the file was empty exit out
    if (names.empty()) {
        cout << "No names loaded. Exiting.\n";
        return 0;
    }

    DoublyLinkedList line;

    cout << "\nStore opens:\n";

    // initial 5 people join the line
    for (int i = 0; i < 5; ++i) {
        // range pattern random in [MIN_NR, MAX_NR]
        int r = rand() % (MAX_NR - MIN_NR + 1) + MIN_NR;
        // map to a valid index into names
        int idx = r % (int)names.size();
        cout << names[idx] << " joins the line.\n";
        line.push_back(idx); // store index of the name
    }
    cout << "Current line: ";
    line.print_names(names); // show names instead of raw indices


    // sim 20 min
    for (int minute = 1; minute <= 20; ++minute) {
        cout << "\nMinute " << minute << ":\n";

        // generate random probabilities for each event
        int pServe = rand() % 100 + 1;
        int pJoin = rand() % 100 + 1;
        int pRear = rand() % 100 + 1;
        int pAny = rand() % 100 + 1;
        int pVIP = rand() % 100 + 1;

        // independent "if" so multiple events can occur

        // EVENT 1: 40% chance to serve front
        if (pServe <= 40) {
            line.delete_pos(0); // serve front
            cout << "   served front\n";
        }

        // EVENT 2: 60% chance to have a new person join rear
        if (pJoin <= 60) {
            int idx = rand() % 100;
            line.push_back(idx);
            cout << " " << names[idx] << " joined rear\n";
        }

        // EVENT 3: 20% chance rear person leaves mad
        if (pRear <= 20;) {
            line.pop_back();
            cout << "  rear left (mad)\n"; 
        }

        // EVENT 4: 10% chance random person leaves
        if (pAny <= 10 && line.size() >= 1) {
            // choose random position between 2 and n-1
            int n = line.size();
            int pos = (rand() % (n - 2)) + 2;  // min max format
            line.delete_pos(pos);
            cout << "  someone in middle left (pos " << pos << ")\n";
            }

        // EVENT 5: 10% chance VIP cuts to front
        if (pVIP <= 10) {
            int idx = rand() % 100;
            line.push_front(idx);
            cout << "  VIP " << names[idx] << " to front!!\n";
        }

        // print current line
        cout << "Line now: ";
        line.print_names(names);
    }

    cout << "\n--- Simulation End ---\n";
    return 0;
}
