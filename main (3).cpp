#include <iostream>
#include <string>
using namespace std;

// Each node represents one Monopoly property.
struct PropertyNode {
    string name;
    int cost;
    string owner;
    PropertyNode* next;

    PropertyNode(string propertyName, int propertyCost) {
        name = propertyName;
        cost = propertyCost;
        owner = "None";
        next = nullptr;
    }
};

// Custom circular linked list.
class CircularLinkedList {
private:
    PropertyNode* head;
    PropertyNode* tail;

public:
    CircularLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    // Adds a property to the end of the list.
    void add(string name, int cost) {
        PropertyNode* newNode = new PropertyNode(name, cost);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            newNode->next = head;
        }
        else {
            tail->next = newNode;
            tail = newNode;
            tail->next = head;
        }
    }

    // Searches for a property by name.
    PropertyNode* search(string name) {
        if (head == nullptr) {
            return nullptr;
        }

        PropertyNode* current = head;

        do {
            if (current->name == name) {
                return current;
            }

            current = current->next;
        }
        while (current != head);

        return nullptr;
    }

    // Removes a property by name.
    bool remove(string name) {
        if (head == nullptr) {
            return false;
        }

        PropertyNode* current = head;
        PropertyNode* previous = tail;

        do {
            if (current->name == name) {
                // Only one node exists.
                if (current == head && current == tail) {
                    head = nullptr;
                    tail = nullptr;
                }
                else {
                    previous->next = current->next;

                    // Removing the first node.
                    if (current == head) {
                        head = current->next;
                        tail->next = head;
                    }

                    // Removing the last node.
                    if (current == tail) {
                        tail = previous;
                        tail->next = head;
                    }
                }

                delete current;
                return true;
            }

            previous = current;
            current = current->next;
        }
        while (current != head);

        return false;
    }

    // Prints every property once.
    void print() {
        if (head == nullptr) {
            cout << "The board is empty." << endl;
            return;
        }

        PropertyNode* current = head;
        int number = 1;

        do {
            cout << number << ". " << current->name
                 << " | Cost: $" << current->cost
                 << " | Owner: " << current->owner << endl;

            current = current->next;
            number++;
        }
        while (current != head);
    }

    PropertyNode* getHead() {
        return head;
    }

    // Deletes all nodes when the program ends.
    ~CircularLinkedList() {
        if (head == nullptr) {
            return;
        }

        tail->next = nullptr;
        PropertyNode* current = head;

        while (current != nullptr) {
            PropertyNode* nodeToDelete = current;
            current = current->next;
            delete nodeToDelete;
        }
    }
};

// Stores information about one player.
struct Player {
    string name;
    int money;
    PropertyNode* position;

    Player(string playerName, PropertyNode* startingPosition) {
        name = playerName;
        money = 1500;
        position = startingPosition;
    }
};

// Moves a player around the circular board.
void movePlayer(Player& player, int spaces) {
    for (int i = 0; i < spaces; i++) {
        player.position = player.position->next;
    }
}

// Attempts to purchase the current property.
void buyProperty(Player& player) {
    PropertyNode* property = player.position;

    if (property->owner != "None") {
        cout << property->name << " is already owned by "
             << property->owner << "." << endl;

        cout << "The property cannot be purchased." << endl;
    }
    else if (player.money >= property->cost) {
        property->owner = player.name;
        player.money = player.money - property->cost;

        cout << player.name << " purchased "
             << property->name << " for $"
             << property->cost << "." << endl;
    }
    else {
        cout << player.name
             << " does not have enough money to buy "
             << property->name << "." << endl;
    }
}

int main() {
    CircularLinkedList board;

    // Add 12 properties to the circular board.
    board.add("Mediterranean Avenue", 60);
    board.add("Baltic Avenue", 60);
    board.add("Oriental Avenue", 100);
    board.add("Vermont Avenue", 100);
    board.add("Connecticut Avenue", 120);
    board.add("St. Charles Place", 140);
    board.add("States Avenue", 140);
    board.add("Virginia Avenue", 160);
    board.add("St. James Place", 180);
    board.add("Tennessee Avenue", 180);
    board.add("New York Avenue", 200);
    board.add("Boardwalk", 400);

    cout << "===== ORIGINAL BOARD =====" << endl;
    board.print();

    // Test the search function.
    cout << endl;
    cout << "===== SEARCH TEST =====" << endl;

    PropertyNode* foundProperty = board.search("Boardwalk");

    if (foundProperty != nullptr) {
        cout << "Boardwalk was found." << endl;
        cout << "Boardwalk costs $"
             << foundProperty->cost << "." << endl;
    }
    else {
        cout << "Boardwalk was not found." << endl;
    }

    // Test the remove function.
    cout << endl;
    cout << "===== REMOVE TEST =====" << endl;

    if (board.remove("Baltic Avenue")) {
        cout << "Baltic Avenue was removed successfully."
             << endl;
    }
    else {
        cout << "Baltic Avenue was not found." << endl;
    }

    // Add Baltic Avenue back after testing remove.
    board.add("Baltic Avenue", 60);

    cout << "Baltic Avenue was added back to the board."
         << endl;

    // Create two players.
    Player player1("Andres", board.getHead());
    Player player2("Player 2", board.getHead());

    // Fixed rolls make the output the same every time.
    int diceRolls[10] = {3, 3, 4, 2, 6, 5, 2, 6, 1, 4};

    cout << endl;
    cout << "===== 10-TURN GAME =====" << endl;

    for (int turn = 0; turn < 10; turn++) {
        Player* currentPlayer;

        // Even turns belong to Player 1.
        // Odd turns belong to Player 2.
        if (turn % 2 == 0) {
            currentPlayer = &player1;
        }
        else {
            currentPlayer = &player2;
        }

        int roll = diceRolls[turn];

        cout << endl;
        cout << "Turn " << turn + 1 << ": "
             << currentPlayer->name
             << " rolled " << roll << "." << endl;

        movePlayer(*currentPlayer, roll);

        cout << currentPlayer->name
             << " landed on "
             << currentPlayer->position->name
             << "." << endl;

        buyProperty(*currentPlayer);

        cout << currentPlayer->name
             << " has $" << currentPlayer->money
             << " remaining." << endl;
    }

    cout << endl;
    cout << "===== FINAL BOARD =====" << endl;
    board.print();

    cout << endl;
    cout << "===== FINAL BALANCES =====" << endl;
    cout << player1.name << ": $" << player1.money << endl;
    cout << player2.name << ": $" << player2.money << endl;

    return 0;
}