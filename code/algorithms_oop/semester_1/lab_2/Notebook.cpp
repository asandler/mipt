#include <iostream>
#include <string>
#include <list>

#define SIMPLE 2309

using namespace std;

struct item {
    string surname, number;
};

list<item> a[2500];

int h(string s) {
    int t = 0;
    for (size_t i = 0; i < s.length(); ++i)
        t = (t + (i + 1) * s[i]) % SIMPLE;
    return t;
}

void PrintItem(int t, list<item>::const_iterator& it) {
    cout << it->surname << ' ' << it->number << endl;
}

void Do_Nothing(int t, list<item>::const_iterator& it) {
}

void DeleteItem(int t, list<item>::const_iterator& it) {
    list<item>::const_iterator _it = it;
    it++;
    a[t].erase(_it);
}

bool Go(int t, string surname, void (*act)(int, list<item>::const_iterator&)) {
    bool flag = false;
    list<item>::const_iterator it = a[t].begin();
    while (it != a[t].end()) {
        if (it->surname == surname || surname == "") {
            act(t, it);
            flag = true;
        }
        if (it != a[t].end())
            it++;
    }
    return flag;
}

string Add(string surname, string number) {
    int t = h(surname);
    if (Go(t, surname, &Do_Nothing))
        return "ALREADY EXISTS\n";
    item ad;
    ad.surname = surname;
    ad.number = number;
    a[t].push_back(ad);
    return "ADDED\n";
}

string Delete(string surname) {
    int t = h(surname);
    return Go(t, surname, &DeleteItem) ? "DELETED\n" : "NOT FOUND\n";
}

string Find(string surname) {
    int t = h(surname);
    return Go(t, surname, &PrintItem) ? "" : "NOT FOUND\n";
}

void List() {
    cout << "-----PHONE_BOOK------" << endl;
    for (int i = 0; i < SIMPLE; ++i)
        Go(i, "", &PrintItem);
    cout << "---------------------" << endl;
}

void Help() {
    cout << "-----HELP-----" << endl;
    cout << "---Commands---" << endl;
    cout << "ADD Surname Number" << endl;
    cout << "FIND Surname" << endl;
    cout << "DELETE Surname" << endl;
    cout << "LIST" << endl;
    cout << "HELP" << endl;
}

void ChooseAct(string d) {
    string surname, number;
    if (d == "ADD") {
        cin >> surname >> number;
        cout << Add(surname, number);
    } else if (d == "DELETE") {
        cin >> surname;
        cout << Delete(surname);
    } else if (d == "FIND") {
        cin >> surname;
        cout << Find(surname);
    } else if (d == "LIST")
        List();
    else if (d == "HELP")
        Help();
    else
        cout << "SYNTAX ERROR\n";
}

void Cast(string& s) {
    string t;
    for (size_t i = 0; i < s.length(); ++i)
        if (s[i] >= 'a' && s[i] <= 'z') {
            char c = s[i] - 'a' + 'A';
            t = t + c;
        } else
            t = t + s[i];
    s = t;
}

int main() {
    string d;
    cin >> d;
    Cast(d);
    while (d != "EXIT") {
        ChooseAct(d);
        cin >> d;
        Cast(d);
    }
    return 0;
}