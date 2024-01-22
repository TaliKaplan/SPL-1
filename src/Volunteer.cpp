#include "../include/Volunteer.h"

Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER),activeOrderId(NO_ORDER),id(id),name(name) {}

int Volunteer::getId() const {
    return id;
}

const string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveOrderId() const {
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    return getActiveOrderId() != NO_ORDER;
}

// CollectorVolunteer
// rule of 3 unnecessary - we don't own resources.

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown): Volunteer(id,name),coolDown(coolDown),timeLeft(coolDown) {}

CollectorVolunteer *CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft = timeLeft - 1;
    }
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return order.getStatus() == OrderStatus::PENDING && order.getCollectorId() == NO_VOLUNTEER && !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    completedOrderId = NO_ORDER;
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const {
    string busyStr = "False";
    string orderIDStr = "None";
    string timeLeftStr = "None";
    if (isBusy()) {
        busyStr = "True";
        orderIDStr = std::to_string(getActiveOrderId());
        timeLeftStr = std::to_string(getTimeLeft());
    }
    
    string res = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + busyStr + "\n"
        "OrderID: " + orderIDStr + "\n"
        "TimeLeft: " + timeLeftStr + "\n"
        "OrdersLeft: No Limit";

    return res;
}

void CollectorVolunteer::step() {
    if (!isBusy()) {
        return;
    }

    bool finished = decreaseCoolDown();
    if (finished) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

// LimitedCollectorVolunteer
// rule of 3 unnecessary - we don't own resources.

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders): CollectorVolunteer(id,name,coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return hasOrdersLeft() && CollectorVolunteer::canTakeOrder(order);
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft - 1;
}

string LimitedCollectorVolunteer::toString() const {
    string busyStr = "False";
    string orderIDStr = "None";
    string timeLeftStr = "None";
    if (isBusy()) {
        busyStr = "True";
        orderIDStr = std::to_string(getActiveOrderId());
        timeLeftStr = std::to_string(getTimeLeft());
    }
    
    string res = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + busyStr + "\n"
        "OrderID: " + orderIDStr + "\n"
        "TimeLeft: " + timeLeftStr + "\n"
        "OrdersLeft: " + std::to_string(getNumOrdersLeft());

    return res;
}

// DriverVolunteer
// rule of 3 unnecessary - we don't own resources.

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer(id,name),maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(distancePerStep) {}

DriverVolunteer *DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft <= distancePerStep) {
        distanceLeft = 0;
        return true;
    }

    distanceLeft = distanceLeft - distancePerStep;
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return order.getStatus() == OrderStatus::COLLECTING && order.getDriverId() == NO_VOLUNTEER && order.getDistance() <= getMaxDistance() && !isBusy();
}

void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    completedOrderId = NO_ORDER;
    distanceLeft = order.getDistance();
}

string DriverVolunteer::toString() const {
    string busyStr = "False";
    string orderIDStr = "None";
    string distanceLeftStr = "None";
    if (isBusy()) {
        busyStr = "True";
        orderIDStr = std::to_string(getActiveOrderId());
        distanceLeftStr = std::to_string(getDistanceLeft());
    }
    
    string res = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + busyStr + "\n"
        "OrderID: " + orderIDStr + "\n"
        "TimeLeft: " + distanceLeftStr + "\n"
        "OrdersLeft: No Limit";

    return res;
}

void DriverVolunteer::step() {
    if (!isBusy()) {
        return;
    }

    bool finished = decreaseDistanceLeft();
    if (finished) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

// LimitedDriverVolunteer
// rule of 3 unnecessary - we don't own resources.

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders): DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return hasOrdersLeft() && DriverVolunteer::canTakeOrder(order);
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft = ordersLeft - 1;
}

string LimitedDriverVolunteer::toString() const {
    string busyStr = "False";
    string orderIDStr = "None";
    string distanceLeftStr = "None";
    if (isBusy()) {
        busyStr = "True";
        orderIDStr = std::to_string(getActiveOrderId());
        distanceLeftStr = std::to_string(getDistanceLeft());
    }
    
    string res = 
        "VolunteerID: " + std::to_string(getId()) + "\n"
        "isBusy: " + busyStr + "\n"
        "OrderID: " + orderIDStr + "\n"
        "TimeLeft: " + distanceLeftStr + "\n"
        "OrdersLeft: " + std::to_string(getNumOrdersLeft());

    return res;
}
