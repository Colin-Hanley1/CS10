#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Info::Info() {
  this->name = "No Name Set";
  this->value = "No Value Set";
  this->next = nullptr;
}

Info::Info(std::string name, std::string value, Info *next) {
  this->name = name;
  this->value = value;
  this->next = next;
}

Contact::Contact() {
  this->first = "No First Set";
  this->last = "No Last Set";
  this->next = nullptr;
  this->headInfoList = nullptr;
}

Contact::Contact(std::string first, std::string last, Contact *next) {
  this->first = first;
  this->last = last;
  this->next = next;
  this->headInfoList = nullptr;
}

ContactList::ContactList() {
  this->headContactList = nullptr;
  this->count = 0;
}

int ContactList::getCount() { return this->count; }

// print the specified contact and its information
// 1. return false and print nothing if the contact is not in the list
// 2. otherwise return true and print the contact
bool ContactList::printContact(std::ostream &os, std::string first,std::string last) {
  Contact *currC = headContactList;
    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            os << "Contact Name: " << currC->first << " "<<currC->last<<endl;
            
            Info *currI = currC->headInfoList;
            while (currI != nullptr) {
                os << "    " << currI->name << " | " << currI->value <<endl;
                currI = currI->next;
            }

            return true;
        }
        currC = currC->next;
    }

    return false;
}

// print all contacts and their information
// print nothing if the list is empty
void ContactList::print(std::ostream &os) {
  Contact *currC = headContactList;

    while (currC != nullptr) {
       os << "Contact Name: " << currC->first << " "<<currC->last<<endl;

        Info *currI = currC->headInfoList;
        while (currI != nullptr) {
            os << "    " << currI->name << " | " << currI->value <<endl;
            currI = currI->next;
        }

        currC = currC->next;
        os << "\n";
    }
}

// add a contact to the back of the list
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the back of the list
// - do not forget to update count
bool ContactList::addContact(std::string first, std::string last) {
    Contact *currC = headContactList;
    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            return false;
        }
        currC = currC->next;
    }
    Contact *newContact = new Contact(first, last, nullptr);
    if (headContactList == nullptr) {
        headContactList = newContact;
    } else {
        Contact *lastContact = headContactList;
        while (lastContact->next != nullptr) {
            lastContact = lastContact->next;
        }
        lastContact->next = newContact;
    }
    count++;
    return true;
}

// add info to the back of a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and
// return true
// 3. otherwise add the info to the back of the contact's list and return true
bool ContactList::addInfo(std::string first, std::string last,
                          std::string infoName, std::string infoVal) {
   Contact *currC = headContactList;

    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            Info *currI = currC->headInfoList;
            while (currI != nullptr) {
                if (currI->name == infoName) {
                    currI->value = infoVal;
                    return true;
                }
                currI = currI->next;
            }

            Info *newInfo = new Info(infoName, infoVal, nullptr);
            if (currC->headInfoList == nullptr) {
                currC->headInfoList = newInfo;
            } else {
                Info *lastInfo = currC->headInfoList;
                while (lastInfo->next != nullptr) {
                    lastInfo = lastInfo->next;
                }
                lastInfo->next = newInfo;
            }

            return true;
        }
        currC = currC->next;
    }

    return false;                        
}

// add a contact to the list in ascending order by last name
//     if last names are equal, then order by first name ascending
// 1. return false and do nothing if the contact is already in the list
// 2. otherwise return true and add the contact to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
// - a compare method/function is recommended
bool ContactList::addContactOrdered(std::string first, std::string last) {
  Contact *currC = headContactList;
    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            return false;
        }
        currC = currC->next;
    }

    Contact *newContact = new Contact(first, last, nullptr);

    if (headContactList == nullptr || last < headContactList->last ||
        (last == headContactList->last && first < headContactList->first)) {
        newContact->next = headContactList;
        headContactList = newContact;
    } else {
        Contact *prevContact = nullptr;
        currC = headContactList;
        
        while (currC != nullptr && (last > currC->last ||
                                             (last == currC->last && first > currC->first))) {
            prevContact = currC;
            currC = currC->next;
        }

        newContact->next = currC;
        prevContact->next = newContact;
    }

    count++;
    return true;
  
  
}

