#ifndef PARKINGLOT_H
#define PARKINGLOT_H
#include <vector>
#include <map>
#include <string>
#include "Car.h"
#include "ParkingSpot.h"
#include "PaymentRecord.h"
#include "Queue.h"
class ParkingLot {
    int maxSpots;            // 车位总数
    int maxQueue;            // 排队上限
    std::vector<ParkingSpot> spots;//存储parkingspot的动态数组
    Queue waitQueue;
    std::map<std::string, Car> carsInLot; // 当前在场车辆（车牌->对象）
    std::vector<PaymentRecord> paymentRecords;//存储缴费记录
    bool isCarInLotOrSpot(const std::string& carNum) const; // 判断车牌号是否重复
public:
    ParkingLot(int n, int m);
    bool carEnter(const std::string& carNum, time_t now);//进入停车场
    bool carEnterAt(const std::string& carNum, time_t now, int spotId); // 指定车位入场
    bool carExit(const std::string& carNum, time_t now);//离场
    int getAvailableSpots() const;//获取空闲车位
    int getQueueSize() const;//等候队列的车辆数量
    int getQueueCapacity() const { return maxQueue; }//获取停车场等待队列的最大容量
    const std::vector<ParkingSpot>& getSpots() const;//获取停车场所有车位的信息
    const Queue& getWaitQueue() const;//获取停车场等待队列的常量引用
    const std::vector<PaymentRecord>& getPaymentRecords() const;//获取停车场的所有缴费记录
    static double calcFee(int durationMin);//计算金额
    // 时间倍率：实际秒数对应的“模拟1分钟”的秒数（默认60）
    static void setSecondsPerSimulatedMinute(int seconds);//int seconds表示 “现实中的秒数，设置时间倍率
    static int getSecondsPerSimulatedMinute();//获取时间倍率
private:
    static int secondsPerSimulatedMinute;//存储时间模拟概率
};
#endif // PARKINGLOT_H
