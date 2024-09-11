#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED,
};

#define NO_VOLUNTEER -1

class Order {

    public:
        Order(int id, int customerId, int distance);
        int getId() const;
        int getCustomerId() const;
        void setStatus(OrderStatus status);
        void setCollectorId(int collectorId);
        void setDriverId(int driverId);
        int getCollectorId() const;
        int getDriverId() const;
        string getStatusString() const;
        OrderStatus getStatus() const;
        int getDistance() const;
        const string toString() const;
        Order *clone() const;

    private:
        const int id;
        const int customerId;
        const int distance;
        OrderStatus status;
        int collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        int driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
};