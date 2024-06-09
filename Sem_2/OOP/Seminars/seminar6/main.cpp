#include "UI.h"
#include "CSVPlayList.h"
#include "RepositoryExceptions.h"
#include "gui.h"
#include <QApplication>

using namespace std;

//int main()
//{
//
//	Repository repo("/Users/maraielciu/Downloads/Seminar6_914/Playlist/Songs.txt");
//	FilePlaylist* p = new CSVPlaylist{};
//	Service serv(repo, p, SongValidator{});
//	UI ui(serv);
//	ui.run();
//	delete p;
//
//	return 0;
//}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Repository repo("/Users/maraielciu/Downloads/Seminar6_914/Playlist/Songs.txt");
    FilePlaylist* p = new CSVPlaylist{};
    Service serv(repo, p, SongValidator{});
    GUI gui(serv);
    gui.show();
}