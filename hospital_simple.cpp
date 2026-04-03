#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <map>
#include <iomanip>

using namespace std;

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
    
    Patient(int i, string n, int a, string g, string c, string addr, bool emerg = false)
        : id(i), name(n), age(a), gender(g), contact(c), address(addr), emergency(emerg) {}
    
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
    
    void display() const {
        cout << "\n┌─────────────────────────────────────────┐\n";
        cout << "│ Patient ID: " << setw(27) << left << id << "│\n";
        cout << "│ Name: " << setw(33) << left << name << "│\n";
        cout << "│ Age: " << setw(34) << left << age << "│\n";
        cout << "│ Gender: " << setw(31) << left << gender << "│\n";
        cout << "│ Contact: " << setw(30) << left << contact << "│\n";
        cout << "│ Address: " << setw(30) << left << address << "│\n";
        cout << "│ Emergency: " << setw(28) << left << (emergency ? "YES" : "NO") << "│\n";
        cout << "└─────────────────────────────────────────┘\n";
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
    
    Doctor(int i, string n, string spec, string c)
        : id(i), name(n), specialization(spec), contact(c), available(true) {}
    
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
    
    void display() const {
        cout << "\n┌─────────────────────────────────────────┐\n";
        cout << "│ Doctor ID: " << setw(28) << left << id << "│\n";
        cout << "│ Name: " << setw(33) << left << name << "│\n";
        cout << "│ Specialization: " << setw(23) << left << specialization << "│\n";
        cout << "│ Contact: " << setw(30) << left << contact << "│\n";
        cout << "│ Available: " << setw(28) << left << (available ? "YES" : "NO") << "│\n";
        cout << "└─────────────────────────────────────────┘\n";
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
    
    Appointment(int i, int pId, int dId, string date)
        : id(i), patientId(pId), doctorId(dId), appointmentDate(date), status("scheduled") {}
    
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
    
    Prescription(int i, int pId, int dId, int aId, string med, string diag, string n)
        : id(i), patientId(pId), doctorId(dId), appointmentId(aId), 
          medicines(med), diagnosis(diag), notes(n) {}
    
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
    
    void display() const {
        cout << "\n╔═════════════════════════════════════════╗\n";
        cout << "║         PRESCRIPTION #" << setw(17) << left << id << "║\n";
        cout << "╠═════════════════════════════════════════╣\n";
        cout << "║ Patient ID: " << setw(27) << left << patientId << "║\n";
        cout << "║ Doctor ID: " << setw(28) << left << doctorId << "║\n";
        cout << "║ Appointment ID: " << setw(23) << left << appointmentId << "║\n";
        cout << "╠═════════════════════════════════════════╣\n";
        cout << "║ DIAGNOSIS:                              ║\n";
        cout << "║ " << setw(39) << left << diagnosis << "║\n";
        cout << "╠═════════════════════════════════════════╣\n";
        cout << "║ MEDICINES:                              ║\n";
        cout << "║ " << setw(39) << left << medicines << "║\n";
        cout << "╠═════════════════════════════════════════╣\n";
        cout << "║ NOTES:                                  ║\n";
        cout << "║ " << setw(39) << left << notes << "║\n";
        cout << "╚═════════════════════════════════════════╝\n";
    }
};

// Hospital Management System class
class HospitalManagementSystem {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Prescription> prescriptions;
    int nextPatientId;
    int nextDoctorId;
    int nextAppointmentId;
    int nextPrescriptionId;
    
    void createDataDirectory() {
        #ifdef _WIN32
            system("if not exist data mkdir data");
        #else
            system("mkdir -p data");
        #endif
    }
    
    void savePatients() {
        ofstream file("data/patients.txt");
        for (const auto& p : patients) {
            file << p.id << "|" << p.name << "|" << p.age << "|" 
                 << p.gender << "|" << p.contact << "|" << p.address 
                 << "|" << p.emergency << "\n";
        }
        file.close();
    }
    
    void loadPatients() {
        ifstream file("data/patients.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
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
                if (p.id >= nextPatientId) nextPatientId = p.id + 1;
            }
            file.close();
        }
    }
    
    void saveDoctors() {
        ofstream file("data/doctors.txt");
        for (const auto& d : doctors) {
            file << d.id << "|" << d.name << "|" << d.specialization 
                 << "|" << d.contact << "|" << d.available << "\n";
        }
        file.close();
    }
    
    void loadDoctors() {
        ifstream file("data/doctors.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                Doctor d;
                string temp;
                getline(ss, temp, '|'); d.id = stoi(temp);
                getline(ss, d.name, '|');
                getline(ss, d.specialization, '|');
                getline(ss, d.contact, '|');
                getline(ss, temp, '|'); d.available = (temp == "1");
                doctors.push_back(d);
                if (d.id >= nextDoctorId) nextDoctorId = d.id + 1;
            }
            file.close();
        }
    }
    
    void saveAppointments() {
        ofstream file("data/appointments.txt");
        for (const auto& a : appointments) {
            file << a.id << "|" << a.patientId << "|" << a.doctorId 
                 << "|" << a.appointmentDate << "|" << a.status << "\n";
        }
        file.close();
    }
    
    void loadAppointments() {
        ifstream file("data/appointments.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                Appointment a;
                string temp;
                getline(ss, temp, '|'); a.id = stoi(temp);
                getline(ss, temp, '|'); a.patientId = stoi(temp);
                getline(ss, temp, '|'); a.doctorId = stoi(temp);
                getline(ss, a.appointmentDate, '|');
                getline(ss, a.status, '|');
                appointments.push_back(a);
                if (a.id >= nextAppointmentId) nextAppointmentId = a.id + 1;
            }
            file.close();
        }
    }
    
    void savePrescriptions() {
        ofstream file("data/prescriptions.txt");
        for (const auto& p : prescriptions) {
            file << p.id << "|" << p.patientId << "|" << p.doctorId 
                 << "|" << p.appointmentId << "|" << p.medicines 
                 << "|" << p.diagnosis << "|" << p.notes << "\n";
        }
        file.close();
    }
    
    void loadPrescriptions() {
        ifstream file("data/prescriptions.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                Prescription p;
                string temp;
                getline(ss, temp, '|'); p.id = stoi(temp);
                getline(ss, temp, '|'); p.patientId = stoi(temp);
                getline(ss, temp, '|'); p.doctorId = stoi(temp);
                getline(ss, temp, '|'); p.appointmentId = stoi(temp);
                getline(ss, p.medicines, '|');
                getline(ss, p.diagnosis, '|');
                getline(ss, p.notes, '|');
                prescriptions.push_back(p);
                if (p.id >= nextPrescriptionId) nextPrescriptionId = p.id + 1;
            }
            file.close();
        }
    }
    
