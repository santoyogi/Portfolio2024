/*****************************************************
Gissel Santoyo
Assignment 6: closedHashing.cpp
Date: Mar. 3, 2022
******************************************************/
//Copyright 2022, Bradley Peterson, Weber State University, all rights reserved. (Feb 2022)

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <cstdio>
#include <algorithm>
#include <stdexcept>


using std::cout;
using std::endl;
using std::map;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::stringstream;
using std::pair;
using std::logic_error;
using std::move;


//forward declarations
template <typename T> class HashTable;
template <typename T> class Iterator;
//************************************************************
//A quick and simple class that simulates a Product object.
//************************************************************
class Product {
public:
    void setCost(int cost);
    void setName(const string&);
    string getName();
    int getCost();
    string getAllInfo();
private:
    string name;
    int cost{ 0 };
};
void Product::setCost(int cost) {
    this->cost = cost;
}
void Product::setName(const string& name) {
    this->name = name;
}
string Product::getName() {
    return name;
}
int Product::getCost() {
    return cost;
}
string Product::getAllInfo() {
    stringstream ss;
    ss << "Name: " << name << ", Cost: " << cost;
    return ss.str();
}


//************************************************************
//The HashTable class
//************************************************************
template <typename T>
class HashTable
{
public:
    unsigned int getNumBuckets() { return capacity; }
    unsigned int getTotalCount() const;
    unsigned int getWorstClump() const;

    // TODO: Create the default constructor
    HashTable();
    // TODO: Create the copy constructor
    HashTable(const HashTable<T>& obj);
    // TODO: Create the copy assignment;
    HashTable<T>& operator=(const HashTable<T>& objToMove);

    // These just disable the move constructor and move assignment
    HashTable<T>& operator=(HashTable<T>&& objToMove) = delete;
    HashTable(HashTable<T>&& objToMove) = delete;

    // TODO: supply these methods
    void create(const string& key, const T& item);// method for L - values
    T retrieve(const string& key); // method(return by value, acts as a read only retrieve)
    T& operator[](const string& key); // method(return by reference which can allow for modification of values)
    bool exists(const string& key);//  method(returns a boolean)
    void remove(const string& key); //method

private:

    unsigned int hash(const string& key) const;

    unsigned int capacity{ 20 };
    unsigned int count{ 0 };

    // These two will need to be dynamically allocated via make_shared<>()
    unique_ptr<int[]> statusArr;
    unique_ptr<pair<string, T>[]> keyValueArr;
};

//************************************************************
// Put HashTable consrtuctors and methods below here
//************************************************************

template <typename T>
HashTable<T>::HashTable() {
    statusArr = make_unique<int[]>(capacity);
    keyValueArr = make_unique<pair<string, T>[]>(capacity);
    for (unsigned int i = 0; i < capacity; i++) {
        statusArr[i] = 0;
    }
}

template <typename T>
HashTable<T>::HashTable(const HashTable<T>& obj) {
    this->capacity = obj.capacity;
    //this->count = obj.count;
    this->statusArr = make_unique<int[]>(capacity);
    this->keyValueArr = make_unique < pair<string, T>[]>(capacity);
    for (unsigned int i = 0; i < this->capacity; i++) {
        this->statusArr[i] = obj.statusArr[i];
        this->keyValueArr[i].first = obj.keyValueArr[i].first; //need to get key
        this->keyValueArr[i] = obj.keyValueArr[i];

    }
}

template <typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T>& obj) {
    if (this != &obj) {
        this->capacity = obj.capacity;
        for (unsigned int i = 0; i < this->capacity; i++) {
            this->statusArr[i] = obj.statusArr[i];
            this->keyValueArr[i].first = obj.keyValueArr[i].first;
            this->keyValueArr[i].second = obj.keyValueArr[i].second;
        }
    }
    return *this;
}


