//
// Created by Mara Ielciu on 24.03.2024.
//

#include <iostream>
#include "Tests/TestTutorial.h"
#include "Tests/TestDynamicArray.h"
#include "Tests/TestRepo.h"
#include "Tests/TestService.h"
#include "UI/Ui.h"

int main() {
    TestTutorial testTutorial;
    TestDynamicArray testDynamicArray;
    TestRepo testRepo;
    TestService testService;
    testTutorial.TestTutorial1();
    testDynamicArray.testDynamicArray1();
    testRepo.testRepo1();
    testService.testService1();
    std::cout << "All tests passed!" << std::endl;
    Ui ui;
    ui.run();
    return 0;
}