#pragma once
#include <vector>
#include "Song.h"
#include <fstream>

class PlayList 
{
protected:
	std::vector<Song> songs;
	int current;

public:
	PlayList();

	// Adds a song to the playlist.
	void add(const Song& song);

	// Returns the song that is currently playing.
	Song getCurrentSong();

	// Starts the playlist - plays the first song.
	void play();

	// Plays the next song in the playlist.
	void next();

	// Checks if the playlist is empty.
	bool isEmpty();

	virtual ~PlayList() {}
};

class FilePlayList: public PlayList
{
protected:
    std::string filename;
public:
    virtual void displayList() = 0;
    virtual void writeToFile() = 0;

};

class CSVPlaylist : public FilePlayList {
public:
    CSVPlaylist(const std::string filename);
    void displayList() override;
    void writeToFile() override;
};

class JSONPlaylist : public FilePlayList {
public:
    JSONPlaylist(const std::string filename);
    void displayList() override;
    void writeToFile() override;
};