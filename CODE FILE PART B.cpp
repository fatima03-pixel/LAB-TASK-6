
#include <iostream>
#include <string>
#include <utility>
using namespace std;

struct Rider
{
    int riderID;
    string name;
    string zone;
    int ordersAssigned;
    Rider* next;
};

class CircularLinkedList
{
private:
    Rider* head;     
    int count;
    int turn;        

    Rider* GetNodeAt(int index)
    {
        if (head == nullptr) return nullptr;
        Rider* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(Rider* a, Rider* b)
    {
        swap(a->riderID, b->riderID);
        swap(a->name, b->name);
        swap(a->zone, b->zone);
        swap(a->ordersAssigned, b->ordersAssigned);
    }

    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

    void FixTurn()
    {
        if (count == 0 || turn >= count) turn = 0;
    }

public:
    CircularLinkedList() : head(nullptr), count(0), turn(0) {}

    bool IsEmpty() { return count == 0; }
    bool IsNotEmpty() { return count > 0; }

    void Insert(Rider* rider)
    {
        if (rider == nullptr) return;
        if (head == nullptr)
        {
            head = rider;
            rider->next = rider;
        }
        else
        {
            rider->next = head->next;
            head->next = rider;
        }
        count++;
    }

    Rider* Remove()
    {
        if (IsEmpty()) return nullptr;
        Rider* first = head->next;
        if (first == head)
            head = nullptr;
        else
            head->next = first->next;
        first->next = nullptr;
        count--;
        FixTurn();
        return first;
    }

    void InsertAt(Rider* rider, int index)
    {
        if (rider == nullptr) return;
        if (count == 0)
        {
            head = rider;
            rider->next = rider;
            count++;
            return;
        }
        if (index < 0) index = NormalizeIndex(index);
        if (index > count) index %= count;

        if (index == 0)
        {
            Insert(rider);
            return;
        }
        if (index == count)
        {
            rider->next = head->next;
            head->next = rider;
            head = rider;
            count++;
            return;
        }
        Rider* previous = GetNodeAt(index - 1);
        rider->next = previous->next;
        previous->next = rider;
        count++;
    }

    Rider* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        index = NormalizeIndex(index);
        if (index == 0) return Remove();
        Rider* previous = GetNodeAt(index - 1);
        Rider* target = previous->next;
        previous->next = target->next;
        if (target == head) head = previous;
        target->next = nullptr;
        count--;
        FixTurn();
        return target;
    }

    void Swap(int index1, int index2)
    {
        if (IsEmpty()) return;
        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);
        if (index1 == index2) return;
        SwapData(GetNodeAt(index1), GetNodeAt(index2));
    }

    void Sort()   
    {
        if (count < 2) return;
        Rider* current = head->next;
        for (int i = 0; i < count - 1; i++)
        {
            Rider* nextNode = current->next;
            for (int j = i + 1; j < count; j++)
            {
                if (current->riderID > nextNode->riderID)
                    SwapData(current, nextNode);
                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    void Reverse()
    {
        if (count < 2) return;
        Rider* oldTail = head;
        Rider* oldFirst = head->next;
        Rider* previous = oldTail;
        Rider* current = oldFirst;
        while (current != oldTail)
        {
            Rider* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        oldTail->next = previous;
        head = oldFirst;
    }

    Rider* AssignOrder()
    {
        if (IsEmpty()) return nullptr;
        Rider* rider = GetNodeAt(turn);
        rider->ordersAssigned++;
        turn = (turn + 1) % count;
        return rider;
    }

    bool VerifyCircular()
    {
        if (IsEmpty()) return true;
        Rider* first = head->next;
        Rider* walker = first;
        for (int i = 0; i < count; i++)
            walker = walker->next;
        return head->next == first && walker == first;
    }

    void PrintLinkCheck()
    {
        if (IsEmpty()) return;
        cout << "Last rider (" << head->name << ") -> next = "
             << head->next->name << " (first rider)\n";
        cout << "Circular link verified: "
             << (VerifyCircular() ? "YES" : "NO") << endl;
    }

    void Print()
    {
        if (IsEmpty())
        {
            cout << "No riders on duty.\n";
            return;
        }
        Rider* current = head->next;
        cout << "\nPos\tID\tName\tZone\t\tOrders\n";
        cout << "---------------------------------------------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i << "\t" << current->riderID << "\t" << current->name
                 << "\t" << current->zone << "\t" << current->ordersAssigned;
            if (i == turn) cout << "   <-- next order goes here";
            cout << endl;
            current = current->next;
        }
        cout << "Total riders: " << count << endl;
    }

    CircularLinkedList(const CircularLinkedList& other)
        : head(nullptr), count(0), turn(other.turn)
    {
        if (other.head == nullptr) return;
        Rider* c = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new Rider{ c->riderID, c->name, c->zone, c->ordersAssigned, nullptr }, count);
            c = c->next;
        }
    }

    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other) return *this;
        while (IsNotEmpty()) delete Remove();
        turn = other.turn;
        if (other.head == nullptr) return *this;
        Rider* c = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new Rider{ c->riderID, c->name, c->zone, c->ordersAssigned, nullptr }, count);
            c = c->next;
        }
        return *this;
    }

    ~CircularLinkedList()
    {
        while (IsNotEmpty()) delete Remove();
    }
};