public:
    HospitalManagementSystem() : nextPatientId(1), nextDoctorId(1), 
                                 nextAppointmentId(1), nextPrescriptionId(1) {
        createDataDirectory();
        loadPatients();
        loadDoctors();
        loadAppointments();
        loadPrescriptions();
        
        // Add default doctors if none exist
        if (doctors.empty()) {
            addDoctor("Dr. Sarah Johnson", "Cardiologist", "+1-555-0101");
            addDoctor("Dr. Michael Chen", "Neurologist", "+1-555-0102");
            addDoctor("Dr. Emily Williams", "Pediatrician", "+1-555-0103");
            addDoctor("Dr. James Brown", "Orthopedic", "+1-555-0104");
        }
    }
    
    // Patient Management
    int addPatient(string name, int age, string gender, string contact, 
                   string address, bool emergency = false) {
        Patient p(nextPatientId++, name, age, gender, contact, address, emergency);
        patients.push_back(p);
        savePatients();
        cout << "\n✓ Patient added successfully! ID: " << p.id << endl;
        return p.id;
    }
    
    void displayAllPatients() {
        if (patients.empty()) {
            cout << "\nNo patients found.\n";
            return;
        }
        
        // Sort: Emergency patients first
        vector<Patient> sorted = patients;
        sort(sorted.begin(), sorted.end(), [](const Patient& a, const Patient& b) {
            if (a.emergency && !b.emergency) return true;
            if (!a.emergency && b.emergency) return false;
            return a.id > b.id;
        });
        
        cout << "\n========== ALL PATIENTS ==========\n";
        for (const auto& p : sorted) {
            p.display();
        }
    }
    
    Patient* searchPatient(int id) {
        for (auto& p : patients) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }
    
    void searchPatientByName(string name) {
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        bool found = false;
        
        cout << "\n========== SEARCH RESULTS ==========\n";
        for (const auto& p : patients) {
            string lowerName = p.name;
            transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            
            if (lowerName.find(name) != string::npos) {
                p.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "\nNo patients found with name: " << name << endl;
        }
    }
    
    bool removePatient(int id) {
        auto it = find_if(patients.begin(), patients.end(),
            [id](const Patient& p) { return p.id == id; });
        
        if (it != patients.end()) {
            patients.erase(it);
            savePatients();
            cout << "\n✓ Patient removed successfully!\n";
            return true;
        }
        
        cout << "\n✗ Patient not found!\n";
        return false;
    }
    
    // Doctor Management
    int addDoctor(string name, string specialization, string contact) {
        Doctor d(nextDoctorId++, name, specialization, contact);
        doctors.push_back(d);
        saveDoctors();
        cout << "\n✓ Doctor added successfully! ID: " << d.id << endl;
        return d.id;
    }
    
    void displayAllDoctors() {
        if (doctors.empty()) {
            cout << "\nNo doctors found.\n";
            return;
        }
        
        cout << "\n========== ALL DOCTORS ==========\n";
        for (const auto& d : doctors) {
            d.display();
        }
    }
    
    Doctor* searchDoctor(int id) {
        for (auto& d : doctors) {
            if (d.id == id) return &d;
        }
        return nullptr;
    }
    
    // Appointment Management
    int bookAppointment(int patientId, int doctorId, string date) {
        Patient* patient = searchPatient(patientId);
        Doctor* doctor = searchDoctor(doctorId);
        
        if (!patient) {
            cout << "\n✗ Patient not found!\n";
            return -1;
        }
        
        if (!doctor) {
            cout << "\n✗ Doctor not found!\n";
            return -1;
        }
        
        Appointment a(nextAppointmentId++, patientId, doctorId, date);
        appointments.push_back(a);
        saveAppointments();
        
        cout << "\n✓ Appointment booked successfully! ID: " << a.id << endl;
        cout << "Patient: " << patient->name << endl;
        cout << "Doctor: " << doctor->name << " (" << doctor->specialization << ")" << endl;
        cout << "Date: " << date << endl;
        
        return a.id;
    }
    
    void displayAllAppointments() {
        if (appointments.empty()) {
            cout << "\nNo appointments found.\n";
            return;
        }
        
        cout << "\n========== ALL APPOINTMENTS ==========\n";
        for (const auto& a : appointments) {
            Patient* patient = searchPatient(a.patientId);
            Doctor* doctor = searchDoctor(a.doctorId);
            
            cout << "\n┌─────────────────────────────────────────┐\n";
            cout << "│ Appointment ID: " << setw(23) << left << a.id << "│\n";
            cout << "│ Patient: " << setw(30) << left << (patient ? patient->name : "Unknown") << "│\n";
            cout << "│ Doctor: " << setw(31) << left << (doctor ? doctor->name : "Unknown") << "│\n";
            cout << "│ Specialization: " << setw(23) << left << (doctor ? doctor->specialization : "Unknown") << "│\n";
            cout << "│ Date: " << setw(33) << left << a.appointmentDate << "│\n";
            cout << "│ Status: " << setw(31) << left << a.status << "│\n";
            cout << "└─────────────────────────────────────────┘\n";
        }
    }
    
    // Prescription Management
    int addPrescription(int patientId, int doctorId, int appointmentId,
                       string medicines, string diagnosis, string notes) {
        Patient* patient = searchPatient(patientId);
        Doctor* doctor = searchDoctor(doctorId);
        
        if (!patient) {
            cout << "\n✗ Patient not found!\n";
            return -1;
        }
        
        if (!doctor) {
            cout << "\n✗ Doctor not found!\n";
            return -1;
        }
        
        Prescription p(nextPrescriptionId++, patientId, doctorId, appointmentId,
                      medicines, diagnosis, notes);
        prescriptions.push_back(p);
        savePrescriptions();
        
        cout << "\n✓ Prescription added successfully! ID: " << p.id << endl;
        return p.id;
    }
    
    void viewPrescriptionsByPatient(int patientId) {
        bool found = false;
        
        cout << "\n========== PRESCRIPTIONS FOR PATIENT ID: " << patientId << " ==========\n";
        
        for (const auto& p : prescriptions) {
            if (p.patientId == patientId) {
                Doctor* doctor = searchDoctor(p.doctorId);
                
                cout << "\n╔═════════════════════════════════════════╗\n";
                cout << "║         PRESCRIPTION #" << setw(17) << left << p.id << "║\n";
                cout << "╠═════════════════════════════════════════╣\n";
                cout << "║ Doctor: " << setw(31) << left << (doctor ? doctor->name : "Unknown") << "║\n";
                cout << "║ Specialization: " << setw(23) << left << (doctor ? doctor->specialization : "Unknown") << "║\n";
                cout << "╠═════════════════════════════════════════╣\n";
                cout << "║ DIAGNOSIS:                              ║\n";
                
                // Word wrap for diagnosis
                stringstream diagStream(p.diagnosis);
                string word;
                string line = "║ ";
                while (diagStream >> word) {
                    if (line.length() + word.length() > 38) {
                        cout << setw(41) << left << line << "║\n";
                        line = "║ " + word + " ";
                    } else {
                        line += word + " ";
                    }
                }
                if (line.length() > 2) {
                    cout << setw(41) << left << line << "║\n";
                }
                
                cout << "╠═════════════════════════════════════════╣\n";
                cout << "║ MEDICINES:                              ║\n";
                
                // Word wrap for medicines
                stringstream medStream(p.medicines);
                line = "║ ";
                while (medStream >> word) {
                    if (line.length() + word.length() > 38) {
                        cout << setw(41) << left << line << "║\n";
                        line = "║ " + word + " ";
                    } else {
                        line += word + " ";
                    }
                }
                if (line.length() > 2) {
                    cout << setw(41) << left << line << "║\n";
                }
                
                if (!p.notes.empty()) {
                    cout << "╠═════════════════════════════════════════╣\n";
                    cout << "║ NOTES:                                  ║\n";
                    
                    // Word wrap for notes
                    stringstream noteStream(p.notes);
                    line = "║ ";
                    while (noteStream >> word) {
                        if (line.length() + word.length() > 38) {
                            cout << setw(41) << left << line << "║\n";
                            line = "║ " + word + " ";
                        } else {
                            line += word + " ";
                        }
                    }
                    if (line.length() > 2) {
                        cout << setw(41) << left << line << "║\n";
                    }
                }
                
                cout << "╚═════════════════════════════════════════╝\n";
                found = true;
            }
        }
        
        if (!found) {
            cout << "\nNo prescriptions found for this patient.\n";
        }
    }
};

