# Quick Start Guide - Hospital Management System

## 🚀 Get Started in 3 Steps

### Step 1: Compile the Backend

**On Windows:**
```cmd
compile.bat
```
Or manually:
```cmd
g++ hospital_simple.cpp -o hospital_simple.exe -std=c++11
```

**On Linux/Mac:**
```bash
chmod +x compile.sh
./compile.sh
```
Or manually:
```bash
g++ hospital_simple.cpp -o hospital_simple -std=c++11
```

### Step 2: Run the Backend

**On Windows:**
```cmd
hospital_simple.exe
```

**On Linux/Mac:**
```bash
./hospital_simple
```

You'll see a menu like this:
```
╔═══════════════════════════════════════════╗
║   🏥 HOSPITAL MANAGEMENT SYSTEM 🏥       ║
╠═══════════════════════════════════════════╣
║  1. Add Patient                           ║
║  2. View All Patients                     ║
║  3. Search Patient                        ║
║  4. Remove Patient                        ║
║  ─────────────────────────────────────    ║
║  5. Add Doctor                            ║
║  6. View All Doctors                      ║
║  ─────────────────────────────────────    ║
║  7. Book Appointment                      ║
║  8. View All Appointments                 ║
║  ─────────────────────────────────────    ║
║  9. Add Prescription                      ║
║  10. View Patient Prescriptions           ║
║  ─────────────────────────────────────    ║
║  0. Exit                                  ║
╚═══════════════════════════════════════════╝
```

### Step 3: Open the Web Interface

Simply open [`index.html`](index.html) in your web browser!

**Recommended browsers:**
- Google Chrome
- Microsoft Edge
- Firefox
- Safari

---

## 📋 Sample Usage Workflow

### 1. Add a Patient
- Choose option `1` from the menu
- Enter patient details:
  - Name: John Doe
  - Age: 35
  - Gender: Male
  - Contact: +1-555-1234
  - Address: 123 Main St
  - Emergency: n

### 2. View All Doctors
- Choose option `6` to see available doctors
- Note the Doctor ID you want to book

### 3. Book an Appointment
- Choose option `7`
- Enter Patient ID (from step 1)
- Enter Doctor ID (from step 2)
- Enter Date & Time: 2024-03-27 10:00

### 4. Add a Prescription
- Choose option `9`
- Enter Patient ID
- Enter Doctor ID
- Enter Appointment ID
- Enter Diagnosis: Common cold
- Enter Medicines: Paracetamol 500mg, Vitamin C
- Enter Notes: Rest for 3 days

### 5. View Patient Prescriptions
- Choose option `10`
- Enter Patient ID
- See all prescriptions for that patient

---

## 🌐 Using the Web Interface

### Adding a Patient via Web
1. Open `index.html` in your browser
2. Click on "Patients" tab
3. Click "+ Add Patient" button
4. Fill in the form
5. Check "Emergency Patient" if needed
6. Click "Save Patient"

### Searching Patients
1. Go to "Patients" tab
2. Type in the search box
3. Results appear instantly

### Booking an Appointment
1. Go to "Appointments" tab
2. Click "+ Book Appointment"
3. Select patient from dropdown
4. Select doctor from dropdown
5. Choose date and time
6. Click "Book Appointment"

### Adding a Prescription (Doctor's View)
1. Go to "Prescriptions" tab
2. Click "+ Add Prescription"
3. Select patient
4. Select appointment
5. Select doctor
6. Enter diagnosis
7. Enter medicines (one per line)
8. Add notes if needed
9. Click "Save Prescription"

### Viewing Prescriptions (Patient's View)
1. Go to "Prescriptions" tab
2. Select patient from dropdown
3. All prescriptions will be displayed with:
   - Doctor name and specialization
   - Diagnosis
   - Medicines prescribed
   - Additional notes

---

## 💾 Data Storage

All data is automatically saved in the `data/` directory:
- `data/patients.txt` - Patient records
- `data/doctors.txt` - Doctor records
- `data/appointments.txt` - Appointment records
- `data/prescriptions.txt` - Prescription records

The web interface uses browser localStorage for demo purposes.

---

## 🎨 Features Highlights

### Emergency Patient Priority
- Emergency patients are marked with 🚨
- They appear at the top of the list
- Red border and background for visibility

### Doctor Specializations
Pre-loaded doctors include:
- Dr. Sarah Johnson - Cardiologist
- Dr. Michael Chen - Neurologist
- Dr. Emily Williams - Pediatrician
- Dr. James Brown - Orthopedic

### Search Functionality
- Real-time search in patients
- Search by name or contact number
- Case-insensitive matching

### Prescription System
- Link prescriptions to appointments
- Track which doctor prescribed what
- View complete prescription history
- Add detailed diagnosis and notes

---

## 🔧 Troubleshooting

### Compilation Error: "g++ not found"
**Solution:** Install MinGW (Windows) or GCC (Linux/Mac)
- Windows: Download from https://sourceforge.net/projects/mingw/
- Linux: `sudo apt-get install g++`
- Mac: `xcode-select --install`

### Web Interface Not Saving Data
**Solution:** The web interface uses localStorage. Make sure:
- You're not in private/incognito mode
- Browser allows localStorage
- You're opening the file from the same location

### Data Directory Not Created
**Solution:** Create it manually:
```bash
mkdir data
```

---

## 📞 Support

For issues or questions:
1. Check the main [README.md](README.md)
2. Review the code comments
3. Ensure all files are in the same directory

---

## 🎯 Next Steps

After getting familiar with the system:
1. Add more patients and doctors
2. Book multiple appointments
3. Create prescriptions for patients
4. Explore the search functionality
5. Try the emergency patient feature

---

**Enjoy using the Hospital Management System! 🏥**
