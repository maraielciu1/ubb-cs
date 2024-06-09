//
// Created by Mara Ielciu on 24.03.2024.
//
#include "TestService.h"
#include <assert.h>
#include "../Service/Service.h"

void TestService::testService1() {
    Service service=Service();
    try{
        service.getServiceElement(-1);
        assert(false);
    }
    catch(std::exception&){
        assert(true);
    }
    Tutorial tutorial1=Tutorial("Tutorial1","Presenter1","www.tutorial1.com",1,10,10);
    service.addService(tutorial1);
    assert(service.getSizeService()==1);
    Tutorial tutorial2=Tutorial("Tutorial2","Presenter2","www.tutorial2.com",2,20,20);
    service.addService(tutorial2);
    assert(service.getSizeService()==2);
    service.addService(tutorial1);
    assert(service.getSizeService()==2);
    assert(service.returnPositionService(tutorial1)==0);
    assert(service.returnPositionService(tutorial2)==1);
    assert(service.getAdminMode()==0);
    service.deleteService(tutorial1);
    assert(service.getSizeService()==1);
    service.deleteService(tutorial1);
    assert(service.getSizeService()==1);
    service.deleteServicePosition(0);
    assert(service.getSizeService()==0);
    service.addService(tutorial1);
    service.updateService(0,tutorial2);
    assert(service.getServiceElement(0)==tutorial2);
    DynamicArray<Tutorial> tutorials=service.getAllService();
    assert(tutorials.getElementArray(0)==tutorial2);
    assert(service.deleteServicePosition(3)==false);
    assert(service.deleteServicePosition(-1)==false);
    assert(service.updateService(3,tutorial1)==false);
    assert(service.updateService(-1,tutorial1)==false);
    service.generateTutorials();
    assert(service.getSizeService()==11);
    assert(service.getAdminMode()==0);
    service.setAdminMode(1);
    assert(service.getAdminMode()==1);
    DynamicArray<int> arr=service.getPositionsTutByPresenter("Mosh");
    assert(arr.getElementArray(0)==1);
    assert(arr.getElementArray(1)==2);
    DynamicArray<int> arr1 = service.getIndexesOfLikedTutorials();
    assert(arr1.getSizeArray()==0);
    service.addToWatchList(0);
    service.addToWatchList(3);
    service.addToWatchList(5);
    arr1 = service.getIndexesOfLikedTutorials();
    assert(arr1.getSizeArray()==3);
    service.removeWatchList(0);
    arr1 = service.getIndexesOfLikedTutorials();
    assert(arr1.getSizeArray()==2);
    int likes=service.getServiceElement(6).getLikes();
    service.increaseLikesByPos(6);
    assert(service.getServiceElement(6).getLikes()==likes+1);
    service.openTutorialInBrowser(2);
}
