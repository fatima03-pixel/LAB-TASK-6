#include <iostream>
#include <string>
#include <limits>
#include <utility>
using namespace std;

struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};

class CircularLinkedList
{
private:
    BookNode* head;
    int count;
    int displayIndex;   // index of the book currently on the screen

    BookNode* GetNodeAt(int index)
    {
        if (head == nullptr) return nullptr;
        BookNode* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

    void FixDisplayIndex()
    {
        if (count == 0 || displayIndex >= count)
            displayIndex = 0;
    }

public:
    CircularLinkedList() : head(nullptr), count(0), displayIndex(0) {}

    int GetCount() const { return count; }
    bool IsEmpty() { return count == 0; }
    bool IsNotEmpty() { return count > 0; }

    void Insert(BookNode* book)
    {
        if (book == nullptr) return;
        if (head == nullptr)
        {
            head = book;
            book->next = book;
        }
        else
        {
            book->next = head->next;
            head->next = book;
        }
        count++;
    }

    BookNode* Remove()
    {
        if (IsEmpty()) return nullptr;
        BookNode* first = head->next;
        if (first == head)
            head = nullptr;
        else
            head->next = first->next;
        first->next = nullptr;
        count--;
        FixDisplayIndex();
        return first;
    }

    void InsertAt(BookNode* book, int index)
    {
        if (book == nullptr) return;
        if (count == 0)
        {
            head = book;
            book->next = book;
            count++;
            return;
        }
        if (index < 0) index = NormalizeIndex(index);
        if (index > count) index %= count;

        if (index == 0)
        {
            Insert(book);
            return;
        }
        if (index == count)
        {
            book->next = head->next;
            head->next = book;
            head = book;
            count++;
            return;
        }
        BookNode* previous = GetNodeAt(index - 1);
        book->next = previous->next;
        previous->next = book;
        count++;
    }

    BookNode* RemoveAt(int index)
    {
        if (IsEmpty()) return nullptr;
        index = NormalizeIndex(index);
        if (index == 0) return Remove();
        BookNode* previous = GetNodeAt(index - 1);
        BookNode* target = previous->next;
        previous->next = target->next;
        if (target == head) head = previous;
        target->next = nullptr;
        count--;
        FixDisplayIndex();
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
        BookNode* current = head->next;
        for (int i = 0; i < count - 1; i++)
        {
            BookNode* nextNode = current->next;
            for (int j = i + 1; j < count; j++)
            {
                if (current->bookID > nextNode->bookID)
                    SwapData(current, nextNode);
                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    void Reverse()
    {
        if (count < 2) return;
        BookNode* oldTail = head;
        BookNode* oldFirst = head->next;
        BookNode* previous = oldTail;
        BookNode* current = oldFirst;
        while (current != oldTail)
        {
            BookNode* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        oldTail->next = previous;
        head = oldFirst;
    }

    BookNode* CurrentBook()
    {
        if (IsEmpty()) return nullptr;
        return GetNodeAt(displayIndex);
    }

    BookNode* MoveNext(bool& wrapped)
    {
        wrapped = false;
        if (IsEmpty()) return nullptr;
        displayIndex++;
        if (displayIndex >= count)
        {
            displayIndex = 0;
            wrapped = true;
        }
        return GetNodeAt(displayIndex);
    }

    void Print()
    {
        if (IsEmpty())
        {
            cout << "No books in the collection.\n";
            return;
        }
        BookNode* current = head->next;
        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "-----------------------------------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i << "\t" << current->bookID << "\t" << current->title;
            if (i == displayIndex) cout << "   <-- now showing";
            cout << endl;
            current = current->next;
        }
        cout << "Total books: " << count << endl;
    }

    CircularLinkedList(const CircularLinkedList& other)
        : head(nullptr), count(0), displayIndex(other.displayIndex)
    {
        if (other.head == nullptr) return;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new BookNode{ current->bookID, current->title, nullptr }, count);
            current = current->next;
        }
    }

    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other) return *this;
        while (IsNotEmpty()) delete Remove();
        displayIndex = other.displayIndex;
        if (other.head == nullptr) return *this;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new BookNode{ current->bookID, current->title, nullptr }, count);
            current = current->next;
        }
        return *this;
    }

    ~CircularLinkedList()
    {
        while (IsNotEmpty()) delete Remove();
    }
};

BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;
    book->bookID = id;
    book->title = title;
    book->next = nullptr;
    return book;
}

int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

BookNode* ReadBook()
{
    int id = ReadInt("Enter Book ID: ");
    string title;
    cout << "Enter Book Title: ";
    getline(cin, title);
    return CreateBook(id, title);
}

void ShowMenu()
{
    cout << "\n========================================\n"
         << "   CIRCULAR LIBRARY BOOK DISPLAY SYSTEM\n"
         << "========================================\n"
         << "1. Display All Books\n"
         << "2. Insert Book at Head\n"
         << "3. Remove Book from Head\n"
         << "4. Insert Book at Index\n"
         << "5. Remove Book at Index\n"
         << "6. Swap Books\n"
         << "7. Sort Books by ID\n"
         << "8. Reverse Book Collection\n"
         << "9. Move to Next Book\n"
         << "10. Check Collection Status\n"
         << "11. Exit\n";
}

int main()
{
    CircularLinkedList library;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            library.Print();
            break;

        case 2:
            library.Insert(ReadBook());
            cout << "Book inserted at the head (first position).\n";
            break;

        case 3:
        {
            BookNode* removed = library.Remove();
            if (removed != nullptr)
            {
                cout << "Removed from head: " << removed->title << endl;
                delete removed;
            }
            else
                cout << "The collection is empty.\n";
            break;
        }

        case 4:
        {
            int index = ReadInt("Enter index (negative or large values wrap around): ");
            library.InsertAt(ReadBook(), index);
            cout << "Book inserted.\n";
            break;
        }

        case 5:
        {
            if (library.IsEmpty())
            {
                cout << "The collection is empty.\n";
                break;
            }
            int index = ReadInt("Enter index (wraps around the circle): ");
            BookNode* removed = library.RemoveAt(index);
            cout << "Removed book: " << removed->title << endl;
            delete removed;
            break;
        }

        case 6:
        {
            if (library.GetCount() < 2)
            {
                cout << "At least two books are needed to swap.\n";
                break;
            }
            int a = ReadInt("Enter first index: ");
            int b = ReadInt("Enter second index: ");
            library.Swap(a, b);
            cout << "Books swapped.\n";
            break;
        }

        case 7:
            library.Sort();
            cout << "Books sorted by ID.\n";
            break;

        case 8:
            library.Reverse();
            cout << "Collection reversed.\n";
            break;

        case 9:
        {
            if (library.IsEmpty())
            {
                cout << "The collection is empty.\n";
                break;
            }
            bool wrapped;
            BookNode* book = library.MoveNext(wrapped);
            if (wrapped)
                cout << "(Reached the last book - returning to the first book)\n";
            cout << "Now showing: [" << book->bookID << "] " << book->title << endl;
            break;
        }

        case 10:
            if (library.IsEmpty())
                cout << "The collection is EMPTY.\n";
            else
            {
                BookNode* cur = library.CurrentBook();
                cout << "The collection is NOT empty. Total books: "
                     << library.GetCount() << endl;
                cout << "Currently showing: " << cur->title << endl;
            }
            break;

        case 11:
            cout << "Exiting. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please enter 1-11.\n";
        }
    } while (choice != 11);

    return 0;
}