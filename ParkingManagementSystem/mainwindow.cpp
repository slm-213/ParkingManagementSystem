#include "mainwindow.h"
#include "ui_mainwindow.h"
// - 初始化与输入校验；
// - 入/出场流程（含轻量行驶动画）；
// - 队列/车位实时刷新与日志输出；
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <ctime>
#include <QRandomGenerator>
void MainWindow::on_btnRandPlate_clicked()//随机生成车牌点击
{
    // 简单随机车牌：字母+数字组合，长度5~6
    static const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const char digits[]  = "0123456789";
    auto rng = QRandomGenerator::global();//随机数生成器
    int len = rng->bounded(5, 7); // 5或6
    QString plate;//存储车牌字符串
    for (int i = 0; i < len; ++i) {//循环生成字符
        if (i == 0 || rng->bounded(2) == 0) {//除了第一个其他都百分之50
            plate += QChar(letters[rng->bounded(int(sizeof(letters) - 1))]);//-1为了排除空符，bound生成0到n-1之间的整数
        } else {
            plate += QChar(digits[rng->bounded(int(sizeof(digits) - 1))]);//数字
        }
    }
    ui->editCarNum->setText(plate);//将随机生成的车牌字符串显示到界面的车牌输入框中
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)//调用父类构造
    , ui(new Ui::MainWindow), lot(nullptr)//创建ui实例，指针为空
{
    ui->setupUi(this);//界面初始化
    timer = new QTimer(this);//定时器初始化
    debounceTimer = new QTimer(this);
    debounceTimer->setSingleShot(true); // 单次触发
    debounceTimer->setInterval(500); // 500ms防抖
    connect(timer, &QTimer::timeout, this, &MainWindow::autoCarExit);
    connect(debounceTimer, &QTimer::timeout, this, &MainWindow::enableEnterButton);
    // 不做手动connect，使用Qt的自动连接（on_<object>_<signal>命名）会自动绑定，避免重复连接
    ui->btnEnter->setAutoDefault(false);
    ui->btnEnter->setDefault(false);
    ui->btnExit->setAutoDefault(false);
    ui->btnExit->setDefault(false);

    // 防止回车触发默认按钮
    ui->btnInit->setAutoDefault(false);
    ui->btnInit->setDefault(false);

    // 初始状态：未初始化时禁用入场和出场按钮
    ui->btnEnter->setEnabled(false);
    ui->btnExit->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (lot) delete lot;
}

void MainWindow::on_btnInit_clicked()//初始化点击
{
    if (isInitializingProcessing) return;//若为真则直接返回
    isInitializingProcessing = true;
    if (hasInitialized) { isInitializingProcessing = false; return; } //是否已完成初始化
    hasInitialized = true;

    bool ok1, ok2;
    int n = ui->editN->text().toInt(&ok1);//从界面上的editN输入框获取文本，尝试转换为整数n并将转换是否成功的结果存入ok1。
    int m = ui->editM->text().toInt(&ok2);//从界面上的editN输入框获取文本，尝试转换为整数m并将转换是否成功的结果存入ok2。
    if (!ok1 || !ok2 || n <= 0 || m < 0) {//转换失败或者范围不符合给出警告
        QMessageBox::warning(this, "提示", "请输入有效的n和m！");
        hasInitialized = false; // 初始化失败，重置标志
        isInitializingProcessing = false;
        return;
    }
    if (m > n) {
        QMessageBox::warning(this, "提示", "队列上限 m 不能超过车位数 n，请修改后再初始化。");
        hasInitialized = false;
        isInitializingProcessing = false;
        ui->editM->setFocus();//将界面焦点设置到editM输入框
        return;
    }
    if (lot) delete lot;//释放旧实例
    lot = new ParkingLot(n, m);//创建新停车场实例
    // 设置时间倍率：1秒=1分钟
    ParkingLot::setSecondsPerSimulatedMinute(1);//设置时间模拟倍率
    updateParkingDisplay();//刷新界面显示
    timer->start(1000); // 启动定时器
    showFeeDetails("停车场已初始化。");
    ui->editCarNum->clear(); //初始化后清空车牌输入
    ui->editCarNum->setFocus(); // 设置焦点到车牌输入框
    // 初始化成功后启用按钮
    ui->btnEnter->setEnabled(true);
    ui->btnExit->setEnabled(true);
    isInitializingProcessing = false;
}

