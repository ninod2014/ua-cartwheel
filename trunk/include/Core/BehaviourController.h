#pragma once

#include <Core/IKVMCController.h>
#include <Core/WorldOracle.h>
#include <MathLib/Segment.h>

namespace CartWheel { namespace Core {

/**
	This class implements an intermediate-level controller. Given a low level controller (of the type IKVMCController, for now),
	this class knows how to set a series of parameters in order to accomplish higher level behaviours.

	NOTE: We will assume a fixed character morphology (i.e. joints & links), and a fixed controller structure 
	(i.e. trajectories,	components).
*/
class BehaviourController{
protected:
	Character* bip;
	IKVMCController* lowLCon;
	WorldOracle* wo;

	//we need to keep track of the position of the swing foot at the beginning of the step
	CartWheel::Math::Point3d swingFootStartPos;

	//this is the length of the leg of the character controlled by this controller
	double legLength;
	double ankleBaseHeight;
	bool shouldPreventLegIntersections;

	//these are attributes/properties of the motion
	double desiredHeading;
	double ubSagittalLean;
	double ubCoronalLean;
	double ubTwist;
	double duckWalk;
	double velDSagittal;
	double velDCoronal;
	double kneeBend;
	double coronalStepWidth;
	double leftElbowBend;
	double rightElbowBend;
	double leftShoulderTwist;
	double leftShoulderCoronal;
	double leftShoulderSagittal;
	double rightShoulderTwist;
	double rightShoulderCoronal;
	double rightShoulderSagittal;

	double stepTime;
	double stepHeight;

public:
/*
	//DEBUG ONLY
	CartWheel::Math::Point3d predSwingFootPosDebug;
	CartWheel::Math::Point3d viaPointSuggestedDebug;
	CartWheel::Math::Point3d suggestedFootPosDebug;
	Segment swingSegmentDebug;
	Segment crossSegmentDebug;
*/

	//alternate planned foot trajectory, for cases where we need to go around the stance foot...
	CartWheel::Math::Trajectory3d alternateFootTraj;

	/**
		a set of useful virtual functions, whose behavior can be overridden
	*/
	virtual void setUpperBodyPose(double leanSagittal, double leanCoronal, double twist);
	virtual void setKneeBend(double v, bool swingAlso = false);
	virtual void setDuckWalkDegree(double v);
	virtual void setDesiredHeading(double v);
	virtual void setVelocities(double velDS, double velDC);

public:
	BehaviourController(Character* b, IKVMCController* llc, WorldOracle* w = NULL);
	virtual ~BehaviourController(void);

	virtual void loadFromFile(FILE * file);
	virtual void saveToFile(FILE * file);

	virtual void adjustStepHeight();

	virtual void setElbowAngles(double leftElbowAngle, double rightElbowAngle);
	virtual void setShoulderAngles(double leftTwist, double rightTwist, double leftAdduction,
			double rightAdduction, double leftSwing, double rightSwing);

	virtual void requestStepTime(double stepTime);
	virtual void requestStepHeight(double stepHeight);
	virtual void requestVelocities(double velDS, double velDC);
	virtual void requestUpperBodyPose(double leanS, double leanC, double twist);
	virtual void requestKneeBend(double kb);
	virtual void requestDuckFootedness(double df);
	virtual void requestCoronalStepWidth(double corSW);
	virtual void requestElbowBend(double leftBend, double rightBend);

	double getDesiredVelocitySagittal() const { return velDSagittal; }
	double getDesiredVelocityCoronal() const { return velDCoronal; }

	double getDesiredSagittalLean() const { return ubSagittalLean; }
	double getDesiredCoronalLean() const { return ubCoronalLean; }
	double getDesiredUpperBodyTwist() const { return ubTwist; }

	double getCoronalStepWidth() const { return coronalStepWidth; }
	double getDesiredKneeBend() const { return kneeBend; }
	double getDesiredDuckFootness() const { return duckWalk; }

	typedef std::pair<double, double> LeftRightDouble;

	LeftRightDouble getDesiredElbowBend() const { return std::make_pair(leftElbowBend, rightElbowBend); }
	LeftRightDouble getDesiredShoulderTwist() const { return std::make_pair(leftShoulderTwist, rightShoulderTwist); }
	LeftRightDouble getDesiredShoulderCoronal() const { return std::make_pair(leftShoulderCoronal, rightShoulderCoronal); }
	LeftRightDouble getDesiredShoulderSagittal() const { return std::make_pair(leftShoulderSagittal, rightShoulderSagittal); }

	double getDesiredStepHeight() const { return stepHeight; }
	double getDesiredStepTime() const { return stepTime; }

	/**
		determines the desired swing foot location
	*/
	virtual void setDesiredSwingFootLocation();

	/**
		determine the estimate desired location of the swing foot, given the estimated position of the COM, and the phase
	*/
	virtual CartWheel::Math::Vector3d computeSwingFootLocationEstimate(const CartWheel::Math::Point3d& comPos, double phase);

	/**
		ask for a heading...
	*/
	virtual void requestHeading(double v);

	/**
		sets a bunch of parameters to some default initial value
	*/
	virtual void initializeDefaultParameters();

	/**
		this method gets called at every simulation time step
	*/
	virtual void simStepPlan(double dt);

	/**
		this method gets called every time the controller transitions to a new state
	*/
	virtual void conTransitionPlan();

	/**
		this method determines the degree to which the character should be panicking
	*/
	virtual double getPanicLevel();

	/**
		this method determines if the character should aim to abort the given plan, and do something else instead (like maybe transition to the
		next state of the FSM early).
	*/
	virtual bool shouldAbort();

	/**
		this method is used to indicate what the behaviour of the character should be, once it decides to abort its plan.
	*/
	virtual void onAbort();

	/**
		determines weather a leg crossing is bound to happen or not, given the predicted final desired position	of the swing foot.
		The suggested via point is expressed in the character frame, relative to the COM position... The via point is only suggested
		if an intersection is detected.
	*/
	bool detectPossibleLegCrossing(const CartWheel::Math::Vector3d& swingFootPos, CartWheel::Math::Vector3d* viaPoint);


	/**
		modify the coronal location of the step so that the desired step width results.
	*/
	double adjustCoronalStepLocation(double IPPrediction);
};

}
}

