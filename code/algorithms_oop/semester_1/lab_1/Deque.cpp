#include <iostream>

using namespace std;

struct item {
    item* pred;
    item* succ;
    int data;
};

item* list_new() //создать список
{
    item* f = new item;
    f->pred = f;
    f->succ = f;
    return f;
}

void list_delete(item*& f) //удалить список
{
    item* g = f->succ;
    while (f != g) {
        item* old = g;
        g = g->succ;
        free(old);
    }
    f->pred = f;
    f->succ = f;
}

void push(item*& f, int t) //добавить в конец
{
    item* g = f->succ;
    item* e = new item;
    e->data = t;
    e->pred = f;
    e->succ = g;
    f->succ = e;
    g->pred = e;
}

bool pop(item*& f, int& t) //убрать из конца
{
    if (f->succ == f)
        return false;
    item* g = f->succ;
    t = g->data;
    f->succ = g->succ;
    item* old = g;
    g = g->succ;
    g->pred = f;
    free(old);
    return true;
}

void unshift(item*& f, int t) //добавить в начало
{
    item* g = f->pred;
    item* e = new item;
    e->data = t;
    e->succ = f;
    e->pred = g;
    g->succ = e;
    f->pred = e;
}

bool shift(item*& f, int& t) //убрать из начала
{
    if (f->succ == f)
        return false;
    item* g = f->pred;
    t = g->data;
    f->pred = g->pred;
    item* old = g;
    g = g->pred;
    g->succ = f;
    free(old);
    return true;
}

void reverse(item*& f) //развернуть список
{
    item* g = f;
    item* g1 = new item;
    do {
        g1->pred = f->succ;
        g1->succ = f->pred;
        f->pred = g1->pred;
        f->succ = g1->succ;
        f = f->succ;
    } while (f != g);
    delete (g1);
}

void output(item* a) //вывести список
{
    cout << "back" << ' ';
    item* g = a->succ;
    while (g != a) {
        cout << g->data << ' ';
        g = g->succ;
    }
    cout << "front" << endl;
}
int main() {
    item* a = list_new();
    push(a, 5);
    push(a, 500);
    push(a, 200000);
    unshift(a, -17);
    unshift(a, 400);
    unshift(a, 22);
    cout << "list\n";
    output(a);
    reverse(a);
    cout << "reverse\n";
    output(a);
    list_delete(a);
    cout << "delete\n";
    output(a);
    push(a, 7);
    unshift(a, 56);
    cout << "list\n";
    output(a);
    int t = 0;
    cout << "pop " << pop(a, t);
    cout << ' ' << t << endl;
    cout << "shift " << shift(a, t);
    cout << ' ' << t << endl;
    cout << "list\n";
    output(a);
    list_delete(a);
    output(a);
    return 0;
}