#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <climits>

using namespace std;

// x: burst time, y: arrival time
vector<pair<int, int>> jobs = {{6, 0},
                               {2, 2},
                               {3, 2},
                               {2, 6},
                               {5, 7},
                               {2, 9}};
int n = 6;  // number of jobs
int best = INT_MAX;
vector<pair<int, int>> tmpArr;
vector<pair<int, int>> bestPermutation;


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
int estimateLB(vector<pair<int, int>> proc) {
    int n = proc.size();
    if (n == 0) {
        return 0;
    }
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

        // save last job arrive time for later counting
        if (i == n - 1) {
            afterCountT = proc[i].second;
        }

    }

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

void DFS(vector<pair<int, int>> jobs, int i, int n) {
    vector<pair<int, int>> scheduled;
    vector<pair<int, int>> toCount;

    // base condition
    if (i == n - 1) {
        for (int j = 0; j < jobs.size(); j++) {
            cout << "[";
            cout << jobs[j].first << ",";
            cout << jobs[j].second;
            if (j < jobs.size() - 1) cout << "],";
            else cout << "]";
            tmpArr.push_back(jobs[j]);
        }
        cout << endl;
        int objectValue = calScheduledSumC(tmpArr);
        tmpArr.clear();
        if (objectValue <= best) {
            best = objectValue;
            bestPermutation = tmpArr;
        }
        cout << best << ", ";
    }


    // estimate LB
    for (int k = 0; k <= i; k++) {
        scheduled.push_back(jobs[k]);
    }
    for (int m = i; m < n; m++) {
        if (m == 5) {
            break;
        }
        toCount.push_back(jobs[m + 1]);
    }
    int LB = calScheduledSumC(scheduled) + estimateLB(toCount);
    cout << "*" << LB << ", ";

    if (LB < best) {
        cout << "%";
        for (int j = i; j < n; j++) {
            swap(jobs[i], jobs[j]);
            DFS(jobs, i + 1, n);
            swap(jobs[i], jobs[j]);
        }
    }
}


int main() {
    clock_t start = clock();
    DFS(::jobs, 0, 6);
    clock_t end = clock();
    double elapsed_secs = double(end - start);
    cout << endl << "Best: " << best;
    cout << endl << "elapsed run time: " << elapsed_secs << " ms, ";

    return 0;
}
