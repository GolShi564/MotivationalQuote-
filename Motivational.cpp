#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <windows.h>
#include <ctime>

/*Version History 
Version Number / Date / Description 
Ver 1.1 / 19/3/2026 Broke down entire code to functions
Ver 1.2 / 19/3/2026 Added in a function to print a quote on the main UI
Ver 1.3 / 20/3/2026 Added in a function to delete quotes from the library
Ver 1.4 / 20/3/2026 Added in a function to randomise a quote from the library
*/

using namespace std;

void displayMenu();
void addQuote();
void showLibrary();
void randomQuote();
void showQuote(int index);
void deleteQuote(int index);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(static_cast<unsigned int>(time(0)));

    string Option;

    // --- New Logic to pick a random index ---
    int totalQuotes = 0;
    ifstream countFile("QuoteArchive.txt");
    int tempIdx;
    string tempQuote;

    // Count how many quotes are in the file
    while (countFile >> tempIdx) {
        getline(countFile, tempQuote);
        totalQuotes++;
    }
    countFile.close();
    
    displayMenu();
    if (totalQuotes > 0) {
        int randomIdx = (rand() % totalQuotes) + 1;
        showQuote(randomIdx);
    }
    else {
        cout << "\n[No quotes found in library yet]\n" << endl;
    }

    cout << "Options: ";
    cin >> Option;

    if (Option == "1")
    {
        addQuote();
    }
    else if (Option == "2")
    {
        randomQuote();
    }
    else if (Option == "3")
    {
        showLibrary();
    }
    else if (Option == "e" || Option == "E")
    {
		cout << "Exiting program." << endl;
		system("pause");
        return 0;
    }
    else if (Option == "4")
    {
        int index;
        showLibrary();
        cout << "type in return to go back to the main menu" << endl;
        cout << "Enter index to delete: ";
        cin >> index;

        deleteQuote(index);
    }
    else
    {
        cout << "Invalid option selected." << endl;
        cin >> Option;
    }

    //system("pause");
    //return 0;
}

void displayMenu()
{
    system("cls");
    cout << "1. Add in a new quote" << endl;
    cout << "2. Randomise your quote" << endl;
    cout << "3. See Quote Library" << endl;
	cout << "4. Delete Quote" << endl;
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
            lastIndex = tempIndex;       // keep updating to last seen index
            getline(inFile, tempLine);   // skip the rest of the line
        }
        inFile.close();
    }

    // Get new quote input
    cout << "Enter your quote: " << endl << "type in 'return' to go back to the previous screen" << endl;
    cin.ignore();
    getline(cin, Quote);

    // Append new indexed quote
    ofstream outFile("QuoteArchive.txt", ios::app);

    if (!outFile)
    {
        cout << "Error opening file!" << endl;
        return;
    }
    
    if (Quote == "return")
    {
        return;
        main();
    }
    outFile << (lastIndex + 1) << " " << Quote << endl;
    outFile.close();

    cout << "Quote added with index " << (lastIndex + 1) << "!" << endl;
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
    cout << setw(10) << "Index" << "Quote" << endl;

    while (inFile >> Index)
    {
        getline(inFile, Quote);
        cout << setw(10) << Index << Quote << endl;
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

void showQuote(int targetIndex)
{
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    int Index;
    string Quote;
    bool found = false;

    while (inFile >> Index)
    {
        getline(inFile, Quote);
        Quote.erase(0, 1);

        if (Index == targetIndex)
        {
            cout << endl << Quote << endl << endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Quote with index " << targetIndex << " not found." << endl;
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
