#pragma once

#include <QObject>
#include <iostream>

class callDelay : public QObject
{
    Q_OBJECT
public:
    explicit callDelay(QObject* parent = nullptr);
    ~callDelay();

    Q_INVOKABLE void doDelay();

signals:
    void delayFinished();


};

