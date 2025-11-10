#ifndef PARKINGSPOT_H
#define PARKINGSPOT_H
#include <string>
class ParkingSpot {
public:
    int id;                 // 车位编号
    bool occupied;          // 是否被占用
    std::string carNumber;  // 当前占用车牌号（无则""）
    ParkingSpot(int spotId) : id(spotId), occupied(false), carNumber("") {}
};
#endif // PARKINGSPOT_H
