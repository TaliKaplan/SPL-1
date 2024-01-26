#include "../include/Action.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

extern WareHouse* backup;
//-----------abstract base class BaseAction------------------

BaseAction::BaseAction() :errorMsg(""), status(ActionStatus::COMPLETED) {}


ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

string BaseAction::ActionStatusToString(ActionStatus status) const{
    string res = "";
    if(status == ActionStatus::COMPLETED)
        res = "COMPLETED";
    else res = "ERROR";
    return res;
}


//-----------class SimulateStep------------------

SimulateStep::SimulateStep(int _numOfSteps): numOfSteps(_numOfSteps){}

void SimulateStep::act(WareHouse &wareHouse) {
    vector<Volunteer*> &volunteers = wareHouse.getVolunteers();
    vector<Order*> &pending = wareHouse.getPendingOrders();
    vector<Order*> &inProcess = wareHouse.getInProcessOrders();
    vector<Order*> &completed = wareHouse.getCompletedOrders();
    
    //stage 1
    for(int i=1; i<=numOfSteps;i++){ //performing a certain number of steps

        for (int index = 0; index < pending.size(); index ++){// we are deleting while iterating so using an iterator messes things up
            Order* p_order = pending[index];
            if(p_order->getStatus() == OrderStatus::PENDING){ //order in status pending
                if(p_order->getCollectorId() == NO_VOLUNTEER){ //order isn't assigned to a collector yet - need to assign a collector
                    for(Volunteer* c: volunteers){
                        if(c->canTakeOrder(*p_order))
                        {//assign order to collector
                            c->acceptOrder(*p_order);
                            p_order->setCollectorId(c->getId());
                            p_order->setStatus(OrderStatus::COLLECTING);;
                            pending.erase(std::next(pending.begin(), index));
                            index --;//if an elemet got deleted then the whole least is now shorter by 1 
                            inProcess.push_back(p_order);
                            break;
                        }
                    }
                }
            }

            else if(p_order->getStatus() == OrderStatus::COLLECTING) { //order in status collecting
                if(p_order->getDriverId() == NO_VOLUNTEER){ //order isn't assigned to a driver yet - need to assign a driver

                    for(Volunteer* d:volunteers){
                        if(d->canTakeOrder(*p_order))
                        {//assign order to driver
                            d->acceptOrder(*p_order);
                            p_order->setDriverId(d->getId());
                            p_order->setStatus(OrderStatus::DELIVERING);
                            pending.erase(std::next(pending.begin(), index));
                            index --;//if an elemet got deleted then the whole least is now shorter by 1
                            inProcess.push_back(p_order);
                            break;
                        }
                    }
                }
            }

        }


        //stages 2 - 4
        for (int index = 0; index < volunteers.size() ; index ++){//for(Volunteer* v: volunteers){ //iterate all volunteers
            Volunteer* v = volunteers[index];
            if (v->getActiveOrderId() == NO_ORDER) // checks if this volunteer is even doing anything
                continue;
            Order o = wareHouse.getOrder(v->getActiveOrderId());
            v->step(); //stage 2 - make a step
            //stage 3
            if(v->getActiveOrderId() == NO_ORDER){ //check if the volunteer is done handling the order
                //change the order to be in a different list
                if(o.getStatus() == OrderStatus::COLLECTING){ //collector is done collecting
                    int i = 0;
                    for(Order* order:inProcess){
                        if(order->getId() == o.getId()){
                            inProcess.erase(std::next(inProcess.begin(), i));
                            pending.push_back(order);
                            break;
                        }
                        i++;
                    }
                }
                else if(o.getStatus() == OrderStatus::DELIVERING){ //driver is done delivering
                    int i = 0;
                    for(Order* order:inProcess){
                        if(order->getId() == o.getId()){
                            order->setStatus(OrderStatus::COMPLETED);
                            inProcess.erase(std::next(inProcess.begin(), i));
                            completed.push_back(order);
                            break;
                        }
                        i++;
                    }
                }

                //stage 4 - delete unnecessary volunteers
                if(!v->hasOrdersLeft() and v->getActiveOrderId() == NO_ORDER){
                    volunteers.erase(std::next(volunteers.begin(), index));
                    index --;//next volunteer will be in the same index as the one that was just deleted
                    delete v;
                }
            }

        }
    }

    wareHouse.addAction(this);
}

std::string SimulateStep::toString() const {
    string res = "simulateStep ";
    res += to_string(numOfSteps);
    res += " " + ActionStatusToString(getStatus());
    return res;
}

SimulateStep * SimulateStep::clone() const {
    return new SimulateStep(*this);
}


//-----------class AddOrder------------------

AddOrder::AddOrder(int id): customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
    if((!wareHouse.customerExists(customerId)) || !wareHouse.getCustomer(customerId).canMakeOrder()){
        error("Cannot place this order");
        std::cout << "Error: " + getErrorMsg() << endl;
    }

    else{
        int orderId = wareHouse.newOrderId();
        int distance = wareHouse.getCustomer(customerId).getCustomerDistance();
        Order* newOrder = new Order(orderId, customerId, distance);
        wareHouse.getCustomer(customerId).addOrder(orderId);
        wareHouse.addOrder(newOrder);
    }
    wareHouse.addAction(this);
}

