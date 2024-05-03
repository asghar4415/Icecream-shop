#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>

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

#define MAX_CUSTOMERS 30

typedef struct {
    char name[50];
    int id;
    int flavor;
    int topping;
    double bill;
} customer;

typedef struct {
    float mutex_time;
    float semaphore_time;
    float monitor_time;
} solution;

solution sol;

int ticket = 30;
int _flavors[NUM_FLAVORS] = {29, 34, 18, 25, 20};
int _toppings[NUM_TOPPINGS] = {20, 34, 28, 22, 18};
double revenue[NUM_COUNTERS] = {0.0};
int noof_customers = 0;

customer customers[MAX_CUSTOMERS];

sem_t ticketSemaphore;
sem_t paymentSemaphore[NUM_COUNTERS];

void* counter(void* arg);
int selectFlavor(int customerId);
int selectTopping(int customerId);

int main() {
    int noC;
    pthread_t counterThread[NUM_COUNTERS];
    struct timespec start, end;
    double elapsed;

    printf("\n\n\n\n\n\n\t\t\t\t====================================================================================\n\n\n\n");
    printf(" \t\t\t\t\t\t\t Welcome to The Eldorado Ice Creams \n\n");
    printf(" \t\t\t\t\t\t\t We serve icecreams and happiness.  \n\n\n\n");
    printf(" \t\t\t\t====================================================================================\n\n\n\n");

    printf("\t\t\t\t\t\t\t\t\tMENU:\n\n\n");
    printf("\t\t\t\t\t\t FLAVOURS   \t\t\t\t TOPPINGS   \n\n");
    printf("\t\t\t\t\t\t 1. Vanilla  $%.2f\t\t\t 1. Sprinkles  $%.2f\n\n", priceFlav_1, priceTopp_1);
    printf("\t\t\t\t\t\t 2. Chocolate $%.2f\t\t\t 2. Chocolate chips $%.2f\n\n", priceFlav_2, priceTopp_2);
    printf("\t\t\t\t\t\t 3. Strawberry $%.2f\t\t\t 3. Whipped cream $%.2f\n\n", priceFlav_3, priceTopp_3);
    printf("\t\t\t\t\t\t 4. Mint $%.2f\t\t\t\t 4. Caramel sauce $%.2f\n\n", priceFlav_4, priceTopp_4);
    printf("\t\t\t\t\t\t 5. Coffee $%.2f\t\t\t\t 5. Nuts $%.2f\n\n\n\n", priceFlav_5, priceTopp_5);

    printf("\n\n\t\t\t\t\t\t\tEnter Number Of Customers [1-30]:  ");
    scanf("%d", &noC);
    printf("\n");

    if (noC > ticket || noC <= 0) {
        printf("\n\t\t\t\t\t\t\t\tInvalid Input!\n");
        return 0;
    }

    noof_customers = noC;

    for (int i = 0; i < noC; i++)
    {
        printf("\t\t\t\t\t\t\tEnter Customer %d name: ", i + 1);
        scanf("%s", customers[i].name);
        printf("\n");
        customers[i].id = i + 1;
    }

    sem_init(&ticketSemaphore, 0, ticket);
    for (int i = 0; i < NUM_COUNTERS; i++) {
        sem_init(&paymentSemaphore[i], 0, 1);
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < NUM_COUNTERS; i++) {
        pthread_create(&counterThread[i], NULL, counter, (void*)(intptr_t)(i + 1));
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        pthread_join(counterThread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Elapsed time: %.5f seconds\n", elapsed);

    return 0;
}

void* counter(void* arg) {
    int counterId = (intptr_t)arg;

    for (int i = counterId - 1; i < ticket && i % NUM_COUNTERS == counterId - 1 && i < noof_customers; i += NUM_COUNTERS)
    {
        double bill = 0.0;
        int flavorChoice, toppingChoice;

        sem_wait(&ticketSemaphore);

        sem_wait(&paymentSemaphore[counterId - 1]);

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

        for (int j = 0; j < noof_customers; j++)
        {
            if (customers[j].id == i + 1)
            {

                customers[j].flavor = flavorChoice;
                customers[j].topping = toppingChoice;
                customers[j].bill = bill;

                printf("\n\n\t\t\t\t\t\t\t\tBILLING COUNTER %d\n", counterId);
                printf("\n\t\t\t\t\t\t\t\tCustomer Name: %s\n", customers[j].name);
                printf("\n\t\t\t\t\t\t\t\tIce Cream Number: %d\n", i + 1);
                printf("\n\t\t\t\t\t\t\t\tCost: $%.2f\n\n", bill);
                printf("\n");

                break;
            }
        }

        revenue[counterId - 1] += bill;

        sem_post(&paymentSemaphore[counterId - 1]);
        sem_post(&ticketSemaphore);
    }

    return NULL;
}

int selectFlavor(int customerId) {
    int flavorChoice;

    for (int i = 0; i < noof_customers; i++)
    {
        if (customers[i].id == customerId)
        {
            printf("\n\t\t\t\t\t\t\t%s, Please select a flavor:\n", customers[i].name);
            printf("\n\t\t\t\t\t\t\t\t1.Vanilla\n");
            printf("\t\t\t\t\t\t\t\t2.Chocolate\n");
            printf("\t\t\t\t\t\t\t\t3.Strawberry\n");
            printf("\t\t\t\t\t\t\t\t4.Mint\n");
            printf("\t\t\t\t\t\t\t\t5.Coffee\n");

            scanf("%d", &flavorChoice);
            break;
        }
    }

    if (flavorChoice < 1 || flavorChoice > NUM_FLAVORS) {
        for (int i = 0; i < noof_customers; i++)
        {
            if (customers[i].id == customerId)
            {
                printf("%s, Invalid flavor choice. Exiting.\n", customers[i].name);
                break;
            }
        }
        return -1;
    }

    return flavorChoice;
}

int selectTopping(int customerId) {
    int toppingChoice;

    for (int i = 0; i < noof_customers; i++)
    {
        if (customers[i].id == customerId)
        {
            printf("\n\t\t\t\t\t\t\t%s, Please select a topping:\n", customers[i].name);
            printf("\n\t\t\t\t\t\t\t\t1.Sprinkles\n");
            printf("\t\t\t\t\t\t\t\t2.Chocolate chips\n");
            printf("\t\t\t\t\t\t\t\t3.Whipped cream\n");
            printf("\t\t\t\t\t\t\t\t4.Caramel sauce\n");
            printf("\t\t\t\t\t\t\t\t5.Nuts\n");

            scanf("%d", &toppingChoice);

            break;

        }

    }

    if (toppingChoice < 1 || toppingChoice > NUM_TOPPINGS) {
        for (int i = 0; i < noof_customers; i++)
        {
            if (customers[i].id == customerId)
            {
                printf("%s, Invalid topping choice. Exiting.\n", customers[i].name);
                break;
            }
        }
        return -1;
    }

    return toppingChoice;

}

