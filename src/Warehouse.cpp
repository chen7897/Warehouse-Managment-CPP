#include "WareHouse.h"

// Implementation for WareHouse class
WareHouse::WareHouse(const string& configFilePath) : isOpen(false), customerCounter(0), volunteerCounter(0), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), 
no_volunteer(new CollectorVolunteer(-1, "no_volunteer", 0)), no_customer(new SoldierCustomer(-1, "no_customer", 0, 0)), no_order(new Order(-1, 0, 0))
 {
    parseInitialFile(configFilePath);
}

WareHouse:: WareHouse(const WareHouse& other) : isOpen(other.isOpen), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), 
no_volunteer(other.no_volunteer->clone()), no_customer(other.no_customer->clone()), no_order(other.no_order->clone())
{
    for (size_t i = 0; i < other.actionsLog.size() ; i++)
    {
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
    for (size_t i = 0; i < other.volunteers.size() ; i++)
    {
        volunteers.push_back(other.volunteers.at(i)->clone());
    }
    for (size_t i = 0; i < other.pendingOrders.size() ; i++)
    {
        pendingOrders.push_back(other.pendingOrders.at(i)->clone());
    }
    for (size_t i = 0; i < other.inProcessOrders.size() ; i++)
    {
        inProcessOrders.push_back(other.inProcessOrders.at(i)->clone());
    }
    for (size_t i = 0; i < other.completedOrders.size() ; i++)
    {
        completedOrders.push_back(other.completedOrders.at(i)->clone());
    }
    for (size_t i = 0; i < other.customers.size() ; i++)
    {
        customers.push_back(other.customers.at(i)->clone());
    }
}   

WareHouse::WareHouse(WareHouse&& other) noexcept
    : isOpen(other.isOpen),customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),pendingOrders(std::move(other.pendingOrders)),inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),customers(std::move(other.customers)),no_volunteer(other.no_volunteer),
      no_customer(other.no_customer),no_order(other.no_order)
{
    other.isOpen = false;
    other.customerCounter = 0;
    other.volunteerCounter = 0;
    other.no_volunteer = nullptr;
    other.no_customer = nullptr;
    other.no_order = nullptr;
    
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}


void WareHouse::start() {
    open();
    std::cout << "Warehouse is open!" << std::endl;
    string usersInput;
    while (isOpen)
    {
        std::getline(std::cin, usersInput);
        lineDecipher(usersInput);
    }
}

void WareHouse::addOrder(Order* order) {
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

void WareHouse::addCustomer(Customer* customer) {
    customers.push_back(customer);
    customerCounter++;
}

Customer& WareHouse::getCustomer(int customerId) const {
    for (const auto& customer : customers) {
        if (customer->getId() == customerId) {
            return *customer;
        }
    }
    return *no_customer;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (const auto& volunteer : volunteers) {
        if (volunteer->getId() == volunteerId) {
            return *volunteer;
        }
    }
    return *no_volunteer;
}

Order& WareHouse::getOrder(int orderId) const {
    for (const auto& order : pendingOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : inProcessOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    for (const auto& order : completedOrders) {
        if (order->getId() == orderId) {
            return *order;
        }
    }
    return *no_order;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

const vector<Volunteer*>& WareHouse::getVolunteers() const {
    return volunteers;
}


void WareHouse::printAllOrders() const {
    for (const auto& order : pendingOrders) {
        std::cout << "OrderID: " +  std::to_string(order->getId()) + ", CustomerID: " << std::to_string(order->getCustomerId()) + ", OrderStatus: " << order->getStatusString() << std::endl;
    }
    for (const auto& order : inProcessOrders) {
        std::cout << "OrderID: " +  std::to_string(order->getId()) + ", CustomerID: " << std::to_string(order->getCustomerId()) + ", OrderStatus: " << order->getStatusString() << std::endl;
    }
    for (const auto& order : completedOrders) {
        std::cout << "OrderID: " +  std::to_string(order->getId()) + ", CustomerID: " << std::to_string(order->getCustomerId()) + ", OrderStatus: " << order->getStatusString() << std::endl;
    }
}

void WareHouse::deleteLimitedVolunteer(int volunteerId) {
    for (size_t i = 0; i < volunteers.size(); i++) {
        if (volunteers.at(i)->getId() == volunteerId) {
            delete volunteers.at(i);
            volunteers.erase(volunteers.begin() + i);
            return;
        }
    }
}

int WareHouse::getCustomerCounter() const {
    return customerCounter;
}

int WareHouse::getOrdersCounter() const {
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

void WareHouse::close() {
    isOpen = false;
}


void WareHouse::open() {
    isOpen = true;
}

void WareHouse::parseInitialFile(const string& configFilePath) 
{
    std::ifstream inputFile(configFilePath);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << configFilePath << std::endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        parseLine(line);
    }
    inputFile.close();
}

void WareHouse::parseLine(string line)
{
    if (line.empty() || line[0] == '#') {
        return;
    }
    string action;
    std::istringstream iss(line);
    iss >> action;
    if (action == "customer") {
        string name, type, distance, maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        if (type == "solider")
            customers.push_back(new SoldierCustomer(customerCounter, name, stoi(distance), stoi(maxOrders)));
        else
            customers.push_back(new CivilianCustomer(customerCounter, name, stoi(distance), stoi(maxOrders)));
        customerCounter++;
    } else if (action == "volunteer") {
        string name, role, coolOrMax, distance;
        string maxOrders = "-1";
        iss >> name >> role >> coolOrMax;
        if (role == "driver" || role == "limited_driver")
        {
            iss >> distance;
        }
        if (role == "limited_collector" || role == "limited_driver")
        {
            iss >> maxOrders;
        }  
        if (role == "collector")
        {
             volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, stoi(coolOrMax))) ;
        }
         else if (role == "limited_collector")
        {
            volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, stoi(coolOrMax), stoi(maxOrders)));
        }
        else if (role == "driver")
        {
             volunteers.push_back(new DriverVolunteer(volunteerCounter, name, stoi(coolOrMax), stoi(distance)));
        }
        else if ( role == "limited_driver")
        {
             volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, stoi(coolOrMax), stoi(distance), stoi(maxOrders)));
        }
        volunteerCounter++;
    }
}

