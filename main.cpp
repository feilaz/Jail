#include <iostream>
#include "header.h"

// preprocessor conditions if value != false, given test case will be compiled and shown
#define JAIL_TEST_STATIC 1
#define JAIL_TEST_DYNAMIC 1

int main() {


#if JAIL_TEST_STATIC

    //                                    --- Restrictions, limits, assumptions ---
    Jail rla("ok", "pls work", 16, 70);
//Restrictions
    // R1
    // trying to assign code smaller than 0
    rla.addPrisoner("invalid", "code", 12, true, -10);
    // try to add prisoner with repeated code
    rla.addCell(1, maximum);
    rla.addPrisoner("original", "code", 12, true, 5);
    rla.addPrisoner("prisoner", "with repeated code", 12, true, 5);

    // R2 , create cell and check cell number(should be 0)
    rla.printCells();

    // R3, hash tab size was set to 70 during initialisation

//limits
    //L1 , trying to fit 3 prisoners in one cell will result in failure
    rla.addPrisoner("first", "prisoner", 12, true, 0, 0);
    rla.addPrisoner("second", "prisoner", 12, true, 1, 0);
    rla.addPrisoner("third", "prisoner", 12, true, 2, 0); // returns 0
//assumptions
    //A1
    std::cout << rla.addPrisoner("third", "prisoner", 12, true, 2, 0) << "\n"; // prints 0, see L1
    std::cout << rla.addCell(2, medium) << "\n";// prints 1, executed correctly
    std::cout << rla.addPrisoner("test", "prisoner", 12, true, 3) << "\n"; // prints 1, executed correctly
    std::cout << rla.addPrisoner("prisoner", "with repeated code", 12, true, 3) << "\n"; // prints 0, see R1
    //A4, adding prisoner without specifying cell, see previous points



    Jail testJail("nameHolder", "streetHolder", 16);

    // print info without cells and prisoners in Jail
    testJail.printCells();
    testJail.printPrisoners();
    testJail.printStat();
    testJail.printPrisoner(1);


// change info
    testJail.changeName("Warsaw Jail");
    testJail.changeStreet("JailStreet");
    testJail.changeStreetNr(5);

    testJail.printStat();

//                                 --- add cell test ---
    std::cout << "\n--- add cell test ---\n";
    testJail.addCell(-1, minimum);
    testJail.addCell(-1, minimum);
    testJail.addCell(-1, minimum);
    testJail.addCell(0, medium);
    testJail.addCell(1, medium);
    testJail.addCell(0, medium);
    testJail.addCell(0, maximum);
    testJail.addCell(1, maximum);
    testJail.addCell(1, maximum);

//                                 --- add prisoner test ---
    std::cout << "\n--- add prisoner test ---\n\n";
// with specified cell
    testJail.addPrisoner("Scarlett", "Solace", 182, false, 0, 0);
    testJail.addPrisoner("Waylon", "Kade", 92, true, 1, 3);
    testJail.addPrisoner("Jasmine", "Marley", 752, false, 2, 0);
    testJail.addPrisoner("Victor", "Victor Roberts", 482, true, 3, 3);

// with function searching for free cell
    testJail.addPrisoner("Nolan", "Hall", 182, false, 5);
    testJail.addPrisoner("Adriel", "Allen", 1820, false, 6, -1);

// wrong input test(all instances returns -1)
    testJail.addPrisoner("adam", "kowalski", -12, true, 10, 1); // wrong sentenceDaysLeft
    testJail.addPrisoner("adam", "kowalski", 12, true, 10, -10); // cell number < 0 and not == -1
    testJail.addPrisoner("adam", "kowalski", 12, true, 10, 100); //  non existent cell
    testJail.addPrisoner("adam", "kowalski", 12, true, 1, 4); //  repeated prisoner's code number
    testJail.addPrisoner("adam", "kowalski", 12, true, 10, 1); // already full cell


    testJail.printCells();
//                                 --- remove cell test ---
    std::cout << "\n--- remove cell test ---\n\n";
//remove cell without prisoners
    testJail.removeCell(8);
    testJail.removeCell(7, true, false);
//remove cells with prisoners
// observe that other cell numbers got decremented
    testJail.removeCell(0);
    testJail.removeCell(1, true, false);

    testJail.printCells();
// try to delete cell with prisoners while allowTrans = false
    testJail.removeCell(2, false, false);

// test wrong inputs
    testJail.removeCell(-1);
    testJail.removeCell(100);
    testJail.removeCell(2, false, true);

// test allowCellCreation
// remove empty cells in order to conduct the test
    testJail.removeCell(4);
    testJail.removeCell(3);
// print before test
    testJail.printCells();
// try to remove prisoners from cell without a place from them to be transferred to
    testJail.removeCell(0, true, false);
// now allowCellCreation = true
    testJail.removeCell(0, true, true);
// cell 5 with medium security on 0 floor was created
    testJail.printCells();


//                                 --- updatePrisoner test ---
    std::cout << "\n--- updatePrisoner test ---\n\n";
    testJail.printPrisoners();
    // rename Waylon Kade to
    // and Victor Roberts to ... and change his sentence
    testJail.updatePrisoner("Miles", "Adams", 92, true, 1);
    testJail.updatePrisoner("Victor", "Roberts", 220, true, 3);

    // try wrong inputs
    testJail.updatePrisoner("Scarlett", "Solace", -10, false, 0); // wrong sentence
    testJail.updatePrisoner("Scarlett", "Solace", 10, false, -5); // wrong code
    testJail.updatePrisoner("Scarlett", "Solace", 10, false, 6); // non-existing prisoner

    //                                 --- move prisoner test ---
    std::cout << "\n--- move prisoner test ---\n\n";
    // create cells to perform tests on
    testJail.addCell(2, medium);
    testJail.addCell(2, medium);
    // test
    testJail.movePrisoner(3, 3);
    testJail.movePrisoner(0, 3);

    testJail.printCells();
    // incorrect scenario test cases
    testJail.movePrisoner(3, 2); // try to move to full cell
    testJail.movePrisoner(7, 0); // non-existent prisoner
    testJail.movePrisoner(3, 6); // non-existent cell
    testJail.movePrisoner(-1, 0);
    testJail.movePrisoner(0, -3);

    testJail.printCells();
    testJail.printPrisoners();

    //                                 --- remove prisoner test ---
    std::cout << "\n--- remove prisoner test ---\n\n";
    //test
    testJail.removePrisoner(3);
    testJail.removePrisoner(6);
    // incorrect scenario test cases
    testJail.removePrisoner(12);//non-existent prisoner
    testJail.removePrisoner(-6); // wrong code

    testJail.printCells();
    testJail.printPrisoners();
    testJail.printStat();



//                                 --- exemplary use case ---
    std::cout << "\n--- exemplary use case ---\n\n";

    Jail example("Exemplary Jail", "Fast", 3, 15);

    //add available cells in prison
    example.addCell(0, minimum);
    example.addCell(0, minimum);
    example.addCell(0, minimum);
    example.addCell(0, minimum);

    example.addCell(1, medium);//4
    example.addCell(1, medium);
    example.addCell(1, medium);
    example.addCell(2, medium);
    example.addCell(2, medium);
    example.addCell(2, medium);

    example.addCell(2, maximum);//10
    example.addCell(2, maximum);

    // print cells to look up the cell's code
    example.printCells();

    //add prisoners
    // to minimum security cell
    example.addPrisoner("James", "Jones", 64, true, 0, 1);
    example.addPrisoner("Victoria", "Murphy", 96, false, 1, 0);
    example.addPrisoner("Erica", "Jones", 83, false, 2, 0);
    // to medium security cell
    example.addPrisoner("Jared", "Byrd", 365, true, 3, 4);
    example.addPrisoner("Patrick", "Carter", 192, true, 4, 4);
    example.addPrisoner("Jacob", "White", 543, true, 5, 5);
    example.addPrisoner("April", "Wilcox", 243, false, 6, 6);
    example.addPrisoner("Kathy", "Williamson", 832, false, 7, 6);
    // to maximum security cell
    example.addPrisoner("Evan", "Brown", 1623, true, 8, 10);
    example.addPrisoner("Russell", "Mcdonald", 9342, true, 9, 11);
    // to any cell
    example.addPrisoner("Samuel", "Hoffman", 10, true, 10);

    // cell 1 is under maintenance , thus is has to emptied end removed from list
    example.removeCell(1);

    // maintenance is finished, let's move prisoner 0 back to it
    example.addCell(0, minimum);
    example.printCells();
    // in the list of printed cell we can see that the cell was given number: 11
    example.movePrisoner(3, 11);

    // prisoner 10 sentence finished, let's remove him
    example.removePrisoner(10);

    // prisoner 6 took a divorce and changed surname, let's update that
    example.updatePrisoner("April", "Morrow", 243, false, 6);

    // print prisoners and statistics
    example.printPrisoners();
    example.printStat();
#endif

#if JAIL_TEST_DYNAMIC

    //                                    --- Restrictions, limits, assumptions ---
    auto *rlaD = new Jail("ok", "pls work", 16, 70);
//Restrictions
    // R1
    // trying to assign code smaller than 0
    rlaD->addPrisoner("invalid", "code", 12, true, -10);
    // try to add prisoner with repeated code
    rlaD->addCell(1, maximum);
    rlaD->addPrisoner("original", "code", 12, true, 5);
    rlaD->addPrisoner("prisoner", "with repeated code", 12, true, 5);

    // R2 , create cell and check cell number(should be 0)
    rlaD->printCells();

    // R3, hash tab size was set to 70 during initialisation

//limits
    //L1 , trying to fit 3 prisoners in one cell will result in failure
    rlaD->addPrisoner("first", "prisoner", 12, true, 0, 0);
    rlaD->addPrisoner("second", "prisoner", 12, true, 1, 0);
    rlaD->addPrisoner("third", "prisoner", 12, true, 2, 0); // returns 0
//assumptions
    //A1
    std::cout << rlaD->addPrisoner("third", "prisoner", 12, true, 2, 0) << "\n"; // prints 0, see L1
    std::cout << rlaD->addCell(2, medium) << "\n";// prints 1, executed correctly
    std::cout << rlaD->addPrisoner("test", "prisoner", 12, true, 3) << "\n"; // prints 1, executed correctly
    std::cout << rlaD->addPrisoner("prisoner", "with repeated code", 12, true, 3) << "\n"; // prints 0, see R1
    //A4, adding prisoner without specifying cell, see previous points

    delete rlaD;

    auto *testJailD = new Jail("nameHolder", "streetHolder", 16);

    // print info without cells and prisoners in Jail
    testJailD->printCells();
    testJailD->printPrisoners();
    testJailD->printStat();
    testJailD->printPrisoner(1);


// change info
    testJailD->changeName("Warsaw Jail");
    testJailD->changeStreet("JailStreet");
    testJailD->changeStreetNr(5);

    testJailD->printStat();

//                                 --- add cell test ---
    std::cout << "\n--- add cell test ---\n";
    testJailD->addCell(-1, minimum);
    testJailD->addCell(-1, minimum);
    testJailD->addCell(-1, minimum);
    testJailD->addCell(0, medium);
    testJailD->addCell(1, medium);
    testJailD->addCell(0, medium);
    testJailD->addCell(0, maximum);
    testJailD->addCell(1, maximum);
    testJailD->addCell(1, maximum);

//                                 --- add prisoner test ---
    std::cout << "\n--- add prisoner test ---\n\n";
// with specified cell
    testJailD->addPrisoner("Scarlett", "Solace", 182, false, 0, 0);
    testJailD->addPrisoner("Waylon", "Kade", 92, true, 1, 3);
    testJailD->addPrisoner("Jasmine", "Marley", 752, false, 2, 0);
    testJailD->addPrisoner("Victor", "Victor Roberts", 482, true, 3, 3);

// with function searching for free cell
    testJailD->addPrisoner("Nolan", "Hall", 182, false, 5);
    testJailD->addPrisoner("Adriel", "Allen", 1820, false, 6, -1);

// wrong input test(all instances returns -1)
    testJailD->addPrisoner("adam", "kowalski", -12, true, 10, 1); // wrong sentenceDaysLeft
    testJailD->addPrisoner("adam", "kowalski", 12, true, 10, -10); // cell number < 0 and not == -1
    testJailD->addPrisoner("adam", "kowalski", 12, true, 10, 100); //  non existent cell
    testJailD->addPrisoner("adam", "kowalski", 12, true, 1, 4); //  repeated prisoner's code number
    testJailD->addPrisoner("adam", "kowalski", 12, true, 10, 1); // already full cell


    testJailD->printCells();
//                                 --- remove cell test ---
    std::cout << "\n--- remove cell test ---\n\n";
//remove cell without prisoners
    testJailD->removeCell(8);
    testJailD->removeCell(7, true, false);
//remove cells with prisoners
// observe that other cell numbers got decremented
    testJailD->removeCell(0);
    testJailD->removeCell(1, true, false);

    testJailD->printCells();
// try to delete cell with prisoners while allowTrans = false
    testJailD->removeCell(2, false, false);

// test wrong inputs
    testJailD->removeCell(-1);
    testJailD->removeCell(100);
    testJailD->removeCell(2, false, true);

// test allowCellCreation
// remove empty cells in order to conduct the test
    testJailD->removeCell(4);
    testJailD->removeCell(3);
// print before test
    testJailD->printCells();
// try to remove prisoners from cell without a place from them to be transferred to
    testJailD->removeCell(0, true, false);
// now allowCellCreation = true
    testJailD->removeCell(0, true, true);
// cell 5 with medium security on 0 floor was created
    testJailD->printCells();


//                                 --- updatePrisoner test ---
    std::cout << "\n--- updatePrisoner test ---\n\n";
    testJailD->printPrisoners();
    // rename Waylon Kade to
    // and Victor Roberts to ... and change his sentence
    testJailD->updatePrisoner("Miles", "Adams", 92, true, 1);
    testJailD->updatePrisoner("Victor", "Roberts", 220, true, 3);

    // try wrong inputs
    testJailD->updatePrisoner("Scarlett", "Solace", -10, false, 0); // wrong sentence
    testJailD->updatePrisoner("Scarlett", "Solace", 10, false, -5); // wrong code
    testJailD->updatePrisoner("Scarlett", "Solace", 10, false, 6); // non-existing prisoner

    //                                 --- move prisoner test ---
    std::cout << "\n--- move prisoner test ---\n\n";
    // create cells to perform tests on
    testJailD->addCell(2, medium);
    testJailD->addCell(2, medium);
    // test
    testJailD->movePrisoner(3, 3);
    testJailD->movePrisoner(0, 3);

    testJailD->printCells();
    // incorrect scenario test cases
    testJailD->movePrisoner(3, 2); // try to move to full cell
    testJailD->movePrisoner(7, 0); // non-existent prisoner
    testJailD->movePrisoner(3, 6); // non-existent cell
    testJailD->movePrisoner(-1, 0);
    testJailD->movePrisoner(0, -3);

    testJailD->printCells();
    testJailD->printPrisoners();

    //                                 --- remove prisoner test ---
    std::cout << "\n--- remove prisoner test ---\n\n";
    //test
    testJailD->removePrisoner(3);
    testJailD->removePrisoner(6);
    // incorrect scenario test cases
    testJailD->removePrisoner(12);//non-existent prisoner
    testJailD->removePrisoner(-6); // wrong code

    testJailD->printCells();
    testJailD->printPrisoners();
    testJailD->printStat();

    delete testJailD;

//                                 --- exemplary use case ---
    std::cout << "\n--- exemplary use case ---\n\n";

    auto *exampleD = new Jail("Exemplary Jail", "Fast", 3, 15);

    //add available cells in prison
    exampleD->addCell(0, minimum);
    exampleD->addCell(0, minimum);
    exampleD->addCell(0, minimum);
    exampleD->addCell(0, minimum);

    exampleD->addCell(1, medium);//4
    exampleD->addCell(1, medium);
    exampleD->addCell(1, medium);
    exampleD->addCell(2, medium);
    exampleD->addCell(2, medium);
    exampleD->addCell(2, medium);

    exampleD->addCell(2, maximum);//10
    exampleD->addCell(2, maximum);

    // print cells to look up the cell's code
    exampleD->printCells();

    //add prisoners
    // to minimum security cell
    exampleD->addPrisoner("James", "Jones", 64, true, 0, 1);
    exampleD->addPrisoner("Victoria", "Murphy", 96, false, 1, 0);
    exampleD->addPrisoner("Erica", "Jones", 83, false, 2, 0);
    // to medium security cell
    exampleD->addPrisoner("Jared", "Byrd", 365, true, 3, 4);
    exampleD->addPrisoner("Patrick", "Carter", 192, true, 4, 4);
    exampleD->addPrisoner("Jacob", "White", 543, true, 5, 5);
    exampleD->addPrisoner("April", "Wilcox", 243, false, 6, 6);
    exampleD->addPrisoner("Kathy", "Williamson", 832, false, 7, 6);
    // to maximum security cell
    exampleD->addPrisoner("Evan", "Brown", 1623, true, 8, 10);
    exampleD->addPrisoner("Russell", "Mcdonald", 9342, true, 9, 11);
    // to any cell
    exampleD->addPrisoner("Samuel", "Hoffman", 10, true, 10);

    // cell 1 is under maintenance , thus is has to emptied end removed from list
    exampleD->removeCell(1);

    // maintenance is finished, let's move prisoner 0 back to it
    exampleD->addCell(0, minimum);
    exampleD->printCells();
    // in the list of printed cell we can see that the cell was given number: 11
    exampleD->movePrisoner(3, 11);

    // prisoner 10 sentence finished, let's remove him
    exampleD->removePrisoner(10);

    // prisoner 6 took a divorce and changed surname, let's update that
    exampleD->updatePrisoner("April", "Morrow", 243, false, 6);

    // print prisoners and statistics
    exampleD->printPrisoners();
    exampleD->printStat();

    delete exampleD;
#endif
    return 0;
}