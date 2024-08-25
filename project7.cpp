#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream> // Added for file handling
#include <limits>

using namespace std;

// Constants
const int NUM_SEATS = 20;

// Seat class
class Seat {
private:
    int seatNumber;
    bool isBooked;
    string passengerName;

public:
    Seat(int number) : seatNumber(number), isBooked(false), passengerName("") {}

    bool isAvailable() const {
        return !isBooked;
    }

    void book(const string& name) {
        isBooked = true;
        passengerName = name;
    }

    void cancelBooking() {
        isBooked = false;
        passengerName = "";
    }

    void display() const {
        cout << "Seat Number: " << setw(2) << seatNumber << " - ";
        if (isBooked)
            cout << "Booked by: " << passengerName << endl;
        else
            cout << "Available" << endl;
    }

    int getSeatNumber() const {
        return seatNumber;
    }

    string getPassengerName() const {
        return passengerName;
    }
};

// Train class
class Train {
private:
    vector<Seat> seats;

public:
    Train() {
        for (int i = 1; i <= NUM_SEATS; ++i) {
            seats.push_back(Seat(i));
        }
    }

    void displayAvailableSeats() const {
        cout << "Available Seats:" << endl;
        for (const auto& seat : seats) {
            seat.display();
        }
        cout << endl;
    }

    bool bookTickets(int numTickets, const vector<string>& passengerNames, const string& destination) {
        int numBooked = 0;
        auto passengerNameIter = passengerNames.begin();
        for (auto& seat : seats) {
            if (numBooked >= numTickets || passengerNameIter == passengerNames.end()) {
                break;
            }
            if (seat.isAvailable()) {
                seat.book(*passengerNameIter);
                ++numBooked;
                ++passengerNameIter;
            }
        }

        if (numBooked == numTickets) {
            cout << "Tickets booked successfully!" << endl;
            cout << "Destination: " << destination << endl;
            return true;
        } else {
            cout << "Insufficient available seats!" << endl;
            return false;
        }
    }

    bool cancelTicket(int seatNumber) {
        if (seatNumber < 1 || seatNumber > NUM_SEATS) {
            cout << "Invalid seat number!" << endl;
            return false;
        }

        Seat& seat = seats[seatNumber - 1];
        if (!seat.isAvailable()) {
            seat.cancelBooking();
            cout << "Ticket canceled successfully!" << endl;
            return true;
        } else {
            cout << "Seat " << seatNumber << " is not booked." << endl;
            return false;
        }
    }

    void displayBookingDetails() const {
        cout << "Booking Details:" << endl;
        for (const auto& seat : seats) {
            if (!seat.isAvailable()) {
                seat.display();
            }
        }
        cout << endl;
    }

    const vector<Seat>& getSeats() const {
        return seats;
    }
};

// Function to read booking details from a file
void readBookingDetailsFromFile(Train& train) {
    ifstream inputFile("booking_details.txt");
    if (inputFile.is_open()) {
        int seatNumber;
        string passengerName;
        while (inputFile >> seatNumber >> passengerName) {
            if (seatNumber > 0 && seatNumber <= NUM_SEATS) {
                train.bookTickets(1, {passengerName}, ""); // Book the seat without destination (destination not saved in file)
            }
        }
        inputFile.close();
    } else {
        cout << "Unable to open booking details file. Starting with no bookings." << endl;
    }
}

// Function to write booking details to a file
void writeBookingDetailsToFile(const Train& train) {
    ofstream outputFile("booking_details.txt");
    if (outputFile.is_open()) {
        for (const auto& seat : train.getSeats()) {
            if (!seat.isAvailable()) {
                outputFile << seat.getSeatNumber() << " " << seat.getPassengerName() << endl;
            }
        }
        outputFile.close();
    } else {
        cout << "Unable to open booking details file for writing." << endl;
    }
}

// Display menu options
void displayMenu() {
    cout << "1. View available seats" << endl;
    cout << "2. Book tickets" << endl;
    cout << "3. Cancel a ticket" << endl;
    cout << "4. View booking details" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

// Main function
int main() {
    Train train;

    // Read booking details from file
    readBookingDetailsFromFile(train);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                train.displayAvailableSeats();
                break;
            case 2: {
                int numTickets;
                cout << "Enter the number of tickets to book: ";
                cin >> numTickets;
                if (numTickets <= 0) {
                    cout << "Invalid number of tickets!" << endl;
                    break;
                }
                vector<string> passengerNames(numTickets);
                cin.ignore(); // Clear newline character from input buffer
                for (int i = 0; i < numTickets; ++i) {
                    cout << "Enter passenger name for ticket " << i + 1 << ": ";
                    getline(cin, passengerNames[i]);
                }
                string destination;
                cout << "Enter destination station name: ";
                getline(cin, destination);
                train.bookTickets(numTickets, passengerNames, destination);
                break;
            }
            case 3: {
                int seatNumber;
                cout << "Enter seat number to cancel booking: ";
                cin >> seatNumber;
                train.cancelTicket(seatNumber);
                break;
            }
            case 4:
                train.displayBookingDetails();
                break;
            case 5:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    // Write booking details to file before exiting
    writeBookingDetailsToFile(train);

    return 0;
}
