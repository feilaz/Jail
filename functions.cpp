#include <cstring>
#include <iostream>
#include "header.h"

#define ERROR_MSG 0

/*--------------------------------------------------------------------------------------------------------------------*/
//                                                 class Prisoner
/*--------------------------------------------------------------------------------------------------------------------*/

Prisoner::Prisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code) {
    setName(name);
    setSurname(surname);
    this->code = code;
    this->sentenceDaysLeft = sentenceDaysLeft;
    this->isMale = isMale;
}

Prisoner::~Prisoner() {
    delete[] name;
    delete[] surname;
}

void Prisoner::setName(const char *newName) {
    char *temp;
    temp = strdup(newName);
    if (!temp) {
        std::cerr << "Prisoner::setName: Allocation memory failure!" << std::endl;
    }
    name = temp;
}

void Prisoner::setSurname(const char *newSurname) {
    char *temp;
    temp = strdup(newSurname);
    if (!temp) {
        std::cerr << "Prisoner::setSurname: Allocation memory failure!" << std::endl;
    }
    surname = temp;
}

void Prisoner::changeName(const char *newName) {
    delete[] name;
    setName(newName);
}

void Prisoner::changeSurname(const char *newSurname) {
    delete[] surname;
    setSurname(newSurname);
}

int &Prisoner::getSentenceRef() { return sentenceDaysLeft; }

char *Prisoner::getName() const { return name; }

char *Prisoner::getSurname() const { return surname; }

int Prisoner::getCode() const { return code; }

bool &Prisoner::getIsMaleRef() { return isMale; }

void Prisoner::print() {
    std::cout << "Name: " << name << "\n";
    std::cout << "Surname: " << surname << "\n";
    std::cout << "code: " << code << "\n";
    std::cout << "Sentence days left: " << sentenceDaysLeft << "\n";
    std::cout << "sex: ";
    std::cout << (isMale ? "Male" : "Female") << "\n";
}

/*--------------------------------------------------------------------------------------------------------------------*/
//                                              class PrisonCell
/*--------------------------------------------------------------------------------------------------------------------*/

PrisonCell::PrisonCell(int cellNumber, int floor, security securityLevel) {
    this->cellNumber = cellNumber;
    this->floor = floor;
    this->securityLevel = securityLevel;
    prisonerPtr[0] = nullptr;
    prisonerPtr[1] = nullptr;
}

PrisonCell::~PrisonCell() {
    removePrisoner(true);
    removePrisoner(false);
}

int &PrisonCell::getFloorRef() { return floor; }

security &PrisonCell::getSecurityLevelRef() { return securityLevel; }

int& PrisonCell::getCellNumberRef() { return cellNumber; }

void PrisonCell::printCell() {
    std::cout << "Cell number: " << cellNumber << "\n";
    std::cout << "Floor: " << floor << "\n";
    std::cout << "Security level: " << securityLevel << "\n";
    std::cout << "Prisoner 1 code: " << (prisonerPtr[0] ? std::to_string(prisonerPtr[0]->getCode()) : "None") << "\n";
    std::cout << "Prisoner 2 code: " << (prisonerPtr[1] ? std::to_string(prisonerPtr[1]->getCode()) : "None") << "\n\n";
}



bool PrisonCell::addPrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code) {
    if (isCellFull()) {
#if ERROR_MSG
        std::cout << "PrisonCell::addPrisoner: cell is already full\n";
#endif
        return false;
    }
    if (code < 0) {
#if ERROR_MSG
        std::cout << "PrisonCell::addPrisoner: code must be equal or greater than zero\n";
#endif
        return false;
    }
    if (sentenceDaysLeft <= 0) {
#if ERROR_MSG
        std::cout << "PrisonCell::addPrisoner: code must be greater than zero\n";
#endif
        return false;
    }
    auto *temp = new Prisoner(name, surname, sentenceDaysLeft, isMale, code);
    if (!temp) {
        std::cerr << "PrisonCell::addPrisoner: couldn't allocate memory\n";
    }
    prisonerPtr[!isCellEmpty()] = temp;
    return true;
}

void PrisonCell::removePrisoner(bool isFirst) {
    delete prisonerPtr[!isFirst];
    prisonerPtr[!isFirst] = nullptr;
}


