//
// Created by olivier on 07/07/16.
//

#include "AbstractArena.h"
#include "CommandTypes.h"

#include "AggressiveBehavior.h"

namespace ai
{

AggressiveBehavior::AggressiveBehavior(bool isEnabled)
    : AbstractBehavior(isEnabled)
{
}

AggressiveBehavior::~AggressiveBehavior()
{
}

void AggressiveBehavior::generateCommands(AbstractArena* arena)
{
    currentTarget_ = arena->findClosestTargetToGoodLine();
    if (currentTarget_ == nullptr) return;

    QuadRobot* quad = &arena->getQuad();
    q_.clear();

    q_.push(std::unique_ptr<FollowCommand>(new FollowCommand(quad, currentTarget_)));
    int nLeftRotations = arena->getNRotationsForOptimalDirection(*currentTarget_);
    switch (nLeftRotations)
    {
        case 1:
            // 180 rotation then right 90 rotation
            //q_.push(std::unique_ptr<FrontInteractionCommand>(new FrontInteractionCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<TopInteractionCommand>(new TopInteractionCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<FollowCommand>(new FollowCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<TopInteractionCommand>(new TopInteractionCommand(quad, currentTarget_)));
            break;

        case 2:
            // 180 rotation
            //q_.push(std::unique_ptr<FrontInteractionCommand>(new FrontInteractionCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<TopInteractionCommand>(new TopInteractionCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<FollowCommand>(new FollowCommand(quad, currentTarget_)));
            q_.push(std::unique_ptr<TopInteractionCommand>(new TopInteractionCommand(quad, currentTarget_)));
            break;
        case 3:
            // single 90 rotation
            q_.push(std::unique_ptr<TopInteractionCommand>(new TopInteractionCommand(quad, currentTarget_)));
            break;
        default:
            // do nothing
            break;
    }
}

int AggressiveBehavior::resolveCurrentStateLevelConcrete(AbstractArena* arena)
{
    currentTarget_ = arena->findClosestTargetToGoodLine();

    if (currentTarget_ == nullptr)
    {
        return 0;
    }

    OrientationEvaluation* evaluation = currentTarget_->getOrientationEvaluation();
    int stateLevel = 1;
    if (evaluation->isGoodIntersection_) {
        // TODO: Something better should be done here
        // We need to clear the q if the target is going in the good direction or the ai will keep executing the
        // interaction commands that are still there.
        q_.clear();
        q_.push(std::unique_ptr<FollowCommand>(new FollowCommand(&arena->getQuad(), currentTarget_)));
    } else {
        stateLevel = 2;
    }
    return stateLevel;
}

}
