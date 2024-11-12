#include <iostream>
#include <string>
#include <limits>
#include <stdexcept> // For exception classes
using namespace std;

// Base class for common attributes and methods
class Entity {
public:
    Entity() {}
    virtual ~Entity() {}
};

// Class for Route
class Route : public Entity {
public:
    Route() : name(""), source(""), destination(""), distance(0), atime(""), dtime(""), fare(0) {}
    Route(const string& name, const string& source, const string& destination, int distance, const string& atime, const string& dtime, double fare)
        : name(name), source(source), destination(destination), distance(distance), atime(atime), dtime(dtime), fare(fare) {}

    string getName() const { return name; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    int getDistance() const { return distance; }
    string getatime() const { return atime; }
    string getdtime() const { return dtime; }
    double getfare() const { return fare; }

private:
    string name;
    string source;
    string destination;
    int distance;
    string atime;
    string dtime;
    double fare;
};

// Class for Bus
class Bus : public Entity {
public:
    Bus() : busNumber(""), totalSeats(0), route(nullptr) {}
    Bus(const string& busNumber, Route* route, int totalSeats)
        : busNumber(busNumber), route(route), totalSeats(totalSeats) {
        for (int i = 0; i < totalSeats; ++i) {
            seats[i] = false;
        }
    }

    bool reserveSeat(int seatNumber) {
        if (seatNumber >= 1 && seatNumber <= totalSeats && !seats[seatNumber - 1]) {
            seats[seatNumber - 1] = true;
            return true;
        }
        return false;
    }

    int getTotalSeats() const { return totalSeats; }

    bool cancelSeat(int seatNumber) {
        if (seatNumber >= 1 && seatNumber <= totalSeats && seats[seatNumber - 1]) {
            seats[seatNumber - 1] = false;
            return true;
        }
        return false;
    }

    void displayBusInfo() const {
        cout << "\nBus Number: " << busNumber << endl;
        cout << "Route: " << route->getName() << " (" << route->getSource() << " to " << route->getDestination() << ")\n";
        cout << "Distance: " << route->getDistance() << " km" << endl;
        cout << "Total Seats: " << totalSeats << endl;
        cout << "Available Seats: ";
        for (int i = 0; i < totalSeats; i++) {
            cout << (seats[i] ? "NA" : to_string(i + 1)) << " ";
        }
        cout << endl;
        cout << "Ticket Fare: " << route->getfare() << endl;
        cout << "Arrival Time: " << route->getatime() << endl;
        cout << "Departure Time: " << route->getdtime() << endl;
    }

    string getBusNumber() const { return busNumber; }
    const Route* getRoute() const { return route; }

private:
    string busNumber;
    Route* route; // Pointer to route instead of object
    int totalSeats;
    bool seats[100]; // Fixed-size array for seats
};

// Class for Ticket
class Ticket : public Entity {
public:
    Ticket() : passengerName(""), seatNumber(0), route(nullptr), bus(nullptr) {}
    Ticket(const string& passengerName, int seatNumber, Route* route, Bus* bus)
        : passengerName(passengerName), seatNumber(seatNumber), route(route), bus(bus) {}

    string getPassengerName() const { return passengerName; }
    int getSeatNumber() const { return seatNumber; }
    const Route* getRoute() const { return route; }
    const Bus* getBus() const { return bus; }

private:
    string passengerName;
    int seatNumber;
    Route* route; // Pointer to route
    Bus* bus;     // Pointer to bus
};

// Class for User
class User : public Entity {
public:
    User() : username(""), password(""), ticketCount(0) {}
    User(const string& username, const string& password)
        : username(username), password(password), ticketCount(0) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }

    void addTicket(Ticket* newTicket) {
        if (ticketCount < 10) {
            tickets[ticketCount++] = newTicket;
        } else {
            cout << "Ticket limit reached." << endl;
        }
    }

    void removeTicket(int index) {
        if (index >= 1 && index <= ticketCount) {
            for (int i = index - 1; i < ticketCount - 1; ++i) {
                tickets[i] = tickets[i + 1];
            }
            ticketCount--;
        } else {
            cout << "Invalid ticket index." << endl;
        }
    }

    void printTicket(Ticket* ticket) const {
        cout << "Passenger: " << ticket->getPassengerName()
             << ", Bus Number: " << ticket->getBus()->getBusNumber()
             << ", Seat Number: " << ticket->getSeatNumber()
             << ", Ticket Fare: " << ticket->getRoute()->getfare()
             << ", Arrival: " << ticket->getRoute()->getatime()
             << ", Departure: " << ticket->getRoute()->getdtime() << endl;
    }

    int numberOfTickets() const { return ticketCount; }
    Ticket* getTicket(int index) { return tickets[index - 1]; }

private:
    string username;
    string password;
    Ticket* tickets[10]; // Fixed-size array for tickets (using pointers)
    int ticketCount;
};

// Bus Reservation System Class
class BusReservationSystem {
public:
    BusReservationSystem() : userCount(0), routeCount(0), busCount(0) {
        // Sample routes
        routes[routeCount++] = new Route("R001", "City A", "City B", 200, "6:00 AM", "6:20 AM", 800);
        routes[routeCount++] = new Route("R002", "City B", "City C", 150, "12:00 PM", "12:20 PM", 600);
        routes[routeCount++] = new Route("R003", "City C", "City D", 180, "7:00 PM", "7:20 PM", 720);

        // Sample buses
        buses[busCount++] = new Bus("B001", routes[0], 20);
        buses[busCount++] = new Bus("B002", routes[1], 15);
        buses[busCount++] = new Bus("B003", routes[2], 18);
    }