void WareHouse:: lineDecipher(string line)
{
    BaseAction* baseAction = nullptr;
    string action;
    std::istringstream iss(line);
    iss >> action;
    if (action == "step") {
        string numOfSteps;
        iss >> numOfSteps;
        baseAction = new SimulateStep(stoi(numOfSteps));
    } else if (action == "order") {
        string id;
        iss >> id;
        baseAction = new AddOrder(stoi(id));
    } else if (action == "customer") {
        string name, type, distance, maxOrders;
        iss >> name >> type >> distance >> maxOrders;
        baseAction = new AddCustomer(name, type, stoi(distance), stoi(maxOrders));
    }
    else if (action == "orderStatus") {
        string orderId;
        iss >> orderId;
        baseAction = new PrintOrderStatus(stoi(orderId));
    } 
    else if (action == "customerStatus") {
        string customerId;
        iss >> customerId;  
        baseAction = new PrintCustomerStatus(stoi(customerId));
    } else if (action == "volunteerStatus") {
        string volunteerId;
        iss >> volunteerId;
        baseAction = new PrintVolunteerStatus(stoi(volunteerId));
    } else if (action == "log") {
        baseAction = new PrintActionsLog();
    } else if (action == "close") {
        baseAction = new Close();
    } else if (action == "backup") {
        baseAction = new BackupWareHouse();
    } else if (action == "restore") {
        baseAction = new RestoreWareHouse();
    }
    if (baseAction != nullptr)
    {
        baseAction->act(*this);
        addAction(baseAction);
    }
}

WareHouse& WareHouse:: operator=(const WareHouse& other)
{
    if (this != &other)
    {
        for (size_t i = 0; i < actionsLog.size() ; i++)
        {
            delete actionsLog.at(i);
            actionsLog.erase(actionsLog.begin() + i);
            i--;
        }
        for (size_t i = 0; i < volunteers.size() ; i++)
        {
            delete volunteers.at(i);
            volunteers.erase(volunteers.begin() + i);
            i--;
        }
        for (size_t i = 0; i < pendingOrders.size() ; i++)
        {
            delete pendingOrders.at(i);
            pendingOrders.erase(pendingOrders.begin() + i);
            i--;
        }
        for (size_t i = 0; i < inProcessOrders.size() ; i++)
        {
            delete inProcessOrders.at(i);
            inProcessOrders.erase(inProcessOrders.begin() + i);
            i--;
        }
        for (size_t i = 0; i < completedOrders.size() ; i++)
        {
            delete completedOrders.at(i);
            completedOrders.erase(completedOrders.begin() + i);
            i--;
        }
        for (size_t i = 0; i < customers.size() ; i++)
        {
            delete customers.at(i);
            customers.erase(customers.begin() + i);
            i--;
        }
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        for (size_t i = 0; i < other.actionsLog.size() ; i++)
        {
            actionsLog.push_back(other.actionsLog.at(i)->clone());
        }
        for (size_t i = 0; i < other.volunteers.size() ; i++)
        {
            volunteers.push_back(other.volunteers.at(i)->clone());
        }
        for (size_t i = 0; i < other.pendingOrders.size() ; i++)
        {
            pendingOrders.push_back(other.pendingOrders.at(i)->clone());
        }
        for (size_t i = 0; i < other.inProcessOrders.size() ; i++)
        {
            inProcessOrders.push_back(other.inProcessOrders.at(i)->clone());
        }
        for (size_t i = 0; i < other.completedOrders.size() ; i++)
        {
            completedOrders.push_back(other.completedOrders.at(i)->clone());
        }
        for (size_t i = 0; i < other.customers.size() ; i++)
        {
            customers.push_back(other.customers.at(i)->clone());
        }
    }
    return *this;
}

