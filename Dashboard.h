#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QGridLayout>

class Dashboard : public QMainWindow {
    Q_OBJECT

private:
    QWidget *mainWidget;
    QGridLayout *layout;

public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();
};

#endif // DASHBOARD_H
