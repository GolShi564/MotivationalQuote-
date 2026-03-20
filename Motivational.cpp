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
*/

using namespace std;

//Function Declarations
void displayMenu();
void addQuote();
void showLibrary();
void randomQuote();
void showQuote();
void displayFavorite();
void deleteQuote(int index);
void displayRandomQuote();

//Main Function
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    
    bool exit = false;
    string Option;

	displayMenu(); // Display the menu at the start of the program
    
    ifstream modeFile("mode.txt"); 
    string mode = "random"; // default

    if (modeFile)
    {
        modeFile >> mode;
    }
    modeFile.close();

    if (mode == "favorite")
    {
        displayFavorite();
    }
    else
    {
        displayRandomQuote();
    }

    cout << "Options: ";
    cin >> Option;

    while (exit != true)
    {
        if (Option == "1")
        {
			cout << "Are you sure you want to add a quote ? Y / N:" << endl;
			cin >> Option;
            if (Option == "Y" || Option == "y")
                addQuote();
            else if (Option == "N" || Option == "n")
                return main();
        }
        else if (Option == "2")
        {
            ifstream modeFile("mode.txt");
            string mode = "random";

            if (modeFile)
                modeFile >> mode;

            modeFile.close();

            ofstream outFile("mode.txt");

            if (mode == "random")
            {
                outFile << "favorite";
                cout << "Switched to FAVORITE quote mode." << endl;
            }
            else
            {
                outFile << "random";
                cout << "Switched to RANDOM quote mode." << endl;
            }

            outFile.close();

			Sleep(2000);
            return main();
        }
        else if (Option == "3")
        {
            showLibrary();
            cout << "Type 'return' to go back to the previous screen" << endl;

            cin >> Option;
            if (Option == "return")
            {
                return main();
            }
            else
                while (Option != "return")
                {
                    cout << "Invalid option selected." << endl;
                    cout << "Options: ";
                    cin >> Option;
                }

        }
        else if (Option == "4")
        {
            int index;
			char Option;
            showLibrary();

            cout << "Are you sure you want to proceed? Y / N:" << endl;
			cin >> Option;
            if (Option == 'y' || Option == 'Y')
            {
                cout << "type in the corresponding index and then press enter to delete the quote" << endl;
                cout << "Enter index to delete: ";
                cin >> index;

                deleteQuote(index);

                cout << "Do you wish to delete another quote N/Y?" << endl;
                cin >> Option;
                if (Option == 'y' || Option == 'Y')
                {
                    continue; // This will go back to the start of the loop and show the library again
                }
                else if (Option == 'n' || Option == 'N')
                {
                    return main(); // This will go back to the main menu
                }
                else
                {
                    cout << "Invalid option selected." << endl;
                    cin >> Option;
                }
            }
            else if (Option == 'n' || Option == 'N')
            {
                return main();
			}
            else 
            {
                cout << "Invalid option selected." << endl;
                cin >> Option;
			}
        }
        else if (Option == "5")
        {
            cout << "Are you sure you want to proceed? Y / N:" << endl;
            cin >> Option;
            if (Option == "Y" || Option == "y")
            {
                system("cls");
                showQuote();
            }
            else if (Option == "N" || Option == "n")
                return main();
        }
        else if (Option == "e" || Option == "E")
        {
            break;
            return main();// Exit the loop and end the program
        }
        else
        {
            cout << "Invalid option selected." << endl;
            cin >> Option;
        }
    }

    cout << "Exiting program." << endl;
    system("pause");
    return 0;
}

void displayMenu()
{
    system("cls");
    cout << "1. Add in a new quote" << endl;
    cout << "2. Randomise your quote" << endl;
    cout << "3. See Quote Library" << endl;
    cout << "4. Delete Quote" << endl;
    cout << "5. Set a Favorite Quote" << endl;
    cout << "e. Close Window" << endl;
}

