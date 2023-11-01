#pragma once
#include "Stage.h"
#include <vector>
#include <memory>
class StageManager
{
public:
    int getStage() { return stage; }

public:
    void setStage(const int& st) { stage = st; }

private:
    std::vector<std::unique_ptr<Stage>> stageList;
    int stage;
};

