#include "Volunteer.h"

Volunteer::Volunteer(int _id, const string &_name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(_id), name(_name) {
}

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
    return activeOrderId != NO_ORDER;
}

CollectorVolunteer::CollectorVolunteer(int _id, string _name, int _coolDown)
    : Volunteer(_id, _name), coolDown(_coolDown), timeLeft(0) {
}

CollectorVolunteer *CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (isBusy()) {
        if (decreaseCoolDown()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

string CollectorVolunteer::getType() const {
    return "CollectorVolunteer";
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft--;
    return !(timeLeft > 0);
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true; 
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
	(void)order;
    return !isBusy(); 
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}

string CollectorVolunteer::toString() const { 
    return "CollectorVolunteer " + this->getName() + " " + std::to_string(this->getId()); 
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int _id, string _name, int _coolDown, int _maxOrders)
    : CollectorVolunteer(_id, _name, _coolDown), maxOrders(_maxOrders), ordersLeft(_maxOrders) {
}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

string LimitedCollectorVolunteer::getType() const {
    return "LimitedCollectorVolunteer";
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    --ordersLeft;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    return "LimitedCollectorVolunteer " + this->getName() + " " + std::to_string(this->getId()); 
}

DriverVolunteer::DriverVolunteer(int _id, string _name, int _maxDistance, int _distancePerStep)
    : Volunteer(_id, _name), maxDistance(_maxDistance), distancePerStep(_distancePerStep), distanceLeft(0) {
}

DriverVolunteer *DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

string DriverVolunteer::getType() const {
    return "DriverVolunteer";
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft > 0) {
        distanceLeft -= distancePerStep;
        if (distanceLeft < 0) {
            distanceLeft = 0;
        }
        return distanceLeft <= 0;
    } else {
        return true;
    }
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true; 
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && (distanceLeft == 0) && (order.getDistance() <= getMaxDistance()); 
}

void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
    if (isBusy()) {
        if (decreaseDistanceLeft()) {
            // true = order is complete, flase = decreased by distancePerStep
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

string DriverVolunteer::toString() const {
    return "DriverVolunteer " + this->getName() + " " + std::to_string(this->getId());
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep, int _maxOrders)
    : DriverVolunteer(_id, _name, _maxDistance, _distancePerStep), maxOrders(_maxOrders), ordersLeft(_maxOrders) {
}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

string LimitedDriverVolunteer::getType() const {
    return "LimitedDriverVolunteer";
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return DriverVolunteer::canTakeOrder(order) && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    --ordersLeft;
}

string LimitedDriverVolunteer::toString() const {
    return "LimitedDriverVolunteer " + this->getName() + " " + std::to_string(this->getId());
}
