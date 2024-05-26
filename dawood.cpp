#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class BookingException : public exception {
private:
    string message;

public:
    BookingException(const string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class Payment {
public:
    enum class Method { CASH, CARD };
};

class Passenger {
private:
    string name;
    double ticketPrice;
    Payment::Method paymentMethod;

public:
    Passenger(const string& name, double ticketPrice, Payment::Method paymentMethod)
            : name(name), ticketPrice(ticketPrice), paymentMethod(paymentMethod) {}

    string getName() const {
        return name;
    }

    double getTicketPrice() const {
        return ticketPrice;
    }

    Payment::Method getPaymentMethod() const {
        return paymentMethod;
    }
};

class PassengerNode {
public:
    Passenger passenger;
    shared_ptr<PassengerNode> next;

    PassengerNode(const Passenger& passenger) : passenger(passenger), next(nullptr) {}
};

class Flight {
private:
    string flightNumber;
    string origin;
    string destination;
    int capacity;
    int bookedSeats;
    double baseTicketPrice;
    shared_ptr<PassengerNode> passengersHead;

public:
    Flight(const string& flightNumber, const string& origin, const string& destination, int capacity, double baseTicketPrice)
            : flightNumber(flightNumber), origin(origin), destination(destination), capacity(capacity), bookedSeats(0), baseTicketPrice(baseTicketPrice), passengersHead(nullptr) {}

    string getFlightNumber() const {
        return flightNumber;
    }

    string getOrigin() const {
        return origin;
    }

    string getDestination() const {
        return destination;
    }

    int getAvailableSeats() const {
        return capacity - bookedSeats;
    }

    double calculateTicketPrice() const {
        return baseTicketPrice;
    }

    void bookSeat(const Passenger& passenger) {
        if (bookedSeats < capacity) {
            bookedSeats++;
            shared_ptr<PassengerNode> newNode = make_shared<PassengerNode>(passenger);
            newNode->next = passengersHead;
            passengersHead = newNode;
        } else {
            throw BookingException("No available seats on this flight.");
        }
    }

    void displayPassengers() const {
        cout << "Passengers on Flight " << flightNumber << ":" << endl;
        cout << "-------------------------" << endl;
        shared_ptr<PassengerNode> current = passengersHead;
        while (current != nullptr) {
            cout << current->passenger.getName() << " (Ticket Price: $" << current->passenger.getTicketPrice() << ", Payment Method: ";
            if (current->passenger.getPaymentMethod() == Payment::Method::CASH) {
                cout << "Cash";
            } else {
                cout << "Card";
            }
            cout << ")" << endl;
            current = current->next;
        }
        cout << "-------------------------" << endl;
    }
};

class Node {
public:
    shared_ptr<Flight> flight;
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(shared_ptr<Flight> flight) : flight(flight), left(nullptr), right(nullptr) {}
};

class FlightManager {
private:
    shared_ptr<Node> root;

    shared_ptr<Node> insertFlight(shared_ptr<Node> node, shared_ptr<Flight> flight) {
        if (node == nullptr) {
            return make_shared<Node>(flight);
        }

        if (flight->getFlightNumber() < node->flight->getFlightNumber()) {
            node->left = insertFlight(node->left, flight);
        } else if (flight->getFlightNumber() > node->flight->getFlightNumber()) {
            node->right = insertFlight(node->right, flight);
        }

        return node;
    }

    void displayFlightsRecursive(shared_ptr<Node> node) const {
        if (node != nullptr) {
            displayFlightsRecursive(node->left);
            const shared_ptr<Flight>& flight = node->flight;
            cout << "Flight Number: " << flight->getFlightNumber() << ", Origin: " << flight->getOrigin()
                 << ", Destination: " << flight->getDestination() << ", Available Seats: " << flight->getAvailableSeats() << ", Base Ticket Price: $" << flight->calculateTicketPrice() << endl;
            displayFlightsRecursive(node->right);
        }
    }

public:
    FlightManager() : root(nullptr) {}

    void addFlight(const string& flightNumber, const string& origin, const string& destination, int capacity, double baseTicketPrice) {
        shared_ptr<Flight> flight = make_shared<Flight>(flightNumber, origin, destination, capacity, baseTicketPrice);
        root = insertFlight(root, flight);
    }

    void displayFlights() const {
        cout << "Available Flights:" << endl;
        cout << "-----------------" << endl;
        displayFlightsRecursive(root);
        cout << "-----------------" << endl;
    }

    void bookSeat(const string& flightNumber, const Passenger& passenger) {
        shared_ptr<Node> current = root;
        while (current != nullptr) {
            if (flightNumber == current->flight->getFlightNumber()) {
                current->flight->bookSeat(passenger);
                cout << "Seat booked successfully for passenger " << passenger.getName() << "." << endl;
                cout << "Ticket Price: $" << passenger.getTicketPrice() << endl;
                cout << "Payment Method: ";
                if (passenger.getPaymentMethod() == Payment::Method::CASH) {
                    cout << "Cash" << endl;
                } else {
                    cout << "Card" << endl;
                }
                return;
            } else if (flightNumber < current->flight->getFlightNumber()) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        cerr << "Flight with number " << flightNumber << " not found." << endl;
    }

    void displayPassengers(const string& flightNumber) const {
        shared_ptr<Node> current = root;
        while (current != nullptr) {
            if (flightNumber == current->flight->getFlightNumber()) {
                current->flight->displayPassengers();
                return;
            } else if (flightNumber < current->flight->getFlightNumber()) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        cerr << "Flight with number " << flightNumber << " not found." << endl;
    }
};

int main() {
    FlightManager flightManager;

    flightManager.addFlight("F001", "New York", "Los Angeles", 100, 150.0);
    flightManager.addFlight("F002", "London", "Paris", 120, 200.0);
    flightManager.addFlight("F003", "Tokyo", "Sydney", 150, 250.0);
    flightManager.addFlight("F004", "Los Angeles", "New York", 100, 180.0);
    flightManager.addFlight("F005", "Paris", "London", 120, 220.0);
    flightManager.addFlight("F006", "Sydney", "Tokyo", 150, 270.0);
    flightManager.addFlight("F007", "Chicago", "Miami", 110, 170.0);
    flightManager.addFlight("F008", "Berlin", "Rome", 130, 210.0);
    flightManager.addFlight("F009", "Moscow", "Beijing", 160, 260.0);
    flightManager.addFlight("F010", "Toronto", "Vancouver", 90, 140.0);
    flightManager.addFlight("F011", "Madrid", "Barcelona", 100, 155.0);
    flightManager.addFlight("F012", "Dubai", "Singapore", 140, 230.0);
    flightManager.addFlight("F013", "Hong Kong", "Shanghai", 180, 290.0);
    flightManager.addFlight("F014", "Seoul", "Osaka", 120, 195.0);
    flightManager.addFlight("F015", "Bangkok", "Phuket", 130, 205.0);
    flightManager.addFlight("F016", "Cairo", "Istanbul", 150, 245.0);
    flightManager.addFlight("F017", "Mexico City", "Cancun", 110, 175.0);
    flightManager.addFlight("F018", "Sao Paulo", "Rio de Janeiro", 100, 160.0);
    flightManager.addFlight("F019", "Buenos Aires", "Santiago", 120, 200.0);
    flightManager.addFlight("F020", "Amsterdam", "Brussels", 110, 175.0);

    int userType;
    do {
        cout << "\nWELCOME TO PAKISTAN AIRPORT :" << endl;
        cout << "\nThis Will Help You:-" << endl;
        cout << "\nUser Type:" << endl;
        cout << "1. Passenger" << endl;
        cout << "2. Manager" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter user type: ";
        cin >> userType;

        switch (userType) {
            case 1: {
                string flightNumber, passengerName;
                double ticketPrice;
                int paymentMethodChoice;
                cout << "Enter flight number: ";
                cin >> flightNumber;
                cout << "Enter passenger name: ";
                cin >> passengerName;
                cout << "Enter ticket price: ";
                cin >> ticketPrice;
                cout << "Select Payment Method:" << endl;
                cout << "1. Cash" << endl;
                cout << "2. Card" << endl;
                cout << "Enter your choice: ";
                cin >> paymentMethodChoice;
                Payment::Method paymentMethod = (paymentMethodChoice == 1) ? Payment::Method::CASH : Payment::Method::CARD;
                flightManager.bookSeat(flightNumber, Passenger(passengerName, ticketPrice, paymentMethod));
                break;
            }
            case 2: {
                int managerChoice;
                do {
                    cout << "\nManager Options:" << endl;
                    cout << "1. Display Flights" << endl;
                    cout << "2. Display Passengers on Flight" << endl;
                    cout << "3. Exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> managerChoice;

                    switch (managerChoice) {
                        case 1:
                            flightManager.displayFlights();
                            break;
                        case 2: {
                            string flightNumber;
                            cout << "Enter flight number: ";
                            cin >> flightNumber;
                            flightManager.displayPassengers(flightNumber);
                            break;
                        }
                        case 3:
                            cout << "Exiting Manager Options..." << endl;
                            break;
                        default:
                            cout << "Invalid choice. Please try again." << endl;
                            break;
                    }
                } while (managerChoice != 3);
                break;
            }
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (userType != 3);

    return 0;
}