bool PrisonCell::updatePrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code) {
    if (code < 0) {
#if ERROR_MSG
        std::cout << "PrisonCell::updatePrisoner: code must be equal or greater than zero\n";
#endif
        return false;
    }
    if (sentenceDaysLeft <= 0) {
#if ERROR_MSG
        std::cout << "PrisonCell::updatePrisoner: code must be greater than zero\n";
#endif
        return false;
    }
    bool isFirst;
    bool found = false;
    if (prisonerPtr[0]) {
        if (prisonerPtr[0]->getCode() == code) { isFirst = true; found = true; }
    }
    else if (prisonerPtr[1]) {
        if (prisonerPtr[1]->getCode() == code) { isFirst = false; found = true;}
    }
    if(!found) {
#if ERROR_MSG
        std::cout << "PrisonCell::updatePrisoner: no prisoner with " << code << " code inside cell\n";
#endif
        return false;
    }
    if (strcmp(name, prisonerPtr[!isFirst]->getName())) {
        prisonerPtr[!isFirst]->changeName(name);
    }
    if (strcmp(surname, prisonerPtr[!isFirst]->getSurname())) {
        prisonerPtr[!isFirst]->changeSurname(surname);
    }
    prisonerPtr[!isFirst]->getSentenceRef() = sentenceDaysLeft;
    prisonerPtr[!isFirst]->getIsMaleRef() = isMale;
    return true;
}

bool PrisonCell::printPrisoner(bool isFirst) {
    if (prisonerPtr[!isFirst]) {
        prisonerPtr[!isFirst]->print();
        return true;
    }
    return false;
}

bool PrisonCell::isCellFull() {
    if (prisonerPtr[0] && prisonerPtr[1]) {
        return true;
    }
    return false;
}

bool PrisonCell::isCellEmpty() {
    if (!prisonerPtr[0] && !prisonerPtr[1]) {
        return true;
    }
    return false;
}


Prisoner **PrisonCell::getPrisonerPtr() { return prisonerPtr; }

/*--------------------------------------------------------------------------------------------------------------------*/
//                                              class CellList
/*--------------------------------------------------------------------------------------------------------------------*/


CellList::CellList() {
    buffSize = 4;
    elements = 0;
    list = new PrisonCell*[buffSize]{};
}

CellList::~CellList() {
    deallocateList();
}


bool CellList::expandList() {
    buffSize *= 2;
    auto **temp = new PrisonCell *[buffSize]{};
    if (!temp) {
        std::cerr << "PrisonerList::expandList(): memory allocation problem\n";
        return false;
    }
    for (int i = 0; i < buffSize / 2 && list[i]; i++) {
        temp[i] = list[i];
    }
    delete []list;
    list = temp;
    return true;
}

bool CellList::addCell(int floor, security securityLevel) {
    if (buffSize <= elements) {
        expandList();
    }
    auto *newCell = new PrisonCell(elements, floor, securityLevel);
    if (!newCell) {
        std::cerr << "CellList::addCell Allocation memory failure!" << std::endl;
        return false;
    }
    list[elements] = newCell;
    elements++;
    return true;
}


bool CellList::removeCell(int cellNumber, bool allowTrans, bool allowCellCreation) {
    if (cellNumber >= elements || cellNumber < 0) {
#if ERROR_MSG
        std::cout << "CellList::removeCell: wrong cell number given\n";
#endif
        return false;
    }
    if(!list[cellNumber]){
#if ERROR_MSG
        std::cout << "CellList::removeCell: cell doesn't exist\n";
#endif
        return false;
    }
    if(!allowTrans && allowCellCreation){
#if ERROR_MSG
        std::cout << "CellList::removeCell: allowTrans can't be false while allowCellCreation is true\n";
#endif
    }
    if (!list[cellNumber]->isCellEmpty()) {
        if (!allowTrans) {
#if ERROR_MSG
            std::cout << "CellList::removeCell: cannot remove cell, prisoner inside and transition is not allowed\n";
#endif
            return false;
        }
        if(!transferPrisoners(cellNumber, allowCellCreation)){
            return false;
        }
    }
    delete list[cellNumber];
    for (int i = cellNumber; i < elements - 1; i++) {
        list[i + 1]->getCellNumberRef() -= 1;
        list[i] = list[i + 1];
    }
    elements -= 1;
    return true;
}


