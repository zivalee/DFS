#include <iostream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

int calScheduledSumC(vector<pair<int, int>> node) {
    vector<int> sum;
    auto sumC = 0;
    for (auto i = 0; i < node.size(); i++) {
        if (i == 0 || node[i].second > sum.back())
            sum.push_back(node[i].first + node[i].second);
        else
            sum.push_back(sum.back() + node[i].first);
    }
    for (auto i = 0; i < sum.size(); i++)
        sumC += sum[i];
    return sumC;
}


// first: Process Length (pj)
// second: Arrival Time (rj)
int estimateLB(vector<pair<int, int>> proc, int n) {
    int diff = 0;
    int afterCountT = 0;
    vector<int> wt;
    vector<int> *heap = new vector<int>();
    // put Process Length of first element into heap
    heap->push_back(proc[0].first);
    // make min heap
    make_heap(heap->begin(), heap->end(), greater<>{});

    // adding process length of elements into heap
    // examine every element's arrival time and adjust the heap before putting the process length into heap
    for (int i = 1; i < n; i++) {
        // arrival time is in increasing order
        // if arrival time of job i is equal to i-1, put the process length of it into heap directly
        // if not, adjust heap then put the length into heap
        if (proc[i].second > proc[i - 1].second) {
            // time passed between two job
            diff = proc[i].second - proc[i - 1].second;

            // if the time passed can't even finished one job
            // root of heap (min job length) > time passed
            if (heap->front() > diff) {
                heap->front() -= diff;
            } else {
                // if the time passed can finish more than one job
                // arrival time of the previous job
                int previousT = proc[i - 1].second;

                while (diff > 0 && heap->begin() != heap->end()) {
                    int temp = heap->front();

                    if (diff >= temp) {
                        // if can finish more jobs
                        // delete root
                        pop_heap(heap->begin(), heap->end(), greater<>{});
                        heap->pop_back();
                        // update time passed (diff)
                        diff -= temp;
                        // count job finished time
                        wt.push_back(temp + previousT);
                        // update previous job finished time
                        previousT = temp + previousT;
                    } else {
                        // if no time left
                        heap->front() -= diff;
                        diff = 0;
                    }
                }
            }
        }

        // insert job length to heap
        heap->push_back(proc[i].first);
        push_heap(heap->begin(), heap->end(), greater<>{});

        // print heap
//        for (int i = 0; i < heap.size(); i++) {
//            cout << heap[i] << ", ";
//        }

        // save last job arrive time for later counting
        if (i == n - 1) {
            afterCountT = proc[i].second;
        }

    }

//    cout << "afterCountT "<< afterCountT << endl;

    // after all process length are put into heap
    // count the finished time of the remaining jobs in heap
    int num = heap->size();
    for (int j = 0; j < num; j++) {
        // count time
        wt.push_back(afterCountT + heap->front());
        afterCountT += heap->front();
        // delete the counted root min
        pop_heap(heap->begin(), heap->end(), greater<>{});
        heap->pop_back();
    }
    int total = 0;
    for (int b = 0; b < wt.size(); b++) {
        total += wt[b];
    }
    delete heap;
    return total;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