template <typename T>
void HashTable<T>::create(const string& key, const T& item) {
    int unsigned indexHash = hash(key);

    if (statusArr[indexHash] == 0 || statusArr[indexHash] == -1) {
        keyValueArr[indexHash].first = key;
        keyValueArr[indexHash].second = item;
        statusArr[indexHash] = 1;
        return;
    }
    for (indexHash; indexHash <= capacity; indexHash++) {
        if (indexHash == capacity) {
            indexHash = 0;
        }
        if (statusArr[indexHash] == 0 || statusArr[indexHash] == -1) {
            keyValueArr[indexHash].first = key;
            keyValueArr[indexHash].second = item;
            statusArr[indexHash] = 1;
            return;
        }

    }
}

template <typename T>
T HashTable<T>::retrieve(const string& key) {
    int unsigned indexHash = hash(key);

    if (keyValueArr[indexHash].first == key) {
        return keyValueArr[indexHash].second;
    }
    for (unsigned int i = indexHash; indexHash <= capacity; i++) {
        if (i == capacity) {
            i = 0;
        }
        if (keyValueArr[i].first == key) {
            return keyValueArr[i].second;
        }

        if (statusArr[i] == -1 || statusArr[i] == 0) {
            break;
        }
    }
    throw std::logic_error("its not hereee");

}

template <typename T>
T& HashTable<T>::operator[](const string& key) {

    int unsigned indexHash = hash(key);

    if (keyValueArr[indexHash].first == key) {
        return keyValueArr[indexHash].second;
    }
    for (unsigned int i = indexHash; indexHash <= capacity; i++) {
        if (i == capacity) {
            i = 0;
        }
        if (keyValueArr[i].first == key) {
            return keyValueArr[i].second;
        }

        if (statusArr[i] == -1 || statusArr[i] == 0) {
            break;
        }
    }
    throw std::logic_error("its not hereee");
}

template <typename T>
bool HashTable<T>::exists(const string& key) {
    int unsigned indexHash = hash(key);

    if (statusArr[indexHash] == 1) {
        if (keyValueArr[indexHash].first == key) {
            return (keyValueArr[indexHash].first == key);
        }
    }
    else if (statusArr[indexHash] == -1 || statusArr[indexHash] == 0) {
        return false;
    }
}

template <typename T>
void HashTable<T>::remove(const string& key) {
    int unsigned indexHash = hash(key);

    if (statusArr[indexHash] == -1 || statusArr[indexHash] == 0) {
        return;
    }
    else {
        statusArr[indexHash] = -1;
    }
}

//************************************************************
// Put HashTable consrtuctors and methods above here


//************************************************************
//Copyright 2022, Bradley Peterson, Weber State University, all rights reserved. (Feb 2022)

template <typename T>
unsigned int HashTable<T>::hash(const string& key) const {

    return std::hash<std::string>{}(key) % capacity;

}

template <typename T>
unsigned int HashTable<T>::getTotalCount() const {
    unsigned int count = 0;
    for (unsigned int i = 0; i < capacity; i++) {
        if (statusArr[i] == 1) {
            count++;
        }
    }
    return count;
}

template <typename T>
unsigned int HashTable<T>::getWorstClump() const {
    unsigned int clumpCount = 0;
    unsigned int maxClump = 0;
    for (unsigned int i = 0; i < capacity; i++) {
        if (statusArr[i] == 1) {
            clumpCount++;
        }
        else {
            if (clumpCount > maxClump) {
                maxClump = clumpCount;
                clumpCount = 0;
            }
        }
    }
    if (clumpCount > maxClump) {
        maxClump = clumpCount;
    }
    return maxClump;
}


//This helps with testing, do not modify.
template <typename T>
string NumberToString(T Number)
{
    stringstream ss;
    ss << Number;
    return ss.str();
}

//This helps with testing, do not modify.
bool checkEmpty(string testName, string whatItIs) {

    if (whatItIs != "") {
        cout << "Passed " << testName << ", the data was " << whatItIs << endl;
        return true;
    }
    else {
        cout << "***Failed test " << testName << " *** " << endl << "   No data was found! " << endl;
        return false;
    }
}

//This helps with testing, do not modify.
bool checkTest(string testName, string whatItShouldBe, string whatItIs) {

    if (whatItShouldBe == whatItIs) {
        cout << "Passed " << testName << endl;
        return true;
    }
    else if (whatItShouldBe == "") {
        cout << "****** Failed test " << testName << " ****** " << endl << "     Output was '" << whatItIs << endl << "'     Output should have been blank" << endl;
        return false;

    }
    else {
        cout << "****** Failed test " << testName << " ****** " << endl << "     Output was " << whatItIs << endl << "     Output should have been " << whatItShouldBe << endl;
        return false;
    }
}

