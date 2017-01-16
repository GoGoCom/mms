#pragma once

#include <QObject>

#include "Maze.h"
#include "Mouse.h"
#include "MouseStats.h"

namespace mms {

class Model : public QObject {

    Q_OBJECT

public:

    static void init();
    static Model* get();

    void simulate();

    void setMaze(const Maze* maze);

    void addMouse(const QString& name, Mouse* mouse);
    bool containsMouse(const QString& name) const;
    MouseStats getMouseStats(const QString& name) const;

signals:

    void newTileLocationTraversed(int x, int y);

private:

    Model();
    static Model* INSTANCE;

    const Maze* m_maze;
    QMap<QString, Mouse*> m_mice;
    QMap<QString, MouseStats> m_stats;

    void checkCollision();
};

} // namespace mms
