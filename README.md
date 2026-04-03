 🏥 MediCare Hospital Management System

A comprehensive, full-featured hospital management system with C++ backend, PostgreSQL database support, and modern responsive HTML/CSS/JavaScript frontend with authentication.

## ✨ Features

### 🔐 Authentication System
- ✅ User login with email/username
- ✅ User registration (Patient/Doctor/Admin)
- ✅ Session management
- ✅ Remember me functionality
- ✅ Secure logout
- ✅ Beautiful gradient UI with hospital logo

### 👥 Patient Management
- ✅ Add new patients with complete details
- ✅ Search patients by name or contact (real-time)
- ✅ Remove patients from the system
- ✅ Emergency patient marking and prioritization
- ✅ View patient details and history
- ✅ Emergency patients displayed with red badge

### 👨‍⚕️ Doctor Management
- ✅ Add doctors with specialization
- ✅ View all available doctors
- ✅ Track doctor availability
- ✅ Book appointments with favorite doctors
- ✅ Pre-loaded specialist doctors

### 📅 Appointment System
- ✅ Book appointments with specific doctors
- ✅ View all scheduled appointments
- ✅ Track appointment status
- ✅ Link appointments to prescriptions
- ✅ Date and time selection

### 💊 Prescription System
- ✅ Doctors can write prescriptions online
- ✅ Add detailed diagnosis
- ✅ Enter multiple medicines
- ✅ View patient prescription history
- ✅ Link prescriptions to appointments
- ✅ Add additional notes
- ✅ Patients can view their medicines

### 🚑 Ambulance Services (NEW!)
- ✅ 24/7 Emergency ambulance request
- ✅ Emergency hotline display
- ✅ Request form with emergency type
- ✅ Track active ambulance requests
- ✅ Real-time status updates
- ✅ Complete/Cancel requests

### 🎨 UI/UX Features
- ✅ Responsive design (mobile, tablet, desktop)
- ✅ Modern gradient theme
- ✅ Hospital logo and branding
- ✅ Sticky header with emergency button
- ✅ Comprehensive footer with contact info
- ✅ Smooth animations and transitions
- ✅ Success/Error message notifications

## Technology Stack

### Backend
- **Language**: C++
- **Database**: PostgreSQL (with file-based fallback)
- **Libraries Used**:
  - `<vector>` - Dynamic arrays for data management
  - `<fstream>` - File handling for data persistence
  - `<string>` - String manipulation
  - `<algorithm>` - Data operations
  - `<map>` - Key-value storage
  - `<libpq-fe.h>` - PostgreSQL C API (optional)

### Frontend
- **HTML5** - Structure
- **CSS3** - Modern styling with gradients and animations
- **JavaScript (ES6+)** - Dynamic functionality and API communication

## 📁 File Structure

```
medicare-hospital-system/
│
├── hospital.cpp          # C++ backend with PostgreSQL integration
├── hospital_simple.cpp   # Simplified C++ backend (no PostgreSQL required)
├── hospital_simple.exe   # Compiled executable (Windows)
│
├── login.html           # Login page with authentication
├── signup.html          # User registration page
├── index.html           # Main dashboard (requires login)
│
├── auth.css             # Authentication pages styling
├── auth.js              # Authentication logic
├── styles.css           # Main application styling
├── script.js            # Main application logic
│
├── compile.bat          # Windows compilation script
├── compile.sh           # Linux/Mac compilation script
│
├── README.md            # Complete documentation
├── QUICKSTART.md        # Quick start guide
│
└── data/                # Auto-created directory for file storage
    ├── patients.txt
    ├── doctors.txt
    ├── appointments.txt
    └── prescriptions.txt
```

## 🚀 Setup Instructions

### Option 1: Quick Start (Recommended)

1. **Compile the simplified C++ backend**:
   
   **Windows:**
   ```cmd
   compile.bat
   ```
   Or manually:
   ```cmd
   g++ hospital_simple.cpp -o hospital_simple.exe -std=c++11
   ```

   **Linux/Mac:**
   ```bash
   chmod +x compile.sh
   ./compile.sh
   ```
   Or manually:
   ```bash
   g++ hospital_simple.cpp -o hospital_simple -std=c++11
   ```

2. **Run the backend**:
   
   **Windows:**
   ```cmd
   hospital_simple.exe
   ```
   
   **Linux/Mac:**
   ```bash
   ./hospital_simple
   ```

3. **Open the web interface**:
   - Open [`login.html`](login.html) in your web browser
   - Create a new account or use existing credentials
   - The system uses localStorage for data persistence

### Option 2: Full Setup with PostgreSQL

1. **Install PostgreSQL**:
   - Download from: https://www.postgresql.org/download/
   - Install and note your username/password

2. **Create Database**:
   ```sql
   CREATE DATABASE hospital_db;
   ```

