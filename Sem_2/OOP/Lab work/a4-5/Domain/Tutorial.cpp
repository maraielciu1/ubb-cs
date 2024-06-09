//
// Created by Mara Ielciu on 22.03.2024.
//
#include <string>
#include <regex>
#include "Tutorial.h"

Tutorial::Tutorial(const std::string &title,const std::string &presenter,const std::string &link, int minutes, int seconds,
                   int likes): title{title}, presenter{presenter}, link{link}, minutes{minutes}, seconds{seconds},
                                likes{likes}
{

}

Tutorial::Tutorial(const Tutorial &t)
{
    this->title = t.title;
    this->presenter = t.presenter;
    this->link = t.link;
    this->minutes = t.minutes;
    this->seconds = t.seconds;
    this->likes = t.likes;
}

Tutorial::~Tutorial() {}

Tutorial &Tutorial::operator=(const Tutorial &t) {
    if(this == &t)
        return *this;
    this->title = t.title;
    this->presenter = t.presenter;
    this->link = t.link;
    this->minutes = t.minutes;
    this->seconds = t.seconds;
    this->likes = t.likes;
    return *this;
}

bool Tutorial::operator==(const Tutorial &t) {
    return this->title == t.title && this->presenter == t.presenter && this->link == t.link &&
           this->minutes == t.minutes && this->seconds == t.seconds && this->likes == t.likes;
}

// getters
std::string Tutorial::getTitle() const {
    return this->title;
}

std::string Tutorial::getPresenter() const {
    return this->presenter;
}

std::string Tutorial::getLink() const {
    return this->link;
}

int Tutorial::getMinutes() const {
    return this->minutes;
}

int Tutorial::getSeconds() const {
    return this->seconds;
}

int Tutorial::getLikes() const {
    return this->likes;
}

// validators
void Tutorial::validateTitle(const std::string &title) {
    if(title.empty())
        throw std::invalid_argument("Title cannot be empty");
}

void Tutorial::validatePresenter(const std::string &presenter) {
    if(presenter.empty())
        throw std::invalid_argument("Presenter cannot be empty");
}

void Tutorial::validateLink(const std::string &link) {
    if(link.empty())
        throw std::invalid_argument("Link cannot be empty");
    const std::regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    if (!std::regex_match(link, pattern))
        throw std::invalid_argument("Invalid link");
}

void Tutorial::validateMinutes(int minutes) {
    if(minutes < 0)
        throw std::invalid_argument("Minutes cannot be negative");
}

void Tutorial::validateSeconds(int seconds) {
    if(seconds < 0 || seconds >= 60)
        throw std::invalid_argument("Seconds cannot be negative or more than 59");
}

void Tutorial::validateLikes(int likes) {
    if(likes < 0)
        throw std::invalid_argument("Likes cannot be negative");
}

void Tutorial::validateTutorial() {
    validateTitle(this->title);
    validatePresenter(this->presenter);
    validateLink(this->link);
    validateMinutes(this->minutes);
    validateSeconds(this->seconds);
    validateLikes(this->likes);
}

// setters
void Tutorial::setTitle(const std::string &title) {
    this->validateTitle(title);
    this->title = title;
}

void Tutorial::setPresenter(const std::string &presenter) {
    this->validatePresenter(presenter);
    this->presenter = presenter;
}

void Tutorial::setLink(const std::string &link) {
    this->validateLink(link);
    this->link = link;
}

void Tutorial::setMinutes(int minutes) {
    this->validateMinutes(minutes);
    this->minutes = minutes;
}

void Tutorial::setSeconds(int seconds) {
    this->validateSeconds(seconds);
    this->seconds = seconds;
}

void Tutorial::setLikes(int likes) {
    this->validateLikes(likes);
    this->likes = likes;
}