void MainWindow::simulateEnterDrive(const QString &carNum, int spotId, time_t now)//模拟车辆入场
{
    if (isDriving) return;
    isDriving = true;
    showFeeDetails(QString("车辆 %1 进入入口，前往车位 %2...").arg(carNum).arg(spotId));
    QTimer::singleShot(400, this, [=]() {//[=]()拿到当前上下文里的变量
        showFeeDetails(QString("车辆 %1 行驶中... (1/2)").arg(carNum));
    });
    QTimer::singleShot(800, this, [=]() {
        showFeeDetails(QString("车辆 %1 行驶中... (2/2)").arg(carNum));
    });
    QTimer::singleShot(1100, this, [=]() {
        // 最终落位
        if (lot->carEnterAt(carNum.toStdString(), now, spotId)) {//尝试停车
            updateParkingDisplay();//刷新显示
            ui->editCarNum->clear();//情况输入框
            ui->editCarNum->setFocus();//聚焦在输入框
            showFeeDetails(QString("车辆 %1 已停入车位 %2").arg(carNum).arg(spotId));
        } else {
            QMessageBox::warning(this, "提示", "该车位已被占用或车牌重复，请重试。");
        }
        isDriving = false;
        // 结束一次入场流程，允许再次点击
        isEnteringProcessing = false;
        debounceTimer->start();//启动防抖计时器
    });
}

void MainWindow::simulateExitDrive(const QString &carNum, time_t now)//用于模拟车辆出场行驶过程
{
    if (isDriving) return;
    isDriving = true;
    showFeeDetails(QString("车辆 %1 正从车位驶向出口...").arg(carNum));//初始化出场日志
    QTimer::singleShot(400, this, [=]() {
        showFeeDetails(QString("车辆 %1 行驶中... (1/2)").arg(carNum));
    });
    QTimer::singleShot(800, this, [=]() {
        showFeeDetails(QString("车辆 %1 行驶中... (2/2)").arg(carNum));
    });
    QTimer::singleShot(1100, this, [=]() {
        if (lot->carExit(carNum.toStdString(), now)) {//如果出场成功则执行
            auto& records = lot->getPaymentRecords();//获取缴费记录
            if (!records.empty()) {//记录不为空
                const auto& rec = records.back();//获取records列表中最后一个元素（即最新添加的记录），并用常量引用的方式保存到rec变量中。
                QString enterStr = QDateTime::fromSecsSinceEpoch(rec.enterTime).toString("yyyy-MM-dd HH:mm:ss");//时间格式转换
                QString exitStr  = QDateTime::fromSecsSinceEpoch(rec.exitTime).toString("yyyy-MM-dd HH:mm:ss");
                QString msg = QString("车牌：%1  车位：%2\n入场：%3\n出场：%4\n时长：%5 分钟  费用：¥%6")//格式化车辆出场的详细信息
                        .arg(QString::fromStdString(rec.carNumber))//用arg()方法按顺序替换占位符
                        .arg(rec.spotId)
                        .arg(enterStr)
                        .arg(exitStr)
                        .arg(rec.duration)
                        .arg(rec.fee, 0, 'f', 2);
                showFeeDetails(msg);//刷新停车场
            }
            updateParkingDisplay();
        } else {
            QMessageBox::warning(this, "提示", "该车未在停车场内！");
        }
        isDriving = false;
        isExitingProcessing = false;
    });
}

