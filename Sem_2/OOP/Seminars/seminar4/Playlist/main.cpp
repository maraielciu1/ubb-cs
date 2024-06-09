#include "UI.h"
#include "RepositoryExceptions.h"

using namespace std;

int main()
{
	system("color f4");

	try
	{
		Repository repo("Songs.txt");
		//PlayList* p = new PlayList{};
		PlayList* p = nullptr;
		cout << "What type of file would you like to use to store the playlist (CSV/JSON)?";
		string fileType{};
		cin >> fileType;
		if (fileType == "JSON")
			p = new JSONPlaylist{ "playlist.json" };
		else
			p = new CSVPlaylist{ "playlist.csv" };
		Service ctrl(repo, p);
		UI ui(ctrl);
		ui.run();
		delete p;
	}
	catch (FileException&)
	{
		cout << "Repository file could not be opened! The application will terminate." << endl;
		return 1;
	}

	return 0;
}