#include "Order.h"

Order::Order(int id, int customerId, int distance)
    : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

void Order::setStatus(OrderStatus newStatus) {
    status = newStatus;
}

void Order::setCollectorId(int newCollectorId) {
    collectorId = newCollectorId;
}

void Order::setDriverId(int newDriverId) {
    driverId = newDriverId;
}

int Order::getCollectorId() const {
    return collectorId;
}

int Order::getDriverId() const {
    return driverId;
}

OrderStatus Order::getStatus() const {
    return status;
}

int Order::getDistance() const {
    return distance;
}

string Order::getStatusString() const {
    if (status == OrderStatus::PENDING) {
        return "Pending";
    } else if (status == OrderStatus::COLLECTING) {
        return "Collecting";
    } else if (status == OrderStatus::DELIVERING) {
        return "Delivering";
    } else {
        return "Completed";
    } 
}

const string Order::toString() const {
    return "OrderID: " + std::to_string(id) +
           ", CustomerID: " + std::to_string(customerId) +
           ", OrderStatus: " + getStatusString();
}

Order *Order::clone() const {
    return new Order(*this);
}
