#ifndef CAR_H
#define CAR_H
#include <string>
#include <ctime>
class Car {
public:
    std::string number;   // 车牌号
    time_t enterTime;     // 入场时间
    time_t exitTime;      // 出场时间
    int spotId;           // 停车位编号(-1表示未停车)
    Car() : number(""), enterTime(0), exitTime(0), spotId(-1) {} // 默认构造
    Car(const std::string& num, time_t enterTm, int spot = -1)
        : number(num), enterTime(enterTm), exitTime(0), spotId(spot) {}
};
#endif // CAR_H
