#pragma once
#include "Block.h"
class GoalBlock : public Block
{
public:
    GoalBlock(int nameId, int typeId);
    GoalBlock(const GoalBlock* data);
    void update(float elapsedTime) override;
    void setGoalFloor(Block* goalBlock) { goalFloor = goalBlock; }
    void draw() override;   
private:
    Block* goalFloor;

};