Rider* CreateRider(int id, string name, string zone)
{
    Rider* r = new Rider;
    r->riderID = id;
    r->name = name;
    r->zone = zone;
    r->ordersAssigned = 0;
    r->next = nullptr;
    return r;
}

int main()
{
    CircularLinkedList riders;

    riders.InsertAt(CreateRider(303, "Ali", "North"), 0);
    riders.InsertAt(CreateRider(301, "Bilal", "South"), 1);
    riders.InsertAt(CreateRider(305, "Hamza", "East"), 2);
    riders.InsertAt(CreateRider(302, "Usman", "West"), 3);
    cout << "INITIAL RIDER CIRCLE";
    riders.Print();
    riders.PrintLinkCheck();

    cout << "\nROUND-ROBIN DISPATCH OF 10 ORDERS (4 riders)\n";
    for (int order = 401; order <= 410; order++)
    {
        Rider* r = riders.AssignOrder();
        cout << "Order " << order << " -> " << r->name << " (" << r->zone << ")\n";
    }
    riders.Print();

    riders.InsertAt(CreateRider(306, "Zain", "Central"), 2);
    cout << "\nAFTER A NEW RIDER JOINS AT POSITION 2";
    riders.Print();

    Rider* removed = riders.RemoveAt(1);
    if (removed != nullptr)
    {
        cout << "\nRider went off duty: " << removed->name << endl;
        delete removed;
    }
    cout << "\nAFTER REMOVING POSITION 1";
    riders.Print();

    riders.Swap(0, 2);
    cout << "\nAFTER SWAPPING POSITION 0 AND 2";
    riders.Print();

    riders.Sort();
    cout << "\nAFTER SORTING BY RIDER ID";
    riders.Print();

    riders.Reverse();
    cout << "\nAFTER REVERSING THE CIRCLE";
    riders.Print();

    riders.Insert(CreateRider(300, "Sana", "Airport"));
    cout << "\nAFTER INSERTING AT THE NODE NEXT TO THE PIVOT";
    riders.Print();

    removed = riders.Remove();
    if (removed != nullptr)
    {
        cout << "\nRemoved from node next to pivot: " << removed->name << endl;
        delete removed;
    }

    cout << "\nDISPATCH 5 MORE ORDERS AFTER THE CHANGES\n";
    for (int order = 411; order <= 415; order++)
    {
        Rider* r = riders.AssignOrder();
        cout << "Order " << order << " -> " << r->name << " (" << r->zone << ")\n";
    }

    cout << "\nFINAL RIDER CIRCLE";
    riders.Print();
    riders.PrintLinkCheck();
    cout << "Riders on duty? " << (riders.IsNotEmpty() ? "Yes" : "No") << endl;

    return 0;
}