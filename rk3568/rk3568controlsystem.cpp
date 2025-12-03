#include "rk3568controlsystem.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QProgressBar>
#include <QSlider>
#include <QComboBox>
#include <QDateTime>
#include <QTimer>

RK3568ControlSystem::RK3568ControlSystem(QWidget *parent) : QMainWindow(parent)
{
    setupUI();

    // 更新时间显示
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RK3568ControlSystem::updateTime);
    timer->start(1000); // 每秒更新一次
    updateTime();
}

void RK3568ControlSystem::updateTime()
{
    QString currentTime = QDateTime::currentDateTime().toString("hh:mm | yyyy/MM/dd");
    timeLabel->setText(currentTime);
}

void RK3568ControlSystem::toggleLED()
{
    ledStatus = !ledStatus;
    ledStatusLabel->setText(ledStatus ? "开启" : "关闭");
    ledToggleButton->setText(ledStatus ? "关闭LED" : "开启LED");
}

void RK3568ControlSystem::toggleFan()
{
    fanStatus = !fanStatus;
    fanStatusLabel->setText(fanStatus ? "开启" : "关闭");
    fanToggleButton->setText(fanStatus ? "关闭风扇" : "开启风扇");
}

void RK3568ControlSystem::toggleBuzzer()
{
    buzzerStatus = !buzzerStatus;
    buzzerStatusLabel->setText(buzzerStatus ? "蜂鸣器已开启" : "蜂鸣器已关闭");
    buzzerToggleButton->setText(buzzerStatus ? "关闭蜂鸣器" : "开启蜂鸣器");
}

void RK3568ControlSystem::setupUI()
{
    setWindowTitle("RK3568 智能控制系统");
    setFixedSize(1000, 700);

    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 顶部标题栏
    setupHeader(mainLayout);

    // 中间内容区域
    QHBoxLayout *contentLayout = new QHBoxLayout();
    setupVideoSection(contentLayout);
    setupControlSection(contentLayout);
    mainLayout->addLayout(contentLayout);

    // 底部传感器区域
    setupSensorSection(mainLayout);
}

void RK3568ControlSystem::setupHeader(QVBoxLayout *mainLayout)
{
    QHBoxLayout *headerLayout = new QHBoxLayout();

    // 系统标题
    QLabel *titleLabel = new QLabel("RK3568 智能控制系统");
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    // 系统状态和时间
    QWidget *statusWidget = new QWidget();
    QVBoxLayout *statusLayout = new QVBoxLayout(statusWidget);

    QLabel *statusLabel = new QLabel("系统正常");
    statusLabel->setStyleSheet("color: green; font-size: 14px;");

    timeLabel = new QLabel();
    timeLabel->setStyleSheet("font-size: 14px;");

    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(timeLabel);
    statusLayout->setAlignment(Qt::AlignRight);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statusWidget);

    mainLayout->addLayout(headerLayout);
}

void RK3568ControlSystem::setupVideoSection(QHBoxLayout *contentLayout)
{
    QGroupBox *videoGroup = new QGroupBox("视频监控");
    QVBoxLayout *videoLayout = new QVBoxLayout(videoGroup);

    // 视频显示区域
    QLabel *videoPlaceholder = new QLabel("视频流加载中...");
    videoPlaceholder->setAlignment(Qt::AlignCenter);
    videoPlaceholder->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ccc; min-height: 300px;");
    videoLayout->addWidget(videoPlaceholder);

    // 视频参数
    QLabel *videoParams = new QLabel("分辨率: 1280x720 | 帧率: 30fps");
    videoParams->setStyleSheet("font-size: 12px; color: #666;");
    videoLayout->addWidget(videoParams);

    // 视频控制按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *recordButton = new QPushButton("开始录制");
    QPushButton *captureButton = new QPushButton("拍照");
    QPushButton *mediaButton = new QPushButton("媒体文件");

    buttonLayout->addWidget(recordButton);
    buttonLayout->addWidget(captureButton);
    buttonLayout->addWidget(mediaButton);
    videoLayout->addLayout(buttonLayout);

    contentLayout->addWidget(videoGroup, 2); // 视频区域占2份宽度
}

void RK3568ControlSystem::setupControlSection(QHBoxLayout *contentLayout)
{
    QGroupBox *controlGroup = new QGroupBox("设备控制");
    QVBoxLayout *controlLayout = new QVBoxLayout(controlGroup);

    // LED控制
    setupLEDControl(controlLayout);

    // 风扇控制
    setupFanControl(controlLayout);

    // 蜂鸣器控制
    setupBuzzerControl(controlLayout);

    contentLayout->addWidget(controlGroup, 1); // 控制区域占1份宽度
}

