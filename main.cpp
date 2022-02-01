#include<iostream>
#include<curses.h>
#include<string>
#include<ctime>
#include<vector>
#include<algorithm>
#include <bits/stdc++.h>
using namespace std;

int busCount=0;
const int ROW=4;
const int COLUMN=10;
string week[7] {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
map<string, int> busNo_vectorIndex;
map< string, vector<string> > station_busNo;

class PK_Travels {
    private:
        string busNo;           // Bus Number
        string source;          // Starting City
        string dest;            // Destination City
        string start_time;      // Time to depart starting city
        string reach_time;      // Time to reach destination city
        float price;            // Ticket price
        vector<int>busDay;      // What all week days bus is available
        int seat[ROW][COLUMN];  // Bus sitting structure
        int vacantSeatCount;    // Total vacancy
    public:
        void setBusDetails();
        void getBusDetails();
        bool checkAvailableBus(string, string, int);
        void bookTicket();
};
vector<PK_Travels*> bus;

// adds vertical line for visual easiness
void verticalLine(string c) {
    for(int i=0; i<80; i++) {
        cout<<c;
    }
    cout<<"\n";
}

// sets parameters for each PK_Travels object
void PK_Travels::setBusDetails() {
    cout<<++busCount<<"th bus detail"<<"\n";
    cout<<"--------------------"<<"\n";
    cout<<"Bus Number: ";
    cin>>busNo;
    busNo_vectorIndex[busNo]=busCount-1;

    cout<<"Starting City: ";
    cin>>source;
    station_busNo[source].push_back(busNo);

    cout<<"Destination City: ";
    cin>>dest;
    station_busNo[dest].push_back(busNo);

    cout<<"Enter week day number on which bus runs (1: Sunday, 7: Saturday): ";
    /* need to improve this */
    int i=0;
    cout<<"\n";
    busDay.reserve(7);
    while (i<7) {
        cout<<"Day "<<i+1<<"? Enter 0 or 1: ";
        cin>>busDay[i];
        ++i;
    }
    cout<<"Time to depart starting station(24 hrs format): ";
    cin>>start_time;

    cout<<"Time to arrive destination(24 hrs format): ";
    cin>>reach_time;

    cout<<"Ticket Price: ";
    cin>>price; 

    vacantSeatCount=ROW*COLUMN;
}

// prints details of bus
void PK_Travels::getBusDetails() {
    verticalLine("-");
    cout<<"Bus Number: "<<busNo<<"\n";
    cout<<"\t"<<source<<"\t--->\t"<<dest<<"\n";
    cout<<"\t"<<start_time<<"\t--->\t"<<reach_time<<"\n";
    cout<<"Seat Availabililty: "<<vacantSeatCount<<"\n";
    cout<<"Rs. "<<price<<"\n";
    int i=0;
    cout<<"Available on\t";
    while(i<7) {
        if(busDay[i]) {
            cout<<week[i]<<"\t";
        }
        i++;
    }
    week[i];
    cout<<"\n";
    verticalLine("-");
}

// Check if bus and its ticket are available
void checkBus() {
    string p_board, p_dest;
    int travelDay;
    cout<<"Passenger Boarding Station: ";
    cin>>p_board;
    cout<<"Passenger Destination Station: ";
    cin>>p_dest;
    /* modify this*/
    cout<<"Week day of travel(1: Sunday, 7: Saturday): ";
    cin>>travelDay;
    cout<<"\n";

    // find bus number with desired boarding and dest station
    vector<string> b=station_busNo[p_board];
    vector<string> d=station_busNo[p_dest];
    sort(b.begin(), b.end());
    sort(d.begin(), d.end());
    int b_len=b.size(), d_len=d.size();
    int i=0, j=0;
    vector<string> intersected_vector;
    while(i<b_len && j<d_len) {
        if(b[i]<d[j])
            i++;
        else if (b[i]>d[j])
            j++;
        else {
            intersected_vector.push_back(b[i]);
            i++;
            j++;
        }
    }
    vector<string> filtered_bus;
    for(int j=0; j<intersected_vector.size(); j++) {
        bool check = false;
        int index=busNo_vectorIndex[intersected_vector[j]];
        check = bus[index]->checkAvailableBus(p_board, p_dest, travelDay);
        if (check) {
            filtered_bus.push_back(intersected_vector[j]);
        }
    }
    cout<<"Total number of buses matches your requirment: "<<filtered_bus.size()<<"\n";
    if(filtered_bus.size()==0) {
        cout<<"\n"<<"No bus matched your requirement!!"<<"\n";
    } else {
        char ch;
        cout<<"\n"<<"shall we proceed with ticket booking(y/n): ";
        cin>>ch;
        if (ch=='y' | ch=='Y') {
            string b_no;
            int i=0;
            if(filtered_bus.size()>1) {
                do {
                    if(i>0) {
                        cout<<"Please select one of the these bus number: "<<"\t";
                        for(int j; j<filtered_bus.size(); j++) {
                            cout<<filtered_bus[j]<<"\t";
                        }
                    }
                    cout<<"\n"<<"Enter bus number: ";
                    cin>>b_no;
                    ++i;
                } while(find(filtered_bus.begin(), filtered_bus.end(), b_no)==filtered_bus.end());
            } else {
                b_no=filtered_bus[0];
            }
            int index=busNo_vectorIndex[b_no];
            bus[index]->bookTicket();
        }
    }
}

// check if bus and its ticket are available
bool PK_Travels::checkAvailableBus(string p_board, string p_dest, int i) {
    if(source==p_board && dest==p_dest && busDay[i-1]==1 && vacantSeatCount>0) {
        getBusDetails();
        return true;
    }
    return false;
}

// Books ticket
void PK_Travels::bookTicket() {    
    int passenger_count=0;
    cout<<"Available seats in bus number "<<busNo<<":\n";
    // visually represent each seat of the bus
    for(int i=0; i<ROW; i++) {
        if(i==0) {
            for(int k=1; k<=COLUMN; k++) {
                cout<<"\t"<<k;
            }
            cout<<"\n";
        }
        for(int j=0; j<COLUMN; j++) {
            if(j==0) {
                cout<<i+1;
            }
            cout<<"\t"<<seat[i][j];
        }
        cout<<"\n";
    }

    // ticket booking
    do {
        cout<<"Enter number of passengers: ";
        cin>>passenger_count;
        int i=0;
        while(i<passenger_count) {
            int r, c;
            ++i;
            cout<<i<<"th passenger details:"<<"\n";
            cout<<"\tRow: ";
            cin>>r;
            cout<<"\tColumn: ";
            cin>>c;
            if(r<1 || r>ROW || c<1 || c>COLUMN) {
                cout<<"Invalid seat number!!"<<"\n";
                i--;
            } else {
                if(seat[r-1][c-1]) {
                    cout<<"\n"<<"Selected seat is already taken. Please select a different seat"<<"\n";
                    i--;
                } else {
                    seat[r-1][c-1]=1;
                    vacantSeatCount--;
                }
            }
            verticalLine("-");
        }
        if(passenger_count<1) {
            cout<<"Number of passengers to be onboarded must be atleast 1"<<"\n";
        }
    } while(passenger_count<1);
}

int addNewBus() {
    bus.push_back(NULL);
    bus[busCount]=new PK_Travels;
    bus[busCount]->setBusDetails();

}

void showBus() {
    int len=bus.size();
    verticalLine("*");
    for(int i=0; i<len ; i++) {
        verticalLine("-");
        cout<<i+1<<"th bus details"<<"\n";
        bus[i]->getBusDetails();
        cout<<"\n";
    }
    verticalLine("*");
}

int main() {
    int a;
    while(1) {
        cout<<"\n";
        verticalLine("-");
        cout<<"\t1. Add New Bus Data"<<"\n";
        cout<<"\t2. Display Bus Data"<<"\n";
        cout<<"\t3. Check Ticket Availabilty"<<"\n";
        cout<<"Enter choice: ";
        cin>>a;
        cout<<"\n";
        switch (a) {
        case 1:
            addNewBus();
            break;
        case 2:
            showBus(); 
            break;
        case 3:
            checkBus();
            break;
        }
    }
    return 0;
}
