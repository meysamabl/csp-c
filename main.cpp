#include <iostream>
#include <random>       /* random */
#include <list>         /* list */
#include <math.h>       /* pow */
#include <fstream>
#include "SampleObject.h"

using namespace std;

double aoq(double pr, int k, int m);

int main()
{
    default_random_engine generator;
    uniform_real_distribution<double> distribution(0.0,1.0);
    SampleObject* samplePtr = nullptr;
    list<SampleObject> productionLine;
    char status;
    double number;
    fstream fout("test_output.dat", ios::out);
    /// Sample object creation
    for(int i = 0; i < 1000; ++i) {
        number = distribution(generator);
        if(number > 0.1) {
            status = 'C';
        } else {
            status = 'N';
        }
        samplePtr = new SampleObject(i+1, status);
        productionLine.push_back(*samplePtr);

    }
    list<SampleObject>::iterator it = productionLine.begin();
    /*
    while(it != productionLine.end()) {
        cout << it->toString() << endl;
        it++;
    }*/
    for(double i = 0.0; i <1; i+=0.001) {
        //cout << aoq(i, 10, 3) << endl;
        fout << i << " " << aoq(i, 1000, 5) << endl;
    }
    system("gnuplot command.txt ");
    return 0;
}

double aoq(double pr, int k, int m) {
    double numerator = pr * (m - 1) * pow((1 - pr), k);
    double denominator = 1 + ((m - 1) * pow((1-pr), k));
    return numerator / denominator;
}