void addQuote()
{
    string Quote;
    int lastIndex = 0;
    int tempIndex;
    string tempLine;

    system("cls");

    // Step 1: Read file to find last index
    ifstream inFile("QuoteArchive.txt");
    if (inFile)
    {
        while (inFile >> tempIndex)
        {
            lastIndex = tempIndex;
            getline(inFile, tempLine);
        }
        inFile.close();
    }

    // Step 2: Get new quote input
    cout << "Enter your quote \nType 'return' to go back to the previous screen \nQuote: ";
    // Clear the buffer and get the line
    cin.ignore(1000, '\n');
    getline(cin, Quote);

    // Step 4: Append new indexed quote
    ofstream outFile("QuoteArchive.txt", ios::app);
    if (!outFile)
    {
        cout << "Error opening file!" << endl;
        Sleep(2000);
        return;
    }

    // Using setw(10) to match your showLibrary formatting
    outFile << "\n" << left << "\t" << (lastIndex + 1) << Quote << endl;
    outFile.close();

    cout << "Quote added with index " << (lastIndex + 1) << "!" << endl;

    // Step 5: Pause then "fall off" the end of the function
    Sleep(2000);
    main();

}

void showLibrary()
{

    system("cls");
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    string Quote;
    int Index;
    string Option;

    cout << left;
    cout  << "Index" << "\t\t\t" << "Quote" << endl;

    while (inFile >> Index)
    {
        getline(inFile, Quote);
        cout << Index << "\t" << Quote << endl;
    }

    inFile.close();
}

void randomQuote()
{
    system("cls");
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    vector<string> quotes;
    string line;

    // Read entire file
    while (getline(inFile, line))
    {
        quotes.push_back(line);
    }

    inFile.close();

    if (quotes.empty())
    {
        cout << "No quotes available." << endl;
        return;
    }

    srand(time(0));
    int randomIndex = rand() % quotes.size();

    cout << "Random Quote: " << quotes[randomIndex] << endl;
}

void showQuote()
{
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    int Index;
    string Quote;

    cout << left << setw(10) << "Index" << "Quote" << endl;

    // Display all quotes
    while (inFile >> Index)
    {
        getline(inFile, Quote);
        Quote.erase(0, 1);

        cout << left << setw(10) << Index << Quote << endl;
    }

    inFile.close();

    // Ask user to choose favorite
    int choice;
    cout << "\nChoose a quote to favorite\n";
    cout << "Index: ";
    cin >> choice;

    // Save favorite index
    ofstream favFile("favorite.txt");

    if (!favFile)
    {
        cout << "Error saving favorite." << endl;
        return;
    }

    favFile << choice;
    favFile.close();

    ofstream modeFile("mode.txt");
    modeFile << "favorite";
    modeFile.close();

    cout << "\nFavorite quote set successfully!\n" << endl;
    Sleep(2000);
    main();
}

void displayFavorite()
{
    ifstream favFile("favorite.txt");

    if (!favFile)
        return; // no favorite yet

    int favIndex;
    favFile >> favIndex;
    favFile.close();

    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
        return;

    int Index;
    string Quote;

    while (inFile >> Index)
    {
        getline(inFile, Quote);
        Quote.erase(0, 1);

        if (Index == favIndex)
        {
            cout << "\n" << Quote << "\n\n";
            break;
        }
    }

    inFile.close();
}

void deleteQuote(int targetIndex)
{
    ifstream inFile("QuoteArchive.txt");
    ofstream tempFile("temp.txt");

    if (!inFile || !tempFile)
    {
        cout << "Error opening file!" << endl;
        return;
    }

    int Index, newIndex = 1;
    string Quote;
    bool found = false;

    while (inFile >> Index)
    {
        getline(inFile, Quote);

        if (Index == targetIndex)
        {
            found = true;
            continue;
        }

        tempFile << newIndex++ << Quote << endl;
    }

    inFile.close();
    tempFile.close();

    remove("QuoteArchive.txt");
    rename("temp.txt", "QuoteArchive.txt");

    if (found)
        cout << "Quote deleted and re-indexed!" << endl;
    else
        cout << "Quote not found." << endl;
}

void displayRandomQuote()
{
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
        return;

    vector<string> quotes;
    string line;

    while (getline(inFile, line))
    {
        quotes.push_back(line);
    }

    inFile.close();

    if (quotes.empty()) return;

    srand(time(0));
    int r = rand() % quotes.size();

    cout << "\n" << quotes[r] << "\n\n";
}
