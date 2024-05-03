#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

using namespace std;

#define NUM_COUNTERS 5
#define NUM_FLAVORS 5
#define NUM_TOPPINGS 5

#define priceFlav_1 1.05
#define priceFlav_2 2.00
#define priceFlav_3 1.67
#define priceFlav_4 1.80
#define priceFlav_5 2.50
#define priceTopp_1 0.8
#define priceTopp_2 0.5
#define priceTopp_3 0.7
#define priceTopp_4 0.6
#define priceTopp_5 0.9

int ticket = 30, _flavors[NUM_FLAVORS] = {29, 34, 18, 25, 20}, _toppings[NUM_TOPPINGS] = {20, 34, 28, 22, 18};
double revenue[NUM_COUNTERS] = {0.0};
int noof_customers=0;

class customer {
    public:
    string name;
        int id;
        int flavor;
        int topping;
        double bill;
};

customer customers[30];

mutex ticketMutex, flavorMutex, toppingMutex, paymentMutex[NUM_COUNTERS];
condition_variable ticketCV, flavorCV, toppingCV, paymentCV[NUM_COUNTERS];

void counter(int counterId);
int selectFlavor(int customerId);
int selectTopping(int customerId);

int main() {
    int noC;
    vector<thread> counterThread(NUM_COUNTERS);
    cout << "\n\n\n\n\n\n\t\t\t\t====================================================================================\n\n\n\n";
    cout << " \t\t\t\t\t\t\t Welcome to The Eldorado Ice Creams \n\n";
    cout << " \t\t\t\t\t\t\t We serve icecreams and happiness.  \n\n\n\n";
    cout << " \t\t\t\t====================================================================================\n\n\n\n";

    cout << "\t\t\t\t\t\t\t\t\tMENU:\n\n\n";
    cout<<"\t\t\t\t\t\t FLAVOURS   "<<"\t\t\t\t TOPPINGS   "<<"\n\n";
    cout<<"\t\t\t\t\t\t 1. Vanilla  $"<<priceFlav_1<<"\t\t\t 1. Sprinkles  $"<<priceTopp_1<<"\n\n";
    cout<<"\t\t\t\t\t\t 2. Chocolate $"<<priceFlav_2<<"\t\t\t 2. Chocolate chips $"<<priceTopp_2<<"\n\n";
    cout<<"\t\t\t\t\t\t 3. Strawberry $"<<priceFlav_3<<"\t\t\t 3. Whipped cream $"<<priceTopp_3<<"\n\n";
    cout<<"\t\t\t\t\t\t 4. Mint $"<<priceFlav_4<<"\t\t\t\t 4. Caramel sauce $"<<priceTopp_4<<"\n\n";
    cout<<"\t\t\t\t\t\t 5. Coffee $"<<priceFlav_5<<"\t\t\t\t 5. Nuts $"<<priceTopp_5<<"\n\n\n\n";



    cout << "\n\n\t\t\t\t\t\t\tEnter Number Of Customers [1-30]:  ";
    cin >> noC;
    cout << "\n";

   

     if(noC > ticket || noC <= 0) {
        cout << "\n\t\t\t\t\t\t\t\tInvalid Input!\n";
        return 0;
    }

     noof_customers = noC;


    for( int i=0; i<noC; i++)
    {
        cout << "\t\t\t\t\t\t\tEnter Customer "<<i+1<<" name: ";
        string name;
        cin >> name;
        cout << "\n";
        customers[i].name = name;
        customers[i].id = i+1;
    }

   

    for (int i = 0; i < NUM_COUNTERS; i++) {
        
        counterThread[i] = thread(counter, i + 1);
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        counterThread[i].join();
    }

    return 0;
}

