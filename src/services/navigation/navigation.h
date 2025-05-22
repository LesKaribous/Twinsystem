#pragma once
#include "services/service.h"

class Navigation : public Service{
private:

public:
    Navigation();

    void attach() override;
    void run() override;


    
    SINGLETON(Navigation);
};
SINGLETON_EXTERN(Navigation, navigation)
