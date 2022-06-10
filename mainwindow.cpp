#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>

LastSolvesDashboard *MainWindow::p_last_solves_dashboard;
ScrambleGenerator *MainWindow::scramble_generator;
Timer *MainWindow::p_cubing_timer;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    setAnimated(true);
    setFocus();
    resize(sizeHint().width(), 500);

    p_window_widget = new QWidget(this);
    p_window_layout = new QHBoxLayout(p_window_widget);

    p_cubing_timer = new Timer(p_window_widget);
    p_cubing_timer->setMinimumWidth(p_cubing_timer->sizeHint().width());

    p_last_solves_dashboard = new LastSolvesDashboard(p_window_widget);
    scramble_generator = new ScrambleGenerator(this);
    setCentralWidget(p_cubing_timer);


    p_window_widget->setLayout(p_window_layout);


    addDockWidget(Qt::LeftDockWidgetArea, p_last_solves_dashboard);
    addDockWidget(Qt::RightDockWidgetArea, scramble_generator);

    setWindowTitle("OpenCubing Timer made by Seweryn P.");

    file_menu = menuBar()->addMenu(tr("&File"));
    help_menu = menuBar()->addMenu(tr("&Help"));

    exit = new QAction(tr("&Exit"));
    exit->setShortcut(QKeySequence(tr("Ctrl+Q")));
    QObject::connect(exit, &QAction::triggered, this, &MainWindow::exit_app );

    QAction *about = new QAction(tr("&About"));
    QObject::connect(about, &QAction::triggered, this, &MainWindow::about);

    QAction *new_session_action = new QAction(tr("&New Session"));
    new_session_action->setShortcut(QKeySequence(tr("Ctrl+N")));
    QObject::connect(new_session_action, &QAction::triggered, this, &MainWindow::new_session);

    QAction *load_session_action = new QAction(tr("&Load Session"));
    load_session_action->setShortcut(QKeySequence(tr("Ctrl+O")));
    QObject::connect(load_session_action, &QAction::triggered, this, [this] { this->load_session(); });

    file_menu->addAction(new_session_action);
    file_menu->addAction(load_session_action);
    file_menu->addAction(exit);
    help_menu->addAction(about);

    QObject::connect(this, &MainWindow::timer_stopped, scramble_generator, &ScrambleGenerator::generate );

    base_dir = new QDir();
    session_file = new QFile();
    session_stream = new QDataStream();


    disable_widgets();
    emit timer_stopped();

    // libusb_hotplug_register_callback(
    // NULL,
    // LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED,
    // LIBUSB_HOTPLUG_ENUMERATE,
    //
    //
    // );

}

MainWindow::~MainWindow()
{
    session_file->close();
}

void MainWindow::first_session()
{
#ifdef __gnu_linux__
        base_dir->setPath(QStandardPaths::standardLocations(QStandardPaths::DataLocation).at(0));
#elif defined(__WIN32)
        base_dir->setPath(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0]);
#else
        #error "The OpenTimer only supports MS Windows and GNU/Linux at the moment!"
#endif
    QFile cache_file( QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0) + "/open_timer_last_session");
    if (cache_file.exists())
    {
        if(cache_file.open(QFile::ReadOnly))
        {
                QDataStream temp;
                temp.setDevice(&cache_file);
                QString filename;
                temp >> filename;
                session_file->setFileName(filename);
                cache_file.close();
                load_session(session_file->fileName());

        }


    }
    if(!base_dir->exists())
    {
          bool result =  base_dir->mkdir(base_dir->path());
          if(!result)
                display_error_msgbox("Couldn't create base directory at: " + base_dir->path());
    }
    if(base_dir->isEmpty())
    {
        QMessageBox session_box;
        session_box.setText("I can see that there's no session file. Do you want to create it?");
        session_box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        session_box.setInformativeText("Do you want to create a new session file?");
        int result = session_box.exec();
        if(result == QMessageBox::Ok)
        {
               new_session();
               enable_widgets();
        }

    }

}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
        if(event->key() == Qt::Key_Space)
        {
                if(p_cubing_timer->getState() == Timer::NEUTRAL)
                {
                        p_cubing_timer->changeState(Timer::State::PREPARING);
                        p_cubing_timer->start_holding();
                }
                else if(p_cubing_timer->getState() == Timer::RUNNING)
                {
                        p_cubing_timer->stop();
                        emit timer_stopped();
                        add_solve_to_dashboard(p_cubing_timer->get_current_time());

                }

        }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
        if(event->isAutoRepeat())
            return;
        if(event->key() == Qt::Key_Space)
        {
            if(p_cubing_timer->getState() == Timer::State::PREPARING)
            {
                p_cubing_timer->changeState(Timer::State::NEUTRAL);
                p_cubing_timer->stop_holding();
            }
            else if (p_cubing_timer->getState() == Timer::READY)
                p_cubing_timer->start();

        }
}

