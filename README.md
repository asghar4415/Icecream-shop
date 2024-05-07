# Ice Creams Factory Problem

Welcome to the Eldorado Ice Creams! This repository contains the source code for a for an ice cream shop problem. Customers can select flavors and toppings for their ice creams, and the system calculates the bill for each customer. We have used multiple techniques(mutex, semaphore, monitor) to implement the concurrency in the system and compared their performance.

## Features

- **Menu Display**: Displays the available flavors and toppings along with their prices.
- **Customer Selection**: Allows customers to select a flavor and a topping for their ice cream.
- **Multi-Counter Support**: Supports multiple billing counters to serve customers concurrently.
- **Billing**: Calculates the bill for each customer based on their flavor and topping choices.
- **Performance Comparison**: Provides implementations using different concurrency solutions (mutex, semaphore, monitor) and compares their performance.

## Usage

1. Clone the repository to your local machine:

git clone https://github.com/asghar4415/Icecream-shop.git


2. Navigate to the project directory:

cd Icecream-shop


3. Compile the source code:

gcc main.c -o output


4. Run the executable:

./output


5. Follow the on-screen instructions to input the number of customers and their details.

## Concurrency Solutions

- **Mutex**: Uses mutex locks to synchronize access to shared resources such as the ticket counter, flavor and topping counts, and customer data.
- **Semaphore**: Utilizes semaphores to control access to the ticket counter and payment process.
- **Monitor**: Implements a monitor-style solution using conditional variables to coordinate access to the ticket counter.

## Performance Comparison

The repository includes implementations of the billing system using different concurrency solutions. After running the program, the execution time of each solution is displayed, allowing you to compare their performance.

