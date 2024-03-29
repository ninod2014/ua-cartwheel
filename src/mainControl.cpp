#include <Core/CartWheel3D.h>
#include <Control/StaticWalkNearPolicy.h>
#include <Control/ExtendedAction.h>
#include "Control/RelationalState.h"
#include "Control/PosState.h"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using std::max;
using std::min;
using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;

double angle;
double speed;

CartWheel3D* simulator;
char* sPath;

static int process_option(const char* option, const char* value);

const double SQRT2 = M_SQRT2;

void init_simulation(int argc, char** argv)
{
  sPath = new char[200];
  strcpy(sPath, "");
  simulator = new CartWheel3D(sPath);
  simulator->addObject("ground", "data/objects/flatGround.rbs", -1);

  //simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);

  string humanName = "Human1";
  string humanModel = "data/characters/bipV3.rbs";
  string humanController = "data/controllers/bipV3/Walking.sbc";

  simulator->addHuman(humanName, humanModel, humanController, Point3d(0.0, 1.0, -2.0), 0);
  string humanName2 = "Human2";

  simulator->addHuman(humanName2, humanModel, humanController, Point3d(0.0, 1.0, 0.0), 0);
  simulator->setHumanSpeed(humanName2, 0);
}

int main(int argc, char** argv)
{
//  StaticWalkNearPolicy pol(1.2, "0");
//  PosState ps;
//  RelationalState rs;
//  FILE* option_fp;
//
//  init_simulation(argc, argv);
//  cout << "HERE" << endl;
//  //for (int timer = 0; timer < 10000; timer++)
//  //{
//
//  std::vector<PosState*> *posStates = new std::vector<PosState*>();
//
//  for(int x = 0; x < 10; x++){
//    rs.reset(ps, simulator);
//    cout<<"relations: "<< rs.toString()<<endl;
//     ps.reset(simulator);
//    pol.getAction(simulator)->execute(simulator, posStates);
//   }
//    //simulator->runStep(.001);
//   // cout << "human location : " << simulator->getHumanPosition(0).getX() << "  "
//     //   << simulator->getHumanPosition(0).getZ() << endl;
//  //}
}