//This helps with testing, do not modify.
bool checkTest(string testName, int whatItShouldBe, int whatItIs) {

    if (whatItShouldBe == whatItIs) {
        cout << "Passed " << testName << endl;
        return true;
    }
    else {
        cout << "****** Failed test " << testName << " ****** " << endl << "     Output was " << whatItIs << endl << "     Output should have been " << whatItShouldBe << endl;
        return false;
    }
}

//This helps with testing, do not modify.
bool checkTestMemory(string testName, int whatItShouldBe, int whatItIs) {

    if (whatItShouldBe == whatItIs) {
        cout << "Passed " << testName << endl;
        return true;
    }
    else {
        cout << "***Failed test " << testName << " *** " << endl << "  There are " << whatItIs << " bytes of memory yet to be reclaimed!" << endl;
        return false;
    }
}

//This helps with testing, do not modify.
void testSimpleIntHash() {

    HashTable<string> myHash;

    //Test #1, add "Monster Mash" into our hash with a key of 619238.  Try to retrieve it.

    string myString = "Monster Mash";

    myHash.create("619238", myString);
    checkTest("testSimpleIntHash #1", "Monster Mash", myHash.retrieve("619238"));

    ////Test #2, attempt to get the Monster Mash using the operator[] code.
    checkTest("testSimpleIntHash #2", "Monster Mash", myHash["619238"]);

    //Test #3, attempt to change the value at this location:

    myHash["619238"] = "Graveyard Smash";

    checkTest("testSimpleIntHash #3", "Graveyard Smash", myHash["619238"]);

    //Test #4,
    //See if we can find it
    if (myHash.exists("619238")) {
        checkTest("testSimpleIntHash #4", "", "");
    }
    else {
        checkTest("testSimpleIntHash #4", "This test should have found an item with key \"619238\"", "This test did NOT find an item with key \"619238\"");
    }

    ////Test #5, see if we can find something that doesn't exist yet.
    if (myHash.exists("1234")) {
        checkTest("testSimpleIntHash #5", "This test should NOT have found an item with key \"1234\".", "This test found an item with key \"1234\"");
    }
    else {
        checkTest("testSimpleIntHash #5", "", "");
    }

    //Test #6, removing it from the hash
    myHash.remove("619238");
    if (myHash.exists("619238")) {
        checkTest("testSimpleIntHash #6", "This test should NOT have found an item with key \"619238\".", "This test found an item with key \"619238\"");
    }
    else {
        checkTest("testSimpleIntHash #6", "", "");
    }

    //Add add in "The Monster Hash" values.  
    myHash.create("13", "flash");
    myHash.create("25", "vampires feast");
    myHash.create("101", "humble abodes");
    myHash.create("77", "Transylvania Twist");
    myHash.create("12", "zombies");
    myHash.create("42", "Wolfman");
    myHash.create("97", "Dracula");
    myHash.create("123", "Dracula's son");
    myHash.create("500", "coffin-bangers");

    //Attempt to retrieve some
    checkTest("testSimpleIntHash #7", "zombies", myHash.retrieve("12"));
    checkTest("testSimpleIntHash #8", "Transylvania Twist", myHash.retrieve("77"));
    checkTest("testSimpleIntHash #9", "Dracula's son", myHash.retrieve("123"));

    //Now count up how many are in there
    checkTest("testSimpleIntHash #10", 9, myHash.getTotalCount());

    //Now just verify that they aren't clumping up badly.
    int worst = 0;
    worst = myHash.getWorstClump();
    if (worst > 7) {
        cout << "***Failed testSimpleIntHash #11!  There exists a clump of " << worst << " consecutive items, it shouldn't be worse than 7." << endl;
    }
    else {
        cout << "Passed testSimpleIntHash #11.  Your worst clump was " << worst << " items." << endl;
    }

}

