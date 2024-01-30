#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"
using std::string;
using std::vector;

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class CustomerType{
    Soldier, Civilian
};


class Customer;

class BaseAction{
    public:
        BaseAction();
        ActionStatus getStatus() const;
        virtual void act(WareHouse& wareHouse)=0;
        virtual string toString() const=0;
        virtual BaseAction* clone() const=0;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;
        string ActionStatusToString(ActionStatus status) const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public BaseAction {

    public:
        SimulateStep(int numOfSteps);
        void act(WareHouse &wareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const int numOfSteps;
};

class AddOrder : public BaseAction {
    public:
        AddOrder(int id);
        void act(WareHouse &wareHouse) override;
        string toString() const override;
        AddOrder *clone() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const int customerId;
};


class AddCustomer : public BaseAction {
    public:
        AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders);
        void act(WareHouse &wareHouse) override;
        AddCustomer *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const string customerName;
        const CustomerType customerType;
        const int distance;
        const int maxOrders;
        CustomerType toCustomerType(const string customerType);
        string typeToString() const;
};



class PrintOrderStatus : public BaseAction {
    public:
        PrintOrderStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintOrderStatus *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const int orderId;
};

class PrintCustomerStatus: public BaseAction {
    public:
        PrintCustomerStatus(int _customerId);
        void act(WareHouse &wareHouse) override;
        PrintCustomerStatus *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const int customerId;
};


class PrintVolunteerStatus : public BaseAction {
    public:
        PrintVolunteerStatus(int id);
        void act(WareHouse &wareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        const int VolunteerId;
};


class PrintActionsLog : public BaseAction {
    public:
        PrintActionsLog();
        void act(WareHouse &wareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
};

class Close : public BaseAction {
    public:
        Close();
        void act(WareHouse &wareHouse) override;
        Close *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
        
};

class BackupWareHouse : public BaseAction {
    public:
        BackupWareHouse();
        void act(WareHouse &wareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.

    private:
};


class RestoreWareHouse : public BaseAction {
    public:
        RestoreWareHouse();
        void act(WareHouse &wareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;

        //No resources - rule of 5 is unneccesary.
        
    private:
};