#include "UI/UI.h"
#include "Tests/Test.h"

//
// Created by Mara Ielciu on 01.04.2024.
//
int main()
{
    Test t=Test();
    t.testAll();
    UI ui=UI();
    ui.run();
    return 0;
}
