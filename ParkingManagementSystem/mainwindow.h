#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ParkingLot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnInit_clicked();
    void on_btnRandPlate_clicked();
    void on_btnEnter_clicked();
    void on_btnExit_clicked();
    void updateParkingDisplay();
    void autoCarExit(); // 预留定时模拟车辆离场
    void enableEnterButton(); // 启用入场/出场按钮（防抖后）

private:
    Ui::MainWindow *ui;
    ParkingLot *lot;
    QTimer *timer; // 模拟时间推进
    QTimer *debounceTimer; // 防抖定时器
    bool hasInitialized = false; // 防止重复初始化
    bool isInitializingProcessing = false; // 防止初始化重复触发
    bool isEnteringProcessing = false; // 防止入场函数重复执行
    bool isExitingProcessing = false; // 防止出场函数重复执行
    bool isDriving = false; // 行驶模拟占用
    void refreshQueueList();
    void refreshParkingSpots();
    void showFeeDetails(const QString &msg);
    void simulateEnterDrive(const QString &carNum, int spotId, time_t now);
    void simulateExitDrive(const QString &carNum, time_t now);
};

#endif // MAINWINDOW_H
