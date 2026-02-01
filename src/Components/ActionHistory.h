#pragma once

class History {
public:
    virtual ~History() = default;
    virtual void doAction() = 0;
    virtual History* getInverse() = 0;
};