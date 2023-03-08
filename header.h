#ifndef HEADER_H
#define HEADER_H

// Used for cell security rating
enum security {
    minimum, medium, maximum
};

// Responsible only for holing information of prisoner
// Function names are self-explanatory
class Prisoner {
private:
    char *name;
    char *surname;
    int code;
    int sentenceDaysLeft;
    bool isMale;

    void setName(const char *newName);

    void setSurname(const char *newSurname);

public:
    Prisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code);

    ~Prisoner();

    void changeName(const char *newName);

    void changeSurname(const char *newSurname);

    int &getSentenceRef();

    char *getName() const;

    char *getSurname() const;

    int getCode() const;

    bool &getIsMaleRef();

    void print();
};

//----------------------------------------------------------------------------------------------------------------------


class PrisonCell {
private:
    int cellNumber;
    int floor;
    security securityLevel;
    Prisoner *prisonerPtr[2];
public:
    PrisonCell(int cellNumber, int floor, security securityLevel);

    ~PrisonCell();

    int &getFloorRef();

    security &getSecurityLevelRef();

    int &getCellNumberRef();

    // Creates and allocates prisoner
    // Check weather cell has a free place and if inputs are correct
    // SentenceDaysLeft must be greater than zero
    // Code must be grater or equal to zero
    bool addPrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code);

    // Deallocates memory of chosen prisoner and sets pointer to null
    void removePrisoner(bool isFirst);

    // Checks if inputs are valid and if operation was executed successfully
    // Because code is used to difference prisoners, it can't be changed.
    // SentenceDaysLeft must be greater than zero
    // Code must be grater or equal to zero
    bool updatePrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code);

    // If isFirst equals true first prisoner info will be printed
    // Logically if isFirst equals false, second prisoner info will be printed
    // If the pointed prisoner doesn't exist, nothing will be printed
    bool printPrisoner(bool isFirst);

    // true if cell is full, false otherwise
    bool isCellFull();

    // true if cell is empty, false otherwise
    bool isCellEmpty();

    void printCell();

    Prisoner **getPrisonerPtr();
};

//----------------------------------------------------------------------------------------------------------------------

class CellList {
private:
    // max number of elements which array can hold
    int buffSize;
    // number of all cells in the list
    int elements;
    PrisonCell **list;

    // Reallocates list to double it's previous size
    bool expandList();

    void deallocateList();

    // Moves prisoners inside pointed cell to different one
    // If allowCellCreation equals true and there is no free place to transfer prisoners to
    // new cell on floor 0 with medium security rating is created
    // cellNumber must be equal or greater than zero
    bool transferPrisoners(int cellNumber, bool allowCellCreation = true);


public:
    CellList();

    ~CellList();

    // Searches and returns number of free cell found
    // cellException excludes the cell from search, even if it's free
    // When cellException is set to any number smaller than zero, it's turned off
    // If secondAvail is true, function will search for second free cell
    // If no cell meets the conditions functions returns -1
    int findFreeCell(int cellException = -1, bool secondAvail = false);

    // cell number is assigned automatically
    bool addCell(int floor, security securityLevel);

    // Checks if inputs are correct
    // If allowTrans is true and there are prisoners inside cell they will be moved to any free cell
    // If allowCellCreation and allowTrans is true new cell
    // will be created when there is no place to transfer prisoners to
    // If prisoners can't be transferred functions returns false and cell is not removed
    // allowsTrans can't be false when allowCellCreation is true
    // If cell is removed other cell numbers might be decremented in order to organise space in the array
    bool removeCell(int cellNumber, bool allowTrans = true, bool allowCellCreation = true);

    // Checks if inputs are correct
    // returns true when operation was successful
    bool movePrisoner(int fromCell, int toCell, bool isFirst);

    void printCells();

    void printPrisoners();

    void printStat();

    // returns number of prison cells in the list
    int getElements() const;

    // returns pointer to the list of cells
    PrisonCell **getListP();

};

// Used only for hash table
// Code is for prisoners code
// Place if for prisoners place, which is cellNumber*2 if prisoners is first in cell and cellNumber*2 + 1 if second
// next pointer is used for chaining in order to avoid collisions
struct HashNode {
    int code{};
    int place{};
    HashNode *next = nullptr;
};

// Class is used to quickly find the place(thus the cell) of prisoner with given code
class HashTab {
private:
    // The bigger the tabSize the faster will the algorithm be for large amount
    // of prisoners, but it will take more memory
    int tabSize;

    HashNode **tab;

    // Calculates index based of prisoner's code
    int hashFunction(int code);

    // because of chaining every row must be deallocated like linked list, thus this function.
    void deallocateRow(int row);

public:
    HashTab(int tabSize = 50);

    ~HashTab();

    void deallocateTab();

    // Adds association of prisoners code with place to the hash table
    void addPrisoner(int code, int place);

    // Remove the association of prisoners code with place from the hash table
    void removePrisoner(int code);

    // Main function of this class, returns prisoner's place,
    // which is cellNumber*2 if prisoners is first in cell and cellNumber*2 + 1 if second
    // If no prisoner is found returns -1
    int findPrisonerPlace(int code);

    void changePrisonerPlace(int code, int newPlace);
};

// Main class used for conducting operations
class Jail {
private:
    char *prisonName;
    char *street;
    int streetNr;
    CellList list;
    HashTab hashTab;

    void setName(const char *name);

    void setStreet(const char *street);

    // Rebuilds hash table
    // Is used in removeCell, to assign new places to prisoners after cell removal
    void makeHash();

public:
    Jail(const char *prisonName, const char *street, int streetNr, int hashTabSize = 50);

    ~Jail();

    // add cell to list with number automatically assigned
    bool addCell(int floor, security securityLevel);

    // Checks if inputs are correct
    // If allowTrans is true and there are prisoners inside cell they will be moved to any free cell
    // If allowCellCreation and allowTrans is true new cell
    // If removal is successful hash table is arranged to the changes
    // For more info see CellList::removeCell
    bool removeCell(int cell, bool allowTrans = true, bool allowCellCreation = true);

    // Print information about all the cells
    void printCells();

    // Print information about all the prisoners
    void printPrisoners();

    // Print information about one particular prisoner
    void printPrisoner(int code);

    // Prints prison info with statistics about number of prisoners in cells with particular safety rating
    void printStat();

    void changeName(const char *name);

    void changeStreet(const char *street);

    void changeStreetNr(int streetNr);

    // Checks if inputs are correct
    // Prisoner's code cannot be repeated
    // If cellNumber is equal to -1 free place in cell will be searched for automatically
    // Prisoner is added both to the cell list and hash table
    bool addPrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code,
                     int cellNumber = -1);

    // Function finds prisoner based on code
    // Checks if inputs are correct
    // Code is one and only parameter that can't be changed
    bool updatePrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code);

    // Function finds prisoner based on code
    // For the operation to be successfully destination cell must exist and not be empty
    bool movePrisoner(int code, int toCell);

    // Will remove prisoner from both cell list and hash table
    bool removePrisoner(int code);
};


#endif //HEADER_H
