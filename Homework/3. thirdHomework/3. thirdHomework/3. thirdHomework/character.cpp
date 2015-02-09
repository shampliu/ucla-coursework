////
////  character.cpp
////  3. thirdHomework
////
////  Created by Chang Liu on 2/8/15.
////  Copyright (c) 2015 Chang Liu. All rights reserved.
////
//
//#include <iostream>
//#include <string>
//using namespace std;
//
////Your declarations and implementations would go here
//class Character {
//public:
//    Character(string name) : m_name(name) { };
//    virtual ~Character() { };
//    string name() const { return m_name; };
//    virtual void printWeapon() const = 0;
//    virtual string attackAction() const { return "rushes toward the enemy"; }
//private:
//    string m_name;
//};
//
//class Dwarf : public Character {
//public:
//    Dwarf(string name) : Character(name) { };
//    virtual ~Dwarf() { cout << "Destroying " + this->name() + " the Dwarf" << endl; }
//    virtual void printWeapon() const { cout << "an axe"; };
//    
//};
//
//class Elf : public Character {
//public:
//    Elf(string name, int arrows) : Character(name), m_arrows(arrows) { };
//    virtual ~Elf() { cout << "Destroying " + this->name() + " the Elf" << endl; }
//    virtual void printWeapon() const { cout << "a bow and quiver of " << m_arrows << " arrows"; }
//private:
//    int m_arrows;
//    
//};
//
//class Boggie : public Character {
//public:
//    Boggie(string name) : Character(name) { };
//    virtual ~Boggie() { cout << "Destroying " + this->name() + " the Boggie" << endl; }
//    virtual void printWeapon() const { cout << "a short sword"; }
//    virtual string attackAction() const { return "whimpers"; }
//    
//};
//
//
//
//void strike(const Character* cp)
//{
//    cout << cp->name() << ", wielding ";
//    cp->printWeapon();
//    cout << ", " << cp->attackAction() << "." << endl;
//}
//
//int main()
//{
//    Character* characters[4];
//    characters[0] = new Dwarf("Gimlet");
//    // Elves have a name and initial number of arrows in their quiver
//    characters[1] = new Elf("Legolam", 10);
//    characters[2] = new Boggie("Frito");
//    characters[3] = new Boggie("Spam");
//    
//    cout << "The characters strike!" << endl;
//    for (int k = 0; k < 4; k++)
//        strike(characters[k]);
//    
//    // Clean up the characters before exiting
//    cout << "Cleaning up" << endl;
//    for (int k = 0; k < 4; k++)
//        delete characters[k];
//}
//
///*
// The characters strike!
// Gimlet, wielding an axe, rushes toward the enemy.
// Legolam, wielding a bow and quiver of 10 arrows, rushes toward the enemy.
// Frito, wielding a short sword, whimpers.
// Spam, wielding a short sword, whimpers.
// Cleaning up
// Destroying Gimlet the dwarf
// Destroying Legolam the elf
// Destroying Frito the boggie
// Destroying Spam the boggie
// */
