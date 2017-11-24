#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <math.h>
#include <unordered_map>
#include <algorithm>

using namespace std;

unordered_map<string, vector<int> > BigMemory;

class WeakClassifier {
public:
    WeakClassifier(int _step) : step(_step), number(0) {}

    vector<int> Classify(string key) {
        if (BigMemory.find(key) != BigMemory.end()) {
            return BigMemory[key];
        }
        size_t first = key.find_first_of(","), last = key.find_last_of(",");
        istringstream in(key.substr(0, first) + " " + key.substr(first + 1, last - first - 1) + " " + key.substr(last + 1, key.length() - last));
        int x, y, z;
        in >> x >> y >> z;
        vector<int> answer;
        for (int i = 0; i < 256; i += step) {
            answer.push_back((x > i) ? 1 : -1);
            answer.push_back((y > i) ? 1 : -1);
            answer.push_back((z > i) ? 1 : -1);
            answer.push_back((x < i) ? 1 : -1);
            answer.push_back((y < i) ? 1 : -1);
            answer.push_back((z < i) ? 1 : -1);
            answer.push_back((x + y > i) ? 1 : -1);
            answer.push_back((y + z > i) ? 1 : -1);
            answer.push_back((z + x > i) ? 1 : -1);
            answer.push_back((x + y < i) ? 1 : -1);
            answer.push_back((y + z < i) ? 1 : -1);
            answer.push_back((z + x < i) ? 1 : -1);
            answer.push_back((x + y + z > i) ? 1 : -1);
            answer.push_back((x + y + z < i) ? 1 : -1);
        }
        BigMemory[key] = answer;
        return answer;
    }

    int ClassifierNumber() {
        if (number == 0) {
            for (int i = 0; i < 256; i += step) {
                number += 14;
            }
        }
        return number;
    }
private:
    int step, number;
};

class AdaBoost {
public:
    AdaBoost(int _step) : C(_step) {}

    void Train(vector<pair<string, int> > train) {
        vector<double> w, errors;
        vector<int> features;

        w.assign(train.size(), 1.0 / train.size());
        size_t T = C.ClassifierNumber(), t = 0;
        bool cleaned = false;

        while (t++ < 70) {
            errors.assign(T, 0);
            for (size_t i = 0; i < train.size(); ++i) {
                features = C.Classify(train[i].first);
                for (size_t j = 0; j < T; ++j) {
                    errors[j] += (features[j] == train[i].second) ? 0 : w[i];
                }
            }
                for (size_t j = 0; j < T; ++j) {
                    if (errors[j] == 0) {
                        errors[j] = 100;
                    }
                }

            double e = 2;
            size_t ind_min;
            for (size_t j = 0; j < T; ++j) {
                if (errors[j] < e) {
                    e = errors[j];
                    ind_min = j;
                }
            }

            if (0.5 - e < 10e-5) {
                return;
            }

            double alpha = 0.5 * log((1 - e) / e);
            cerr << t << ' ' << e << ' ' << endl;
            answer.push_back(make_pair(alpha, ind_min));

            double w0 = 0;
            for (size_t i = 0; i < train.size(); ++i) {
                w[i] *= exp(-alpha * train[i].second * BigMemory[train[i].first][ind_min]);
                w0 += w[i];
            }
            for (size_t i = 0; i < train.size(); ++i) {
                w[i] /= w0;
            }
        }
    }

    int Classify(string key) {
        double sum = 0;
        vector<int> features = C.Classify(key);
        for (vector<pair<double, size_t> >::iterator it = answer.begin(); it != answer.end(); ++it) {
            sum += (*it).first * features[(*it).second];
        }
        return (sum > 0) ? 1 : -1;
    }

    void Test(vector<string> test) {
        int count = 1;
        cout << "id,y" << endl;
        for (vector<string>::iterator it = test.begin(); it != test.end(); ++it) {
            cout << count++ << ',' << Classify(*it) << endl;
        }
    }

    double Precision(vector<pair<string, int> > test) {
        int count = 0;
        int pl = 0, mun = 0;
        for (vector<pair<string, int> >::iterator it = test.begin(); it != test.end(); ++it) {
            if (Classify((*it).first) == (*it).second) {
                count++;
                if ((*it).second == 1) {
                    pl++;
                } else {
                    mun++;
                }
            }
        }
        cout << "plus = " << pl << ", minus = " << mun << endl;
        return (double)count / (double)test.size();
    }

private:
    vector<pair<double, size_t> > answer;
    WeakClassifier C;
};

int main() {
    vector<pair<string, int> > train, tiktak;
    vector<string> test;
    string s;
    bool flag = false;
    getline(cin, s);
    srand(time(0));
    while (getline(cin, s)) {
        size_t ind = s.find_last_of(",");
        size_t ind2 = s.find_first_of(",");
        if (s == "id,R,G,B") {
            flag = true;
        } else {
            if (!flag) {
                if (rand() % 100 > 30) {
                    train.push_back(make_pair(s.substr(0, ind), (s.substr(ind + 1, s.length() - ind - 1) == "1") ? 1 : -1));
                } else {
                    tiktak.push_back(make_pair(s.substr(0, ind), (s.substr(ind + 1, s.length() - ind - 1) == "1") ? 1 : -1));
                }
            } else {
                test.push_back(s.substr(ind2 + 1, s.length() - ind2));
            }
        }
    }
    cerr << "Train size = " << train.size() << endl;
    AdaBoost A(1);
    A.Train(train);
    cout << A.Precision(tiktak) << endl;
//    A.Test(test);
    return 0;
}