void RK3568ControlSystem::setupLEDControl(QVBoxLayout *parentLayout)
{
    QGroupBox *ledGroup = new QGroupBox("LED 控制");
    QVBoxLayout *ledLayout = new QVBoxLayout(ledGroup);

    // LED状态
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->addWidget(new QLabel("LED 状态:"));
    ledStatusLabel = new QLabel("关闭");
    statusLayout->addWidget(ledStatusLabel);
    statusLayout->addStretch();
    ledLayout->addLayout(statusLayout);

    // LED亮度控制
    ledLayout->addWidget(new QLabel("LED 亮度"));
    QSlider *ledBrightness = new QSlider(Qt::Horizontal);
    ledBrightness->setRange(0, 100);
    ledBrightness->setValue(50);
    ledLayout->addWidget(ledBrightness);

    // LED开关
    ledToggleButton = new QPushButton("开启LED");
    connect(ledToggleButton, &QPushButton::clicked, this, &RK3568ControlSystem::toggleLED);
    ledLayout->addWidget(ledToggleButton);

    parentLayout->addWidget(ledGroup);
}

void RK3568ControlSystem::setupFanControl(QVBoxLayout *parentLayout)
{
    QGroupBox *fanGroup = new QGroupBox("风扇控制");
    QVBoxLayout *fanLayout = new QVBoxLayout(fanGroup);

    // 风扇状态
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->addWidget(new QLabel("风扇状态:"));
    fanStatusLabel = new QLabel("关闭");
    statusLayout->addWidget(fanStatusLabel);
    statusLayout->addStretch();
    fanLayout->addLayout(statusLayout);

    // 风速模式
    fanLayout->addWidget(new QLabel("风速模式"));
    QComboBox *speedMode = new QComboBox();
    speedMode->addItems({"低速", "中速", "高速"});
    fanLayout->addWidget(speedMode);

    // 转向控制
    QHBoxLayout *directionLayout = new QHBoxLayout();
    QPushButton *forwardButton = new QPushButton("正转");
    QPushButton *reverseButton = new QPushButton("反转");
    directionLayout->addWidget(forwardButton);
    directionLayout->addWidget(reverseButton);
    fanLayout->addLayout(directionLayout);

    // 转速调节
    fanLayout->addWidget(new QLabel("转速调节"));
    QSlider *speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(0, 100);
    speedSlider->setValue(30);
    fanLayout->addWidget(speedSlider);

    // 风扇开关
    fanToggleButton = new QPushButton("开启风扇");
    connect(fanToggleButton, &QPushButton::clicked, this, &RK3568ControlSystem::toggleFan);
    fanLayout->addWidget(fanToggleButton);

    parentLayout->addWidget(fanGroup);
}

void RK3568ControlSystem::setupBuzzerControl(QVBoxLayout *parentLayout)
{
    QGroupBox *buzzerGroup = new QGroupBox("蜂鸣器控制");
    QVBoxLayout *buzzerLayout = new QVBoxLayout(buzzerGroup);

    // 蜂鸣器状态
    buzzerStatusLabel = new QLabel("状态: 蜂鸣器已关闭");
    buzzerLayout->addWidget(buzzerStatusLabel);

    // 蜂鸣器开关
    buzzerToggleButton = new QPushButton("开启蜂鸣器");
    connect(buzzerToggleButton, &QPushButton::clicked, this, &RK3568ControlSystem::toggleBuzzer);
    buzzerLayout->addWidget(buzzerToggleButton);

    parentLayout->addWidget(buzzerGroup);
}

void RK3568ControlSystem::setupSensorSection(QVBoxLayout *mainLayout)
{
    QGroupBox *sensorGroup = new QGroupBox("传感器数据");
    QHBoxLayout *sensorLayout = new QHBoxLayout(sensorGroup);

    // 温度
    QWidget *tempWidget = createSensorWidget("温度", "23.3", "°C");
    sensorLayout->addWidget(tempWidget);

    // 湿度
    QWidget *humidityWidget = createSensorWidget("湿度", "49.7", "%RH");
    sensorLayout->addWidget(humidityWidget);

    // 光照
    QWidget *lightWidget = createSensorWidget("光照", "100", "lux");
    sensorLayout->addWidget(lightWidget);

    // 红外
    QWidget *irWidget = createSensorWidget("红外", "无物体", "检测状态");
    sensorLayout->addWidget(irWidget);

    mainLayout->addWidget(sensorGroup);
}

QWidget* RK3568ControlSystem::createSensorWidget(const QString &name, const QString &value, const QString &unit)
{
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-weight: bold;");

    QLabel *valueLabel = new QLabel(value);
    valueLabel->setAlignment(Qt::AlignCenter);
    valueLabel->setStyleSheet("font-size: 18px;");

    QLabel *unitLabel = new QLabel(unit);
    unitLabel->setAlignment(Qt::AlignCenter);
    unitLabel->setStyleSheet("font-size: 12px; color: #666;");

    layout->addWidget(nameLabel);
    layout->addWidget(valueLabel);
    layout->addWidget(unitLabel);

    widget->setStyleSheet("border: 1px solid #ccc; border-radius: 5px; padding: 10px;");

    return widget;
}

