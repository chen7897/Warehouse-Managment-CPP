#include "Action.h"


// Implementation for BaseAction class
BaseAction::BaseAction(): errorMsg(""), status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}


// Implementation for SimulateStep class
SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
   for (int i = 0; i < numOfSteps; i++) {
        oneStep(wareHouse);
   }
   complete();
}

void SimulateStep::oneStep(WareHouse &wareHouse) 
{
    wareHouse.stepInPendingOrders();
    wareHouse.stepInInProcessOrders();
    wareHouse.stepInVolunteers();
}

string SimulateStep::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "SimulateStep " + std::to_string(numOfSteps)+ " COMPLETED";
    else
        return "SimulateStep " + std::to_string(numOfSteps)+ " ERROR";
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}


// Implementation for AddOrder class
AddOrder::AddOrder(int id): customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {
    if(customerId >= wareHouse.getCustomerCounter() || customerId < 0){
        error("Cannot place this order");
        return;
    }
    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.canMakeOrder()) {
        Order *order = new Order(wareHouse.getOrdersCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
        customer.addOrder(order->getId());
        wareHouse.addOrder(order);
        complete();
    } else {
        error("Cannot place this order");
    }
}

string AddOrder::toString() const {
    if (getStatus() == ActionStatus::COMPLETED) {
        return "order " + std::to_string(customerId) + " COMPLETED";
    } else {
        return "order " + std::to_string(customerId) + " ERROR";
    }
}

AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);
}


// Implementation for AddCustomer class
AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) : customerName(customerName), customerType((customerType == "Civilian") ? CustomerType::Civilian : CustomerType::Soldier), distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {
    if (customerType == CustomerType::Soldier) {
        Customer *customer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
        complete();
    } else if (customerType == CustomerType::Civilian) {
        Customer *customer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.addCustomer(customer);
        complete();
    }
}

string AddCustomer::toString() const {
    string customerType = (this->customerType == CustomerType::Civilian) ? "Civilian" : "Soldier";
    if (getStatus() == ActionStatus::COMPLETED) {
        return "customer " + customerName + " " + customerType + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " COMPLETED";
    } else {
        return "customer " + customerName + " " + customerType + " " + std::to_string(distance) + " " + std::to_string(maxOrders) + " ERROR";
    }
}

AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}


// Implementation for PrintOrderStatus class
PrintOrderStatus::PrintOrderStatus(int id): orderId(id) {}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order &order = wareHouse.getOrder(orderId);
    if (order.getId() == -1){
        error("Order doesn’t exist");
        return;
    }
    string orderStatus = order.getStatusString();
    std::cout << "OrderId: " << order.getId() << std::endl;
    std::cout << "OrderStatus: " << orderStatus << std::endl;
    std::cout << "CustomerId: " << order.getCustomerId() << std::endl;
    if (order.getCollectorId() != -1) {
        std::cout << "Collector: " << order.getCollectorId() << std::endl;
    }
    else {
        std::cout << "Collector: None" << std::endl;
    }
    if (order.getDriverId() != -1) {
        std::cout << "Driver: " << order.getDriverId() << std::endl;
    }
    else {
        std::cout << "Driver: None" << std::endl;
    }
    complete();
}

string PrintOrderStatus::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "orderStatus " + std::to_string(orderId) + " COMPLETED";
    else
        return "orderStatus " + std::to_string(orderId) + " ERROR";
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}


// Implementation for PrintCustomerStatus class
PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    Customer &customer = wareHouse.getCustomer(customerId);
    if (customer.getId() == -1){
        error("Customer doesn't exist");
        return;
    }
    std::cout << "CustomerID: " << customer.getId() << std::endl;
    for (const auto &order : customer.getOrdersIds()) {
        std::cout << "OrderID: " + std::to_string(order) << std::endl;
        std::cout << "OrderStatus: " + wareHouse.getOrder(order).getStatusString() << std::endl;
    }
    std::cout << "numOrdersLeft: " + std::to_string(customer.getMaxOrders() - customer.getNumOrders()) << std::endl;
    complete();
}

string PrintCustomerStatus::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "customerStatus " + std::to_string(customerId) + " COMPLETED";
    else
        return "customerStatus " + std::to_string(customerId) + " ERROR";
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}


// Implementation for PrintVolunteerStatus class
PrintVolunteerStatus::PrintVolunteerStatus(int id): volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);
    if (volunteer.getId() != -1) {
        string volType = volunteer.getType();
        std::cout << "VolunteerID: " + std::to_string(volunteerId) << std::endl;
        if(wareHouse.getVolunteer(volunteerId).isBusy())
            std::cout << "isBusy: True" << std::endl;
        else
            std::cout << "isBusy: False" << std::endl;    
        if(wareHouse.getVolunteer(volunteerId).isBusy()) {
            std::cout << "OrderID: " + std::to_string(wareHouse.getVolunteer(volunteerId).getActiveOrderId()) << std::endl;
            if(volType == "CollectorVolunteer" || volType == "LimitedCollectorVolunteer")
                std::cout << "TimeLeft: " << dynamic_cast<CollectorVolunteer&>(volunteer).getTimeLeft() << std::endl;
            if (volType == "DriverVolunteer" || volType == "LimitedDriverVolunteer")
                std::cout << "TimeLeft: " << dynamic_cast<DriverVolunteer&>(volunteer).getDistanceLeft() << std::endl;
        }    
        else {
            std::cout << "OrderID: None" << std::endl;
            std::cout << "Time Left: None" << std::endl;
            }
        if (volType == "CollectorVolunteer" || volType == "DriverVolunteer")
            std::cout << "OrdersLeft: No Limit" << std::endl;
        else if (volType == "LimitedCollectorVolunteer")
            std::cout << "OrdersLeft: " + std::to_string(dynamic_cast<LimitedCollectorVolunteer&>(volunteer).getNumOrdersLeft()) << std::endl;
        else if (volType == "LimitedDriverVolunteer")
            std::cout << "OrdersLeft: " + std::to_string(dynamic_cast<LimitedDriverVolunteer&>(volunteer).getNumOrdersLeft()) << std::endl;
        complete();        
    }
    else 
        error("Volunteer doesn't exist"); 
}

string PrintVolunteerStatus::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "volunteerStatus " + std::to_string(volunteerId) + " COMPLETED";
    else
        return "volunteerStatus " + std::to_string(volunteerId) + " ERROR";
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}


// Implementation for PrintActionsLog class
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (const auto &action : wareHouse.getActions()) {
        std::cout << action->toString() << std::endl;
    }
    complete();
}

string PrintActionsLog::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "log COMPLETED";
    else
        return "log ERROR";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}


// Implementation for Close class
Close::Close() {}

void Close::act(WareHouse &wareHouse) {
    wareHouse.printAllOrders();
    wareHouse.close();
}

string Close::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "close COMPLETED";
    else
        return "close ERROR";
}

Close *Close::clone() const {
    return new Close(*this);
}


// Implementation for BackupWareHouse class
BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        delete backup;
    } 
    backup = new WareHouse(wareHouse);
    complete();
}

string BackupWareHouse::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "backup COMPLETED";
    else
        return "backup ERROR";
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}


// Implementation for RestoreWareHouse class
RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("No backup available");
        return;
    }
    wareHouse = *backup;
    complete();
}

string RestoreWareHouse::toString() const {
    if (getStatus() == ActionStatus::COMPLETED)
        return "restore COMPLETED";
    else
        return "restore ERROR";
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}


