#include <iostream>
#include <chrono>
#include <random>       /* random */
#include <list>         /* list */
#include <math.h>       /* pow */
#include <fstream>
#include "SampleObject.h"

using namespace std;

double aoq(double pr, int k, int m, int c);

template <typename Iterator>
size_t safe_advance(Iterator& it, Iterator const& end, size_t n)
{
  size_t i = 0;
  for (; i != n && it != end; ++i, ++it);
  return n - i;
}

int main()
{
    int k, m, c;
    list<int> vals;
    cout << "Please enter k, m and c respectively: ";
    cin >> k;
    cin >> m;
    cin >> c;
    /// obtain a seed from the system clock:
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed1);
    uniform_real_distribution<double> distribution(0.0,1.0);
    uniform_int_distribution<int> intDistro(0,m);
    SampleObject* samplePtr = nullptr;
    list<SampleObject> productionLine;
    int stage1Counter = 0, stage2Counter = 0;
    char status;
    double number;
    fstream fout("test_output.dat", ios::out);
    fstream pout("result.dat", ios::out);
    double defaultNoOfDefects = 0;
    for(int j = 0; j <= 99; j++) {
        //defaultNoOfDefects = 0;
        /// Sample object creation ------------------------
        for(int i = 0; i < 1000; ++i) {
                number = distribution(generator);
                if(number > 0.1) {
                    status = 'C';
                } else {
                    defaultNoOfDefects++;
                    status = 'N';
                }
                samplePtr = new SampleObject(i+1, status);
                productionLine.push_back(*samplePtr);

            }
            /// Stage 1 starts here -----------------------------
            list<SampleObject>::iterator it = productionLine.begin();
            //cout << "------------- STAGE 1 --------------" <<endl;
            while(it != productionLine.end()) {
                if(it->Getstatus() == 'N') {
                    //cout << "Found non-conforming sample in stage 1" << endl;
                    //cout << "Fixing non-conforming sample: " << it->toString() << endl;
                    it->Setstatus('C');
                    stage1Counter = 0;
                } else {
                    stage1Counter++;
                }
                it++;
                if(stage1Counter > k) {
                    stage1Counter = 0;
                    stage2Counter = 0;
                    //cout << "------------- STAGE 2 --------------" <<endl;
                    while(it != productionLine.end()) {
                        int radomNumber = intDistro(generator);
                        //cout << "Random index from the batch: " << radomNumber << endl;

                        safe_advance (it, productionLine.end(), radomNumber);
                        if(it != productionLine.end()) {
                            if(it->Getstatus() == 'N') {
                                //cout << "Found non-conforming sample in stage 2" << endl;
                                //cout << "Fixing non-conforming sample: " << it->toString() << endl;
                                it->Setstatus('C');
                                stage2Counter++;
                            }
                        }
                        if(stage2Counter > c) {
                            safe_advance (it, productionLine.end(), 1);
                            if(it != productionLine.end()) {
                                //cout << "------------- STAGE 1 --------------" <<endl;
                            }
                            break;
                        }
                        safe_advance (it, productionLine.end(), m - radomNumber);
                        //cout << "sample after the batch " << it->toString() << endl;
                    }
                }
            }
            int numberOfDefects = 0;
            for(it = productionLine.begin(); it != productionLine.end(); ++it) {
                if(it->Getstatus() == 'N') {
                    numberOfDefects++;
                }
            }
            //cout << "-------------------------------------\n";
            //cout << "Results: \nk: " << k << ", m: " << m << ", c: " << c << endl;
            cout << "Number of outgoing defects: " << numberOfDefects << endl;
            vals.push_back(numberOfDefects);
            productionLine.clear();
    }
    double myAvg = 0;
    for(list<int>::iterator intIt = vals.begin(); intIt != vals.end(); intIt++) {
        myAvg += (double)*intIt;
    }
    myAvg = myAvg/100/1000;
    pout << defaultNoOfDefects/1000/100 << " " << myAvg << endl;
    cout << "------------------------------------------" << endl;
    cout << "Results: \nk: " << k << ", m: " << m << ", c: " << c << endl;
    cout << "Empirical AOQ: " << myAvg  << endl;
    cout << "Theoretical AOQ: " << aoq(defaultNoOfDefects/1000/100, k, m, c) << endl;
    for(double i = 0.0; i <=1; i+=0.001) {
        //cout << aoq(i, 10, 3) << endl;
        fout << i << " " << aoq(i, k, m, c) << endl;
    }
    double bestTheoretical = 0;
    for(int m = 2; m <= 10; m++) {
        for(int k = 5; k <= 50; k++) {
            bestTheoretical = aoq(0.1, k, m, 3);
            if(bestTheoretical <= 0.03) {
                cout << "k: " << k << ", m: " << m << endl;
                cout << "Theoretical AOQ: " << bestTheoretical << "-------------" << endl << endl;
                break;
            }
        }
    }
    system("gnuplot command.txt ");
    return 0;
}

double aoq(double pr, int k, int m, int c) {
    double numerator = pr * (m - 1) * pow((1 - pr), k) * (c+1);
    double denominator = 1 + (((m * (c+1)) - 1) * pow((1-pr), k));
    return numerator / denominator;
}
