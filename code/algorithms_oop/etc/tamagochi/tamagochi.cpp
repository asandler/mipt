#include <iostream>
#include "tamagochi.h"
#include <memory>
#include <ctime>
#include <stdio.h>

using namespace std;

int main() {
        srand((unsigned int)time(0));
        auto_ptr <TAnimal> Animal (new TDragon());
        Greeting();
        while (!Animal->End()) {
                if (!Animal->Asleep()) {
                        cout << "E=" << Animal->Eat() << ", ";
                        cout << "S=" << Animal->Sleep() << ", ";
                        cout << "P=" << Animal->Play() << ", ";
                        cout << "W=" << Animal->Walk() << endl;
                        cout << "Your move: ";
                        int move = 1;
                        cin >> move;
                        if (move > 4 || move < 1)
                                std::cout << "Net takogo xoda" << std::endl;
                        else
                                Animal->Act(move);
                } else {
                        Animal->Act(0);
                        cout << "Z";
                        fflush(stdout);
                        system("sleep 1");
                        if (!Animal->Asleep())
                                cout << endl;
                }
        }
}
