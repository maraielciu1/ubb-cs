#pragma once
#include "Repository.h"
#include "PlayList.h"

class Service
{
private:
	Repository repo;
	PlayList* playList;
	
public:
	Service(const Repository& r, PlayList* p) : repo{ r }, playList{ p } {}

	Repository getRepo() const { return repo; }
	PlayList* getPlaylist() const { return playList; }

	/*
		Adds a song with the given data to the song repository.
		Throws: SongException - if the song is not valid
				DuplicateSongException - if there is another song with the same artist and title
				Throws: FileException - if the repository file cannot be opened.
	*/
	void addSongToRepository(const std::string& artist, const std::string& title, double minutes, double seconds, const std::string& source);

	/*
		Adds a given song to the current playlist.
		Input: song - Song, the song must belong to the repository.
		Output: the song is added to the playlist.
	*/
	void addSongToPlaylist(const Song& song);

	// Adds all the songs from the repository, that have the given artist, to the current playlist.
	void addAllSongsByArtistToPlaylist(const std::string& artist);

	void startPlaylist();
	void nextSongPlaylist();

	/*
		Saves the playlist.
		Throws: FileException - if the given file cannot be opened.
	*/
	void savePlaylist();

	/*
	Opens the playlist, with an appropriate application.
	Throws: FileException - if the given file cannot be opened.
	*/
	void openPlaylist() const;
};

