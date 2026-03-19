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
*/

using namespace std;

void displayMenu();
void addQuote();
void showLibrary();
void randomQuote();

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    string Option;
    displayMenu();

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
    else if (Option == "e")
    {
		cout << "Exiting program." << endl;
		system("pause");
        return 0;
    }
    else
    {
        cout << "Invalid option selected." << endl;
    }

    //system("pause");
    //return 0;
}

void displayMenu()
{
    cout << "1. Add in a new quote" << endl;
    cout << "2. Randomise your quote" << endl;
    cout << "3. See Quote Library" << endl;
    cout << "e. Close Window" << endl;
}

void addQuote()
{
    string Quote;
    int lastIndex = 0;
    int tempIndex;
    string tempLine;

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

    // Step 2: Get new quote input
    cout << "Enter your quote: " << endl << "type in return to go back to the previous screen" << endl;
    cin.ignore();
    getline(cin, Quote);

    // Step 3: Append new indexed quote
    ofstream outFile("QuoteArchive.txt", ios::app);

    if (!outFile)
    {
        cout << "Error opening file!" << endl;
        return;
    }
    
    if (Quote == "return")
    {
        main();
    }
    outFile << (lastIndex + 1) << " " << Quote << endl;
    outFile.close();

    cout << "Quote added with index " << (lastIndex + 1) << "!" << endl;
}

void showLibrary()
{
    ifstream inFile("QuoteArchive.txt");

    if (!inFile)
    {
        cout << "Unable to open file" << endl;
        return;
    }

    string Quote;
    int Index;

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
