#pragma once

#include "draw.h"

#include <stdlib.h>
#include <list>

const size_t MAX_SIZE = 50;

class BaseObject;

class Field {
public:
    static Field& Instance() {
        static Field obj;
        return obj;
    }

    void initialize(int _h, int _w);
    void greeting();
    void redraw();

    void newDay();
    void pause();
    bool end() { return false; }

    void addAnt(BaseObject* A) { ants_.push_back(A); }
    void randomMove(BaseObject* A) {}

    int H() { return h; }
    int W() { return w; }
    int pAnt() { return p_ant; }
    int pBuilder() { return p_builder; }

    BaseObject* cell(int i0, int j0) { return cell_[i0][j0]; }

    std::list<BaseObject*>::const_iterator antsBegin() { return ants_.begin(); }
    std::list<BaseObject*>::const_iterator antsEnd() { return ants_.end(); }

private:
    Field() {}
    Field(const Field&);
    ~Field() {}

    std::list <BaseObject*> ants_;
    BaseObject* cell_[MAX_SIZE][MAX_SIZE];

    int h;
    int w;
    int p_ant;
    int p_queen;
    int p_cell;
    int p_builder;
};

class BaseObject {
public:
    BaseObject(int _i, int _j, int _p, int _m, Color _c, const wchar_t* _s)
        : i(_i), j(_j), p(_p), m(_m), color_(_c), str_(_s)
        {}

    virtual ~BaseObject() {}

    virtual bool act(Field& F) = 0;
    virtual void incMeal() = 0;
    virtual bool decMeal() = 0;
    virtual int hungry() = 0;

    void feed(BaseObject* A) {
        if (decMeal()) {
            A->incMeal();
        }
    }

    bool setCoord(int i0, int j0) {
        if (i0 >= 0 && i0 < Field::Instance().H() && j0 >= 0 && j0 < Field::Instance().W()) {
            i = i0;
            j = j0;
            return true;
        }

        return false;
    }

    int I() { return i; }
    int J() { return j; }
    int meal() { return m; }
    const wchar_t* str() { return str_; }
    Color color() { return color_; }

protected:
    int i;
    int j;
    int p;
    int m;
    Color color_;
    const wchar_t* str_;

private:
    BaseObject(const BaseObject &);
    BaseObject& operator=(const BaseObject &);
};

class Ant : public BaseObject {
public:
    Ant(int _i, int _j, int _p, int _m, Color _c, const wchar_t* _s)
        : BaseObject(_i, _j, _p, _m, _c, _s)
        {}

    bool act(Field& F);

    void incMeal() {
        ++m;
        color_ = ANT_WITH_MEAL_COLOR;
    }

    bool decMeal() {
        if (m <= 0) {
            return false;
        }
        if (--m == 0) {
            color_ = ANT_COLOR;
        }
    }

    int hungry() {
        return m ? 0 : 1;
    }

protected:
    void goTo(BaseObject* A);
    void getFood(BaseObject* A);
    void randomMove();
    void feedSomebody(BaseObject* A);
};

class Queen : public BaseObject {
public:
    Queen(int _i, int _j, int _p, int _m, Color _c, const wchar_t* _s)
        : BaseObject(_i, _j, _p, _m, _c, _s)
        {}

    bool act(Field& F);

    void incMeal() {
        ++m;
    }

    bool decMeal() {
        if (m <= 0) {
            return false;
        }
        --m;
        return true;
    }

    int hungry() {
        return 3;
    }
};

class Cell : public BaseObject {
public:
    Cell(int _i, int _j, int _p, int _m, Color _c, const wchar_t* _s)
        : BaseObject(_i, _j, _p, _m, _c, _s)
        {}

    bool act(Field& F);

    void incMeal() {
        ++m;
        if (m > 0) {
            str_ = FOOD_STR;
        }
    }

    bool decMeal() {
        --m;
        if (m == 0) {
            color_ = CELL_COLOR;
            str_ = EMPTY_STR;
        } else if (m < 0) {
            color_ = ROAD_COLOR;
            str_ = GROUND_STR;
        }
        return true;
    }

    int hungry() {
        return 0;
    }
};

class Builder : public BaseObject {
public:
    Builder(int _i, int _j, int _p, int _m, Color _c, const wchar_t* _s)
        : BaseObject(_i, _j, _p, _m, _c, _s)
        {}

    bool act(Field& F);

    void incMeal() {
        ++m;
    }

    bool decMeal() {
        if (m <= 0) {
            return false;
        }
        m--;
        return true;
    }

    int hungry() {
        return 2;
    }

private:
    void randomMove();
};
