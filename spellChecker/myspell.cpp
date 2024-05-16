#include <iostream>
#include "hashtable.h"
#include <sstream>
#include <algorithm>

using namespace std;
using namespace cop4530;

//generate candidate words by replacing each character with 'a' to 'z'
vector<string> generateCandidates(const string &word, HashTable<string> ht) {
    vector<string> candidates;
    candidates.reserve(10); //the 10 candidates

    for (size_t i = 0; i < word.length(); ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            string candidate = word;
            candidate[i] = c;
            // check if in hashtable aka a candidate
            if (ht.contains(candidate) && find(candidates.begin(), candidates.end(), candidate) == candidates.end()) {
                candidates.push_back(candidate);

                // if 10 found then stop
                if (candidates.size() >= 10) {
                    return candidates;
                }
            }
        }
    }

    return candidates;
}
//separate function to make word all caps, which is used on the misspelled ones
string capitalizeWord(const string &word) {
    string capitalizedWord = word;
    for (char &c: capitalizedWord) {
        c = toupper(c);
    }
    return capitalizedWord;
}

void menu() {
    cout << "\n\n";
    cout << "l - Load Dictionary From File" << endl;
    cout << "a - Add Word" << endl;
    cout << "r - Remove Word" << endl;
    cout << "c - Clear HashTable" << endl;
    cout << "f - Find Word" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";
}