void MainWindow::add_solve_to_dashboard(QTime time)
{
       p_last_solves_dashboard->addRecord(time.toString(Timer::_TIME_FORMAT), session_stream);
}

void MainWindow::display_error_msgbox(const QString &error)
{
    QMessageBox::critical(this, "An error accured!", error);
}

void MainWindow::disable_widgets()
{
        for(auto *widget: findChildren<QDockWidget*>())
        {
                widget->setEnabled(false);
        }
        p_cubing_timer->setEnabled(false);
        isInActiveState = false;

}

void MainWindow::enable_widgets()
{
        for(auto *widget: findChildren<QDockWidget*>())
        {
                widget->setEnabled(true);
        }
        p_cubing_timer->setEnabled(true);

        isInActiveState = true;
}

void MainWindow::get_into_focus_mode()
{
   p_last_solves_dashboard->hide();
   scramble_generator->hide();
   p_cubing_timer->setAlignment(Qt::AlignCenter);
}

void MainWindow::get_outof_focus()
{
   p_last_solves_dashboard->show();
   scramble_generator->show();
}
/* ============================ SLOTS =================================== */

void MainWindow::exit_app()
{
       QDir cache_dir = QDir(QStandardPaths::standardLocations(QStandardPaths::TempLocation).at(0));
       QFile last_session(cache_dir.path() + "/open_timer_last_session");
       if( last_session.open(QFile::WriteOnly) )
       {
        QDataStream temp_stream;
        temp_stream.setDevice(&last_session);
        temp_stream << session_file->fileName();
        last_session.close();
       }
       QApplication::quit();
}

void MainWindow::about()
{
        QMessageBox::information(this, "About", "This program is used to measure your solve times on different kinds of Rubik\'s cubes. It is an open source software under GPL v3  License created by Seweryn P.");
}

void MainWindow::new_session()
{
        QString previous_path;
        QString file_name = QFileDialog::getSaveFileName(this, "Select new session file", base_dir->path(), tr("OpenTimer Session File (*.ots)"));

        if(file_name == "")
            return;
        QFileInfo file_info(*session_file);
        previous_path = file_info.path();

        session_file->close();

        session_file->setFileName(file_name);
       if(session_file->open(QIODevice::WriteOnly))
       {
          base_dir->setPath(file_info.dir().path());
          session_stream->setDevice(session_file);
          p_last_solves_dashboard->clear();
          enable_widgets();

       }
       else
       {
                display_error_msgbox("Couldn't open file at: " + file_name);
       }
}

void MainWindow::load_session(const QString& location)
{ 
    QString file_name, previous_path;
    if(location == "")
        file_name = QFileDialog::getOpenFileName(this, "Select new session file", base_dir->path(), tr("OpenTimer Session File (*.ots)"));
    else
        file_name = location;

    if(file_name == "")
        return;

    QFileInfo file_info(*session_file);
    previous_path = file_info.path();

    session_file->close();
    session_file->setFileName(file_name);

    if(session_file->open(QIODevice::ReadWrite))
    {
          base_dir->setPath(file_info.dir().path());
          session_stream->setDevice(session_file);
          p_last_solves_dashboard->clear();
          p_last_solves_dashboard->read_solves_from_session(session_stream);

          if(!isInActiveState)
              enable_widgets();
    }
    else
    {
        display_error_msgbox("Couldn't open file at: " + file_name);
    }

}
