#ifndef SCRAMBLEGENERATOR_H
#define SCRAMBLEGENERATOR_H

#include "types.hpp"
#include "regularcubemap.hpp"

#include <QDockWidget>
#include <QObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QComboBox>


class ScrambleButton : public QPushButton
{
public:
    explicit ScrambleButton(const QString & text, QWidget * parent = nullptr): QPushButton(text, parent) {}
protected:
    void keyPressEvent(QKeyEvent*) override {
        QKeyEvent *space = new QKeyEvent(QEvent::KeyPress,Qt::Key_Space, Qt::NoModifier);
        QCoreApplication::postEvent(parent()->parent(), space);
    }
};

class ScrambleGenerator : public QDockWidget
{
    Q_OBJECT
public:

    explicit ScrambleGenerator(QWidget *parent = nullptr);
    ~ScrambleGenerator();
    void generate_scramble(ushort length);
    int get_length() {return length_edit->text().toUInt();}
private:
    QLabel *scramble, *cube_type;
    QComboBox *choose_cube_type;
    QVBoxLayout *layout;
    QWidget *main_widget;
    QFont *heading_font;
    QLineEdit *length_edit;
    ScrambleButton *generate_button;
    RegularCubeMap *scrambled_cube_map;

    CubeType current_cube;
    const QString moves_for_3x3[18] = {
            "R", "R\'",   "R2",
            "L", "L\'",   "L2",
            "U", "U\'",   "U2",
            "D", "D\'",   "D2",
            "F", "F\'",  "F2",
             "B", "B\'",  "B2",
    };
public slots:
    void generate();
    void cube_type_changed();

};

#endif // SCRAMBLEGENERATOR_H
