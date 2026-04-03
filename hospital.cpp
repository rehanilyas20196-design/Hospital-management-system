#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <map>
#include <libpq-fe.h>

using namespace std;

// Database connection class
class Database {
private:
    PGconn* conn;
    
public:
    Database() {
        conn = PQconnectdb("host=localhost port=5432 dbname=hospital_db user=postgres password=postgres");
        
        if (PQstatus(conn) != CONNECTION_OK) {
            cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
            PQfinish(conn);
            conn = nullptr;
        } else {
            cout << "Connected to database successfully!" << endl;
            initializeTables();
        }
    }
    
    void initializeTables() {
        const char* createPatients = 
            "CREATE TABLE IF NOT EXISTS patients ("
            "id SERIAL PRIMARY KEY,"
            "name VARCHAR(100) NOT NULL,"
            "age INTEGER NOT NULL,"
            "gender VARCHAR(10) NOT NULL,"
            "contact VARCHAR(20) NOT NULL,"
            "address TEXT,"
            "emergency BOOLEAN DEFAULT FALSE,"
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ");";
        
        const char* createDoctors = 
            "CREATE TABLE IF NOT EXISTS doctors ("
            "id SERIAL PRIMARY KEY,"
            "name VARCHAR(100) NOT NULL,"
            "specialization VARCHAR(100) NOT NULL,"
            "contact VARCHAR(20) NOT NULL,"
            "available BOOLEAN DEFAULT TRUE"
            ");";
        
        const char* createAppointments = 
            "CREATE TABLE IF NOT EXISTS appointments ("
            "id SERIAL PRIMARY KEY,"
            "patient_id INTEGER REFERENCES patients(id),"
            "doctor_id INTEGER REFERENCES doctors(id),"
            "appointment_date TIMESTAMP NOT NULL,"
            "status VARCHAR(20) DEFAULT 'scheduled'"
            ");";
        
        const char* createPrescriptions = 
            "CREATE TABLE IF NOT EXISTS prescriptions ("
            "id SERIAL PRIMARY KEY,"
            "patient_id INTEGER REFERENCES patients(id),"
            "doctor_id INTEGER REFERENCES doctors(id),"
            "appointment_id INTEGER REFERENCES appointments(id),"
            "medicines TEXT NOT NULL,"
            "diagnosis TEXT,"
            "notes TEXT,"
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
            ");";
        
        executeQuery(createPatients);
        executeQuery(createDoctors);
        executeQuery(createAppointments);
        executeQuery(createPrescriptions);
    }
    
    PGresult* executeQuery(const char* query) {
        PGresult* res = PQexec(conn, query);
        
        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
            cerr << "Query execution failed: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            return nullptr;
        }
        
        return res;
    }
    
    PGconn* getConnection() {
        return conn;
    }
    
    ~Database() {
        if (conn) {
            PQfinish(conn);
        }
    }
};

// Patient class
class Patient {
public:
    int id;
    string name;
    int age;
    string gender;
    string contact;
    string address;
    bool emergency;
    
    Patient() : id(0), age(0), emergency(false) {}
    
    Patient(string n, int a, string g, string c, string addr, bool emerg = false)
        : name(n), age(a), gender(g), contact(c), address(addr), emergency(emerg), id(0) {}
    
    string toJSON() const {
        stringstream ss;
        ss << "{\"id\":" << id 
           << ",\"name\":\"" << name << "\""
           << ",\"age\":" << age
           << ",\"gender\":\"" << gender << "\""
           << ",\"contact\":\"" << contact << "\""
           << ",\"address\":\"" << address << "\""
           << ",\"emergency\":" << (emergency ? "true" : "false")
           << "}";
        return ss.str();
    }
};

// Doctor class
class Doctor {
public:
    int id;
    string name;
    string specialization;
    string contact;
    bool available;
    
    Doctor() : id(0), available(true) {}
    
    Doctor(string n, string spec, string c)
        : name(n), specialization(spec), contact(c), available(true), id(0) {}
    