WareHouse::~WareHouse()
{
    for (size_t i = 0; i < actionsLog.size() ; i++)
    {
        delete actionsLog.at(i);
    }
    for (size_t i = 0; i < volunteers.size() ; i++)
    {
        delete volunteers.at(i);
    }
    for (size_t i = 0; i < pendingOrders.size() ; i++)
    {
        delete pendingOrders.at(i);
    }
    for (size_t i = 0; i < inProcessOrders.size() ; i++)
    {
        delete inProcessOrders.at(i);
    }
    for (size_t i = 0; i < completedOrders.size() ; i++)
    {
        delete completedOrders.at(i);
    }
    for (size_t i = 0; i < customers.size() ; i++)
    {
        delete customers.at(i);
    }
    delete no_volunteer;
    delete no_customer;
    delete no_order;
}

// Move Assignment
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) {
        for (auto& action : actionsLog) {
            delete action;
        }
        for (auto& volunteer : volunteers) {
            delete volunteer;
        }
        for (auto& order : pendingOrders) {
            delete order;
        }
        for (auto& order : inProcessOrders) {
            delete order;
        }
        for (auto& order : completedOrders) {
            delete order;
        }
        for (auto& customer : customers) {
            delete customer;
        }

        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();

        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);

        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();

        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
    }

    return *this;
}


Volunteer& WareHouse::getAvailableVolunteer(Order* order ) const {
    string volunteerType;
    if (order->getStatusString() == "Pending") {
        volunteerType = "CollectorVolunteer";
    } else if (order->getStatusString() == "Collecting") {
        volunteerType = "DriverVolunteer";
    }
    for (const auto& volunteer : volunteers) {
        if (volunteer->canTakeOrder(*order) && ((volunteer->getType() == volunteerType) || (volunteer->getType() == "Limited" + volunteerType))) {
            return *volunteer;
        }
    }
    return *no_volunteer;
}

void WareHouse:: stepInPendingOrders()
{
    for (size_t i = 0; i < pendingOrders.size(); i++)
    {
        Order* order = pendingOrders.at(i);
        Volunteer& volunteer = getAvailableVolunteer(order);
        if (volunteer.getId() != -1)
        {
            if (volunteer.getType() == "CollectorVolunteer" || volunteer.getType() == "LimitedCollectorVolunteer")
            {
                order->setCollectorId(volunteer.getId());
            }
            else
            {
                order->setDriverId(volunteer.getId());
            }
            order->setStatus(static_cast<OrderStatus>(static_cast<int>(order->getStatus()) + 1));
            volunteer.acceptOrder(*order);
            inProcessOrders.push_back(order);
            pendingOrders.erase(pendingOrders.begin() + i);
            i--;
        }
    }
}

void WareHouse:: stepInInProcessOrders()
{
    for (size_t i = 0; i < inProcessOrders.size(); i++)
    {
        Order* order = inProcessOrders.at(i);
        int id;
        if (order->getStatus() == OrderStatus::COLLECTING)
            id = order->getCollectorId();
        else    
            id = order->getDriverId();
        Volunteer& volunteer = getVolunteer(id);
        if (volunteer.getId() != -1)
        {
            volunteer.step();
            if (volunteer.getActiveOrderId() == NO_ORDER)
            {
                if (volunteer.getType() == "CollectorVolunteer" || volunteer.getType() == "LimitedCollectorVolunteer")
                {
                    pendingOrders.push_back(order);
                }
                else
                {
                    order->setStatus(static_cast<OrderStatus>(static_cast<int>(order->getStatus()) + 1));
                    completedOrders.push_back(order);
                }
                inProcessOrders.erase(inProcessOrders.begin() + i);
                i--;
            }
        }
    }
}


void WareHouse::stepInVolunteers()
{
    for (size_t i = 0; i < volunteers.size(); i++)
    {
        Volunteer* volunteer = volunteers.at(i);
        if (volunteer->getActiveOrderId() == NO_ORDER)
        {
            if (!(volunteer->hasOrdersLeft()))
            {
                deleteLimitedVolunteer(volunteer->getId());
                i--;
            }
        }
    }
}
