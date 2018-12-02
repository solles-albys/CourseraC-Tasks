// Реализуйте функции и методы классов и при необходимости добавьте свои
#import <iostream>
#import <string>
#import <sstream>
#import <set>
#import <map>
#import <iomanip>

using namespace std;

class Date {
public:
    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

    void SetDay(const int& day) {
       this->day = day;
    }

    void SetMonth(const int& month) {
       this->month = month;
    }

    void SetYear(const int& year) {
       this->year = year;
    }
private:
   int day = 0, month = 0, year = 0;
};

bool correctDelimiter(istream& in, const char& delimiter = 0) {
    return in.peek() == delimiter;
}

istream& operator>>(istream& in, Date& date) {
    string input;
    int day, month, year;
    stringstream ss;
    in >> input;
    auto err = invalid_argument("Wrong date format: " + input);
    ss = stringstream(input);

    ss >> year;
    if (ss.peek() != '-') {
        throw err;
    }
    ss.ignore(1);
    ss >> input;
    if (!isdigit(input[0]) && !isdigit(input[1])) {
        throw err;
    }
    ss = stringstream(input);
    ss >> month;
    if (ss.peek() != '-') {
        throw err;
    }
    ss.ignore(1);
    ss >> input;
    if (!isdigit(input[0]) && !isdigit(input[1])) {
        throw err;
    }
    ss = stringstream(input);
    ss >> day;
    if (ss.peek() != ' ' && ss.peek() != -1) {
        throw err;
    }

    if (month < 1 || month > 12) {
        throw invalid_argument("Month value is invalid: " + to_string(month));
    }
    if (day < 1 || day > 31) {
        throw invalid_argument("Day value is invalid: " + to_string(day));
    }

    date.SetYear(year);
    date.SetMonth(month);
    date.SetDay(day);

    return in;
}

ostream& operator<<(ostream& out, const Date& date) {
    int year = date.GetYear();
    out << setw(4) << setfill('0') << abs(year);
    out << '-' << setw(2) << setfill('0') << date.GetMonth();
    out << '-' << setw(2) << setfill('0') << date.GetDay();
}

bool operator<(const Date &lhs, const Date &rhs) {
    int yl = lhs.GetYear(), yr = rhs.GetYear();
    if (yl != yr) {
        return yl < yr;
    }
    yl = lhs.GetMonth();
    yr = rhs.GetMonth();
    if (yl != yr) {
        return yl < yr;
    }

    return lhs.GetDay() < rhs.GetDay();
};


class Database {
public:
    void AddEvent(const Date &date, const string &event) {
        db[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (db.count(date) == 0) {
            return false;
        }
        if (db[date].count(event) == 0) {
            return false;
        }
        db[date].erase(event);
        return true;
    }

    int DeleteDate(const Date &date) {
        if (db.count(date) == 0) {
            return false;
        }
        int size = db.at(date).size();
        db.erase(date);
        return size;
    }

    set<string> Find(const Date &date) const {
        if (db.count(date) == 0) {
            return set<string>();
        }
        return db.at(date);
    }

    void Print() const {
        for (const auto& rec : db) {
            for (const auto& event : rec.second) {
                cout << rec.first << " " << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> db;
};

void handle(stringstream& stream, Database& db) {
    string command;
    Date date;
    string event;
    stream >> command;
    if (command == "Add") {
        stream >> date;
        stream >> event;
        db.AddEvent(date, event);
    } else if (command == "Del") {
        stream >> date;
        if (stream.peek() == -1) {
            int count_deleted = db.DeleteDate(date);
            cout << "Deleted " << count_deleted << " events\n";
        } else {
            stream >> event;
            bool success = db.DeleteEvent(date, event);
            if (success) {
                cout << "Deleted successfully\n";
            } else {
                cout << "Event not found\n";
            }
        }
    } else if (command == "Find") {
        stream >> date;
        auto container = db.Find(date);
        for (const auto& item : container) {
            cout << item << endl;
        }
    } else if (command == "Print") {
        db.Print();
    } else if (command.empty()) {
        return;
    } else {
        throw invalid_argument("Unknown command: " + command);
    }
}

int main() {
    Database db;

    string command;
    try {
        while (getline(cin, command)) {
            stringstream ss(command);
            handle(ss, db);
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
    return 0;
}