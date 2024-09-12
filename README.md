
# Warehouse Management System

This is a C++ simulation of a warehouse management system where customers place orders, and volunteers (collectors and drivers) process them. The program simulates the warehouse operations step by step, allowing the user to place orders, check the status of orders and volunteers, and log all actions.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Running Example](#running-example)
- [License](#license)

## Features
- Place customer orders and process them in steps.
- Assign collectors and drivers to orders.
- Monitor the status of customers, orders, and volunteers.
- Log and review all actions taken during the simulation.
- Ability to close the warehouse and print the final status of all orders.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/chen7897/Warehouse-Managment-CPP.git
   cd Warehouse-Managment-CPP
   ```
2. Compile the project using the provided `Makefile`:
   ```bash
   make
   ```

## Usage
To run the program, you need to provide a configuration file as an argument. The configuration file defines the initial state of the warehouse, such as customers, volunteers, and other parameters.

Example:
```bash
./bin/warehouse.out ./bin/configFileExample.txt
```

The program will load the configuration and provide an interactive interface where you can input commands.

## Commands
- `order <customerID>`: Place an order for the customer with the given `customerID`.
- `orderStatus <orderID>`: Print the status of the order with the given `orderID`.
- `volunteerStatus <volunteerID>`: Print the status of the volunteer with the given `volunteerID`.
- `step <n>`: Perform `n` steps in the simulation to advance the warehouse processes.
- `log`: Print all actions taken so far.
- `close`: Close the warehouse and print the final status of all orders.

## Running Example

Below is a running example, according to the configuration file `configFileExample.txt`.

```bash
• Warehouse is open!
• order 17
Error: Cannot place this order

• order 0
• orderStatus 0
OrderId: 0
OrderStatus: Pending
CustomerID: 0
Collector: None
Driver: None

• step 1
• orderStatus 0
OrderId: 0
OrderStatus: Collecting
CustomerID: 0
Collector: 0
Driver: None

• volunteerStatus 0
VolunteerID: 0
isBusy: True
OrderId: 0
TimeLeft: 1
OrdersLeft: No Limit
```

For the full running example, see the `RunningExample.pdf` included in the repository.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.
