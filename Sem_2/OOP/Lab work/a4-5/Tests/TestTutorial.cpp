//
// Created by Mara Ielciu on 24.03.2024.
//

#include "TestTutorial.h"
#include <assert.h>
#include "../Domain/Tutorial.h"

void TestTutorial::TestTutorial1() {
    Tutorial correct_tutorial{"title","presenter","https://www.youtube.com/watch?v=ZzaPdXTrSb8",60,30,100};
    //test getters
    assert(correct_tutorial.getTitle() == "title");
    assert(correct_tutorial.getPresenter() == "presenter");
    assert(correct_tutorial.getLink() == "https://www.youtube.com/watch?v=ZzaPdXTrSb8");
    assert(correct_tutorial.getMinutes() == 60);
    assert(correct_tutorial.getSeconds() == 30);
    assert(correct_tutorial.getLikes() == 100);
    //test validators
    try
    {
        correct_tutorial.validateTutorial();
        assert(true);
    }
    catch (std::exception &e)
    {
        assert(false);
    }
    try {
        correct_tutorial.validateTitle("");
        assert(false);

    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validatePresenter("");
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validateLink("abc");
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validateLink("");
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validateMinutes(-1);
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validateSeconds(-1);
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    try {
        correct_tutorial.validateLikes(-1);
        assert(false);
    }
    catch (std::exception &e)
    {
        assert(true);
    }
    //test setters
    correct_tutorial.setTitle("new title");
    correct_tutorial.setPresenter("new presenter");
    correct_tutorial.setLink("https://www.youtube.com/watch?v=ZzaPdXTrSb9");
    correct_tutorial.setMinutes(70);
    correct_tutorial.setSeconds(40);
    correct_tutorial.setLikes(200);
    assert(correct_tutorial.getTitle() == "new title");
    assert(correct_tutorial.getPresenter() == "new presenter");
    assert(correct_tutorial.getLink() == "https://www.youtube.com/watch?v=ZzaPdXTrSb9");
    assert(correct_tutorial.getMinutes() == 70);
    assert(correct_tutorial.getSeconds() == 40);
    assert(correct_tutorial.getLikes() == 200);
    //test copy constructor
    Tutorial copy_tutorial{correct_tutorial};
    assert(copy_tutorial.getTitle() == "new title");
    assert(copy_tutorial.getPresenter() == "new presenter");
    assert(copy_tutorial.getLink() == "https://www.youtube.com/watch?v=ZzaPdXTrSb9");
    assert(copy_tutorial.getMinutes() == 70);
    assert(copy_tutorial.getSeconds() == 40);
    assert(copy_tutorial.getLikes() == 200);

    try{
        copy_tutorial.validateTutorial();
        assert(true);
    }
    catch (std::exception &e)
    {
        assert(false);
    }
    assert(copy_tutorial == correct_tutorial);

    Tutorial sourceTutorial("Title", "Presenter", "https://www.example.com", 10, 20, 30);

    // Create a target object with a different state
    Tutorial targetTutorial("Different Title", "Different Presenter", "https://www.different.com", 40, 50, 60);

    targetTutorial = sourceTutorial;
    assert(targetTutorial.getTitle() == "Title");
    assert(targetTutorial.getPresenter() == "Presenter");
    assert(targetTutorial.getLink() == "https://www.example.com");
    assert(targetTutorial.getMinutes() == 10);
    assert(targetTutorial.getSeconds() == 20);
    assert(targetTutorial.getLikes() == 30);

    assert(sourceTutorial.getTitle() == "Title");
    assert(sourceTutorial.getPresenter() == "Presenter");
    assert(sourceTutorial.getLink() == "https://www.example.com");
    assert(sourceTutorial.getMinutes() == 10);
    assert(sourceTutorial.getSeconds() == 20);
    assert(sourceTutorial.getLikes() == 30);

    targetTutorial = targetTutorial;
    assert(targetTutorial.getTitle() == "Title");
    assert(targetTutorial.getPresenter() == "Presenter");
    assert(targetTutorial.getLink() == "https://www.example.com");
    assert(targetTutorial.getMinutes() == 10);
    assert(targetTutorial.getSeconds() == 20);
    assert(targetTutorial.getLikes() == 30);

    Tutorial t1;
    assert(t1.getTitle() == "");
    assert(t1.getPresenter() == "");
    assert(t1.getLink() == "");
    assert(t1.getMinutes() == 0);
    assert(t1.getSeconds() == 0);
    assert(t1.getLikes() == 0);

}
