#include "ants.h"

#include <cmath>
#include <iostream>

size_t metric(BaseObject *A, BaseObject *B) {
    return abs(A->I() - B->I()) + abs(A->J() - B->J());
}

size_t metric(int i0, int j0, int i1, int j1) {
    return abs(i1 - i0) + abs(j1 - j0);
}

int Dec(int a, int m) {
    return (a <= 0) ? 1000000 : a - 1;
}

int Inc(int a, int m) {
    return (a >= m - 1) ? 1000000 : a + 1;
}

bool Ant::act(Field &F) {
    if (rand() % 100 < p) {
        return true;
    }

    BaseObject* T = NULL;
    size_t min_s = MAX_SIZE * 100;
    int want = 0;

    if (hungry()) {
        for (int i0 = 0; i0 < F.H(); ++i0) {
            for (int j0 = 0; j0 < F.W(); ++j0) {
                if (F.cell(i0, j0)->meal() > 0 &&
                    metric(this, F.cell(i0, j0)) < min_s)
                {
                    T = F.cell(i0, j0);
                    min_s = metric(this, T);
                }
            }
        }
        if (T != NULL) {
            getFood(T);
        }
    } else {
        for (std::list< BaseObject* >::const_iterator it = F.antsBegin(); it != F.antsEnd(); ++it) {
            if ((*it)->hungry() > want && metric(this, (*it)) < min_s) {
                T = *it;
                min_s = metric(this, T);
                want = (*it)->hungry();
            }
        }
        if (T != NULL) {
            feedSomebody(T);
        }
    }

    return false;
}

void Ant::goTo(BaseObject* A) {
    int act[4] = {0, 0, 0, 0};
    size_t size = 0;
    size_t curDistance = metric(this, A);

    if (metric(Dec(I(), Field::Instance().H()), J(), A->I(), A->J()) < curDistance)
        act[++size] = 1;
    if (metric(Inc(I(), Field::Instance().H()), J(), A->I(), A->J()) < curDistance)
        act[++size] = 2;
    if (metric(I(), Dec(J(), Field::Instance().W()), A->I(), A->J()) < curDistance)
        act[++size] = 3;
    if (metric(I(), Inc(J(), Field::Instance().W()), A->I(), A->J()) < curDistance)
        act[++size] = 4;

    while (true) {
        size_t choice = act[(rand() % size) + 1];
        switch (choice) {
            case 1:
                setCoord(Dec(I(), Field::Instance().H()), J()); return;
            case 2:
                setCoord(Inc(I(), Field::Instance().H()), J()); return;
            case 3:
                setCoord(I(), Dec(J(), Field::Instance().W())); return;
            case 4:
                setCoord(I(), Inc(J(), Field::Instance().W())); return;
        }
    }
}

void Ant::getFood(BaseObject* A) {
    if (metric(this, A) != 0) {
        if (rand() % 100 < 50) {
            goTo(A);
        } else {
            randomMove();
        }
    } else {
        A->feed(this);
    }
}

void Ant::feedSomebody(BaseObject* A) {
    if (metric(this, A) != 0) {
        if (rand() % 100 < 70) {
            goTo(A);
        } else {
            randomMove();
        }
    } else {
        feed(A);
    }
}

void Ant::randomMove() {
    while (true) {
        switch (rand() % 4) {
            case 0:
                if (setCoord(I() - 1, J())) { return; }
            case 1:
                if (setCoord(I() + 1, J())) { return; }
            case 2:
                if (setCoord(I(), J() - 1)) { return; }
            case 3:
                if (setCoord(I(), J() + 1)) { return; }
        }
    }
}

bool Queen::act(Field& F) {
    if (rand() % 100 < p) {
        return true;
    }
    if (m > 4) {
        if (rand() % 100 < 5) {
            F.addAnt(new Builder(i, j, F.pBuilder(), 0, BUILDER_COLOR, ANT_STR));
        } else {
            F.addAnt(new Ant(i, j, F.pAnt(), 0, ANT_COLOR, ANT_STR));
        }
        m -= 5;
    }
    return false;
}

bool Cell::act(Field& F) {
    if (rand() % 100 < p) {
        incMeal();
    }
    return false;
}

bool Builder::act(Field &F) {
    if (rand() % 100 < p) {
        return true;
    }

    randomMove();
    if (F.cell(i, j)->meal() == 0) {
        F.cell(i, j)->feed(this);
    }

    return false;
}

void Builder::randomMove() {
    while (true) {
        switch (rand() % 4) {
            case 0:
                setCoord(I() + 1, J()); return;
            case 1:
                setCoord(I() - 1, J()); return;
            case 2:
                setCoord(I(), J() - 1); return;
            case 3:
                setCoord(I(), J() + 1); return;
        }
    }
}

void Field::greeting() {
    put_str(9, 20, EMPTY_STR, WHITE_COLOR);
    put_str(9, 22, FOOD_STR, GREEN_COLOR);
    put_str(9, 24, GROUND_STR, WHITE_COLOR);
    put_str(9, 26, ANT_STR, YELLOW_COLOR);
    put_str(9, 28, QUEEN_STR, RED_COLOR);

    put_str(10, 14, L"Welcome to the Anthill!", WHITE_COLOR);
    put_str(11, 6, L"Please enter the parametres for each type of ants", WHITE_COLOR);
    put_str(13, 14, L"Working ant: ", WHITE_COLOR);
    scanw("%d", &p_ant);
    put_str(14, 14, L"Builder ant: ", WHITE_COLOR);
    scanw("%d", &p_builder);
    put_str(15, 14, L"Queen: ", WHITE_COLOR);
    scanw("%d", &p_queen);
    put_str(16, 14, L"Cell: ", WHITE_COLOR);
    scanw("%d", &p_cell);
    clear();
}

void Field::initialize(int _h, int _w) {
    greeting();

    h = (_h - 1) % MAX_SIZE + 1;
    w = (_w - 1) % MAX_SIZE + 1;

    for (int i1 = 0; i1 < h; i1++) {
        for (int j1 = 0; j1 < w; j1++) {
            if (i1 != h / 2 || j1 != w / 2) {
                cell_[i1][j1] = new Cell(i1, j1, p_cell, 1, CELL_COLOR, FOOD_STR);
            } else {
                cell_[i1][j1] = new Cell(i1, j1, p_cell, 0, CELL_COLOR, EMPTY_STR);
            }
        }
    }

    addAnt(new Queen(h / 2, w / 2, p_queen, 50, QUEEN_COLOR, QUEEN_STR));
}

void Field::pause() {
    refresh();
    wrefresh(window);
    usleep(80000);
}

void Field::redraw() {
    clear();

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cell_[i][j]->act(Field::Instance());
            put_str(i + 3, j * 2 + 10, cell_[i][j]->str(), cell_[i][j]->color());
        }
    }

    for (std::list<BaseObject*>::const_iterator it = ants_.begin(); it != ants_.end(); ++it) {
        put_str((*it)->I() + 3, (*it)->J() * 2 + 10, (*it)->str(), (*it)->color());
    }
}

void Field::newDay() {
    for (std::list<BaseObject*>::iterator it = ants_.begin(); it != ants_.end();) {
        if ((*it)->act(Field::Instance())) {
            BaseObject* A = *it;
            it = ants_.erase(it);
            delete A;
        } else {
            it++;
        }
    }
}
