#ifndef SAMPLEOBJECT_H
#define SAMPLEOBJECT_H

#include <string>
#include <sstream>

using namespace std;

class SampleObject
{
    public:
        SampleObject(int id, char status);
        virtual ~SampleObject();
        string Getid() const { return id; }
        void Setid(string val) { id = val; }
        char Getstatus() const { return status; }
        void Setstatus(char val) { status = val; }
        string toString() { return id + ", Status: " + status;}
    protected:
    private:
        string id;
        char status;
};

#endif // SAMPLEOBJECT_H