// Main Menu
void displayMenu() {
    cout << "\n╔═══════════════════════════════════════════╗\n";
    cout << "║   🏥 HOSPITAL MANAGEMENT SYSTEM 🏥       ║\n";
    cout << "╠═══════════════════════════════════════════╣\n";
    cout << "║  1. Add Patient                           ║\n";
    cout << "║  2. View All Patients                     ║\n";
    cout << "║  3. Search Patient                        ║\n";
    cout << "║  4. Remove Patient                        ║\n";
    cout << "║  ─────────────────────────────────────    ║\n";
    cout << "║  5. Add Doctor                            ║\n";
    cout << "║  6. View All Doctors                      ║\n";
    cout << "║  ─────────────────────────────────────    ║\n";
    cout << "║  7. Book Appointment                      ║\n";
    cout << "║  8. View All Appointments                 ║\n";
    cout << "║  ─────────────────────────────────────    ║\n";
    cout << "║  9. Add Prescription                      ║\n";
    cout << "║  10. View Patient Prescriptions           ║\n";
    cout << "║  ─────────────────────────────────────    ║\n";
    cout << "║  0. Exit                                  ║\n";
    cout << "╚═══════════════════════════════════════════╝\n";
    cout << "\nEnter your choice: ";
}

int main() {
    HospitalManagementSystem hms;
    int choice;
    
    cout << "\n╔═══════════════════════════════════════════╗\n";
    cout << "║   Welcome to Hospital Management System   ║\n";
    cout << "╚═══════════════════════════════════════════╝\n";
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline
        
        switch (choice) {
            case 1: {
                string name, gender, contact, address;
                int age;
                char emergencyChoice;
                
                cout << "\n--- Add New Patient ---\n";
                cout << "Name: ";
                getline(cin, name);
                cout << "Age: ";
                cin >> age;
                cin.ignore();
                cout << "Gender (Male/Female/Other): ";
                getline(cin, gender);
                cout << "Contact: ";
                getline(cin, contact);
                cout << "Address: ";
                getline(cin, address);
                cout << "Emergency patient? (y/n): ";
                cin >> emergencyChoice;
                cin.ignore();
                
                hms.addPatient(name, age, gender, contact, address, 
                              (emergencyChoice == 'y' || emergencyChoice == 'Y'));
                break;
            }
            
            case 2:
                hms.displayAllPatients();
                break;
            
            case 3: {
                string searchName;
                cout << "\nEnter patient name to search: ";
                getline(cin, searchName);
                hms.searchPatientByName(searchName);
                break;
            }
            
            case 4: {
                int id;
                cout << "\nEnter patient ID to remove: ";
                cin >> id;
                cin.ignore();
                hms.removePatient(id);
                break;
            }
            
            case 5: {
                string name, specialization, contact;
                cout << "\n--- Add New Doctor ---\n";
                cout << "Name: ";
                getline(cin, name);
                cout << "Specialization: ";
                getline(cin, specialization);
                cout << "Contact: ";
                getline(cin, contact);
                hms.addDoctor(name, specialization, contact);
                break;
            }
            
            case 6:
                hms.displayAllDoctors();
                break;
            
            case 7: {
                int patientId, doctorId;
                string date;
                cout << "\n--- Book Appointment ---\n";
                cout << "Patient ID: ";
                cin >> patientId;
                cin.ignore();
                cout << "Doctor ID: ";
                cin >> doctorId;
                cin.ignore();
                cout << "Date & Time (e.g., 2024-03-27 10:00): ";
                getline(cin, date);
                hms.bookAppointment(patientId, doctorId, date);
                break;
            }
            
            case 8:
                hms.displayAllAppointments();
                break;
            
            case 9: {
                int patientId, doctorId, appointmentId;
                string medicines, diagnosis, notes;
                
                cout << "\n--- Add Prescription ---\n";
                cout << "Patient ID: ";
                cin >> patientId;
                cin.ignore();
                cout << "Doctor ID: ";
                cin >> doctorId;
                cin.ignore();
                cout << "Appointment ID: ";
                cin >> appointmentId;
                cin.ignore();
                cout << "Diagnosis: ";
                getline(cin, diagnosis);
                cout << "Medicines (comma separated): ";
                getline(cin, medicines);
                cout << "Additional Notes: ";
                getline(cin, notes);
                
                hms.addPrescription(patientId, doctorId, appointmentId, 
                                   medicines, diagnosis, notes);
                break;
            }
            
            case 10: {
                int patientId;
                cout << "\nEnter patient ID: ";
                cin >> patientId;
                cin.ignore();
                hms.viewPrescriptionsByPatient(patientId);
                break;
            }
            
            case 0:
                cout << "\n✓ Thank you for using Hospital Management System!\n";
                cout << "✓ All data has been saved.\n\n";
                return 0;
            
            default:
                cout << "\n✗ Invalid choice! Please try again.\n";
        }
        
        cout << "\nPress Enter to continue...";
        cin.get();
    }
    
    return 0;
}
