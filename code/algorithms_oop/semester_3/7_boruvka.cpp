#include <set>
#include <exception>
#include <vector>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/time.h>
#include <semaphore.h>
#include "dsu.h"

using namespace std;

map <string, pthread_mutex_t> Mutexes;
map <pair <string, string>, int> Edges, Answer;
map <string, pair <pair<string, string>, int> > Requests;
DSU <string> Leaders;
set <string> Vertices;
int THREADS_NUMBER, components;
pthread_t thr[10], mthr;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;
sem_t semafor;


void input() {
        int m, w;
        string s, t;
        cin >> THREADS_NUMBER >> components >> m;
        for (int i = 0; i < m; i++) {
                cin >> s >> t >> w;
                Edges[make_pair(s, t)] = w;
                Vertices.insert(s);
                Vertices.insert(t);
                pthread_mutex_init(&Mutexes[s], NULL);
                pthread_mutex_init(&Mutexes[t], NULL);
        }
}

void *consumer(void *ptr) {
        int start = (int) ptr;

        while (components > 1) {
                map <pair <string, string>, int>::iterator it = Edges.begin();
                size_t t = start;

        //        sem_wait(&semafor);
                std::advance(it, t);
                for (; t < Edges.size(); std::advance(it, THREADS_NUMBER), t += THREADS_NUMBER) {
                        string s = Leaders.Find_set(it->first.first);
                        string t = Leaders.Find_set(it->first.second);
                        if (s != t) {

                                pthread_mutex_lock(&Mutexes[s]);
                                if (Requests[s].second > it->second) {
                                        Requests[s] = make_pair(make_pair(it->first.first, it->first.second), it->second);
                                }
                                pthread_mutex_unlock(&Mutexes[s]);
                        }
                }
                sem_post(&semafor);
                pthread_mutex_lock(&count_mutex);
                pthread_cond_wait(&condition_var, &count_mutex);
                pthread_mutex_unlock(&count_mutex);
        }
        return NULL;
}

void initRequests() {
        for (set<string>::iterator it = Vertices.begin(); it != Vertices.end(); it++) {
                Requests[*it] = make_pair(make_pair("", ""), 100500);
        }
}

void *mainthread(void *ptr) {
        while (components > 1) {
                for (int i = 0; i < THREADS_NUMBER; i++) {
                        sem_wait(&semafor);
                }
                for (map<string, pair<pair<string, string>, int> >::iterator it = Requests.begin(); it != Requests.end(); it++) {
                        /*
                        cout << it->first << ' ' << it->second.first.first << ' ' << it->second.first.second << endl;
                        */
                        if (it->second.second < 100500) {
                                Leaders.Union_sets(it->second.first.first, it->second.first.second);
                                if (it->second.first.first < it->second.first.second) {
                                        if (Answer[make_pair(it->second.first.first, it->second.first.second)] != it->second.second) {
                                                components--;
                                                Answer[make_pair(it->second.first.first, it->second.first.second)] = it->second.second;
                                        }
                                } else {
                                        if (Answer[make_pair(it->second.first.second, it->second.first.first)] != it->second.second) {
                                                components--;
                                                Answer[make_pair(it->second.first.second, it->second.first.first)] = it->second.second;
                                        }
                                }
                        }
                }
                initRequests();
                /*
                cout << components << endl;
                cout << "---------------------------" << endl;
                */
                pthread_mutex_lock(&count_mutex);
                pthread_cond_broadcast(&condition_var);
                pthread_mutex_unlock(&count_mutex);
        }
        return NULL;
}

int main() {
        struct timeval tv1, tv2;

        input();
        initRequests();
        for (set<string>::iterator it = Vertices.begin(); it != Vertices.end(); it++) {
                Leaders.Make_set(*it);
        }

        gettimeofday(&tv1, NULL);

        sem_init(&semafor, 1, 0);
        pthread_create(&mthr, NULL, mainthread, NULL);
        for (int i = 0; i < THREADS_NUMBER; i++) {
                pthread_create(&thr[i], NULL, consumer, (void *) i);
        }

        for (int i = 0; i < THREADS_NUMBER; i++) {
                pthread_join(thr[i], NULL);
        }
        pthread_join(mthr, NULL);

        gettimeofday(&tv2, NULL);

	cout << (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec << endl;

        for (map <pair<string, string>, int>::iterator it = Answer.begin(); it != Answer.end(); it++) {
                cout << it->first.first << ' ' << it->first.second << ' ' << it->second << endl;
        }
        return 0;
}
