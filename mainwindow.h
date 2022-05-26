#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "timer.hpp"
#include "lastsolvesdashboard.h"
#include "scramblegenerator.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QApplication>



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void first_session();

    void display_error_msgbox(const QString& error);
    static void get_into_focus_mode();
    static void get_outof_focus();

private:
    static Timer *p_cubing_timer;
    static LastSolvesDashboard *p_last_solves_dashboard;
    QWidget *p_window_widget;
    QHBoxLayout *p_window_layout;

    QMenu *file_menu;
    QMenu *help_menu;
    QAction *exit;

    QFile *session_file;
    QDir *base_dir;
    QDataStream *session_stream;

    static ScrambleGenerator *scramble_generator;
    bool isInActiveState = true;


    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    void add_solve_to_dashboard(QTime time);
    void disable_widgets();
    void enable_widgets();



signals:
    void timer_stopped();
private slots:
    void exit_app();
    void about();
    void new_session();
    void load_session(const QString& location = "");
};
#endif // MAINWINDOW_H
