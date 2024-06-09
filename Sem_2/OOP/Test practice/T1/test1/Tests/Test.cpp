//
// Created by Mara Ielciu on 02.04.2024.
//

#include "Test.h"
#include "../Service/Service.h"
#include <cassert>

void Test::testAll() {
    // test for add
    Service s=Service();
    Player p1{"Nora_Mork","NOR","Larvik",83};
    s.generatePlayers();
    assert(s.getSizeServ()==5);
    assert(s.addPlayerService(p1)== false);
    Player p2{"Ana","NOR","Larvik",10};
    assert(s.addPlayerService(p2)==true);
    assert(s.getSizeServ()==6);

    //test for sort
    assert(s.get_player(0)==p2); // it is on the first position because it has the least nr of goals
    Player p3{"Pena","ESP","Cargo",59 };
    assert(s.get_player(1)==p3); // this player is the second

    std::cout<<"All tests passed\n";

}