// add info to a contact's info list in ascending order by infoName
// 1. return false and do nothing if the contact is not in the list
// 2. if the infoName is already in the info list, update the infoValue and
// return true
// 3. otherwise add the info to the contact's list and return true
bool ContactList::addInfoOrdered(std::string first, std::string last,
                                 std::string infoName, std::string infoVal) {
  Contact *currC = headContactList;

    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            Info *currI = currC->headInfoList;
            while (currI != nullptr) {
                if (infoName == currI->name) {
                    currI->value = infoVal;
                    return true;
                } else if (infoName < currI->name) {
                    Info *newInfo = new Info(infoName, infoVal, currI);
                    currC->headInfoList = newInfo;
                    return true;
                }

                Info *nextInfo = currI->next;
                if (nextInfo != nullptr && infoName < nextInfo->name) {
                    Info *newInfo = new Info(infoName, infoVal, nextInfo);
                    currI->next = newInfo;
                    return true;
                }

                currI = currI->next;
            }

            Info *newInfo = new Info(infoName, infoVal, nullptr);
            if (currC->headInfoList == nullptr) {
                currC->headInfoList = newInfo;
            } else {
                Info *lastInfo = currC->headInfoList;
                while (lastInfo->next != nullptr) {
                    lastInfo = lastInfo->next;
                }
                lastInfo->next = newInfo;
            }

            return true;
        }
        currC = currC->next;
    }

    return false;
}

// remove the contact and its info from the list
// 1. return false and do nothing if the contact is not in the list
// 2. otherwise return true and remove the contact and its info
// - do not forget to update count
bool ContactList::removeContact(std::string first, std::string last) {
  Contact *currC = headContactList;
    Contact *prevContact = nullptr;

    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            if (prevContact == nullptr) {
                headContactList = currC->next;
            } else {
                prevContact->next = currC->next;
            }

            delete currC;

            count--;
            return true;
        }

        prevContact = currC;
        currC = currC->next;
    }

    return false;
}

// remove the info from a contact's info list
// 1. return false and do nothing if the contact is not in the list
// 2. return false and do nothing if the info is not in the contact's info list
// 3. otherwise return true and remove the info from the contact's list
bool ContactList::removeInfo(std::string first, std::string last,
                             std::string infoName) {

  Contact *currC = headContactList;

    while (currC != nullptr) {
        if (currC->first == first && currC->last == last) {
            Info *prevInfo = nullptr;
            Info *currI = currC->headInfoList;

            while (currI != nullptr) {
                if (currI->name == infoName) {
                    if (prevInfo == nullptr) {
                        currC->headInfoList = currI->next;
                    } else {
                        prevInfo->next = currI->next;
                    }

                    delete currI;

                    return true;
                }

                prevInfo = currI;
                currI = currI->next;
            }

            return false;
        }

        currC = currC->next;
    }

    return false;
}

// destroy the list by removing all contacts and their infos
ContactList::~ContactList() {
    Contact *currC = headContactList;

    while (currC != nullptr) {
        Contact *tempContact = currC;
        currC = currC->next;

        Info *currI = tempContact->headInfoList;
        while (currI != nullptr) {
            Info *tempInfo = currI;
            currI = currI->next;
            delete tempInfo;
        }

        delete tempContact;
    }

}

// deep copy the source list
// - do not forget to update count
ContactList::ContactList(const ContactList &src) {
    headContactList = nullptr;
    count = 0;

    Contact *currC = src.headContactList;

    while (currC != nullptr) {
        addContact(currC->first, currC->last);

        Info *currI = currC->headInfoList;
        while (currI != nullptr) {
            addInfo(currC->first, currC->last, currI->name, currI->value);
            currI = currI->next;
        }

        currC = currC->next;
    }
}

// remove all contacts and their info then deep copy the source list
// - do not forget to update count
const ContactList &ContactList::operator=(const ContactList &src) {
if (this != &src) {
        Contact *currC = headContactList;

        while (currC != nullptr) {
            Contact *tempContact = currC;
            currC = currC->next;

            Info *currI = tempContact->headInfoList;
            while (currI != nullptr) {
                Info *tempInfo = currI;
                currI = currI->next;
                delete tempInfo;
            }

            delete tempContact;
        }

        headContactList = nullptr;
        count = 0;

        currC = src.headContactList;

        while (currC != nullptr) {
            addContact(currC->first, currC->last);

            Info *currI = currC->headInfoList;
            while (currI != nullptr) {
                addInfo(currC->first, currC->last, currI->name, currI->value);
                currI = currI->next;
            }

            currC = currC->next;
        }
    }

    return *this;
}
