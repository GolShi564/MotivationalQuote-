#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <ctime>

/*Version History
Version Number - Date / Description
Ver 1.1 - 19/3/2026 Broke down entire code into functions
Ver 1.2 - 19/3/2026 Added in a function to print a quote on the main UI
Ver 1.3 - 19/3/2026 Added in a function to delete quotes from the library
Ver 1.4 - 19/3/2026 Added in a function to randomise a quote from the library
Ver 1.5 - 19/3/2026 Added in a feature to keep the main UI and library contained in the same loop
Ver 1.6 - 19/3/2026 Bug fixes in Option 4
Ver 1.7 - 20/3/2026 Implemented the random quote function & Favorite quote function to the main UI
Ver 1.8 - 20/3/2026 Qualtiy improvements to addQuote and Option 1
Ver 1.8a - 20/3/2026 Redesigned Option 5's exit to main loop process
Ver 1.9 - 20/3/2026 Fixed a bug within option 4
Ver 2.0 - 21/3/2026 Bug fixes in option 5
Ver 2.0a - Modified code to utilise classes,polymorphism and inheritance for improved modularity,readability and maintainability
*/

using namespace std;

// ================= BASE CLASS =================
class QuoteManager
{
protected:
    string filename = "QuoteArchive.txt";

public:
    virtual void display() = 0; // Polymorphism

    vector<string> readAllQuotes()
    {
        ifstream inFile(filename);
        vector<string> quotes;
        string line;

        while (getline(inFile, line))
        {
            quotes.push_back(line);
        }

        return quotes;
    }
};

// ================= LIBRARY CLASS =================
class LibraryManager : public QuoteManager
{
public:
    void display() override
    {
        system("cls");

        ifstream inFile(filename);

        int index;
        string quote;

        cout << "Index\tQuote\n";

        while (inFile >> index)
        {
            getline(inFile, quote);
            cout << index << "\t" << quote << endl;
        }

        inFile.close();
    }

    void addQuote()
    {
        int lastIndex = 0, tempIndex;
        string tempLine;

        ifstream inFile(filename);
        while (inFile >> tempIndex)
        {
            lastIndex = tempIndex;
            getline(inFile, tempLine);
        }
        inFile.close();

        cin.ignore();
        string quote;
        cout << "Enter quote:\n";
        getline(cin, quote);

        ofstream outFile(filename, ios::app);
        outFile << "\n" << (lastIndex + 1) << " " << quote;
        outFile.close();

        cout << "Quote added!\n";
        Sleep(1500);
    }

    void deleteQuote(int target)
    {
        ifstream inFile(filename);
        ofstream temp("temp.txt");

        int index, newIndex = 1;
        string quote;

        while (inFile >> index)
        {
            getline(inFile, quote);

            if (index == target) continue;

            temp << newIndex++ << quote << endl;
        }

        inFile.close();
        temp.close();

        remove("QuoteArchive.txt");
        rename("temp.txt", "QuoteArchive.txt");

        cout << "Quote deleted!\n";
        Sleep(1500);
    }
};

// ================= FAVORITE CLASS =================
class FavoriteManager : public QuoteManager
{
public:
    void display() override
    {
        ifstream favFile("favorite.txt");
        if (!favFile) return;

        int favIndex;
        favFile >> favIndex;
        favFile.close();

        ifstream inFile(filename);

        int index;
        string quote;

        while (inFile >> index)
        {
            getline(inFile, quote);

            if (index == favIndex)
            {
                cout << "\n" << quote << "\n\n";
                break;
            }
        }

        inFile.close();
    }

    void setFavorite()
    {
        ifstream inFile(filename);

        int index;
        string quote;

        cout << "Index\tQuote\n";

        while (inFile >> index)
        {
            getline(inFile, quote);
            cout << index << "\t" << quote << endl;
        }

        inFile.close();

        int choice;
        cout << "\nChoose favorite index: ";
        cin >> choice;

        ofstream favFile("favorite.txt");
        favFile << choice;
        favFile.close();

        ofstream modeFile("mode.txt");
        modeFile << "favorite";
        modeFile.close();

        cout << "Favorite saved!\n";
        Sleep(1500);
    }
};

// ================= RANDOM CLASS =================
class RandomManager : public QuoteManager
{
public:
    void display() override
    {
        vector<string> quotes = readAllQuotes();

        if (quotes.empty()) return;

        srand(time(0));
        int r = rand() % quotes.size();

        cout << "\n" << quotes[r] << "\n\n";
    }
};

// ================= MENU =================
void displayMenu()
{
    cout << "1. Add in a new quote\n";
    cout << "2. Toggle Random/Favorite\n";
    cout << "3. See Quote Library\n";
    cout << "4. Delete Quote\n";
    cout << "5. Set a Favorite Quote\n";
    cout << "e. Close Window\n\n";
}

// ================= MAIN =================
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    LibraryManager library;
    FavoriteManager favorite;
    RandomManager random;

    string option;

    do
    {
        system("cls");

        displayMenu();

        // ===== MODE HANDLING =====
        ifstream modeFile("mode.txt");
        string mode = "random";

        if (modeFile)
            modeFile >> mode;

        modeFile.close();

        QuoteManager* manager;

        if (mode == "favorite")
            manager = &favorite;
        else
            manager = &random;

        manager->display(); // 🔥 POLYMORPHISM

        // ===== INPUT =====
        cout << "Options: ";
        cin >> option;

        // ===== OPTIONS =====
        if (option == "1")
        {
            string confirm;
            cout << "Add quote? (Y/N): ";
            cin >> confirm;

            if (confirm == "Y" || confirm == "y")
                library.addQuote();
        }

        else if (option == "2")
        {
            ofstream out("mode.txt");

            if (mode == "random")
            {
                out << "favorite";
                cout << "Switched to FAVORITE mode.\n";
            }
            else
            {
                out << "random";
                cout << "Switched to RANDOM mode.\n";
            }

            out.close();
            Sleep(1500);
        }

        else if (option == "3")
        {
            library.display();
            system("pause");
        }

        else if (option == "4")
        {
            int index;
            library.display();

            cout << "\nEnter index to delete: ";
            cin >> index;

            library.deleteQuote(index);
        }

        else if (option == "5")
        {
            favorite.setFavorite();
        }

        else if (option == "e" || option == "E")
        {
            cout << "Exiting program...\n";
            break;
        }

        else
        {
            cout << "Invalid option selected.\n";
            Sleep(1000);
        }

    } while (true);

    return 0;
}
