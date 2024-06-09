//
// Created by Mara Ielciu on 11.04.2024.
//

#ifndef OOP_A6_7_MARAIELCIU1_EXCEPTIONS_H
#define OOP_A6_7_MARAIELCIU1_EXCEPTIONS_H
#pragma once
#include <exception>
#include <string>
#include <vector>

class FileException : public std::exception
{
private:
    std::string message;

public:
    FileException(const std::string &message);
    virtual const char *what();
};

class RepositoryException : public std::exception
{
protected:
    std::string message;

public:
    RepositoryException();
    RepositoryException(const std::string &message);
    virtual ~RepositoryException(){};
    virtual const char *what();
};

class DuplicateTutException : public RepositoryException
{
public:
    const char *what();
};

class InvalidPositionException : public RepositoryException
{
public:
    const char *what();
};

#endif //OOP_A6_7_MARAIELCIU1_EXCEPTIONS_H