int main(int argc, char *argv[]) {
    //exe without arguments output
    if (argc == 1) {
        long userHashSizeInput;
        cout << "Enter preferred hash table capacity: ";
        cin >> userHashSizeInput;
        HashTable<string> ht(userHashSizeInput);
        cout << ht.getSize();
        char userInput;
        while (userInput != 'x') {
            menu();
            cin >> userInput;
            //ALL OF THESE ARE SIMPLE OUTPUTS OF THE FUNCTIONS WE MADE, refer to hpp for how these functs work
            switch (userInput) {
                case 'l': {
                    string dictInput;
                    cout << "Enter dictionary filename to load from: ";
                    cin >> dictInput;
                    const char *dict = dictInput.c_str();
                    //used this function in cases where file needed to be const char*
                    if (ht.load(dict)) {
                        cout << "Dictionary loaded successfully." << endl;
                    } else {
                        cerr << "Cannot open file " << dictInput << endl;
                    }
                    break;
                }

                case 'a': {
                    string word;
                    cout << "Enter word: ";
                    cin >> word;
                    if (ht.insert(word)) {
                        cout << "\nWord " << word << " added." << endl;
                    }
                    break;
                }

                case 'r': {
                    string word;
                    cout << "Enter word: ";
                    cin >> word;
                    if (ht.remove(word)) {
                        cout << "Word " << word << " deleted." << endl;
                    } else {
                        cout << "*****: Word not found.   Could not delete" << endl;
                    }
                    break;
                }

                case 'c': {
                    ht.clear();
                    break;
                }
                case 'f': {
                    string word;
                    cout << "Enter word: ";
                    cin >> word;
                    if (ht.contains(word)) {
                        cout << "Word " << word << " found." << endl;
                    } else {
                        cout << "Word " << word << " not found." << endl;
                    }
                    break;
                }
                case 'd': {
                    ht.dump();
                    break;
                }

                case 's': {
                    cout << "Size of hashtable: " << ht.getSize() << endl;
                    break;
                }

                case 'w': {
                    string fileToWriteTo;
                    cout << "Enter dictionary file name to write to: ";
                    cin >> fileToWriteTo;
                    const char *outputFile = fileToWriteTo.c_str();
                    //used this function in cases where file needed to be const char*
                    ht.write_to_file(outputFile);
                    cout << "Written successfully." << endl;
                    break;
                }

                default: {
                    cout << "*****Error: Invalid entry:   Try again.\n" << endl;
                    break;
                }
            }
        }

    }
    else if (argc == 4) {
        //exe with the proper parameters
        const char *dictionaryFile = argv[1];
        const char *readingFile = argv[2];
        const char *outputFile = argv[3];
        ifstream dict(dictionaryFile);
        ifstream read(readingFile);
        ofstream out(outputFile);
        HashTable<string> ht;

        if (dict.is_open() && read.is_open()) {
            //if all is working: rest of program work, everything out of if statement is error output
            cout << ht.getSize() << endl;
            string word;
            while (dict >> word) {
                if (!word.empty()) {
                    //converting all words in dictionary to lowercase as asked using algorithm function
                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                }
            }
            ht.load(dictionaryFile);
            //loads up all the lowercase words
            string line;
            while (getline(read, line)) {
                istringstream linePulled(line);
                string words;
                string correctedLine;

                while (linePulled >> words) {
                    //removing commas and other no alpha chars from word. Had to research how to do when going
                    //through a stringstream
                    words.erase(remove_if(words.begin(), words.end(), [](unsigned char c) {
                        return !isalpha(c);
                    }), words.end());
                    //transforming to lowercase to just check against the hashtable words
                    string lowercaseWord = words;
                    transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);
                    // check if the lowercase word is in the hash table
                    if (!words.empty() && !ht.contains(lowercaseWord)) {
                        //Output the entire line with the misspelled word in all caps
                        size_t pos = line.find(words);
                        while (pos != string::npos) {
                            transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);
                            if (!ht.contains(lowercaseWord)) {
                                line.replace(pos, words.length(), capitalizeWord(words));
                                //using function to capitalize the misspelled word
                            }
                            pos = line.find(words, pos + words.size());
                        }
                        //if incorrect word, begin with the candidate presentation
                        vector<string> wordCandidates = generateCandidates(lowercaseWord, ht);
                        cout << line << endl;
                        cout << "====================================" << endl;
                        if (!wordCandidates.empty()) {
                            for (size_t i = 0; i < wordCandidates.size(); i++) {
                                string candidate = wordCandidates.at(i);
                                cout << i << ") " << candidate << endl;
                            }
                        }
                        cout << "n (no change): " << endl;
                        cout << "====================================" << endl;
                        //so for every loop finding an incorrect word it will show u this output and
                        //loop through the candidates found in the hashtable, obviously stopping at max 10
                        cout << "Your choice: ";
                        char input;
                        do{
                            cin >> input;
                            if (input == 'n') {
                                correctedLine += words+" ";
                                break;
                            }

                            //since you have numbers and int, i did char so it continues if no changes, otherwise will
                            //change input to int and get the option for which candidate you want to replace misspelled
                            //word with
                            int choice = input - '0'; // Convert char to int

                            if(choice >= 0 && static_cast<size_t>(choice) < wordCandidates.size()) {
                                lowercaseWord = wordCandidates[choice];
                                correctedLine += lowercaseWord + " ";
                                break;
                                //if they choose candidate, adds to the new corrected line with candidate word
                            }
                            else {
                                cout<<"invalid choice"<<endl;
                                cout << "====================================" << endl;
                                if (!wordCandidates.empty()) {
                                    for (size_t i = 0; i < wordCandidates.size(); i++) {
                                        string candidate = wordCandidates.at(i);
                                        cout << i << ") " << candidate << endl;
                                    }
                                }
                                cout << "n (no change): " << endl;
                                cout << "====================================" << endl;
                                cin.ignore();
                            }
                            //loop for valid input, making sure it is one of options for candidate word
                        }while(true);
                    } else {
                        //if word is fine then it is added to corrected line as is
                        correctedLine += words + " ";
                    }
                }
                //end of reading a line means end out line for when writing to output file
                out << correctedLine << endl;
            }
            read.close();
            out.close();
            dict.close();
            //closing all the files
            //error checking below for parameter input
        } else if ((!dict.is_open() && !read.is_open()) || (!dict.is_open() && read.is_open())) {
            cout << ht.getSize() << endl;
            cout << "Cannot open file " << dictionaryFile << endl;
            cout << "Global dictionary cannot be loaded" << endl;
            return 1;
        } else if (dict.is_open() && !read.is_open()) {
            cout << ht.getSize() << endl;
            cout << "Word file cannot be opened" << endl;
            return 1;
        }
    }
    else{
        cout<<argv[1]<<" dictionary check_file output_file"<<endl;
    }

}