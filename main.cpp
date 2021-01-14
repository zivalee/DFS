#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <bits/stdc++.h>
using namespace std;

// x: burst time, y: arrival time
vector<pair<int, int>> jobs;
int best = INT_MAX;
vector<pair<int, int>> tmpArr;
vector<int> bestPermutation;


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
int estimateLB(vector<pair<int, int>> proc, int nowTime) {
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
        if(proc[i].second < nowTime){
            proc[i].second = nowTime;
        }
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

void DFS(vector<pair<int, int>> nowJobs, int i, int n, int count) {
    vector<pair<int, int>> scheduled;
    vector<pair<int, int>> toCount;

    // base condition
    if (i == n - 1) {
        for (int j = 0; j < n; j++) {
            tmpArr.push_back(nowJobs[j]);
        }
        int objectValue = calScheduledSumC(tmpArr);
        if (objectValue <= best) {
            best = objectValue;
            bestPermutation.clear();
            for (int k = 0; k < n; k++) {
                auto it = find(::jobs.begin(), ::jobs.end(), nowJobs[k]);
                int index = it - ::jobs.begin() + 1;
                bestPermutation.push_back(index);
            }
        }
        tmpArr.clear();
    }

    // estimate LB
    int nowTime = 0;
    for (int k = 0; k <= i; k++) {
        scheduled.push_back(nowJobs[k]);
        nowTime += nowJobs[k].first;
    }
    for (int m = i; m < n; m++) {
        if (m == n-1) {
            break;
        }
        toCount.push_back(nowJobs[m + 1]);
    }
    int LB = calScheduledSumC(scheduled) + estimateLB(toCount, nowTime);

    if (LB <= best) {
        for (int j = i; j < n; j++) {
            swap(nowJobs[i], nowJobs[j]);
            DFS(nowJobs, i + 1, n, count);
            count += 1;
            swap(nowJobs[i], nowJobs[j]);
        }
    }
}


int main() {

    // read csv
    ifstream myFile("/home/ziva/CLionProjects/C++/DFS/test_instance.csv");

    // Make sure the file is open
    if (!myFile.is_open()) throw runtime_error("Could not open file");

    // helper vars
    string line, colname;
    int tempCol = 0;
    // read first line (the column names)
    if (myFile.good()) {
        getline(myFile, line);

        // create a stringstream from line
        stringstream ss(line);

        // extract each column name
        while (getline(ss, colname, ',')) {
            // initialize and add <colname, int> pairs to data
            stringstream colnameS(colname);
            colnameS >> tempCol;
            jobs.push_back({tempCol, 0});
        }
    }

    while (getline(myFile, line)) {
        stringstream ss(line);
        string val;

        int colIdx = 0;
        int temp = 0;

        while (getline(ss, val, ',')) {
            // convert string to int
            stringstream valS(val);
            valS >> temp;

            jobs[colIdx].second = temp;
            colIdx++;
        }
    }
    myFile.close();

    // delete first column r_j, p_j
    jobs.erase(jobs.begin());

    int n = 15;  // job number
    int count = n;


    clock_t start = clock();
    DFS(::jobs, 0, n, count);
    clock_t end = clock();
    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
    cout << "n: " << n;
    cout << endl << "Best: " << best << endl;
    cout << "Best Permutation: ";
    for (int x = 0; x < bestPermutation.size(); x++) {
        cout << bestPermutation[x] << ", ";
    }
    cout << endl << "elapsed run time: " << elapsed_secs << " s, " << endl;
    cout << "number of nodes visited: " << count;

    return 0;
}
