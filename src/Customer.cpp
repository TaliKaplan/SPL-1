#include "../include/Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): id(id), name(name), locationDistance(locationDistance), maxOrders(maxOrders), ordersId() {}

int Customer::getId() const {
    return id;
}

const string& Customer::getName() const {
    return name;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {
    return ordersId.size();
}

int Customer::getOrdersLeft() const{
    return getMaxOrders() - getNumOrders();
}

bool Customer::canMakeOrder() const {
    return getNumOrders() < getMaxOrders();
}

const vector<int>& Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {
    if (!canMakeOrder()) {
        return -1;
    }

    ordersId.push_back(orderId);
    return orderId;
}

// SoldierCustomer
// rule of 3 unnecessary - we don't own resources.

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id,name,locationDistance,maxOrders) {}

SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

// CivilianCustomer
// rule of 3 unnecessary - we don't own resources.

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): Customer(id,name,locationDistance,maxOrders) {}

CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
