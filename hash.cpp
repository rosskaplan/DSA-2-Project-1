#include "hash.h"
#include <iostream>

using namespace std;

hashTable::hashTable(int size) {

    capacity = getPrime(size);
    data.resize(capacity);

    for (int i = 0; i < data.size(); i++) {
        data[i].isOccupied = false;
        data[i].isDeleted = false;
    }

    filled = 0;
}

int hashTable::insert(const string &key, void *pv) {
    int which_bucket = hash(key);
    while (data[which_bucket].isOccupied == true) {
        if (data[which_bucket].key == key) {
            if (!data[which_bucket].isDeleted) {
                return 1; //Success! Exists in hashTable already.
            } else {
                data[which_bucket].isDeleted = false;
                data[which_bucket].pv=pv;
                return 0; //Success! Inserted into hashTable, no rehash.
            }
        } else {
            which_bucket++;
            if (which_bucket >= capacity)
                which_bucket %= capacity; //Never go out of bounds of the vector
        }
    } //Fill in the next location

    data[which_bucket].key = key;
    data[which_bucket].isOccupied = true;
    data[which_bucket].isDeleted = false;
    data[which_bucket].pv = pv;
    filled++;
    if (filled > .5*capacity) {
        if (rehash()) {
            return 0; //Success! Inserted into hashTable with successful rehash.
        }
        else {
            return -1; //Failure.  Rehash culd not be done.
        }
    }
    return 0; //Success! Inserted into hashTable, no rehash
}

bool hashTable::contains(const string &key) {
    return (findPos(key) >=0) ? true : false;
}

int hashTable::hash(const string &key) {
//Using the hashing algorithm from textbook (2006 book, page 188)

    int hash_val = 0;
    int i = 0;
    
    for (i = 0; i < key.length(); i++) 
        hash_val = 37*hash_val + key[i];
    
    hash_val %= capacity;   
    if (hash_val < 0)
        hash_val += capacity;
    
    return hash_val;
}

int hashTable::findPos(const string &key) {
    int which_bucket = hash(key);

    while (data[which_bucket].isOccupied == true) { 
//Loop while still occupied.  If find the matching key, it must be still valid.
//If it's been lazily deleted, return we have not found the word as valid.
        if (data[which_bucket].key == key) {
            return (data[which_bucket].isDeleted == false) ? which_bucket : -1;
        } else {
            which_bucket++;
            if (which_bucket >= capacity)
                which_bucket %= capacity;
        }
    }
    return -1;
}

bool hashTable::rehash() {
    vector<hashItem> temp = data;
    int i;
     
    for (i = 0; i < capacity; i++) {
        data[i].key = "";
        data[i].isOccupied = false;
        data[i].isDeleted = false;
        data[i].pv = NULL;
    }
    filled = 0;
    capacity = getPrime(capacity);
    try {
        data.resize(capacity);
    } catch(exception& e) {
        cerr << "Failed to resize" << endl;
        return false;
    }
    for (i = 0; i < temp.size(); i++) {
        if ((temp[i].isOccupied) && !(temp[i].isDeleted)) 
            insert(temp[i].key, NULL);
    }
    return true;
}

unsigned int hashTable::getPrime(int size) {
    int i;
    long long int primearray[] = {193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};
//Good Hashtable primes from http://planetmath.org/goodhashtableprimes 

    for (i = 0; i <= 21 ; i++) {
        if (primearray[i] > size) {
            return primearray[i];
        }
    }
    return 1610612741;
} 
