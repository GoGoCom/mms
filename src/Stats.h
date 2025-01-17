#pragma once

#include <QLineEdit>
#include <QMap>

namespace mms {

enum class StatsEnum{
    SPEED_RUNLEFT,
    SPEED_RUNRIGHT,
    SPEED_TURNLEFT45,
    SPEED_TURNRIGHT45,
    SPEED_HALFFORWARD,
    SPEED_EDGEFORWARD,
    SPEEDSUM,
    COUNT_RUNLEFT,
    COUNT_RUNRIGHT,
    COUNT_TURNLEFT45,
    COUNT_TURNRIGHT45,
    COUNT_TURNLEFT90,
    COUNT_TURNRIGHT90,
    COUNT_TURNBACK180,
    COUNT_FULLFORWARD,
    COUNT_HALFFORWARD,
    COUNT_EDGEFORWARD,
    TOTAL_DISTANCE,
    TOTAL_TURNS,
    BEST_RUN_DISTANCE,
    BEST_RUN_TURNS,
    CURRENT_RUN_DISTANCE,
    CURRENT_RUN_TURNS,
    TOTAL_EFFECTIVE_DISTANCE,
    BEST_RUN_EFFECTIVE_DISTANCE,
    CURRENT_RUN_EFFECTIVE_DISTANCE,
    SCORE // has a text box but is not saved in an array
};

class Stats
{
public:
    Stats();
    void updateScore();
    void resetAll(); // Reset all score stats
    void addDistance(StatsEnum stat, int distance); // Increase the distance and effective distance
    void addTurn(StatsEnum stat); // Increment the number of turns
    void bindText(StatsEnum stat, QLineEdit* uiText); // Indicate which QLineEdit to use for that stat
    void startRun(); // A run starts when the mouse exits the starting tile.
    void finishRun(); // A run finishes when the mouse enters the goal.
    void endUnfinishedRun(); // A run ends unfinished when the mouse returns to the start tile
    void penalizeForReset(); // Applies a penalty when the mouse resets to the start tile
    QString getStat(StatsEnum stat); // Return the current value of the requested stat

private:
    QMap<StatsEnum, float> statValues;
    QMap<StatsEnum, QLineEdit*> textField;
    bool startedRun;
    bool solved;
    float penalty;
    void increment(StatsEnum stat, float increase);
    void setStat(StatsEnum stat, float value);
    static float getEffectiveDistance(int distance);
    void reset(StatsEnum stat);
    bool isInteger(StatsEnum stat);
};

}