string AddOrder::toString() const{
    string res = "order " + to_string(customerId) + " " + ActionStatusToString(getStatus());
    return res;
}


AddOrder * AddOrder::clone() const {
    return new AddOrder(*this);
}


//-----------class AddCustomer------------------

CustomerType AddCustomer::toCustomerType(const string customerType){
    if(customerType == "soldier")
        return CustomerType::Soldier;
    return CustomerType::Civilian;    
}

string AddCustomer::typeToString() const{
    if (customerType == CustomerType::Soldier)
        return "soldier";
    return "civilian";
}

AddCustomer::AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders): customerName(_customerName), customerType(toCustomerType(_customerType)), distance(_distance), maxOrders(_maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.addCustomer(customerName, typeToString(), distance, maxOrders);
    wareHouse.addAction(this);
}

AddCustomer * AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    string res = "customer ";
    res += customerName;
    res += " " + typeToString();
    res += " " + to_string(distance);
    res += " " + to_string(maxOrders);
    res += " " + ActionStatusToString(getStatus());
    return res;
}


//-----------class PrintOrderStatus------------------

PrintOrderStatus::PrintOrderStatus(int id): orderId(id){}
    
void PrintOrderStatus::act(WareHouse &wareHouse){
    if(!wareHouse.orderExists(orderId)){ //check if order exists
        error("Order doesn't exist");
        std::cout << "Error: " + getErrorMsg() << endl;
    }
    else cout << wareHouse.getOrder(orderId).toString() << endl; //order does exist
    
    wareHouse.addAction(this);
}
    
PrintOrderStatus * PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}
    
string PrintOrderStatus::toString() const{
    string res = "orderStatus ";
    res += to_string(orderId);
    res += " " + ActionStatusToString(getStatus());
    return res;
}


//-----------class PrintCustomerStatus------------------

PrintCustomerStatus::PrintCustomerStatus(int _customerId): customerId(_customerId){}
    
void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(!wareHouse.customerExists(customerId)){ //check if customer exists
        error("Customer doesn't exist");
        std::cout << "Error: " + getErrorMsg() << endl;
    }
    else{ //customer does exist
        std::cout << "CustomerID: " + to_string(customerId) << endl;
    
        vector<int> orderIds = wareHouse.getCustomer(customerId).getOrdersIds();
        
        for(int id:orderIds){
            std::cout << "OrderID: " + to_string(id) << endl ;
            std::cout << "OrderStatus: " + wareHouse.getOrder(id).StatusToString() << endl;
        }

        std::cout << "numOrderLeft: " + to_string(wareHouse.getCustomer(customerId).getOrdersLeft()) << endl;
    }

    wareHouse.addAction(this);
}
    
PrintCustomerStatus * PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}
    
string PrintCustomerStatus::toString() const{
    string res = "customerStatus " + to_string(customerId) + " " + ActionStatusToString(getStatus());
    return res;
}


//-----------class PrintVolunteerStatus------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id): VolunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(!wareHouse.volunteerExists(VolunteerId)){ //check if volunteer exists
        error("Volunteer doesn't exist");
        std::cout << "Error: " << getErrorMsg()<< std::endl;
    }
    else std::cout <<wareHouse.getVolunteer(VolunteerId).toString() << std::endl; //customer does exist

    wareHouse.addAction(this);
}
        
PrintVolunteerStatus * PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    string res = "volunteerStatus " + to_string(VolunteerId) + " " + ActionStatusToString(getStatus());
    return res;
}


//-----------class PrintActionsLog------------------

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse) {
    wareHouse.printOrders();
    wareHouse.addAction(this);
}

PrintActionsLog * PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    string res = "log "  + ActionStatusToString(getStatus());
    return res;
}


//-----------class Close------------------

Close::Close(){}

void Close::act(WareHouse &wareHouse) {
    vector<Order*> orders = wareHouse.getOrders();

    for(Order* o: orders){
        cout << "OrderID: " << o->getId() <<", CustomerID: " << o->getCustomerId() << ",OrderStatus: " << o->StatusToString() <<endl;
    }

    //we don't add the current action to the action list because all the data are about to get deleted anyway.
    wareHouse.freeResources();
    wareHouse.close();
}

Close * Close::clone() const{
    return new Close(*this);
}

string Close::toString() const {
    string res = "close ";
    res += ActionStatusToString(getStatus());
    return res;
}


//-----------class BackupWareHouse------------------

BackupWareHouse::BackupWareHouse(){}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr)
        backup = new WareHouse(wareHouse);
    else
        *backup = wareHouse;
    wareHouse.addAction(this);
}

BackupWareHouse * BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    string res = "backup ";
    res += ActionStatusToString(getStatus());
    return res;
}


//-----------class RestoreWareHouse------------------

RestoreWareHouse::RestoreWareHouse(){}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if(backup == nullptr)
    {
        error("No backup available");
        std::cout << "Error: " + getErrorMsg() << endl;
    }

    else{
        wareHouse = *backup;
    }

    //wareHouse.addAction(this); // might worth to ask but i think this should never apear in the log since its done just before going to the previous backup
}

RestoreWareHouse * RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    string res = "restore ";
    res += ActionStatusToString(getStatus());
    return res;
}