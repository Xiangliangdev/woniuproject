#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent)
{
    // 设置窗口属性
    setWindowTitle("RK3568 系统登录");
    setFixedSize(400, 300);

    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel("RK3568 控制系统");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 用户名输入框
    usernameInput = new QLineEdit();
    usernameInput->setPlaceholderText("用户名");
    usernameInput->setStyleSheet("padding: 8px; font-size: 14px;");
    usernameInput->setFixedWidth(250);
    mainLayout->addWidget(usernameInput);

    // 密码输入框
    passwordInput = new QLineEdit();
    passwordInput->setPlaceholderText("密码");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setStyleSheet("padding: 8px; font-size: 14px;");
    passwordInput->setFixedWidth(250);
    mainLayout->addWidget(passwordInput);

    // 登录按钮
    loginButton = new QPushButton("登录");
    loginButton->setStyleSheet("padding: 8px 16px; font-size: 16px;");
    loginButton->setFixedWidth(150);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
    mainLayout->addWidget(loginButton, 0, Qt::AlignCenter);

    // 初始化控制系统窗口
    controlSystem = new RK3568ControlSystem();
    controlSystem->hide();
}

void LoginWindow::attemptLogin()
{
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    // 固定账号密码
    if (username == "admin" && password == "123456") {
        this->hide();
        controlSystem->show();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}
