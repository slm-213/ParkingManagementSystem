#ifndef PAYMENTRECORD_H
#define PAYMENTRECORD_H
#include <string>
class PaymentRecord {
public:
    std::string carNumber;//车牌号
    int duration;//停车时常
    double fee;
    time_t enterTime;    // 入场时间
    time_t exitTime;     // 出场时间
    int spotId;          // 所停车位号
    PaymentRecord(const std::string& num, int dur, double charge,
                  time_t enterTm, time_t exitTm, int spot)
        : carNumber(num), duration(dur), fee(charge),
          enterTime(enterTm), exitTime(exitTm), spotId(spot) {}
};
#endif // PAYMENTRECORD_H