    void registerUser(const string& username, const string& password) {
        try {
            if (userCount < 100) {
                users[userCount++] = new User(username, password);
                cout << "User registered successfully.\n";
            } else {
                throw overflow_error("User limit reached.");
            }
        } catch (const overflow_error& e) {
            cout << e.what() << endl;
        }
    }

    bool authenticateUser(const string& username, const string& password) {
        try {
            for (int i = 0; i < userCount; i++) {
                if (users[i]->getUsername() == username && users[i]->getPassword() == password) {
                    return true;
                }
            }
            throw runtime_error("Authentication failed.");
        } catch (const runtime_error& e) {
            cout << e.what() << endl;
            return false;
        }
    }

    User* findUser(const string& username) {
        for (int i = 0; i < userCount; i++) {
            if (users[i]->getUsername() == username) {
                return users[i];
            }
        }
        return nullptr; // In case no user is found
    }

    void displayRoutes() const {
        cout << "Available Routes:\n";
        for (int i = 0; i < routeCount; i++) {
            cout << "Route Name: " << routes[i]->getName() << " (" << routes[i]->getSource() << " to " << routes[i]->getDestination() << ")\n";
        }
    }

    void displayBuses() const {
        cout << "Available Buses:\n";
        for (int i = 0; i < busCount; i++) {
            buses[i]->displayBusInfo();
        }
    }

    void displayBusesForRoute(int selectedRoute) const {
        try {
            if (selectedRoute < 1 || selectedRoute > routeCount) {
                throw out_of_range("Invalid route selected.");
            }
            cout << "Buses for Route " << routes[selectedRoute - 1]->getName() << ":\n";
            for (int i = 0; i < busCount; i++) {
                if (buses[i]->getRoute()->getName() == routes[selectedRoute - 1]->getName()) {
                    buses[i]->displayBusInfo();
                }
            }
        } catch (const out_of_range& e) {
            cout << e.what() << endl;
        }
    }

    void bookTicket(const string& username) {
        try {
            int selectedRoute, selectedBus, selectedSeat;

            displayRoutes();
            cout << "Select a Route (Enter the Route Number): ";
            string routeNumber;
            cin >> routeNumber;

            bool validRoute = false;
            for (int i = 0; i < routeCount; i++) {
                if (routeNumber == routes[i]->getName()) {
                    selectedRoute = i + 1;
                    validRoute = true;
                    break;
                }
            }

            if (!validRoute) {
                throw invalid_argument("Invalid route number.");
            }

            displayBusesForRoute(selectedRoute);

            cout << "Select a Bus (Enter the Bus Number): ";
            string busNumber;
            cin >> busNumber;

            validRoute = false;
            for (int i = 0; i < busCount; i++) {
                if (busNumber == buses[i]->getBusNumber() && buses[i]->getRoute()->getName() == routes[selectedRoute - 1]->getName()) {
                    selectedBus = i + 1;
                    validRoute = true;
                    break;
                }
            }

            if (!validRoute) {
                throw invalid_argument("Invalid bus number.");
            }

            cout << "Enter the seat number you want to book: ";
            cin >> selectedSeat;

            if (cin.fail() || selectedSeat < 1 || selectedSeat > buses[selectedBus - 1]->getTotalSeats()) {
                throw out_of_range("Invalid seat number.");
            }

            if (buses[selectedBus - 1]->reserveSeat(selectedSeat)) {
                Ticket* ticket = new Ticket(username, selectedSeat, routes[selectedRoute - 1], buses[selectedBus - 1]);
                User* user = findUser(username);
                if (user) {
                    user->addTicket(ticket);
                    cout << "Ticket booked successfully!\n";
                    user->printTicket(ticket);
                }
            } else {
                throw runtime_error("Seat booking failed. The seat is already reserved or does not exist.");
            }
        } catch (const exception& e) {
            cout << e.what() << endl;
        }
    }

    void run() {
        while (true) {
            try {
                cout << "1. Register\n2. Log In\n3. Exit\nEnter your choice: ";
                int choice;
                cin >> choice;

                if (choice == 1) {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    registerUser(username, password);
                } else if (choice == 2) {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    if (authenticateUser(username, password)) {
                        cout << "Logged in successfully!\n";
                        userMenu(username);
                    } else {
                        cout << "Authentication failed! Please check your username and password.\n";
                    }
                } else if (choice == 3) {
                    cout << "Exiting the Bus Reservation System. Goodbye!\n";
                    break;
                }
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
        }
    }

    void userMenu(const string& username) {
        while (true) {
            try {
                cout << "1. Display Routes\n2. Display Buses\n3. Book Ticket\n4. Logout\nEnter your choice: ";
                int choice;
                cin >> choice;

                if (choice == 1) {
                    displayRoutes();
                } else if (choice == 2) {
                    displayBuses();
                } else if (choice == 3) {
                    bookTicket(username);
                } else if (choice == 4) {
                    cout << "Logged out successfully.\n";
                    break;
                }
            } catch (const exception& e) {
                cout << e.what() << endl;
            }
        }
    }

private:
    User* users[100]; // Pointer array for users
    Route* routes[10]; // Pointer array for routes
    Bus* buses[10];    // Pointer array for buses
    int userCount;
    int routeCount;
    int busCount;
};

// Main function to run the system
int main() {
    BusReservationSystem system;
    system.run();

}