3. **Install PostgreSQL Development Libraries**:
   
   **On Ubuntu/Debian**:
   ```bash
   sudo apt-get install libpq-dev
   ```
   
   **On macOS**:
   ```bash
   brew install postgresql
   ```
   
   **On Windows**:
   - Download PostgreSQL installer which includes development files
   - Add PostgreSQL bin directory to PATH

4. **Update Database Connection**:
   Edit `hospital.cpp` line 16 to match your PostgreSQL credentials:
   ```cpp
   conn = PQconnectdb("host=localhost port=5432 dbname=hospital_db user=YOUR_USER password=YOUR_PASSWORD");
   ```

5. **Compile with PostgreSQL**:
   
   **On Linux/macOS**:
   ```bash
   g++ hospital.cpp -o hospital -lpq -std=c++11
   ```
   
   **On Windows (MinGW)**:
   ```cmd
   g++ hospital.cpp -o hospital.exe -I"C:\Program Files\PostgreSQL\15\include" -L"C:\Program Files\PostgreSQL\15\lib" -lpq
   ```

6. **Run the backend**:
   ```bash
   ./hospital
   ```

7. **Open the frontend**:
   - Open `index.html` in your web browser

## Usage Guide

### Adding a Patient

1. Click on "Patients" tab
2. Click "+ Add Patient" button
3. Fill in patient details:
   - Name
   - Age
   - Gender
   - Contact number
   - Address
   - Check "Emergency Patient" if urgent
4. Click "Save Patient"

### Searching Patients

- Use the search box in the Patients section
- Type patient name or contact number
- Results update in real-time

### Booking an Appointment

1. Go to "Appointments" tab
2. Click "+ Book Appointment"
3. Select patient from dropdown
4. Select doctor from dropdown
5. Choose date and time
6. Click "Book Appointment"

### Adding a Prescription

1. Go to "Prescriptions" tab
2. Click "+ Add Prescription"
3. Select patient
4. Select appointment
5. Select doctor
6. Enter diagnosis
7. Enter medicines (one per line)
8. Add any additional notes
9. Click "Save Prescription"

### Viewing Prescriptions

1. Go to "Prescriptions" tab
2. Select a patient from the dropdown
3. All prescriptions for that patient will be displayed

## Features Demonstration

### Emergency Patient Handling
- Emergency patients are marked with a red badge (🚨 EMERGENCY)
- They appear at the top of the patient list
- Cards have a distinct red border and background

### Doctor Specializations
- Cardiologist
- Neurologist
- Pediatrician
- Orthopedic
- And more can be added

### Data Persistence
- All data is saved automatically
- File-based backup in `data/` directory
- PostgreSQL database for production use
- localStorage for frontend demo mode

## API Endpoints (Backend)

The C++ backend provides these endpoints:

- `GET /patients` - Get all patients
- `POST /patients` - Add new patient
- `GET /patients/search` - Search patients
- `POST /patients/delete` - Remove patient
- `GET /doctors` - Get all doctors
- `POST /doctors` - Add new doctor
- `GET /appointments` - Get all appointments
- `POST /appointments` - Book appointment
- `POST /prescriptions` - Add prescription
- `GET /prescriptions` - Get patient prescriptions

## Data Models

### Patient
```cpp
{
    int id;
    string name;
    int age;
    string gender;
    string contact;
    string address;
    bool emergency;
}
```

### Doctor
```cpp
{
    int id;
    string name;
    string specialization;
    string contact;
    bool available;
}
```

### Appointment
```cpp
{
    int id;
    int patientId;
    int doctorId;
    string appointmentDate;
    string status;
}
```

### Prescription
```cpp
{
    int id;
    int patientId;
    int doctorId;
    int appointmentId;
    string medicines;
    string diagnosis;
    string notes;
}
```

## Browser Compatibility

- ✅ Chrome/Edge (Recommended)
- ✅ Firefox
- ✅ Safari
- ✅ Opera

## Troubleshooting

### PostgreSQL Connection Issues
- Verify PostgreSQL is running: `sudo service postgresql status`
- Check credentials in `hospital.cpp`
- Ensure database `hospital_db` exists

### Compilation Errors
- Install required libraries: `libpq-dev`
- Use C++11 or later: `-std=c++11`
- Check include paths for PostgreSQL headers

### Frontend Not Loading Data
- Check browser console for errors (F12)
- Ensure backend is running
- Clear browser cache and localStorage

## Future Enhancements

- [ ] User authentication and roles
- [ ] Email notifications for appointments
- [ ] Medical reports generation (PDF)
- [ ] Payment and billing system
- [ ] Medicine inventory management
- [ ] Patient medical history timeline
- [ ] Video consultation integration
- [ ] Mobile app version

## License

This project is created for educational purposes.

## Support

For issues or questions, please check the code comments or create an issue in the repository.

---

**Made with ❤️ for better healthcare management**


