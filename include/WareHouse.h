#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Action.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        // Rule of 5 : 
        WareHouse(const WareHouse& other); // Copy Constructor
        WareHouse(WareHouse&& other) noexcept; // Move Constructor
        WareHouse& operator=(const WareHouse& other); // Copy Assignment
        WareHouse& operator=(WareHouse&& other) noexcept; // Move Assignment
        ~WareHouse(); // Destructor
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        const vector<Volunteer*> &getVolunteers() const;
        void printAllOrders() const;
        void deleteLimitedVolunteer(int volunteerId);
        int getCustomerCounter() const;
        int getOrdersCounter() const;
        void close();
        void open();
        void stepInPendingOrders();
        void stepInInProcessOrders();
        void stepInVolunteers();

    private:
        bool isOpen;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        Volunteer* no_volunteer;
        Customer* no_customer;
        Order* no_order;
        void parseInitialFile(const string& configFilePath);
        void parseLine(string line);
        void lineDecipher(string line);
        Volunteer &getAvailableVolunteer(Order* order) const;
};