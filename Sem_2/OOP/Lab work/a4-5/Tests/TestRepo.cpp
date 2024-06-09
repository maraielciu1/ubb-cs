//
// Created by Mara Ielciu on 24.03.2024.
//

#include "TestRepo.h"
#include <assert.h>
#include "../Repository/Repo.h"

void TestRepo::testRepo1() {
    Repo repo;
    assert(repo.getSizeRepo() == 0);
    Tutorial tutorial1("Tutorial1","Tutorial1", "www.tutorial1.com", 100, 10, 100);
    repo.addRepo(tutorial1);
    assert(repo.getSizeRepo() == 1);
    Tutorial tutorial2("Tutorial2","Tutorial2", "www.tutorial2.com", 200, 20, 200);
    repo.addRepo(tutorial2);
    assert(repo.getSizeRepo() == 2);
    repo.deleteRepo(tutorial1);
    assert(repo.getSizeRepo() == 1);
    repo.updateRepo(0, tutorial1);
    assert(repo.getSizeRepo()==1);
    assert(repo.getRepoElement(0) == tutorial1);
    repo.deleteRepoPosition(0);
    assert(repo.getSizeRepo() == 0);
    assert(repo.validatePosition(0) == false);
    assert(repo.validatePosition(1) == false);
    assert(repo.validatePosition(-1) == false);
    repo.addRepo(tutorial1);
    assert(repo.returnPosition(tutorial1) == 0);
    DynamicArray<Tutorial> tutorials = repo.getAllRepo();
    assert(tutorials.getElementArray(0) == tutorial1);
    repo.modifyLikesPosition(0,50);
    assert(repo.getRepoElement(0).getLikes()==150);

}
