/* 
 * Ross Kaplan
 * DSA 2
 * Fall Semester 2016
 * Program 1: Hashtable Dictionary
 * Goal: Implement a dictionary using a hashtable to quickly check if all words in a file are valid
*/

#include <iostream>
#include "hash.h"
#include <cmath>
#include <ctime>
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>

using namespace std;

void loadDictionary(hashTable &); //prompt for dictionary file, read input file, and load it

void spellCheckDoc(hashTable &); //prompt for doc to be spell checked and output file
                                 //compute the output and send it out

string wordToLower(string); //make word lowercase to store in hashtable

string processInput(hashTable &, string, int);

int main() {

    clock_t clock1, clock2;
    hashTable hash_table(100000);
    clock1 = clock();
    loadDictionary(hash_table);
    clock2 = clock(); 
    long double diff_time = double(clock2 - clock1) / CLOCKS_PER_SEC;
    cout << "Loading of dictionary took " << diff_time << " seconds." << endl;
    
    clock1 = clock();
    spellCheckDoc(hash_table);
    clock2 = clock();
    diff_time = (clock2 - clock1) / CLOCKS_PER_SEC;
    cout << "Spell checking of document took " << diff_time << " seconds." << endl;

return 0;

}

void loadDictionary(hashTable &hash_table) {

    string dict_file;
    cout << "Dictionary file: " << endl;
    cin >> dict_file;

    ifstream dict (dict_file.c_str());
    
    if (dict.fail()) {
        cerr << "Failed to open " << dict_file << endl;
        exit(-1);
    }
    
    string temp_word;
    while (dict >> temp_word) {
        if (hash_table.insert(wordToLower(temp_word)) == -1) {
            cerr << "Memory allocation issues!" << endl;
            exit(-1);
        }
    }

    return;

}

string wordToLower(string temp_word) {

    for (int i = 0; i < temp_word.length(); i++) {
        temp_word[i] = tolower(temp_word[i]);
    }
return temp_word;
}

void spellCheckDoc(hashTable &hash_table) {

    string spell_check_doc;
    cout << "Document to spell check: " << endl;
    cin >> spell_check_doc;

    string output_doc;
    cout << "Document to output to: " << endl;
    cin >> output_doc;

    ifstream infile (spell_check_doc.c_str());
    ofstream outfile (output_doc.c_str());
    if (infile.fail()) {
        cerr << "Failed to open for reading: " << spell_check_doc << endl;
        exit(-1);
    }
    
    if (outfile.fail()) {
        cerr << "Failed to open for writing: " << output_doc << endl;
        exit(-1);
    }

    string temp_word, processed;
    int line_number = 1;
    char c;
    while (infile.get(c)) {
        if (isalnum(c) || (c == '-') || (c == '\'')) {
            temp_word += tolower(c);
            continue;
        }

        if (c == '\n' && temp_word == "") {
            line_number++;
            temp_word = "";
            continue;
        }
        if (temp_word != "") {
            processed = processInput(hash_table, temp_word, line_number);
            
            if (processed != "")
                outfile << processed << endl; 
        }
        temp_word = "";

        if (c == '\n')
            line_number++; 
    }
}

string processInput(hashTable &hash_table, string temp_word, int line_number) {
    for (int i = 0; i < temp_word.length(); i++) {
        if (isdigit(temp_word[i])) 
            return "";
    }
    if (temp_word.length() > 20) {
        return "Long word at line " + to_string(line_number) + ", starts: " + temp_word.substr(0,20);
    }
    if (hash_table.contains(temp_word)) {
        return ""; 
    }
    if (!hash_table.contains(temp_word)) {
        return "Unknown word at line " + to_string(line_number) + ": " + temp_word;
    }
}
