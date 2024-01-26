#include "../include/Order.h"

Order::Order(int _id, int _customerId, int _distance) :id(_id), customerId(_customerId), distance(_distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const{
    return customerId;
}

void Order::setStatus(OrderStatus status){
    this->status = status;
}

void Order::setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    this->driverId=driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

int Order::getDistance() const{
    return distance;
}

OrderStatus Order::getStatus() const{
    return status;
}

string Order::StatusToString() const{
    string res = "";
    if(status == OrderStatus::PENDING)
        res = "PENDING";
    else if(status == OrderStatus::COLLECTING)
        res = "COLLECTING";
    else if(status == OrderStatus::DELIVERING)
        res = "DELIVERING";
    else res = "COMPLETED";
    return res;
}

const string Order::toString() const{
    string res = "";
    res += "OrderId: " + std::to_string(id) + "\n";
    res += "OrderStatus: " + StatusToString() + "\n";
    res += "CustomerID: " + std::to_string(customerId) + "\n";
    res += "Collector: ";
    
    if(collectorId == NO_VOLUNTEER)
        res += "None\n";
    else res += std::to_string(collectorId) +"\n";
    res += "Driver: ";
    if(driverId == NO_VOLUNTEER)
        res += "None";
    else res += std::to_string(driverId);

    return res;

}

Order * Order::clone() const{
    return new Order(*this);
}

