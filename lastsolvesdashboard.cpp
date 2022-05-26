#include "lastsolvesdashboard.h"

#include <QFontDatabase>
#include <QTime>
#include <QRegularExpression>
#include <QDebug>

LastSolvesDashboard::LastSolvesDashboard(QWidget* parent): QDockWidget("Last Solves", parent)
{
        heading_font = QFont( "Roboto Mono", 40);
        heading_font.setBold(true);
        regular_font = QFont("Roboto Mono", 16);

        heading.setText("Last Solves:");
        heading.setFont(heading_font);
        setWidget(&background_widget);
        background_widget.setLayout(&layout);
        layout.addWidget(&heading);
        layout.addStretch(5);

        ao5 = new QLabel("Ao5: -");
        ao12 = new QLabel("Ao12: -");
        ao5->setFont(regular_font);
        ao12->setFont(regular_font);
        ao5->setAlignment(Qt::AlignHCenter);
        ao12->setAlignment(Qt::AlignHCenter);


        avg_solves_layout = new QHBoxLayout();
        layout.addLayout(avg_solves_layout);
        avg_solves_layout->addWidget(ao5);
        avg_solves_layout->addWidget(ao12);

        setStyleSheet("LastSolvesDashboard { border: 3px solid black; }");


}

void LastSolvesDashboard::addRecord(QString text, QDataStream *str)
{
    if(last_solves.size() == 12)
    {
        delete last_solves.front();
        last_solves.pop_front();
    }
    number_of_solves++;
    if(str)
        text.prepend(QString::number(number_of_solves) + ".");
    last_solves.push_back(new QLabel(text));
    QLabel *back = last_solves.back();
    back->setFont(regular_font);
    back->setAlignment(Qt::AlignHCenter);

    if(last_solves.size() >= 5)
        ao5->setText(get_aofx(5));
    if(last_solves.size() >= 12)
    {
        ao12->setText(get_aofx(12));

    }

    unsigned short before_last_item = last_solves.size() + 1;
    layout.insertWidget(before_last_item, back);
    if(str)
        write_to_session(text, str);


}

/*
*
* This function takes the last x time to calculate the average time
* and returns the result string such as "Aof5: 00:21.32"
*
*/

QString LastSolvesDashboard::get_aofx(unsigned short x)
{
    unsigned short position = last_solves.size() - x;
    qint64 sum = 0;

    for(std::deque<QLabel*>::size_type i = position; i < last_solves.size(); i++)
        sum += label_to_ms(*(last_solves[i]));

    QTime result(0,0,0,0);
    QString result_string =  result.addMSecs(sum / x).toString("mm:ss.z");
    result_string.prepend("Ao" + QString::number(x) + ": ");
    return result_string;


}

qint64 LastSolvesDashboard::label_to_ms(const QLabel& time)
{
        QString text = time.text();
        text.remove(QRegularExpression("^[0-9]+" + QRegularExpression::escape(".")));
        QTime tmp = QTime::fromString(text, "mm:ss.z");
        return tmp.msecsSinceStartOfDay();
}

void LastSolvesDashboard::write_to_session(const QString& time, QDataStream* stream)
{
        *stream << time;
}

void LastSolvesDashboard::read_solves_from_session(QDataStream *stream)
{
    QString tmp;
    while(!stream->atEnd())
    {
      *stream >> tmp;
      addRecord(tmp);
    }
}

void LastSolvesDashboard::clear()
{
        for(auto i: last_solves)
            delete i;
        last_solves.clear();
        number_of_solves = 0;
        ao5->setText("Ao5: ");
        ao12->setText("Ao12: ");
}