bool CellList::transferPrisoners(int cellNumber, bool allowCellCreation) {
    if (!list[cellNumber]->isCellEmpty()) {
        int moveTo = findFreeCell(cellNumber);
        if (!list[cellNumber]->isCellFull()) {
            if (moveTo == -1) {
                if (allowCellCreation) {
                    addCell(0, medium);
                    movePrisoner(cellNumber, elements - 1, list[cellNumber]->getPrisonerPtr()[0]);
                }
                else{
#if ERROR_MSG
      std::cout << "CellList::transferPrisoners: allowCellCreation is false and there is no place to transfer prisoners to\n"   ;
#endif
                    return false;
                }
            }
            else{movePrisoner(cellNumber, moveTo, list[cellNumber]->getPrisonerPtr()[0]);}
        } else {
            int moveTo2 = findFreeCell(cellNumber, true);
            if (moveTo == -1 || (!list[moveTo]->isCellEmpty() && moveTo2 == -1)) {
                if (allowCellCreation) {
                    addCell(0, medium);
                    movePrisoner(cellNumber, elements - 1, true);
                    movePrisoner(cellNumber, elements - 1, false);
                }
                else{
#if ERROR_MSG
                    std::cout << "CellList::transferPrisoners: allowCellCreation is false and there is no place to transfer prisoners to\n"   ;
#endif
                    return false;
                }
            }else {
                movePrisoner(cellNumber, (list[moveTo]->isCellEmpty() ? moveTo : moveTo2), true);
                movePrisoner(cellNumber, moveTo, false);
            }
        }
    }
    return true;
}

bool CellList::movePrisoner(int fromCell, int toCell, bool isFirst) {
    if (list[toCell]->isCellFull()) {
#if ERROR_MSG
        std::cout << "CellList::movePrisoner: cannot move prisoner to full cell\n";
#endif
        return false;
    }
    else if (toCell >= elements){
#if ERROR_MSG
        std::cout << "CellList::movePrisoner: destination cell doesn't exist\n";
#endif
        return false;
    }
    (list[toCell]->getPrisonerPtr()[0] ? list[toCell]->getPrisonerPtr()[1]
                                       : list[toCell]->getPrisonerPtr()[0]) = list[fromCell]->getPrisonerPtr()[!isFirst];
    list[fromCell]->getPrisonerPtr()[!isFirst] = nullptr;
    return true;
}

int CellList::findFreeCell(int cellException, bool secondAvail) {
    bool firstFound = false;
    for (int i = 0; i < elements; i++) {
        if (!list[i]->isCellFull() && i != cellException) {
            if (!firstFound && secondAvail) {
                firstFound = true;
            } else {
                return i;
            }
        }
    }
    return -1;
}

void CellList::printCells() {
    for (int i = 0; i < elements; i++) {
        list[i]->printCell();
    }
}

void CellList::printPrisoners() {
    for (int i = 0; i < elements; i++) {
        if(list[i]->printPrisoner(true)){
            std::cout << "Prisoner's place: " <<  i * 2 << "\n\n";}
        if(list[i]->printPrisoner(false)){
            std::cout << "Prisoner's place: " <<  i * 2 + 1 << "\n\n";}
    }
}

void CellList::printStat() {
    int prisonersCount[4] = {};
    security tempSec;
    for (int i = 0; i < elements; i++) {
        if (list[i]) {
            tempSec = list[i]->getSecurityLevelRef();
            if (list[i]->isCellFull()) {
                prisonersCount[0] += 2;
                prisonersCount[1 + tempSec] += 2;
            } else if (!list[i]->isCellFull() && !list[i]->isCellEmpty()) {
                prisonersCount[0]++;
                prisonersCount[1 + tempSec]++;
            }
        }
    }
    std::cout << "Prisoners overall: " << prisonersCount[0] << "\n";
    std::cout << "Prisoners in minimum security cells: " << prisonersCount[1] << "\n";
    std::cout << "Prisoners in medium security cells: " << prisonersCount[2] << "\n";
    std::cout << "Prisoners in maximum security cells: " << prisonersCount[3] << "\n\n";
}

