#include "Service.h"
#include <algorithm>
#include "FilePlaylist.h"
#include "RepositoryExceptions.h"

using namespace std;

void Service::addSongToRepository(const std::string& artist, const std::string& title, double minutes, double seconds, const std::string& source)
{
	Song s{ artist, title, Duration{minutes, seconds}, source };
	this->validator.validate(s);
	this->repo.addSong(s);
    std::unique_ptr<Action> a=std::make_unique<ActionAdd>(s,this->repo);
    this->undoStack.push_back(std::move(a));
}

void Service::removeSongFromRepository(const std::string & artist, const std::string & title)
{
	Song s = this->repo.findByArtistAndTitle(artist, title);
	this->repo.removeSong(s);
    std::unique_ptr<Action> a=std::make_unique<ActionRemove>(s,this->repo);
    this->undoStack.push_back(std::move(a));
}

void Service::addSongToPlaylist(const Song& song)
{
	if (this->playList == nullptr)
		return;
	this->playList->add(song);
}

void Service::addAllSongsByArtistToPlaylist(const std::string& artist)
{
	vector<Song> songs = this->repo.getSongs();
	int nSongs = static_cast<int>(count_if(songs.begin(), songs.end(),
		[artist](const Song& s)
		{
			return s.getArtist() == artist;
		}));

	vector<Song> songsByArtist(nSongs);
	copy_if(songs.begin(), songs.end(), songsByArtist.begin(),
		[artist](const Song& s)
		{
			return s.getArtist() == artist;
		});

	for (auto s : songsByArtist)
		this->playList->add(s);
}

void Service::startPlaylist()
{
	if (this->playList == nullptr)
		return;
	this->playList->play();
}

void Service::nextSongPlaylist()
{
	if (this->playList == nullptr)
		return;
	this->playList->next();
}

void Service::savePlaylist(const std::string& filename)
{
	if (this->playList == nullptr)
		return;

	this->playList->setFilename(filename);
	this->playList->writeToFile();
}

void Service::openPlaylist() const
{
	if (this->playList == nullptr)
		return;

	this->playList->displayPlaylist();
}

void Service::undo() {
    if(this->undoStack.empty())
        throw RepositoryException("No more undos!");
    std::unique_ptr<Action> a=std::move(this->undoStack.back());
    a->executeUndo();
    this->undoStack.pop_back();
    this->redoStack.push_back(std::move(a));
}

void Service::redo() {
    if(this->redoStack.empty())
        throw RepositoryException("No more redos!");
    std::unique_ptr<Action> a=std::move(this->redoStack.back());
    a->executeRedo();
    this->redoStack.pop_back();
    this->undoStack.push_back(std::move(a));
}

void Service::clearRedo() {
    this->redoStack.clear();
}
