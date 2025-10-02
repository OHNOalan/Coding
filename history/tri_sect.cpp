#include "bits/stdc++.h"
using namespace std;

struct KKK {
    multiset<int> front, mid, rear;
    int k;
    KKK(int k) : k(k) {}
    int size() { return front.size() + mid.size() + rear.size(); }
    void add(int v) {
        if (front.size() < k) front.insert(v);
        else if (rear.size() < k) {
            if (v > *front.rbegin()) rear.insert(v);
            else {
                auto p = prev(front.end());
                rear.insert(*p);
                front.erase(p);
                front.insert(v);
            }
        } else {
            if (v < *front.rbegin()) {
                auto p = prev(front.end());
                mid.insert(*p);
                front.erase(p);
                front.insert(v);
            } else if (v > *rear.begin()) {
                auto p = rear.begin();
                mid.insert(*p);
                rear.erase(p);
                rear.insert(v);
            } else mid.insert(v);
        }
    }
    void remove(int v) {
        if (mid.size() > 0) {
            if (v < *mid.begin()) {
                front.extract(v);
                front.insert(*mid.begin());
                mid.erase(mid.begin());
            } else if (v > *mid.rbegin()) {
                rear.extract(v);
                rear.insert(*mid.rbegin());
                mid.erase(prev(mid.end()));
            } else mid.extract(v);
        } else if (rear.size() > 0) {
            if (v < *rear.begin()) front.extract(v);
            else rear.extract(v);
        } else front.extract(v);
    }
};