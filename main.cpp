#include <iostream>
#include <map>
#include <string>

//Clasa pentru generat ID-uri
class IDGenerator {
private:
    static int id;
public:
    static int generateID() {
        return ++id;
    }
};
int IDGenerator::id = 0;

template<typename T>
class Avion {
protected:
    int id;
    std::string name;
public:
    Avion(std::string name) : id(IDGenerator::generateID()), name(name) {}

    int getID() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    virtual void fly() {
        std::cout << "Avionul " << name << " zboara" << std::endl;
    }

    // Metoda clone
    virtual Avion<T>* clone() const {
        return new Avion<T>(*this);
    }
};

// Clasa template specializata
class AvionMilitar : public Avion<AvionMilitar> {
private:
    std::string misiune;
public:
    AvionMilitar(std::string name, std::string misiune) : Avion<AvionMilitar>(name), misiune(misiune) {}

    std::string getMission() const {
        return misiune;
    }

    void fly() override {
        std::cout << "Avionul militar " << name << " zboara in misiune spre " << misiune << std::endl;
    }

    AvionMilitar* clone() const {
        return new AvionMilitar(*this);
    }
};

// Clasa builder
class AvionBuilder {
private:
    std::string name;
    std::string misiune;

public:
    AvionBuilder& setName(std::string name) {
        this->name = name;
        return *this;
    }

    AvionBuilder& setMission(std::string misiune) {
        this->misiune = misiune;
        return *this;
    }

    AvionMilitar* build() {
        return new AvionMilitar(name, misiune);
    }
};

int main() {
    std::map<int, AvionMilitar*> avioane;
    AvionBuilder builder;
    while (true) {
        std::cout << "1. Creeaza avion" << std::endl;
        std::cout << "2. Zboara avion" << std::endl;
        std::cout << "3. Cloneaza avion" << std::endl;
        std::cout << "4. Iesire" << std::endl;

        int alegere;
        std::cin >> alegere;

        if (alegere == 1) {
            std::string name;
            std::string misiune;
            std::cout << "Introdu numele avionului: ";
            std::cin >> name;
            std::cout << "Introdu misiunea avionului: ";
            std::cin >> misiune;
            AvionMilitar *avion = builder.setName(name).setMission(misiune).build();
            avioane[avion->getID()] = avion;
            std::cout << "Avionul creat cu ID-ul: " << avion->getID() << std::endl;
        } else if (alegere == 2) {
            int id;
            std::cout << "Introdu ID-ul avionului care trebuie sa zboare: ";
            std::cin >> id;
            if (avioane.find(id) != avioane.end()) {
                avioane[id]->fly();
            } else {
                std::cout << "ID invalid" << std::endl;
            }
        } else if (alegere == 3) {
            int id;
            std::cout << "Introdu ID-ul avionului care trebuie clonat: ";
            std::cin >> id;
            if (avioane.find(id) != avioane.end()) {
                AvionMilitar *clonaAvion = avioane[id]->clone();
                avioane[clonaAvion->getID()] = clonaAvion;
                std::cout << "Avion clonat cu ID-ul: " << clonaAvion->getID() << std::endl;
                std::cout << "Nume: "<<clonaAvion->getName()<<"\n";
                std::cout << "Misiune: "<<clonaAvion->getMission()<<"\n";
            } else {
                std::cout << "ID invalid" << std::endl;
            }
        } else if (alegere == 4) {
            break;
        } else {
            std::cout << "Alegere invalida" << std::endl;
        }
    }

    // Clean-up automat pentru obiectele alocate dinamic
    for (auto &kv : avioane) {
        delete kv.second;
    }
    return 0;
}
