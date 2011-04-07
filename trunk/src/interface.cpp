#include <Control/WrapperAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>
#include <Control/ActRecognizerInterface.h>
#include <boost/assign/std/vector.hpp>

using std::vector;
using std::cout;
using std::endl;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace boost::assign;

#define ACTREC

int main(int argc, char** argv)
{
  SimulationInterface interface(true);

  string actor1 = "Human1";
  string actor2 = "Human2";

  // NEW
  StartStatePtr s1(new StartState(actor1, 2, 2, -3.14 / 1.5));
  StartStatePtr s2(new StartState(actor2, -1, -1, 0.0));

  vector<StartStatePtr> start_state;
  start_state += s1, s2;

  // FIRST HUMAN
  vector<double> params1;
  params1 += 5.0, 0.5;

  vector<double> params2;
  params2 += 5.0, 0.0; //no second param for standStill

  vector<double> params3;
  params3 += 5.0, -0.5;

  vector<ExtendedActionPtr> actions1;
  ExtendedActionPtr a1(new WrapperAction("walk", actor1, params1));
  //ExtendedActionPtr a2(new WrapperAction("standStill", actor1, params2));
  ExtendedActionPtr a2(new WrapperAction("walk", actor1, params2));
  ExtendedActionPtr a3(new WrapperAction("walk", actor1, params3));
  actions1 += a1, a2, a3;

  // SECOND HUMAN
  vector<double> params4;
  params4 += 10.0, 0.5;
  ExtendedActionPtr a4(new WrapperAction("walk", actor2, params4));

  vector<double> params5;
  params5 += 20.0; //no second param for standStill
  ExtendedActionPtr a5(new WrapperAction("standStill", actor2, params5));

  vector<ExtendedActionPtr> actions2;
  actions2 += a4; // Partial duration, comment out for empty action vector
//  actions2 += a5; // Comment in for full duration

  vector<vector<ExtendedActionPtr> > actions;
  actions += actions1, actions2;

  interface.simulate(start_state, actions);
  vector<PosState*> trajectory = interface.getPositions();
  vector<CapsuleState*> capsule_states = interface.getCapsules();
  vector<RelationalState*> rel_states = interface.getRelations();
  cout << "GOT " << trajectory.size() << " STATES" << endl;

#ifdef ACTREC
  const vector<string>& namers = interface.getLastHumanNames();

  ActRecognizerInterface ari(string("meet"), namers);
#endif

  for (int i = 0; i < trajectory.size(); ++i)
  {
    PosState* pos_state = trajectory[i];
    CapsuleState* cap_state = capsule_states[i];

    cout << "STATE " << i << "\n=============================\n";
    cout << "POSITIONS:" << endl;
    for (int j = 0; j < pos_state->getNumVectors(); ++j)
    {
      cout << pos_state->getName(j) << ": " << pos_state->getPosition(j).x << " " << pos_state->getPosition(j).z
          << endl;

    }
    cout << "RELATIONS " << i << "\n=============================\n";
    cout << rel_states[i]->toString() << endl;

#ifdef ACTREC
    ari.progress(*(rel_states[i]));
    if (ari.getCurTerminal())
      cout << ari.getFullVerbName() << " achieved at step " << i << endl;
#endif

    cout << "CAPSULES:" << endl;
    for (int j = 0; j < cap_state->getNumEntities(); ++j)
    {
      cout << cap_state->getName(j) << ": " << cap_state->getCapsules(j).size() << endl;
    }
  }

  cout << "running a second time..." << endl;
  interface.simulate(start_state, actions);

}
