//
// Created by Mara Ielciu on 25.04.2024.
//

#include "DBRepo.h"
#include "Exceptions.h"

DBRepo::DBRepo(const std::string &dbname) {
    this->dbName = dbname;
    int errorCode = sqlite3_open(this->dbName.c_str(), &this->dbWithTutorials);

    if (errorCode != SQLITE_OK)
        throw RepositoryException("Could not open database");

    std::string createTableStatement = "CREATE TABLE IF NOT EXISTS Tutorials (Title TEXT PRIMARY KEY, Presenter TEXT, Link TEXT,Minutes INTEGER, Seconds INTEGER, Likes INTEGER);";
    char *errorMessage = nullptr;
    errorCode = sqlite3_exec(this->dbWithTutorials, createTableStatement.c_str(), NULL, 0, &errorMessage);
    if (errorCode != SQLITE_OK) {
        std::string errorStr = "Could not create table. Error: ";
        if (errorMessage) {
            errorStr += errorMessage;
            sqlite3_free(errorMessage); // Use sqlite3_free to free errorMessage
        }
        throw RepositoryException(errorStr);
    }

}

DBRepo::~DBRepo() {
    sqlite3_close(this->dbWithTutorials);
}

void DBRepo::readFromFile() {
    // First, check if the database already has entries
    std::string countQuery = "SELECT COUNT(*) FROM Tutorials;";
    sqlite3_stmt* countStmt;
    if (sqlite3_prepare_v2(this->dbWithTutorials, countQuery.c_str(), -1, &countStmt, nullptr) != SQLITE_OK) {
        throw RepositoryException("Failed to prepare count query");
    }

    int count = 0;
    if (sqlite3_step(countStmt) == SQLITE_ROW) {
        count = sqlite3_column_int(countStmt, 0);
    }
    sqlite3_finalize(countStmt);

    // If there are no tutorials, proceed to read from the file/database
    if (count == 0) {
        std::string selectStatement = "SELECT * FROM Tutorials;";
        sqlite3_stmt* statement;
        int errorCode = sqlite3_prepare_v2(this->dbWithTutorials, selectStatement.c_str(), -1, &statement, nullptr);
        if (errorCode != SQLITE_OK)
            throw RepositoryException("Could not read from database");

        while (sqlite3_step(statement) == SQLITE_ROW) {
            std::string title = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 0)));
            std::string presenter = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
            std::string link = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, 2)));
            int minutes = sqlite3_column_int(statement, 3);
            int seconds = sqlite3_column_int(statement, 4);
            int likes = sqlite3_column_int(statement, 5);
            Tutorial tutorial{title, presenter, link, minutes, seconds, likes};
            this->addRepo(tutorial);
        }
        errorCode = sqlite3_finalize(statement);
        if (errorCode != SQLITE_OK)
            throw RepositoryException("Could not finalize statement");
    }
}


void DBRepo::addRepo(const Tutorial &tutorial) {
    Repo::addRepo(tutorial);
    std::string insertStatement = "INSERT INTO Tutorials VALUES ('" + tutorial.getTitle() + "', '" + tutorial.getPresenter() + "', '"+ tutorial.getLink() +"', "+ std::to_string(tutorial.getMinutes()) + ", " + std::to_string(tutorial.getSeconds()) + ", " + std::to_string(tutorial.getLikes()) + ");";
    char *errorMessage;
    int errorCode = sqlite3_exec(this->dbWithTutorials, insertStatement.c_str(), NULL, 0, &errorMessage);
    free(errorMessage);
    if (errorCode != SQLITE_OK)
        throw RepositoryException("Could not add tutorial");


}

void DBRepo::deleteRepo(const Tutorial &tutorial) {
    std::string deleteStatement = "DELETE FROM Tutorials WHERE Title = '" + tutorial.getTitle() + "' AND Presenter = '" + tutorial.getPresenter() + "' AND Link = '" + tutorial.getLink() + "';";
    char *errorMessage;
    int errorCode = sqlite3_exec(this->dbWithTutorials, deleteStatement.c_str(), NULL, 0, &errorMessage);
    free(errorMessage);
    if (errorCode != SQLITE_OK)
        throw RepositoryException("Could not remove tutorial");
    Repo::deleteRepo(tutorial);

}

void DBRepo::updateRepo(int position, const Tutorial &tutorial) {
    std::string updateStatement = " UPDATE Tutorials SET Title = '" + tutorial.getTitle() + "', Presenter = '" + tutorial.getPresenter() + "', Link = '" + tutorial.getLink() + "', Minutes = " + std::to_string(tutorial.getMinutes()) + ", Seconds = " + std::to_string(tutorial.getSeconds()) + ", Likes = " + std::to_string(tutorial.getLikes()) + " WHERE rowid = " + std::to_string(position + 1) + ";";
    Repo::updateRepo(position, tutorial);
    char *errorMessage;
    int errorCode = sqlite3_exec(this->dbWithTutorials, updateStatement.c_str(), NULL, 0, &errorMessage);
    free(errorMessage);
    if (errorCode != SQLITE_OK)
        throw RepositoryException("Could not update tutorial");

}

void DBRepo::deleteRepoPosition(int position) {
    this->deleteRepo(this->tutorials[position]);
}
