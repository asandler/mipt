#pragma once

#include <iostream>
#include <cstdlib>

class TAnimal {
public:
        TAnimal() {}
        virtual int Eat() = 0;
        virtual int Sleep() = 0;
        virtual int Play() = 0;
        virtual int Walk() = 0;
        virtual bool Asleep() = 0;
        virtual void Act(int n) = 0;
        virtual bool End() = 0;
        virtual ~TAnimal() {}
};

class TDragon : public TAnimal {
public:
        TDragon() {
                PlusParametres();
        }
        virtual int Eat() {
                return eat;
        }
        virtual int Sleep() {
                return sleep;
        }
        virtual int Play() {
                return play;
        }
        virtual int Walk() {
                return walk;
        }
        virtual bool Asleep() {
                return asleep > 0;
        }
        virtual void Act(int n) {
                if (asleep > 0)
                        asleep--;
                else {
                        switch (n) {
                        case 1: //eating
                                if (eat > 0)
                                        eat--;
                                break;
                        case 2: //sleeping
                                if (eat == 0 && play == 0 && walk == 0) {
                                        asleep = sleep;
                                        sleep = 0;
                                }
                                break;
                        case 3: //playing
                                if (play > 0)
                                        play--;
                                break;
                        case 4: //walking
                                if (walk > 0)
                                        walk--;
                                break;
                        default:
                                return;
                                break;
                        }
                        PlusParametres();
                        if (eat > 5)
                                fail = (!fail) ? 1 : fail;
                        if (sleep > 5)
                                fail = (!fail) ? 2 : fail;
                        if (play > 3)
                                fail = (!fail) ? 3 : fail;
                        if (walk > 4)
                                fail = (!fail) ? 4 : fail;
                }
        }
        virtual bool End() {
                return fail > 0;
        }
        ~TDragon() {
                switch (fail) {
                case 1:
                        std::cout << "End! You have to feed your dragon!" << std::endl;
                        break;
                case 2:
                        std::cout << "End! You let your dragon sleep!" << std::endl;
                        break;
                case 3:
                        std::cout << "End! You have to play with your pet!" << std::endl;
                        break;
                case 4:
                        std::cout << "End! You have to walk your pet!" << std::endl;
                        break;
                }
        }
private:
        void PlusParametres() {
                if (rand() % 100 < 20)
                        ++eat;
                if (rand() % 100 < 20)
                        ++sleep;
                if (rand() % 100 < 15)
                        ++play;
                if (rand() % 100 < 15)
                        ++walk;
        }

        int eat, sleep, play, walk, asleep, fail;
};

class TMonkey : public TAnimal {
};

class TKitten : public TAnimal {
};

class TRabbit : public TAnimal {
};

void Greeting() {
        std::cout << "Welcome to tamagochi 1.0!" << std::endl;
        std::cout << "Game control: 1 - Eat, 2 - Sleep, 3 - Play, 4 - Walk" << std::endl;
        std::cout << "Exit: all other strings you enter" << std::endl;
        std::cout << "Choose an animal" << std::endl;
        std::cout << "Dragon - 1" << std::endl;
        std::cout << "Monkey - 2" << std::endl;
        std::cout << "Kitten - 3" << std::endl;
        std::cout << "Rabbit - 4" << std::endl;
}
