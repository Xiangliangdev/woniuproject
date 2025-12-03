#ifndef RK3568CONTROLSYSTEM_H
#define RK3568CONTROLSYSTEM_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class RK3568ControlSystem : public QMainWindow
{
    Q_OBJECT

public:
    explicit RK3568ControlSystem(QWidget *parent = nullptr);

private slots:
    void updateTime();
    void toggleLED();
    void toggleFan();
    void toggleBuzzer();

private:
    void setupUI();
    void setupHeader(QVBoxLayout *mainLayout);
    void setupVideoSection(QHBoxLayout *contentLayout);
    void setupControlSection(QHBoxLayout *contentLayout);
    void setupLEDControl(QVBoxLayout *parentLayout);
    void setupFanControl(QVBoxLayout *parentLayout);
    void setupBuzzerControl(QVBoxLayout *parentLayout);
    void setupSensorSection(QVBoxLayout *mainLayout);
    QWidget* createSensorWidget(const QString &name, const QString &value, const QString &unit);

    // 成员变量
    QLabel *timeLabel;
    QLabel *ledStatusLabel;
    QLabel *fanStatusLabel;
    QLabel *buzzerStatusLabel;
    QPushButton *ledToggleButton;
    QPushButton *fanToggleButton;
    QPushButton *buzzerToggleButton;

    bool ledStatus = false;
    bool fanStatus = false;
    bool buzzerStatus = false;
};

#endif // RK3568CONTROLSYSTEM_H
