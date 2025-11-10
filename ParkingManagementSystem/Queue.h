#ifndef QUEUE_H
#define QUEUE_H
#include <vector>
#include <string>
class Queue {
    std::vector<std::string> data;
    int capacity;
    int front, rear, size;//队头，下一个元素入队时的存储位置，实际储存的元素数量
public:
    Queue(int cap) : data(cap), capacity(cap), front(0), rear(0), size(0) {}
    bool enqueue(const std::string& carNum) {//入队方法
        if (size == capacity) return false;
        data[rear] = carNum;//将新元素存入队尾
        rear = (rear + 1) % capacity;//更新队尾索引
        ++size;
        return true;
    }
    bool dequeue(std::string& carNum) {//出队
        if (size == 0) return false;
        carNum = data[front];//队头指向元素给carnum
        front = (front + 1) % capacity;//更新队头
        --size;
        return true;
    }
    int getSize() const { return size; }//获取当前队列中实际存储的元素数量
    bool isFull() const { return size == capacity; }//判断是否满
    bool isEmpty() const { return size == 0; }//判断是否空
    std::string frontValue() const { return size > 0 ? data[front] : ""; }//获取队列头部元素
    // 用于队列遍历显示
    std::vector<std::string> getAll() const {
        std::vector<std::string> res;
        for (int i = 0, idx = front; i < size; ++i, idx = (idx + 1) % capacity) {//从队头开始遍历
            res.push_back(data[idx]);
        }
        return res;
    }
};
#endif // QUEUE_H
