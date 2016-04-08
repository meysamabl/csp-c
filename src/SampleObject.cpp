#include "SampleObject.h"

SampleObject::SampleObject(int id, char status) : status(status)
{
    ostringstream ss;
    ss << id;
    this->id = "Sample #" + ss.str();
}

SampleObject::~SampleObject()
{
    //dtor
}
