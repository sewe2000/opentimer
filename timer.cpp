#include "timer.hpp"
#include "mainwindow.h"

#include <QTime>
#include <QThread>
#include <QTimer>
#include <QFontDatabase>

Timer::Timer(QWidget *parent) :QLabel("00:00.000", parent)
{

        p_font = new QFont( "Roboto Mono", 60);
        p_font->setBold(true);
        p_timer = new QElapsedTimer();
        background = new QWidget(parent);
        setFont(*p_font);


        holding_timer.setInterval(_holding_time);
        holding_timer.callOnTimeout([this] {this->changeState(READY);});
}

Timer::~Timer()
{
    delete p_timer;
    delete p_font;
}

void Timer::changeState(State target_state)
{
        if(target_state == State::PREPARING)
           setStyleSheet("Timer { color: red; }");
        else if(target_state == State::NEUTRAL || target_state == RUNNING)
        {
           setStyleSheet("Timer { color: black; }");
           if(target_state == State::NEUTRAL)
               emit blur();
        }
        else if(target_state == State::READY)
        {
           setStyleSheet("Timer { color: green; }");
           emit focus();
        }

        e_state = target_state;

}

void Timer::start()
{
    QTime time(0,0,0);
    stop_holding();
    refresh_timer.setInterval(1);
    refresh_timer.start();
    p_timer->restart();
    refresh_timer.callOnTimeout([this] {this->refresh(this->p_timer->elapsed());});
    changeState(RUNNING);

}

void Timer::stop()
{
   reset();
   changeState(NEUTRAL);
}

void Timer::refresh(const qint64& time_in_ms)
{
    QTime t(0,0,0,0);
    t = t.addMSecs(time_in_ms);
    setText(t.toString(_TIME_FORMAT));
}

