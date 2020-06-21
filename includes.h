#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#include "utility.h"
#include "classes.h"

//Globals for board and array for pieces
bool redTurn = true;
bool gameEnd = false;
BoardPoint board[9][10];

//All black pieces (side == false)
Piece* blackChariot1 = new Chariot(Chariot(0, 0, false));
Piece* blackHorse1 = new Horse(Horse(1, 0, false));
Piece* blackElephant1 = new Elephant(Elephant(2, 0, false));
Piece* blackAdvisor1 = new Advisor(Advisor(3, 0, false));
Piece* blackGeneral = new General(General(4, 0, false));
Piece* blackAdvisor2 = new Advisor(Advisor(5, 0, false));
Piece* blackElephant2 = new Elephant(Elephant(6, 0, false));
Piece* blackHorse2 = new Horse(Horse(7, 0, false));
Piece* blackChariot2 = new Chariot(Chariot(8, 0, false));
Piece* blackCannon1 = new Cannon(Cannon(1, 2, false));
Piece* blackCannon2 = new Cannon(Cannon(7, 2, false));
Piece* blackSoldier1 = new Soldier(Soldier(0, 3, false));
Piece* blackSoldier2 = new Soldier(Soldier(2, 3, false));
Piece* blackSoldier3 = new Soldier(Soldier(4, 3, false));
Piece* blackSoldier4 = new Soldier(Soldier(6, 3, false));
Piece* blackSoldier5 = new Soldier(Soldier(8, 3, false));

//All red pieces (side == true)
Piece* redChariot1 = new Chariot(Chariot(0, 9, true));
Piece* redHorse1 = new Horse(Horse(1, 9, true));
Piece* redElephant1 = new Elephant(Elephant(2, 9, true));
Piece* redAdvisor1 = new Advisor(Advisor(3, 9, true));
Piece* redGeneral = new General(General(4, 9, true));
Piece* redAdvisor2 = new Advisor(Advisor(5, 9, true));
Piece* redElephant2 = new Elephant(Elephant(6, 9, true));
Piece* redHorse2 = new Horse(Horse(7, 9, true));
Piece* redChariot2 = new Chariot(Chariot(8, 9, true));
Piece* redCannon1 = new Cannon(Cannon(1, 7, true));
Piece* redCannon2 = new Cannon(Cannon(7, 7, true));
Piece* redSoldier1 = new Soldier(Soldier(0, 6, true));
Piece* redSoldier2 = new Soldier(Soldier(2, 6, true));
Piece* redSoldier3 = new Soldier(Soldier(4, 6, true));
Piece* redSoldier4 = new Soldier(Soldier(6, 6, true));
Piece* redSoldier5 = new Soldier(Soldier(8, 6, true));

//Putting all pieces into an array for easy access for later.
Piece* pieces[32] = {blackChariot1, 
					 blackHorse1,
					 blackElephant1,
					 blackAdvisor1,
					 blackGeneral,
					 blackAdvisor2,
					 blackElephant2,
					 blackHorse2,
					 blackChariot2,
					 blackCannon1,
					 blackCannon2,
					 blackSoldier1,
					 blackSoldier2,
					 blackSoldier3,
					 blackSoldier4,
					 blackSoldier5,
					 redChariot1,
					 redHorse1,
					 redElephant1,
					 redAdvisor1,
					 redGeneral,
					 redAdvisor2,
					 redElephant2,
					 redHorse2,
					 redChariot2,
					 redCannon1,
					 redCannon2,
					 redSoldier1,
					 redSoldier2,
					 redSoldier3,
					 redSoldier4,
					 redSoldier5};

#endif