void CellList::deallocateList() {
    for (int i = 0; i < elements; i++) {
        delete list[i];
    }
    delete[] list;
}

int CellList::getElements() const {
    return elements;
}

PrisonCell **CellList::getListP() {
    return list;
}

/*--------------------------------------------------------------------------------------------------------------------*/
//                                              class Jail
/*--------------------------------------------------------------------------------------------------------------------*/

Jail::Jail(const char *prisonName, const char *street, int streetNr, int hashTabSize): hashTab(hashTabSize) {
    setName(prisonName);
    setStreet(street);
    this->streetNr = streetNr;
}

Jail::~Jail() {
    delete[] prisonName;
    delete[] street;
}

void Jail::setName(const char *name) {
    char *temp = strdup(name);
    if (!temp) {
        std::cerr << "Jail::setName: Allocation memory failure!" << std::endl;
        return;
    }
    prisonName = temp;
}

void Jail::setStreet(const char *street) {
    char *temp = strdup(street);
    if (!temp) {
        std::cerr << "Jail::setName: Allocation memory failure!" << std::endl;
        return;
    }
    this->street = temp;
}

void Jail::changeName(const char *name) {
    delete[] prisonName;
    setName(name);
}

void Jail::changeStreet(const char *street) {
    delete[] this->street;
    setStreet(street);
}

bool Jail::addCell(int floor, security securityLevel) {
    return list.addCell(floor, securityLevel);
}

bool Jail::removeCell(int cell, bool allowTrans, bool allowCellCreation) {
     if(list.removeCell(cell, allowTrans, allowCellCreation)){
         makeHash();
         return true;
     }
    return false;
}

void Jail::printCells() {
    list.printCells();
}

void Jail::printPrisoners() {
    list.printPrisoners();
}

void Jail::printStat() {
    std::cout << "Prison name: " << prisonName << "\n";
    std::cout << "Street: " << street << " " << streetNr << "\n";
    list.printStat();
}

void Jail::makeHash(){
    PrisonCell** listP = list.getListP();
    hashTab.deallocateTab();
    for(int i = 0; i < list.getElements(); i++){
        if (listP[i]->getPrisonerPtr()[0]){
            hashTab.addPrisoner(listP[i]->getPrisonerPtr()[0]->getCode(), 2*i);
        }
        if (listP[i]->getPrisonerPtr()[1]){
            hashTab.addPrisoner(listP[i]->getPrisonerPtr()[1]->getCode(), 2*i + 1);
        }
    }
}

bool
Jail::addPrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code, int cellNumber) {
    if(hashTab.findPrisonerPlace(code) != -1){
#if ERROR_MSG
        std::cout << "Jail::addPrisoner: prisoner with this code already listed\n";
#endif
        return false;
    }
    int moveTo = cellNumber;
    if(cellNumber == -1){
        moveTo = list.findFreeCell();
    }
    if(moveTo == -1){
#if ERROR_MSG
        std::cout << "Jail::addPrisoner: couldn't find free cell\n";
#endif
        return false;
    }
    if(moveTo < 0 || moveTo >= list.getElements()){
#if ERROR_MSG
      std::cout << "Jail::addPrisoner: wrong cell number given\n";
#endif
        return false;
    }
    if(list.getListP()[moveTo]->addPrisoner(name, surname, sentenceDaysLeft, isMale, code)) {
        hashTab.addPrisoner(code, moveTo + (list.getListP()[moveTo]->isCellFull() ? 1 : 0));
        return true;
    }
    return false;
}

bool Jail::updatePrisoner(const char *name, const char *surname, int sentenceDaysLeft, bool isMale, int code) {
    int prisonerPlace = hashTab.findPrisonerPlace(code);
    if(prisonerPlace == -1){
#if ERROR_MSG
        std::cout << "Jail::updatePrisoner: couldn't find prisoner with given code\n";
#endif
        return false;
    }
    return list.getListP()[hashTab.findPrisonerPlace(code) / 2]->updatePrisoner(name, surname, sentenceDaysLeft, isMale, code);
}