void counter(int counterId) {
    lock_guard<mutex> ticketLock(ticketMutex);
    lock_guard<mutex> paymentLock(paymentMutex[counterId - 1]);

    for (int i = counterId - 1; i < ticket && i % NUM_COUNTERS == counterId - 1 && i < noof_customers; i += NUM_COUNTERS)
    {
        double bill = 0.0;
        int flavorChoice, toppingChoice;

        ticket--;
        
        flavorChoice = selectFlavor(i + 1);
        if (flavorChoice == -1) continue;

        switch (flavorChoice) {
            case 1:
                bill += priceFlav_1;
                break;
            case 2:
                bill += priceFlav_2;
                break;
            case 3:
                bill += priceFlav_3;
                break;
            case 4:
                bill += priceFlav_4;
                break;
            case 5:
                bill += priceFlav_5;
                break;
        }

        toppingChoice = selectTopping(i + 1);
        if (toppingChoice == -1) continue;

        switch (toppingChoice) {
            case 1:
                bill += priceTopp_1;
                break;
            case 2:
                bill += priceTopp_2;
                break;
            case 3:
                bill += priceTopp_3;
                break;
            case 4:
                bill += priceTopp_4;
                break;
            case 5:
                bill += priceTopp_5;
                break;

        
        }

        // Displaying ice cream number and its cost
        for(int j=0; j<noof_customers; j++)
        {
            if(customers[j].id == i+1)
            {
                
                customers[j].flavor = flavorChoice;
                customers[j].topping = toppingChoice;
                customers[j].bill = bill;

                cout<<"\n\n\t\t\t\t\t\t\t\tBILLING COUNTER "<<counterId<<"\n";
                cout<<"\n\t\t\t\t\t\t\t\tCustomer Name: "<<customers[j].name<<"\n";
                cout<<"\n\t\t\t\t\t\t\t\tIce Cream Number: "<<i+1<<"\n";
                cout<<"\n\t\t\t\t\t\t\t\tCost: $"<<bill<<"\n\n";
                cout<<"\n";

                break;
            }
        }


        revenue[counterId - 1] += bill;
    }

    // cout << "\n\t\t\t\t\t\t\t\tCounter " << counterId << " closed. Total revenue: $" << revenue[counterId - 1] << "\n";
}

int selectFlavor(int customerId) {

    lock_guard<mutex> flavorLock(flavorMutex);
    int flavorChoice;

for(int i=0; i<noof_customers; i++)
{
    if(customers[i].id == customerId)
    {
        cout <<"\n\t\t\t\t\t\t\t"<<customers[i].name<<", Please select a flavor:\n";
        cout <<"\n\t\t\t\t\t\t\t\t1.Vanilla\n";
        cout <<"\t\t\t\t\t\t\t\t2.Chocolate\n";
        cout <<"\t\t\t\t\t\t\t\t3.Strawberry\n";
        cout <<"\t\t\t\t\t\t\t\t4.Mint\n";
        cout <<"\t\t\t\t\t\t\t\t5.Coffee\n";

    cin >> flavorChoice;
    break;
        
    }


}


    if (flavorChoice < 1 || flavorChoice > NUM_FLAVORS) {

        for(int i=0; i<noof_customers; i++)
        {
            if(customers[i].id == customerId)
            {
                cout <<customers[i].name<<", Invalid flavor choice. Exiting.\n";
                break;
            }
        }
        return -1;
    }

    return flavorChoice;
}

int selectTopping(int customerId) {
    lock_guard<mutex> toppingLock(toppingMutex);
    int toppingChoice;

    for(int i=0; i<noof_customers; i++)
    {
        if(customers[i].id == customerId)
        {
        cout <<"\n\t\t\t\t\t\t\t"<<customers[i].name<<", Please select a topping:\n";
        cout <<"\n\t\t\t\t\t\t\t\t1.Sprinkles\n";
        cout <<"\t\t\t\t\t\t\t\t2.Chocolate chips\n";
        cout <<"\t\t\t\t\t\t\t\t3.Whipped cream\n";
        cout <<"\t\t\t\t\t\t\t\t4.Caramel sauce\n";
        cout <<"\t\t\t\t\t\t\t\t5.Nuts\n";


            cin >> toppingChoice;
            break;
        }
    }

    if (toppingChoice < 1 || toppingChoice > NUM_TOPPINGS) {
        for(int i=0; i<noof_customers; i++)
        {
            if(customers[i].id == customerId)
            {
                cout <<customers[i].name<<", Invalid topping choice. Exiting.\n";
                break;
            }
        }
        return -1;
    }

    return toppingChoice;
}