void testHashOfObjects() {

    HashTable<Product> myHash;

    //Test #1, add in a studentObject.  Try to retrive it.
    Product tempProduct;
    tempProduct.setCost(5);
    tempProduct.setName("Silly string");
    myHash.create("12341-51231", tempProduct);
    checkTest("testHashOfObjects #1", "Silly string", myHash.retrieve("12341-51231").getName());

    //Test #2, attempt to get the product using its ID code
    checkTest("testHashOfObjects #2", "Silly string", myHash["12341-51231"].getName());

    //Test #3, see what happens if two products have the same ID code.  This should overwrite the former.
    tempProduct.setCost(18);
    tempProduct.setName("Novelty foam hat");
    myHash["12341-51231"] = tempProduct;
    checkTest("testHashOfObjects #3", "Novelty foam hat", myHash["12341-51231"].getName());

    //Test #4,
    //See if we can find it
    if (myHash.exists("12341-51231")) {
        checkTest("testHashOfObjects #4", "", "");
    }
    else {
        checkTest("testHashOfObjects #4", "This test should have found an item with key 12341-51231", "This test did NOT find an item with key 12341-51231");
    }

    //Test #5, see if we can find something that doesn't exist yet.
    if (myHash.exists("56756-75675")) {
        checkTest("testHashOfObjects #5", "This test should NOT have found an item with key 56756-75675.", "This test found an item with key56756-75675");
    }
    else {
        checkTest("testHashOfObjects #5", "", "");
    }

    //Test #6, removing it from the hash
    myHash.remove("12341-51231");
    if (myHash.exists("12341-51231")) {
        checkTest("testHashOfObjects #6", "This test should NOT have found an item with key 12341-51231.", "This test found an item with key 12341-51231");
    }
    else {
        checkTest("testHashOfObjects #6", "", "");
    }

}

void testCopies() {

    HashTable<string> myHash;

    for (char letter = 'a'; letter <= 'n'; letter++) {
        string sLetter;
        sLetter += letter;
        myHash.create(sLetter, sLetter);
    }
    HashTable<string> copyConstructorObj(myHash);
    if (myHash["a"] == "a" && copyConstructorObj["a"] == "a") {
        checkTest("testCopies #1", "", "");
    }
    else {
        checkTest("testCopies #1", "The values for keys \"a\" didn't match", "The values for keys \"a\" matched");
    }

    if (myHash["n"] == "n" && copyConstructorObj["n"] == "n") {
        checkTest("testCopies #2", "", "");
    }
    else {
        checkTest("testCopies #2", "The values for keys \"n\" didn't match", "The values for keys \"n\" matched");
    }

    HashTable<string> copyAssignmentObj;
    copyAssignmentObj = myHash;
    if (myHash["a"] == "a" && copyAssignmentObj["a"] == "a") {
        checkTest("testCopies #3", "", "");
    }
    else {
        checkTest("testCopies #3", "The values for keys \"a\" didn't match", "The values for keys \"a\" matched");
    }

    if (myHash["n"] == "n" && copyAssignmentObj["n"] == "n") {
        checkTest("testCopies #4", "", "");
    }
    else {
        checkTest("testCopies #4", "The values for keys \"n\" didn't match", "The values for keys \"n\" matched");
    }

    copyAssignmentObj = copyAssignmentObj;
    if (myHash["a"] == "a" && copyAssignmentObj["a"] == "a") {
        checkTest("testCopies #5", "", "");
    }
    else {
        checkTest("testCopies #5", "The values for keys \"a\" didn't match", "The values for keys \"a\" matched");
    }

    if (myHash["n"] == "n" && copyAssignmentObj["n"] == "n") {
        checkTest("testCopies #6", "", "");
    }
    else {
        checkTest("testCopies #6", "The values for keys \"n\" didn't match", "The values for keys \"n\" matched");
    }

}

void pressEnterToContinue() {
    cout << "Press any key to continue...";

    cout << endl;
}

int main() {

    testSimpleIntHash();
    pressEnterToContinue();

    testHashOfObjects();
    pressEnterToContinue();

    testCopies();
    pressEnterToContinue();

    pressEnterToContinue();
    return 0;
}