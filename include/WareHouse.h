#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"


class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        // rule of 5
        virtual ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse& operator=(const WareHouse &other);
        WareHouse(WareHouse &&other);
        WareHouse& operator=(WareHouse &&other);

        // Custom methods
        int newCustomerId(); // Returns an available ID for a new customer, increments customerCounter by 1.
        int newVolunteerId(); // Returns an available ID for a volunteer, increments volunteerCounter by 1.
        int newOrderId(); // Returns an available ID for a new order, increments orderCounter by 1.
        void addCustomer(string name, string type, int dist, int maxOrders);// Adds a customer.
        void addVolunteer(Volunteer* volunteer);// Adds a volunteer.
        bool customerExists(int customerId) const; // We need this to call get only if the customer exists.
        bool volunteerExists(int volunteerId) const; // We need this to call get only if the volunteer exists.
        bool orderExists(int orderId) const; // We need this to call get only if the order exists.
        void freeResources(); // Clears all allocated resources.
        vector<Order*>& getPendingOrders(); //Returns a reference of the Vector with all pending orders.
        vector<Order*>& getInProcessOrders(); //Returns a reference of the Vector with all inProcess orders.
        vector<Order*>& getCompletedOrders(); //Returns a reference of the Vector with all completed orders.
        vector<Volunteer*>& getVolunteers(); //Returns a reference of the Vector with all volunteers.
        void printOrders() const; // Prints all the orders.
        void printActions() const;// Prints all previous made actions

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
};

#include "Action.h"
