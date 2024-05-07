# Eldorado Ice Creams Billing System

Welcome to the Eldorado Ice Creams Billing System repository! This repository contains the source code for a simple billing system for an ice cream shop. Customers can select flavors and toppings for their ice creams, and the system calculates the bill for each customer.

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

cd eldorado-ice-creams


3. Compile the source code:

gcc -o icecream_billing icecream_billing.c -lpthread


4. Run the executable:

./icecream_billing


5. Follow the on-screen instructions to input the number of customers and their details.

## Concurrency Solutions

- **Mutex**: Uses mutex locks to synchronize access to shared resources such as the ticket counter, flavor and topping counts, and customer data.
- **Semaphore**: Utilizes semaphores to control access to the ticket counter and payment process.
- **Monitor**: Implements a monitor-style solution using conditional variables to coordinate access to the ticket counter.

## Performance Comparison

The repository includes implementations of the billing system using different concurrency solutions. After running the program, the execution time of each solution is displayed, allowing you to compare their performance.

## Contributing

Contributions to this project are welcome! If you find any issues or have suggestions for improvements, please open an issue or create a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
