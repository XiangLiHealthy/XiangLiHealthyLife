#ifndef TEST_
#define TEST_

#include "proto.h"

class Test : public Proto
{
public:
    Test();
    const char* getName() override;
    int dispatch(const Json::Value& jData, Handle fd) override;
private:



};
#endif