void MainWindow::on_btnEnter_clicked()//入场点击
{
    // 双重保险：防止重复执行
    if (isEnteringProcessing) return;
    if (!lot) return;//lot为空则停车场还没初始化
    
    QString carNum = ui->editCarNum->text().trimmed();//获取用户输入的车牌号并去除首尾空格
    if (carNum.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入车牌号！");
        return; // 输入为空时直接返回，不设置标志位
    }
    
    // 设置处理标志，禁用按钮，防止重复点击
    isEnteringProcessing = true;
    ui->btnEnter->setEnabled(false);
    
    time_t now = std::time(nullptr);

    // 允许用户选择空车位
    if (lot->getAvailableSpots() > 0) {//通过lot对象判断是否有空车位
        QStringList emptySpots;//创建字符串来收集空车位id
        for (const auto &s : lot->getSpots()) {//遍历所有车位
            if (!s.occupied) emptySpots << QString::number(s.id);
        }
        bool ok = false;
        QString chosen = QInputDialog::getItem(this, "选择车位", "请选择空车位：", emptySpots, 0, false, &ok);//空闲将车辆信息输入
        if (!ok || chosen.isEmpty()) {
            // 用户取消，恢复按钮与标志位
            isEnteringProcessing = false;
            debounceTimer->start();
            return;
        }
        int spotId = chosen.toInt();
        // 行驶模拟
        simulateEnterDrive(carNum, spotId, now);
    } else if (lot->carEnter(carNum.toStdString(), now)) {
        updateParkingDisplay();
        ui->editCarNum->clear(); // 入场成功后自动清空
        ui->editCarNum->setFocus(); // 重新聚焦到输入框
        showFeeDetails(QString("车辆 %1 已成功入场").arg(carNum));
    } else {//无车位
        // 判断失败原因
        if (lot->getAvailableSpots() > 0) {
            QMessageBox::warning(this, "提示", "该车牌已存在，不允许重复入场！");
        } else if (lot->getQueueSize() >= lot->getQueueCapacity()) {
            QMessageBox::information(this, "提示", "停车场已满且通道排队已满，无法进入。");
        } else {
            QMessageBox::information(this, "提示", "停车场已满，已排入便道等候队列。");
        }
    }
    
    // 启动防抖计时器，计时结束后再恢复按钮与处理标志
    debounceTimer->start(); // 500ms后启用按钮
}

void MainWindow::enableEnterButton()//恢复入场和出场按钮的可点击状态，并重置相关流程标志位
{
    // 防抖定时器触发，重新启用按钮，并释放处理标志
    isEnteringProcessing = false;
    isExitingProcessing = false;
    if (lot) {
        ui->btnEnter->setEnabled(true);
        ui->btnExit->setEnabled(true);
    }
}

void MainWindow::on_btnExit_clicked()//出场点击
{
    // 防重复
    if (isExitingProcessing) return;
    if (!lot) return;
    // 从当前在场车辆中选择
    QStringList inLotPlates;//创建字符串列表，用于存储所有当前停在停车场内的车辆车牌号
    for (const auto &s : lot->getSpots()) {//遍历所有车位，用occupied判断是否被占用，占用的车牌号转qstring后收集
        if (s.occupied) inLotPlates << QString::fromStdString(s.carNumber);
    }
    if (inLotPlates.isEmpty()) {
        QMessageBox::information(this, "提示", "当前停车场内没有车辆。");
        return;
    }
    bool ok = false;
    QString carNum = QInputDialog::getItem(this, "出场", "请选择出场车辆车牌号：", inLotPlates, 0, false, &ok);//弹出车辆选择对话框
    if (!ok || carNum.isEmpty()) return;

    // 设置处理标志并禁用按钮，启动防抖
    isExitingProcessing = true;
    ui->btnExit->setEnabled(false);
    time_t now = std::time(nullptr);
    simulateExitDrive(carNum, now);
    // 启动防抖计时器，结束后统一释放标志并启用按钮
    debounceTimer->start();
}

void MainWindow::updateParkingDisplay()//实时更新停车场界面显示状态
{
    refreshParkingSpots();//刷新车列表
    refreshQueueList();//刷新排队列表
    ui->labelSpots->setText(QString("空余车位：%1")
        .arg(lot ? lot->getAvailableSpots() : 0));
    ui->labelQueue->setText(QString("排队车辆数：%1")
        .arg(lot ? lot->getQueueSize() : 0));
}

void MainWindow::refreshParkingSpots()//刷新车位状态显示
{
    if (!lot) return;
    auto& spots = lot->getSpots();//获取车位数据
    ui->listParking->clear();//清空重建列表
    for (const auto& s : spots) {//遍历每个车位并给出日志
        QString spotInfo = QString("车位%1：").arg(s.id);
        spotInfo += s.occupied ? ("占用 " + QString::fromStdString(s.carNumber)) : "空闲";
        ui->listParking->addItem(spotInfo);//添加列表控件
    }
}

void MainWindow::refreshQueueList()//刷新等候列表显示
{
    if (!lot) return;
    ui->listQueue->clear();
    auto& q = lot->getWaitQueue();
    auto waitVec = q.getAll();
    for (int i = 0; i < waitVec.size(); ++i) {
        ui->listQueue->addItem(QString("%1").arg(QString::fromStdString(waitVec[i])));
    }
}

void MainWindow::autoCarExit()
{
    // 自动车辆出场逻辑（还未写）
}

void MainWindow::showFeeDetails(const QString &msg)//在界面上显示停车场相关操作日志或费用详情
{
    ui->textFee->append(msg);
}
