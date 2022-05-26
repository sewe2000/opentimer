#ifndef TIMER_HPP
#define TIMER_HPP

#include <QObject>
#include <QLabel>
#include <QElapsedTimer>
#include <QTimer>
#include <QTime>
#include <QDockWidget>

class Timer :public QLabel
{
        Q_OBJECT
   public:

        enum State { NEUTRAL, PREPARING, READY, RUNNING};
        static constexpr int _holding_time = 1000;
        static constexpr char _TIME_FORMAT[] = "mm:ss.zzz";

        Timer(QWidget *parent = nullptr);
        ~Timer();

        void changeState(State target_state);
        State getState() {return e_state;}
        void start();
        void stop();

        void start_holding() {holding_timer.start();}
        void stop_holding() {holding_timer.stop(); holding_timer.setInterval(_holding_time);}
        QTime get_current_time() { return QTime(0,0,0,0).addMSecs(p_timer->elapsed()); }



   private:
        QElapsedTimer *p_timer;
        QFont *p_font;
        QTimer refresh_timer;
        QTimer holding_timer;
        QWidget *background;

        State e_state = State::NEUTRAL;

        QString convertTimeToString(qint64 milisecs);

        void refresh(const qint64& time_in_ms);
        void reset() { refresh_timer.stop(); stop_holding();}


};

#endif // TIMER_HPP