    string toJSON() const {
        stringstream ss;
        ss << "{\"id\":" << id 
           << ",\"name\":\"" << name << "\""
           << ",\"specialization\":\"" << specialization << "\""
           << ",\"contact\":\"" << contact << "\""
           << ",\"available\":" << (available ? "true" : "false")
           << "}";
        return ss.str();
    }
};

// Appointment class
class Appointment {
public:
    int id;
    int patientId;
    int doctorId;
    string appointmentDate;
    string status;
    
    Appointment() : id(0), patientId(0), doctorId(0), status("scheduled") {}
    
    string toJSON() const {
        stringstream ss;
        ss << "{\"id\":" << id 
           << ",\"patientId\":" << patientId
           << ",\"doctorId\":" << doctorId
           << ",\"appointmentDate\":\"" << appointmentDate << "\""
           << ",\"status\":\"" << status << "\""
           << "}";
        return ss.str();
    }
};

// Prescription class
class Prescription {
public:
    int id;
    int patientId;
    int doctorId;
    int appointmentId;
    string medicines;
    string diagnosis;
    string notes;
    
    Prescription() : id(0), patientId(0), doctorId(0), appointmentId(0) {}
    
    string toJSON() const {
        stringstream ss;
        ss << "{\"id\":" << id 
           << ",\"patientId\":" << patientId
           << ",\"doctorId\":" << doctorId
           << ",\"appointmentId\":" << appointmentId
           << ",\"medicines\":\"" << medicines << "\""
           << ",\"diagnosis\":\"" << diagnosis << "\""
           << ",\"notes\":\"" << notes << "\""
           << "}";
        return ss.str();
    }
};

// Hospital Management System class
class HospitalManagementSystem {
private:
    Database* db;
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Prescription> prescriptions;
    
    void saveToFile() {
        // Save patients to file
        ofstream patientFile("data/patients.txt");
        for (const auto& p : patients) {
            patientFile << p.id << "|" << p.name << "|" << p.age << "|" 
                       << p.gender << "|" << p.contact << "|" << p.address 
                       << "|" << p.emergency << "\n";
        }
        patientFile.close();
        
        // Save doctors to file
        ofstream doctorFile("data/doctors.txt");
        for (const auto& d : doctors) {
            doctorFile << d.id << "|" << d.name << "|" << d.specialization 
                      << "|" << d.contact << "|" << d.available << "\n";
        }
        doctorFile.close();
    }
    
    void loadFromFile() {
        // Load patients from file
        ifstream patientFile("data/patients.txt");
        if (patientFile.is_open()) {
            string line;
            while (getline(patientFile, line)) {
                stringstream ss(line);
                Patient p;
                string temp;
                getline(ss, temp, '|'); p.id = stoi(temp);
                getline(ss, p.name, '|');
                getline(ss, temp, '|'); p.age = stoi(temp);
                getline(ss, p.gender, '|');
                getline(ss, p.contact, '|');
                getline(ss, p.address, '|');
                getline(ss, temp, '|'); p.emergency = (temp == "1");
                patients.push_back(p);
            }
            patientFile.close();
        }
        
        // Load doctors from file
        ifstream doctorFile("data/doctors.txt");
        if (doctorFile.is_open()) {
            string line;
            while (getline(doctorFile, line)) {
                stringstream ss(line);
                Doctor d;
                string temp;
                getline(ss, temp, '|'); d.id = stoi(temp);
                getline(ss, d.name, '|');
                getline(ss, d.specialization, '|');
                getline(ss, d.contact, '|');
                getline(ss, temp, '|'); d.available = (temp == "1");
                doctors.push_back(d);
            }
            doctorFile.close();
        }
    }
    
public:
    HospitalManagementSystem(Database* database) : db(database) {
        // Create data directory if it doesn't exist
        system("mkdir data 2>nul");
        loadFromFile();
    }
    
    // Patient Management
    int addPatient(const Patient& patient) {
        stringstream query;
        query << "INSERT INTO patients (name, age, gender, contact, address, emergency) VALUES ('"
              << patient.name << "', " << patient.age << ", '" << patient.gender << "', '"
              << patient.contact << "', '" << patient.address << "', " 
              << (patient.emergency ? "TRUE" : "FALSE") << ") RETURNING id;";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        int newId = 0;
        
        if (res && PQntuples(res) > 0) {
            newId = atoi(PQgetvalue(res, 0, 0));
            Patient p = patient;
            p.id = newId;
            patients.push_back(p);
            saveToFile();
            PQclear(res);
        }
        
        return newId;
    }
    
