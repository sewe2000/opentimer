#ifndef LASTSOLVESDASHBOARD_H
#define LASTSOLVESDASHBOARD_H

#include <deque>

#include <QObject>
#include <QLabel>
#include <QDockWidget>
#include <QVBoxLayout>

class LastSolvesDashboard : public QDockWidget
{
    Q_OBJECT
public:
    LastSolvesDashboard(QWidget* parent = nullptr);

        void addRecord(QString text, QDataStream *str = nullptr);
        void read_solves_from_session(QDataStream *stream);
        void clear();
private:
        QLabel heading;
        QLabel *ao5, *ao12;
        QHBoxLayout *avg_solves_layout;
        QFont heading_font;
        QFont regular_font;
        QVBoxLayout layout;
        QWidget background_widget;
        unsigned short number_of_solves = 0;
        std::deque<QLabel*> last_solves;

        QString get_aofx(unsigned short x);

        qint64 label_to_ms(const QLabel& time);
        QLabel ms_to_label(const qfloat16& time_in_ms);


        void write_to_session(const QString& time, QDataStream* data_stream);

};

#endif // LASTSOLVESDASHBOARD_H
