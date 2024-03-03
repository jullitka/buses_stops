#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string query_type_str;
    is >> query_type_str;
    
    if (query_type_str == "NEW_BUS") {
        q.type = QueryType::NewBus;
        is >> q.bus;
        int stop_count;
        is >> stop_count;
        q.stops.resize(stop_count);
        for (string& stop : q.stops) {
            is >> stop;
        }
    } else if (query_type_str == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (query_type_str == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (query_type_str == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }

    return is;
}

struct BusesForStopResponse {
    // Наполните полями эту структуру
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    // Реализуйте эту функцию
    if (r.buses.size() == 0) {
        os << "No stop"s;
    } else {
        bool is_first = true;
        for (const string& bus : r.buses) {
            if (!is_first) {
                os << " "s;
            }
            is_first = false;
            os << bus;
        }
    }
    return os;
}

struct StopsForBusResponse {
    // Наполните полями эту структуру
     vector<pair<string, vector<string>>> stops;
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    // Реализуйте эту функцию
    if (r.stops.size() == 0) {
        os << "No bus"s;
    } else {
        bool is_first = true;
        for (auto& stop : r.stops) {
            if (!is_first) {
                os << endl;
            }
            is_first = false;
            os << "Stop " << stop.first << ":"s;
            if (stop.second.size() == 0) {
                os << " no interchange"s;
            } else {
                for (const string& bus : stop.second) {
                    os << " "s << bus;
                }
            }
        }
    }
    return os;
}

struct AllBusesResponse {
    // Наполните полями эту структуру
    map<string, vector<string>> buses;
    
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    // Реализуйте эту функцию
    if (r.buses.empty()) {
        os << "No buses"s;
    } else {
        bool is_first = true;
        for (auto& bus : r.buses) {
            if (!is_first) {
                os << endl;
            }
            is_first = false;
            os << "Bus "s << bus.first << ":"s;
            for (const string& stop : bus.second) {
                os << " "s << stop;
            }
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        // Реализуйте этот метод
        buses_to_stops[bus] = stops;
        for (const string& stop : stops) {
            stops_to_buses[stop].push_back(bus);
        }
        
        
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        // Реализуйте этот метод
        BusesForStopResponse response;
        if (stops_to_buses.count(stop)) {
            response.buses = stops_to_buses.at(stop);
            return response;
        } else {
            return response;
        }
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        // Реализуйте этот метод
        StopsForBusResponse response;
        if (buses_to_stops.count(bus)) {
            for (const string& stop : buses_to_stops.at(bus)) {
                pair<string, vector<string>> stop_to_buses;
                
                stop_to_buses.first = stop;
                  
                if (stops_to_buses.at(stop).size() > 1) {
                    for (const string& other_bus : stops_to_buses.at(stop)) {
                        if (bus != other_bus) {
                            stop_to_buses.second.push_back(other_bus);
                        }
                    }
                }
                response.stops.push_back(stop_to_buses);
            }
        }
        return response;
    }

    AllBusesResponse GetAllBuses() const {
        // Реализуйте этот метод
        AllBusesResponse response;
        response.buses = buses_to_stops;
        return response;
    }
private: 
    map<string, vector<string>> buses_to_stops;
    map<string, vector<string>> stops_to_buses;
};

// Реализуйте функции и классы, объявленные выше, чтобы эта функция main
// решала задачу "Автобусные остановки"

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
            case QueryType::NewBus:
                bm.AddBus(q.bus, q.stops);
                break;
            case QueryType::BusesForStop:
                cout << bm.GetBusesForStop(q.stop) << endl;
                break;
            case QueryType::StopsForBus:
                cout << bm.GetStopsForBus(q.bus) << endl;
                break;
            case QueryType::AllBuses:
                cout << bm.GetAllBuses() << endl;
                break;
        }
    }
}
