//
// Created by Mara Ielciu on 22.03.2024.
//
#include <string>
#ifndef OOP_A4_5_MARAIELCIU1_TUTORIAL_H
#define OOP_A4_5_MARAIELCIU1_TUTORIAL_H
#pragma once
/*You are very passionate about programing (otherwise you wouldn't be reading this) and C++ is a language close to your
 * heart. On your way to becoming a guru, you study a lot and watch many tutorials.
 * To make sure you do not miss any good tutorials, you absolutely need a software application to help you manage
 * your tutorials and create watch lists. The application can be used in two modes: administrator and user.
 * When the application is started, it will offer the option to choose the mode.
 * Administrator mode: The application will have a database , which holds all the tutorials.
 * You must be able to update the database, meaning: add a new tutorial, delete a tutorial and update the
 * information of a tutorial. Each Tutorial has a title, a presenter (name of the presenter person),
 * a duration (minutes and seconds), a number of likes and a link towards the online resource containing the tutorial.
 * The administrators will also have the option to see all the tutorials in the database.
 */

class Tutorial {
private:
    std::string title,presenter,link;
    int minutes,seconds,likes;
public:
    Tutorial(const std::string &title="",const std::string &presenter="",const std::string &link="", int minutes=0, int seconds=0,
             int likes=0);
    Tutorial(const Tutorial &t);
    ~Tutorial();
    Tutorial& operator=(const Tutorial &t);
    std::string getTitle() const;
    std::string getPresenter() const;
    std::string getLink() const;
    int getMinutes() const;
    int getSeconds() const;
    int getLikes() const;
    void setTitle(const std::string &title);
    void setPresenter(const std::string &presenter);
    void setLink(const std::string &link);
    void setMinutes(int minutes);
    void setSeconds(int seconds);
    void setLikes(int likes);
    bool operator==(const Tutorial &t);
    void validateTitle(const std::string &title);
    void validatePresenter(const std::string &presenter);
    void validateLink(const std::string &link);
    void validateMinutes(int minutes);
    void validateSeconds(int seconds);
    void validateLikes(int likes);
    void validateTutorial();


};


#endif //OOP_A4_5_MARAIELCIU1_TUTORIAL_H