    vector<Patient> getAllPatients() {
        patients.clear();
        PGresult* res = db->executeQuery("SELECT id, name, age, gender, contact, address, emergency FROM patients ORDER BY emergency DESC, id DESC;");
        
        if (res) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                Patient p;
                p.id = atoi(PQgetvalue(res, i, 0));
                p.name = PQgetvalue(res, i, 1);
                p.age = atoi(PQgetvalue(res, i, 2));
                p.gender = PQgetvalue(res, i, 3);
                p.contact = PQgetvalue(res, i, 4);
                p.address = PQgetvalue(res, i, 5);
                p.emergency = (string(PQgetvalue(res, i, 6)) == "t");
                patients.push_back(p);
            }
            PQclear(res);
        }
        
        return patients;
    }
    
    Patient* searchPatient(int id) {
        for (auto& p : patients) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }
    
    vector<Patient> searchPatientByName(const string& name) {
        vector<Patient> results;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& p : patients) {
            string lowerPatientName = p.name;
            transform(lowerPatientName.begin(), lowerPatientName.end(), lowerPatientName.begin(), ::tolower);
            
            if (lowerPatientName.find(lowerName) != string::npos) {
                results.push_back(p);
            }
        }
        
        return results;
    }
    
    bool removePatient(int id) {
        stringstream query;
        query << "DELETE FROM patients WHERE id = " << id << ";";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        bool success = (res != nullptr);
        
        if (success) {
            patients.erase(remove_if(patients.begin(), patients.end(),
                [id](const Patient& p) { return p.id == id; }), patients.end());
            saveToFile();
            PQclear(res);
        }
        
        return success;
    }
    
    // Doctor Management
    int addDoctor(const Doctor& doctor) {
        stringstream query;
        query << "INSERT INTO doctors (name, specialization, contact, available) VALUES ('"
              << doctor.name << "', '" << doctor.specialization << "', '"
              << doctor.contact << "', TRUE) RETURNING id;";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        int newId = 0;
        
        if (res && PQntuples(res) > 0) {
            newId = atoi(PQgetvalue(res, 0, 0));
            Doctor d = doctor;
            d.id = newId;
            doctors.push_back(d);
            saveToFile();
            PQclear(res);
        }
        
        return newId;
    }
    
    vector<Doctor> getAllDoctors() {
        doctors.clear();
        PGresult* res = db->executeQuery("SELECT id, name, specialization, contact, available FROM doctors;");
        
        if (res) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                Doctor d;
                d.id = atoi(PQgetvalue(res, i, 0));
                d.name = PQgetvalue(res, i, 1);
                d.specialization = PQgetvalue(res, i, 2);
                d.contact = PQgetvalue(res, i, 3);
                d.available = (string(PQgetvalue(res, i, 4)) == "t");
                doctors.push_back(d);
            }
            PQclear(res);
        }
        
        return doctors;
    }
    
    // Appointment Management
    int bookAppointment(int patientId, int doctorId, const string& date) {
        stringstream query;
        query << "INSERT INTO appointments (patient_id, doctor_id, appointment_date, status) VALUES ("
              << patientId << ", " << doctorId << ", '" << date << "', 'scheduled') RETURNING id;";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        int newId = 0;
        
        if (res && PQntuples(res) > 0) {
            newId = atoi(PQgetvalue(res, 0, 0));
            PQclear(res);
        }
        
        return newId;
    }
    
    vector<Appointment> getAllAppointments() {
        appointments.clear();
        PGresult* res = db->executeQuery("SELECT id, patient_id, doctor_id, appointment_date, status FROM appointments ORDER BY appointment_date DESC;");
        
        if (res) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                Appointment a;
                a.id = atoi(PQgetvalue(res, i, 0));
                a.patientId = atoi(PQgetvalue(res, i, 1));
                a.doctorId = atoi(PQgetvalue(res, i, 2));
                a.appointmentDate = PQgetvalue(res, i, 3);
                a.status = PQgetvalue(res, i, 4);
                appointments.push_back(a);
            }
            PQclear(res);
        }
        
        return appointments;
    }
    
    // Prescription Management
    int addPrescription(int patientId, int doctorId, int appointmentId, 
                       const string& medicines, const string& diagnosis, const string& notes) {
        stringstream query;
        query << "INSERT INTO prescriptions (patient_id, doctor_id, appointment_id, medicines, diagnosis, notes) VALUES ("
              << patientId << ", " << doctorId << ", " << appointmentId << ", '"
              << medicines << "', '" << diagnosis << "', '" << notes << "') RETURNING id;";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        int newId = 0;
        
        if (res && PQntuples(res) > 0) {
            newId = atoi(PQgetvalue(res, 0, 0));
            PQclear(res);
        }
        
        return newId;
    }
    
    vector<Prescription> getPrescriptionsByPatient(int patientId) {
        prescriptions.clear();
        stringstream query;
        query << "SELECT id, patient_id, doctor_id, appointment_id, medicines, diagnosis, notes "
              << "FROM prescriptions WHERE patient_id = " << patientId << " ORDER BY created_at DESC;";
        
        PGresult* res = db->executeQuery(query.str().c_str());
        
        if (res) {
            int rows = PQntuples(res);
            for (int i = 0; i < rows; i++) {
                Prescription p;
                p.id = atoi(PQgetvalue(res, i, 0));
                p.patientId = atoi(PQgetvalue(res, i, 1));
                p.doctorId = atoi(PQgetvalue(res, i, 2));
                p.appointmentId = atoi(PQgetvalue(res, i, 3));
                p.medicines = PQgetvalue(res, i, 4);
                p.diagnosis = PQgetvalue(res, i, 5);
                p.notes = PQgetvalue(res, i, 6);
                prescriptions.push_back(p);
            }
            PQclear(res);
        }
        
        return prescriptions;
    }
    
    // Export data to JSON for frontend
    string exportPatientsJSON() {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < patients.size(); i++) {
            ss << patients[i].toJSON();
            if (i < patients.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }
    
    string exportDoctorsJSON() {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < doctors.size(); i++) {
            ss << doctors[i].toJSON();
            if (i < doctors.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }
    
    string exportAppointmentsJSON() {
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < appointments.size(); i++) {
            ss << appointments[i].toJSON();
            if (i < appointments.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }
    
    string exportPrescriptionsJSON(int patientId) {
        vector<Prescription> presc = getPrescriptionsByPatient(patientId);
        stringstream ss;
        ss << "[";
        for (size_t i = 0; i < presc.size(); i++) {
            ss << presc[i].toJSON();
            if (i < presc.size() - 1) ss << ",";
        }
        ss << "]";
        return ss.str();
    }
};

// HTTP Server simulation (simple file-based API)
void handleAPIRequest(HospitalManagementSystem& hms, const string& endpoint, const string& method, const map<string, string>& params) {
    ofstream responseFile("api_response.json");
    
    if (endpoint == "/patients" && method == "GET") {
        hms.getAllPatients();
        responseFile << hms.exportPatientsJSON();
    }
    else if (endpoint == "/patients" && method == "POST") {
        Patient p(params.at("name"), stoi(params.at("age")), params.at("gender"),
                 params.at("contact"), params.at("address"), 
                 params.count("emergency") && params.at("emergency") == "true");
        int id = hms.addPatient(p);
        responseFile << "{\"success\":true,\"id\":" << id << "}";
    }
    else if (endpoint == "/patients/search" && method == "GET") {
        if (params.count("name")) {
            vector<Patient> results = hms.searchPatientByName(params.at("name"));
            responseFile << "[";
            for (size_t i = 0; i < results.size(); i++) {
                responseFile << results[i].toJSON();
                if (i < results.size() - 1) responseFile << ",";
            }
            responseFile << "]";
        }
    }
    else if (endpoint == "/patients/delete" && method == "POST") {
        bool success = hms.removePatient(stoi(params.at("id")));
        responseFile << "{\"success\":" << (success ? "true" : "false") << "}";
    }
    else if (endpoint == "/doctors" && method == "GET") {
        hms.getAllDoctors();
        responseFile << hms.exportDoctorsJSON();
    }
    else if (endpoint == "/doctors" && method == "POST") {
        Doctor d(params.at("name"), params.at("specialization"), params.at("contact"));
        int id = hms.addDoctor(d);
        responseFile << "{\"success\":true,\"id\":" << id << "}";
    }
    else if (endpoint == "/appointments" && method == "GET") {
        hms.getAllAppointments();
        responseFile << hms.exportAppointmentsJSON();
    }
    else if (endpoint == "/appointments" && method == "POST") {
        int id = hms.bookAppointment(stoi(params.at("patientId")), 
                                     stoi(params.at("doctorId")), 
                                     params.at("date"));
        responseFile << "{\"success\":true,\"id\":" << id << "}";
    }
    else if (endpoint == "/prescriptions" && method == "POST") {
        int id = hms.addPrescription(stoi(params.at("patientId")), 
                                     stoi(params.at("doctorId")),
                                     stoi(params.at("appointmentId")),
                                     params.at("medicines"),
                                     params.at("diagnosis"),
                                     params.at("notes"));
        responseFile << "{\"success\":true,\"id\":" << id << "}";
    }
    else if (endpoint == "/prescriptions" && method == "GET") {
        responseFile << hms.exportPrescriptionsJSON(stoi(params.at("patientId")));
    }
    
    responseFile.close();
}

int main() {
    cout << "=== Hospital Management System ===" << endl;
    
    Database db;
    HospitalManagementSystem hms(&db);
    
    // Add some sample doctors
    Doctor d1("Dr. Sarah Johnson", "Cardiologist", "+1-555-0101");
    Doctor d2("Dr. Michael Chen", "Neurologist", "+1-555-0102");
    Doctor d3("Dr. Emily Williams", "Pediatrician", "+1-555-0103");
    Doctor d4("Dr. James Brown", "Orthopedic", "+1-555-0104");
    
    hms.addDoctor(d1);
    hms.addDoctor(d2);
    hms.addDoctor(d3);
    hms.addDoctor(d4);
    
    cout << "\nBackend server ready!" << endl;
    cout << "Data will be saved to PostgreSQL database and local files." << endl;
    cout << "\nAPI endpoints available:" << endl;
    cout << "- GET /patients - Get all patients" << endl;
    cout << "- POST /patients - Add new patient" << endl;
    cout << "- GET /patients/search - Search patients by name" << endl;
    cout << "- POST /patients/delete - Remove patient" << endl;
    cout << "- GET /doctors - Get all doctors" << endl;
    cout << "- POST /doctors - Add new doctor" << endl;
    cout << "- GET /appointments - Get all appointments" << endl;
    cout << "- POST /appointments - Book appointment" << endl;
    cout << "- POST /prescriptions - Add prescription" << endl;
    cout << "- GET /prescriptions - Get patient prescriptions" << endl;
    
    // Keep the server running
    cout << "\nPress Ctrl+C to stop the server..." << endl;
    
    while (true) {
        // In a real implementation, this would listen for HTTP requests
        // For this demo, we'll use file-based communication
        ifstream requestFile("api_request.txt");
        if (requestFile.is_open()) {
            string endpoint, method;
            map<string, string> params;
            
            getline(requestFile, endpoint);
            getline(requestFile, method);
            
            string line;
            while (getline(requestFile, line)) {
                size_t pos = line.find('=');
                if (pos != string::npos) {
                    params[line.substr(0, pos)] = line.substr(pos + 1);
                }
            }
            requestFile.close();
            
            handleAPIRequest(hms, endpoint, method, params);
            
            // Remove the request file after processing
            remove("api_request.txt");
        }
        
        // Sleep for a short time to avoid busy waiting
        #ifdef _WIN32
            Sleep(100);
        #else
            usleep(100000);
        #endif
    }
    
    return 0;
}
