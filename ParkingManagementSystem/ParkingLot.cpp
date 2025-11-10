// 停车场状态与规则的实现。
// - 车位资源管理（占用/释放，指定车位入场）。
// - 排队与补位（容量上限为 min(n, m)）。
// - 入/出场流程与计费（按 15 分钟为一个计费单位）。。
#include "ParkingLot.h"
#include <algorithm>

// 判车辆和所有车位同步唯一性
bool ParkingLot::isCarInLotOrSpot(const std::string& carNum) const {//检查号码牌是否在停车场
    if (carsInLot.count(carNum)) return true;
    for(const auto& spot : spots) {//遍历停车场的所有车位
        if (spot.occupied && spot.carNumber == carNum)
            return true;
    }
    return false;
}

ParkingLot::ParkingLot(int n, int m)
    : maxSpots(n), maxQueue(std::min(n, m)), spots(), waitQueue(std::min(n, m)) {
    for (int i = 0; i < n; ++i) spots.emplace_back(i+1);//给spot容器添加n个车位对象
}

// 尝试入场：当有空位时占用首个空位，否则进入排队。
// 返回：true=成功（直接入场或成功排队），false=失败（重复或队列已满）。
bool ParkingLot::carEnter(const std::string& carNum, time_t now) {//尝试车辆进入停车场
    if (isCarInLotOrSpot(carNum)) return false;
    auto waiting = waitQueue.getAll();//获取等待队列中所有车辆的车牌信息
    if (std::find(waiting.begin(), waiting.end(), carNum) != waiting.end())//检查当前要入场的车辆（carNum）是否已在等待队列（waitQueue）中
        return false;
    for (auto& spot : spots) {//遍历车位
        if (!spot.occupied) {//检查空位
            spot.occupied = true;//占用车位
            spot.carNumber = carNum;
            carsInLot[carNum] = Car(carNum, now, spot.id);
            return true;
        }
    }
    if (!waitQueue.isFull()) {//无空位加入等待
        if (std::find(waiting.begin(), waiting.end(), carNum) == waiting.end())
            return waitQueue.enqueue(carNum);
    }
    return false;
}

// 指定车位入场：
// - 仅在该车位空闲且车不在场/队列时成功；
// - 若指定车位无效或已被占用，则失败；
// - 若当前无空位，则回退为排队逻辑
bool ParkingLot::carEnterAt(const std::string& carNum, time_t now, int spotId) {
    // 判重：场内或排队中存在则拒绝
    if (isCarInLotOrSpot(carNum)) return false;
    auto waiting = waitQueue.getAll();//获得所有车辆信息
    if (std::find(waiting.begin(), waiting.end(), carNum) != waiting.end())
        return false;

    // 如果有空位，检查指定车位
    for (auto& spot : spots) {//遍历车位查找目标
        if (spot.id == spotId) {
            if (!spot.occupied) {//空闲
                spot.occupied = true;
                spot.carNumber = carNum;
                carsInLot[carNum] = Car(carNum, now, spot.id);
                return true;
            } else {
                return false; // 指定车位非空
            }
        }
    }

    // 未找到该车位或无空位，则按队列逻辑处理
    if (!waitQueue.isFull()) {//检查车位是否满
        if (std::find(waiting.begin(), waiting.end(), carNum) == waiting.end())//检查车辆是否在队列中
            return waitQueue.enqueue(carNum);
    }
    return false;
}

// 车辆出场：
// - 计算停车时长与费用并记录。
// - 释放对应车位；
bool ParkingLot::carExit(const std::string& carNum, time_t now) {//处理车辆离场流程
    auto it = carsInLot.find(carNum);//容器中查找指定号码
    if (it == carsInLot.end()) return false;
    Car& car = it->second;//把详情取出来起名为car
    car.exitTime = now;
    // 把真实秒数按配置的“秒/模拟分钟”换算为停车分钟数。
    int factor = secondsPerSimulatedMinute > 0 ? secondsPerSimulatedMinute : 60;
    int duration = static_cast<int>(difftime(car.exitTime, car.enterTime)) / factor;//计算车辆的实际停车时长
    double fee = calcFee(duration);
    paymentRecords.emplace_back(car.number, duration, fee, car.enterTime, car.exitTime, car.spotId);//将车辆的停车缴费记录添加到缴费记录列表中
    // 释放停车位
    for (auto& spot : spots) {//遍历所有车位定位精确的定位
        if (spot.id == car.spotId && spot.occupied && spot.carNumber == carNum) {
            spot.occupied = false;
            spot.carNumber = "";
            break;
        }
    }
    carsInLot.erase(it);//删除
    // 队列补位
    std::string nextCarNum;
    if (!waitQueue.isEmpty() && waitQueue.dequeue(nextCarNum)) {//当等待队列中有车辆，且成功从队列中取出一辆车的车牌时
        if (!isCarInLotOrSpot(nextCarNum)) {//检查从等待队列中取出的下一辆准备入场的车辆（nextCarNum）是否不在停车场内且未占用任何车位。
            for (auto& spot : spots)//等待队列中取出的车辆（nextCarNum）分配一个空闲车位并完成入场操作
                if (!spot.occupied) {
                    spot.occupied = true;
                    spot.carNumber = nextCarNum;
                    carsInLot[nextCarNum] = Car(nextCarNum, now, spot.id);
                    break;
                }
        }
    }
    return true;
}

int ParkingLot::getAvailableSpots() const {//计算并返回停车场当前的空闲车位数
    int cnt = 0;
    for (const auto& spot : spots) if (!spot.occupied) ++cnt;
    return cnt;
}
int ParkingLot::getQueueSize() const { return waitQueue.getSize(); }//获取当前等待队列中的车辆数量
const std::vector<ParkingSpot>& ParkingLot::getSpots() const { return spots; }//存储车位信息
const Queue& ParkingLot::getWaitQueue() const { return waitQueue; }//获取排列信息
const std::vector<PaymentRecord>& ParkingLot::getPaymentRecords() const { return paymentRecords; }//提供缴费记录
// 计费规则：
// - 15分钟为一个计费单位，不足一单位不收费；
double ParkingLot::calcFee(int durationMin) {//计算停车费用
    int units = durationMin / 15;
    return units <= 0 ? 0.0 : units * 1.5;
}

// 静态成员定义与访问器
int ParkingLot::secondsPerSimulatedMinute = 60;
void ParkingLot::setSecondsPerSimulatedMinute(int seconds) {//设置时间倍率的方法
    secondsPerSimulatedMinute = seconds > 0 ? seconds : 60;
}
int ParkingLot::getSecondsPerSimulatedMinute() { return secondsPerSimulatedMinute; }//获取当前时间倍率的方法