bool Jail::movePrisoner(int code, int toCell) {
    int prisPlace = hashTab.findPrisonerPlace(code);
    if(prisPlace == -1){
#if ERROR_MSG
        std::cout << "Jail::movePrisoner: couldn't find prisoner with given code\n";
#endif
        return false;
    }
    if(toCell < 0 || list.getElements() <= toCell){
#if ERROR_MSG
        std::cout << "Jail::movePrisoner: wrong destination cell number\n";
#endif
        return false;
    }
    if(!list.movePrisoner(hashTab.findPrisonerPlace(code) / 2, toCell, !(hashTab.findPrisonerPlace(code) % 2))){return false;}
    hashTab.changePrisonerPlace(code, 2 * toCell + (list.getListP()[toCell]->isCellFull() ? 1 : 0));
    return true;
}

bool Jail::removePrisoner(int code) {
    int prisPlace = hashTab.findPrisonerPlace(code);
    if(prisPlace ==-1){
#if ERROR_MSG
      std::cout << "Jail::removePrisoner: couldn't find prisoner with given code\n";
#endif
        return false;
    }
    list.getListP()[prisPlace / 2]->removePrisoner(!(prisPlace % 2));
    hashTab.removePrisoner(code);
    return true;
}


void Jail::changeStreetNr(int streetNr) {
    this->streetNr = streetNr;
}

void Jail::printPrisoner(int code) {
    int prisonersPlace = hashTab.findPrisonerPlace(code);
    if (prisonersPlace == -1){
#if ERROR_MSG
        std::cout << "Jail::printPrisoner: no prisoner with this code in the jail\n";
#endif
        return;
    }
    list.getListP()[prisonersPlace / 2]->printPrisoner(prisonersPlace % 2);
    std::cout << "Prisoner's place: " << prisonersPlace << "\n\n";
}


/*--------------------------------------------------------------------------------------------------------------------*/
//                                              class HashTab
/*--------------------------------------------------------------------------------------------------------------------*/

int HashTab::hashFunction(int code) {
    return code % tabSize;
}

void HashTab::deallocateTab() {
    for (int i = 0; i < tabSize; i++) {
        if (tab[i]) {
            deallocateRow(i);
        }
    }
}

void HashTab::deallocateRow(int row) {
    HashNode *temp = tab[row];
    while (tab[row]) {
        tab[row] = tab[row]->next;
        delete temp;
        temp = tab[row];
    }
    tab[row] = nullptr;
}

HashTab::HashTab(int tabSize) {
    if (tabSize < 0) {
        std::cout << "HashTab::HashTab: incorrect tab size";
    }
    this->tabSize = tabSize;
    tab = new HashNode *[tabSize]{};
}

HashTab::~HashTab() {
    deallocateTab();
    delete[] tab;
}

void HashTab::addPrisoner(int code, int place) {
    auto *pNode = new HashNode{code, place};
    HashNode *temp = tab[hashFunction(code)];
    if(!temp){
        tab[hashFunction(code)] = pNode;
    }
    else {
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = pNode;
    }
}

void HashTab::removePrisoner(int code) {
    HashNode *temp[3] = {tab[hashFunction(code)], tab[hashFunction(code)]->next};
    if (temp[1]) {
        temp[2] = temp[1]->next;
    }
    if (temp[0]->code == code) {
        tab[hashFunction(code)] = temp[1];
        delete temp[0];
    }
    while (temp[1]) {
        if (temp[1]->code == code) {
            temp[0]->next = temp[2];
            delete temp[1];
            break;
        }
        temp[0] = temp[1];
        temp[1] = temp[2];
        if(temp[2] -> next){
            temp[2] = temp[2]->next;
        }
        else{temp[2] = nullptr;}
    }
}

int HashTab::findPrisonerPlace(int code) {
    if(code < 0){
#if ERROR_MSG
      std::cout << "HashTab::findPrisonerPlace: wrong code given\n";
#endif
return -1;
    }
    HashNode *temp = tab[hashFunction(code)];
    while (temp) {
        if (temp->code == code) {
            return temp->place;
        }
        temp = temp->next;
    }
    return -1;
}

void HashTab::changePrisonerPlace(int code, int newPlace){
    HashNode *temp = tab[hashFunction(code)];
    while (temp) {
        if (temp->code == code) {
            temp->place = newPlace;
        }
        temp = temp->next;
    }
}