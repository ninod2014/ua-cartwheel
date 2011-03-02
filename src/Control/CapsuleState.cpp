/*
 * CapsuleState.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#include <Control/CapsuleState.h>
#include <Core/Human.h>

using CartWheel::CartWheel3D;
using CartWheel::Core::Human;
using CartWheel::Math::Point3d;
using CartWheel::Math::Capsule;
using CartWheel::Physics::ArticulatedRigidBody;
using CartWheel::Physics::CapsuleCDP;

CapsuleState::CapsuleState()
{
}

CapsuleState::CapsuleState(CartWheel3D* cw)
{
  populate(cw);
}

CapsuleState::~CapsuleState()
{
}

void CapsuleState::populate(CartWheel3D* cw)
{
  for (int i = 0; i < cw->getHumanCount(); ++i)
  {
    Human* human = cw->getHuman(i);

    names_.push_back(human->getName());
    std::vector<Capsule*> capsules;
    for (int j = 0; j < human->getCharacter()->getArticulatedRigidBodyCount(); ++j)
    {
      ArticulatedRigidBody* bodyPart = human->getCharacter()->getArticulatedRigidBody(j);
      Point3d position = bodyPart->getCMPosition();
      CapsuleCDP* cdp = (CapsuleCDP*) (bodyPart->getCDP(0));

      Point3d p1 = bodyPart->getWorldCoordinates(cdp->getPoint1());
      Point3d p2 = bodyPart->getWorldCoordinates(cdp->getPoint2());

      capsules.push_back(new Capsule(p1, p2, cdp->getRadius()));
    }

    capsules_.push_back(capsules);
  }
}

int CapsuleState::getNumEntities()
{
  return names_.size();
}

std::string CapsuleState::getName(int index)
{
  return names_[index];
}

std::vector<CartWheel::Math::Capsule*> CapsuleState::getCapsules(int index)
{
  return capsules_[index];
}

void CapsuleState::clear()
{
  names_.clear();
  capsules_.clear();
}