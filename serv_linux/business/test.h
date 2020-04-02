#ifndef TEST_
#define TEST_

#include "proto.h"

class Test : public Proto
{
public:
    Test();
    int dispatch(const Json::Value& jData, Handle fd) override;
private:



};
#endif