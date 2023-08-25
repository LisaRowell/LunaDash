#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget *mainWidget;
    QGridLayout *layout;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAIN_WINDOW_H
