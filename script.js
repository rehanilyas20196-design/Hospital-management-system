// Global data storage
let patientsData = [];
let doctorsData = [];
let appointmentsData = [];
let prescriptionsData = [];
let ambulanceRequests = [];

// Initialize the application
document.addEventListener('DOMContentLoaded', function() {
    checkAuthentication();
    loadAllData();
    loadAmbulanceRequests();
});

// Authentication Check
function checkAuthentication() {
    const session = localStorage.getItem('hospital_session') || sessionStorage.getItem('hospital_session');
    
    if (!session) {
        window.location.href = 'login.html';
        return;
    }
    
    const userData = JSON.parse(session);
    document.getElementById('user-name').textContent = userData.name || userData.username;
}

function logout() {
    if (confirm('Are you sure you want to logout?')) {
        localStorage.removeItem('hospital_session');
        sessionStorage.removeItem('hospital_session');
        window.location.href = 'login.html';
    }
}

// Section Navigation
function showSection(sectionName) {
    // Hide all sections
    document.querySelectorAll('.section').forEach(section => {
        section.classList.remove('active');
    });
    
    // Remove active class from all nav buttons
    document.querySelectorAll('.nav-btn').forEach(btn => {
        btn.classList.remove('active');
    });
    
    // Show selected section
    document.getElementById(sectionName + '-section').classList.add('active');
    
    // Add active class to clicked button
    event.target.classList.add('active');
    
    // Load data for the section
    if (sectionName === 'patients') {
        loadPatients();
    } else if (sectionName === 'doctors') {
        loadDoctors();
    } else if (sectionName === 'appointments') {
        loadAppointments();
    } else if (sectionName === 'prescriptions') {
        loadPrescriptionsSection();
    }
}

// Load all data on startup
function loadAllData() {
    loadPatients();
    loadDoctors();
    loadAppointments();
}

// API Communication Functions
async function apiRequest(endpoint, method = 'GET', data = null) {
    try {
        // Simulate API call with localStorage for demo
        // In production, this would be actual HTTP requests to C++ backend
        
        if (method === 'GET') {
            return getFromStorage(endpoint);
        } else if (method === 'POST') {
            return saveToStorage(endpoint, data);
        } else if (method === 'DELETE') {
            return deleteFromStorage(endpoint, data);
        }
    } catch (error) {
        console.error('API Error:', error);
        showMessage('Error communicating with server', 'error');
        return null;
    }
}

function getFromStorage(endpoint) {
    const data = localStorage.getItem(endpoint);
    return data ? JSON.parse(data) : [];
}

function saveToStorage(endpoint, data) {
    let existing = getFromStorage(endpoint);
    
    if (endpoint.includes('delete')) {
        // Handle delete
        const id = data.id;
        const key = endpoint.split('/')[1];
        existing = existing.filter(item => item.id !== id);
        localStorage.setItem('/' + key, JSON.stringify(existing));
        return { success: true };
    } else {
        // Handle add
        data.id = Date.now(); // Generate unique ID
        existing.push(data);
        localStorage.setItem(endpoint, JSON.stringify(existing));
        return { success: true, id: data.id };
    }
}

function deleteFromStorage(endpoint, data) {
    return saveToStorage(endpoint, data);
}

// Patient Management Functions
async function loadPatients() {
    const patientsList = document.getElementById('patients-list');
    patientsList.innerHTML = '<p class="loading">Loading patients...</p>';
    
    patientsData = await apiRequest('/patients', 'GET');
    
    if (!patientsData || patientsData.length === 0) {
        patientsList.innerHTML = '<p class="info">No patients found. Add your first patient!</p>';
        return;
    }
    
    // Sort: Emergency patients first
    patientsData.sort((a, b) => {
        if (a.emergency && !b.emergency) return -1;
        if (!a.emergency && b.emergency) return 1;
        return b.id - a.id;
    });
    
    patientsList.innerHTML = patientsData.map(patient => `
        <div class="card ${patient.emergency ? 'emergency' : ''}">
            <div class="card-header">
                <div>
                    <div class="card-title">${patient.name}</div>
                    ${patient.emergency ? '<span class="emergency-badge">🚨 EMERGENCY</span>' : ''}
                </div>
            </div>
            <div class="card-body">
                <div class="card-info">
                    <div class="info-item">
                        <span class="info-label">Age</span>
                        <span class="info-value">${patient.age} years</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Gender</span>
                        <span class="info-value">${patient.gender}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Contact</span>
                        <span class="info-value">${patient.contact}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Address</span>
                        <span class="info-value">${patient.address}</span>
                    </div>
                </div>
            </div>
            <div class="card-actions">
                <button class="btn btn-info" onclick="viewPatientDetails(${patient.id})">View Details</button>
                <button class="btn btn-danger" onclick="deletePatient(${patient.id})">Remove</button>
            </div>
        </div>
    `).join('');
}

function showAddPatientForm() {
    document.getElementById('add-patient-form').style.display = 'block';
}

function hideAddPatientForm() {
    document.getElementById('add-patient-form').style.display = 'none';
    document.getElementById('add-patient-form').querySelector('form').reset();
}

async function addPatient(event) {
    event.preventDefault();
    
    const patient = {
        name: document.getElementById('patient-name').value,
        age: parseInt(document.getElementById('patient-age').value),
        gender: document.getElementById('patient-gender').value,
        contact: document.getElementById('patient-contact').value,
        address: document.getElementById('patient-address').value,
        emergency: document.getElementById('patient-emergency').checked
    };
    
    const result = await apiRequest('/patients', 'POST', patient);
    
    if (result && result.success) {
        showMessage('Patient added successfully!', 'success');
        hideAddPatientForm();
        loadPatients();
    } else {
        showMessage('Failed to add patient', 'error');
    }
}

function searchPatients() {
    const searchTerm = document.getElementById('search-patient').value.toLowerCase();
    
    if (!searchTerm) {
        loadPatients();
        return;
    }
    
    const filtered = patientsData.filter(patient => 
        patient.name.toLowerCase().includes(searchTerm) ||
        patient.contact.includes(searchTerm)
    );
    
    const patientsList = document.getElementById('patients-list');
    
    if (filtered.length === 0) {
        patientsList.innerHTML = '<p class="info">No patients found matching your search.</p>';
        return;
    }
    
    patientsList.innerHTML = filtered.map(patient => `
        <div class="card ${patient.emergency ? 'emergency' : ''}">
            <div class="card-header">
                <div>
                    <div class="card-title">${patient.name}</div>
                    ${patient.emergency ? '<span class="emergency-badge">🚨 EMERGENCY</span>' : ''}
                </div>
            </div>
            <div class="card-body">
                <div class="card-info">
                    <div class="info-item">
                        <span class="info-label">Age</span>
                        <span class="info-value">${patient.age} years</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Gender</span>
                        <span class="info-value">${patient.gender}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Contact</span>
                        <span class="info-value">${patient.contact}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Address</span>
                        <span class="info-value">${patient.address}</span>
                    </div>
                </div>
            </div>
            <div class="card-actions">
                <button class="btn btn-info" onclick="viewPatientDetails(${patient.id})">View Details</button>
                <button class="btn btn-danger" onclick="deletePatient(${patient.id})">Remove</button>
            </div>
        </div>
    `).join('');
}

async function deletePatient(id) {
    if (!confirm('Are you sure you want to remove this patient?')) {
        return;
    }
    
    const result = await apiRequest('/patients/delete', 'POST', { id });
    
    if (result && result.success) {
        showMessage('Patient removed successfully!', 'success');
        loadPatients();
    } else {
        showMessage('Failed to remove patient', 'error');
    }
}

function viewPatientDetails(id) {
    const patient = patientsData.find(p => p.id === id);
    if (patient) {
        alert(`Patient Details:\n\nName: ${patient.name}\nAge: ${patient.age}\nGender: ${patient.gender}\nContact: ${patient.contact}\nAddress: ${patient.address}\nEmergency: ${patient.emergency ? 'Yes' : 'No'}`);
    }
}

// Doctor Management Functions
async function loadDoctors() {
    const doctorsList = document.getElementById('doctors-list');
    doctorsList.innerHTML = '<p class="loading">Loading doctors...</p>';
    
    doctorsData = await apiRequest('/doctors', 'GET');
    
    if (!doctorsData || doctorsData.length === 0) {
        // Add default doctors if none exist
        const defaultDoctors = [
            { name: 'Dr. Sarah Johnson', specialization: 'Cardiologist', contact: '+1-555-0101', available: true },
            { name: 'Dr. Michael Chen', specialization: 'Neurologist', contact: '+1-555-0102', available: true },
            { name: 'Dr. Emily Williams', specialization: 'Pediatrician', contact: '+1-555-0103', available: true },
            { name: 'Dr. James Brown', specialization: 'Orthopedic', contact: '+1-555-0104', available: true }
        ];
        
        for (const doctor of defaultDoctors) {
            await apiRequest('/doctors', 'POST', doctor);
        }
        
        doctorsData = await apiRequest('/doctors', 'GET');
    }
    
    doctorsList.innerHTML = doctorsData.map(doctor => `
        <div class="card doctor-card">
            <div class="card-header">
                <div>
                    <div class="card-title">${doctor.name}</div>
                    <div class="doctor-specialization">${doctor.specialization}</div>
                </div>
            </div>
            <div class="card-body">
                <div class="card-info">
                    <div class="info-item">
                        <span class="info-label">Contact</span>
                        <span class="info-value">${doctor.contact}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Status</span>
                        <span class="info-value">${doctor.available ? '✅ Available' : '❌ Unavailable'}</span>
                    </div>
                </div>
            </div>
            <div class="card-actions">
                <button class="btn btn-primary" onclick="bookWithDoctor(${doctor.id})">Book Appointment</button>
            </div>
        </div>
    `).join('');
    
    // Update doctor dropdowns
    updateDoctorDropdowns();
}

function showAddDoctorForm() {
    document.getElementById('add-doctor-form').style.display = 'block';
}

function hideAddDoctorForm() {
    document.getElementById('add-doctor-form').style.display = 'none';
    document.getElementById('add-doctor-form').querySelector('form').reset();
}

async function addDoctor(event) {
    event.preventDefault();
    
    const doctor = {
        name: document.getElementById('doctor-name').value,
        specialization: document.getElementById('doctor-specialization').value,
        contact: document.getElementById('doctor-contact').value,
        available: true
    };
    
    const result = await apiRequest('/doctors', 'POST', doctor);
    
    if (result && result.success) {
        showMessage('Doctor added successfully!', 'success');
        hideAddDoctorForm();
        loadDoctors();
    } else {
        showMessage('Failed to add doctor', 'error');
    }
}

function bookWithDoctor(doctorId) {
    showSection('appointments');
    document.querySelector('.nav-btn:nth-child(3)').click();
    showBookAppointmentForm();
    document.getElementById('appointment-doctor').value = doctorId;
}

// Appointment Management Functions
async function loadAppointments() {
    const appointmentsList = document.getElementById('appointments-list');
    appointmentsList.innerHTML = '<p class="loading">Loading appointments...</p>';
    
    appointmentsData = await apiRequest('/appointments', 'GET');
    
    if (!appointmentsData || appointmentsData.length === 0) {
        appointmentsList.innerHTML = '<p class="info">No appointments scheduled. Book your first appointment!</p>';
        return;
    }
    
    appointmentsList.innerHTML = appointmentsData.map(appointment => {
        const patient = patientsData.find(p => p.id === appointment.patientId);
        const doctor = doctorsData.find(d => d.id === appointment.doctorId);
        
        return `
            <div class="card appointment-card">
                <div class="card-header">
                    <div>
                        <div class="card-title">Appointment #${appointment.id}</div>
                        <span class="status-badge status-${appointment.status}">${appointment.status.toUpperCase()}</span>
                    </div>
                </div>
                <div class="card-body">
                    <div class="appointment-datetime">📅 ${formatDateTime(appointment.appointmentDate)}</div>
                    <div class="card-info">
                        <div class="info-item">
                            <span class="info-label">Patient</span>
                            <span class="info-value">${patient ? patient.name : 'Unknown'}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Doctor</span>
                            <span class="info-value">${doctor ? doctor.name : 'Unknown'}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Specialization</span>
                            <span class="info-value">${doctor ? doctor.specialization : 'Unknown'}</span>
                        </div>
                    </div>
                </div>
                <div class="card-actions">
                    <button class="btn btn-info" onclick="addPrescriptionForAppointment(${appointment.id})">Add Prescription</button>
                </div>
            </div>
        `;
    }).join('');
    
    updateAppointmentDropdowns();
}

function showBookAppointmentForm() {
    document.getElementById('book-appointment-form').style.display = 'block';
    updatePatientDropdowns();
    updateDoctorDropdowns();
}

function hideBookAppointmentForm() {
    document.getElementById('book-appointment-form').style.display = 'none';
    document.getElementById('book-appointment-form').querySelector('form').reset();
}

async function bookAppointment(event) {
    event.preventDefault();
    
    const appointment = {
        patientId: parseInt(document.getElementById('appointment-patient').value),
        doctorId: parseInt(document.getElementById('appointment-doctor').value),
        appointmentDate: document.getElementById('appointment-date').value,
        status: 'scheduled'
    };
    
    const result = await apiRequest('/appointments', 'POST', appointment);
    
    if (result && result.success) {
        showMessage('Appointment booked successfully!', 'success');
        hideBookAppointmentForm();
        loadAppointments();
    } else {
        showMessage('Failed to book appointment', 'error');
    }
}

function addPrescriptionForAppointment(appointmentId) {
    showSection('prescriptions');
    document.querySelector('.nav-btn:nth-child(4)').click();
    showAddPrescriptionForm();
    
    const appointment = appointmentsData.find(a => a.id === appointmentId);
    if (appointment) {
        document.getElementById('prescription-patient').value = appointment.patientId;
        document.getElementById('prescription-doctor').value = appointment.doctorId;
        loadPatientAppointments();
        setTimeout(() => {
            document.getElementById('prescription-appointment').value = appointmentId;
        }, 100);
    }
}

// Prescription Management Functions
async function loadPrescriptionsSection() {
    updatePatientDropdowns();
    document.getElementById('prescriptions-list').innerHTML = '<p class="info">Select a patient to view their prescriptions</p>';
}

function showAddPrescriptionForm() {
    document.getElementById('add-prescription-form').style.display = 'block';
    updatePatientDropdowns();
    updateDoctorDropdowns();
}

function hideAddPrescriptionForm() {
    document.getElementById('add-prescription-form').style.display = 'none';
    document.getElementById('add-prescription-form').querySelector('form').reset();
}

async function addPrescription(event) {
    event.preventDefault();
    
    const prescription = {
        patientId: parseInt(document.getElementById('prescription-patient').value),
        doctorId: parseInt(document.getElementById('prescription-doctor').value),
        appointmentId: parseInt(document.getElementById('prescription-appointment').value),
        diagnosis: document.getElementById('prescription-diagnosis').value,
        medicines: document.getElementById('prescription-medicines').value,
        notes: document.getElementById('prescription-notes').value
    };
    
    const result = await apiRequest('/prescriptions', 'POST', prescription);
    
    if (result && result.success) {
        showMessage('Prescription added successfully!', 'success');
        hideAddPrescriptionForm();
        viewPrescriptions();
    } else {
        showMessage('Failed to add prescription', 'error');
    }
}

async function viewPrescriptions() {
    const patientId = parseInt(document.getElementById('view-prescription-patient').value);
    
    if (!patientId) {
        document.getElementById('prescriptions-list').innerHTML = '<p class="info">Select a patient to view their prescriptions</p>';
        return;
    }
    
    const prescriptionsList = document.getElementById('prescriptions-list');
    prescriptionsList.innerHTML = '<p class="loading">Loading prescriptions...</p>';
    
    prescriptionsData = await apiRequest('/prescriptions', 'GET');
    const patientPrescriptions = prescriptionsData.filter(p => p.patientId === patientId);
    
    if (!patientPrescriptions || patientPrescriptions.length === 0) {
        prescriptionsList.innerHTML = '<p class="info">No prescriptions found for this patient.</p>';
        return;
    }
    
    prescriptionsList.innerHTML = patientPrescriptions.map(prescription => {
        const patient = patientsData.find(p => p.id === prescription.patientId);
        const doctor = doctorsData.find(d => d.id === prescription.doctorId);
        
        return `
            <div class="card prescription-card">
                <div class="card-header">
                    <div class="card-title">Prescription #${prescription.id}</div>
                </div>
                <div class="card-body">
                    <div class="card-info">
                        <div class="info-item">
                            <span class="info-label">Patient</span>
                            <span class="info-value">${patient ? patient.name : 'Unknown'}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Doctor</span>
                            <span class="info-value">${doctor ? doctor.name : 'Unknown'}</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Specialization</span>
                            <span class="info-value">${doctor ? doctor.specialization : 'Unknown'}</span>
                        </div>
                    </div>
                    <div class="prescription-content">
                        <div class="prescription-section">
                            <h4>📋 Diagnosis</h4>
                            <p>${prescription.diagnosis}</p>
                        </div>
                        <div class="prescription-section">
                            <h4>💊 Medicines</h4>
                            <div class="medicines-list">
                                <p>${prescription.medicines}</p>
                            </div>
                        </div>
                        ${prescription.notes ? `
                            <div class="prescription-section">
                                <h4>📝 Additional Notes</h4>
                                <p>${prescription.notes}</p>
                            </div>
                        ` : ''}
                    </div>
                </div>
            </div>
        `;
    }).join('');
}

function loadPatientAppointments() {
    const patientId = parseInt(document.getElementById('prescription-patient').value);
    const appointmentSelect = document.getElementById('prescription-appointment');
    
    appointmentSelect.innerHTML = '<option value="">Select Appointment</option>';
    
    if (!patientId) return;
    
    const patientAppointments = appointmentsData.filter(a => a.patientId === patientId);
    
    patientAppointments.forEach(appointment => {
        const option = document.createElement('option');
        option.value = appointment.id;
        option.textContent = `Appointment on ${formatDateTime(appointment.appointmentDate)}`;
        appointmentSelect.appendChild(option);
    });
}

// Helper Functions
function updatePatientDropdowns() {
    const selects = [
        document.getElementById('appointment-patient'),
        document.getElementById('prescription-patient'),
        document.getElementById('view-prescription-patient')
    ];
    
    selects.forEach(select => {
        if (select) {
            const currentValue = select.value;
            select.innerHTML = '<option value="">Select Patient</option>';
            
            patientsData.forEach(patient => {
                const option = document.createElement('option');
                option.value = patient.id;
                option.textContent = `${patient.name} (${patient.age}y, ${patient.gender})`;
                select.appendChild(option);
            });
            
            if (currentValue) select.value = currentValue;
        }
    });
}

function updateDoctorDropdowns() {
    const selects = [
        document.getElementById('appointment-doctor'),
        document.getElementById('prescription-doctor')
    ];
    
    selects.forEach(select => {
        if (select) {
            const currentValue = select.value;
            select.innerHTML = '<option value="">Select Doctor</option>';
            
            doctorsData.forEach(doctor => {
                const option = document.createElement('option');
                option.value = doctor.id;
                option.textContent = `${doctor.name} - ${doctor.specialization}`;
                select.appendChild(option);
            });
            
            if (currentValue) select.value = currentValue;
        }
    });
}

function updateAppointmentDropdowns() {
    const select = document.getElementById('prescription-appointment');
    
    if (select) {
        select.innerHTML = '<option value="">Select Appointment</option>';
        
        appointmentsData.forEach(appointment => {
            const patient = patientsData.find(p => p.id === appointment.patientId);
            const option = document.createElement('option');
            option.value = appointment.id;
            option.textContent = `${patient ? patient.name : 'Unknown'} - ${formatDateTime(appointment.appointmentDate)}`;
            select.appendChild(option);
        });
    }
}

function formatDateTime(dateString) {
    const date = new Date(dateString);
    return date.toLocaleString('en-US', {
        year: 'numeric',
        month: 'long',
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit'
    });
}

function showMessage(message, type) {
    const messageDiv = document.createElement('div');
    messageDiv.className = type === 'success' ? 'success-message' : 'error-message';
    messageDiv.textContent = message;
    
    const section = document.querySelector('.section.active');
    section.insertBefore(messageDiv, section.firstChild);
    
    setTimeout(() => {
        messageDiv.remove();
    }, 3000);
}

// Ambulance Functions
function callAmbulance() {
    showSection('ambulance');
    document.querySelectorAll('.nav-btn').forEach(btn => btn.classList.remove('active'));
    document.querySelectorAll('.nav-btn')[4].classList.add('active');
    requestEmergencyAmbulance();
}

function requestEmergencyAmbulance() {
    document.getElementById('ambulance-request-form').style.display = 'block';
}

function hideAmbulanceForm() {
    document.getElementById('ambulance-request-form').style.display = 'none';
    document.getElementById('ambulance-request-form').querySelector('form').reset();
}

async function submitAmbulanceRequest(event) {
    event.preventDefault();
    
    const request = {
        id: Date.now(),
        patientName: document.getElementById('ambulance-patient-name').value,
        contact: document.getElementById('ambulance-contact').value,
        location: document.getElementById('ambulance-location').value,
        emergencyType: document.getElementById('ambulance-emergency-type').value,
        details: document.getElementById('ambulance-details').value,
        status: 'active',
        requestTime: new Date().toISOString()
    };
    
    ambulanceRequests.push(request);
    localStorage.setItem('/ambulance-requests', JSON.stringify(ambulanceRequests));
    
    showMessage('Ambulance request submitted! Help is on the way!', 'success');
    hideAmbulanceForm();
    loadAmbulanceRequests();
    
    // Simulate ambulance dispatch
    setTimeout(() => {
        alert('🚑 Ambulance dispatched!\n\nEstimated arrival time: 10-15 minutes\nAmbulance ID: AMB-' + request.id.toString().slice(-4) + '\n\nPlease stay calm and keep the patient comfortable.');
    }, 1000);
}

async function loadAmbulanceRequests() {
    ambulanceRequests = await apiRequest('/ambulance-requests', 'GET') || [];
    
    const requestsList = document.getElementById('ambulance-requests-list');
    
    if (!requestsList) return;
    
    if (ambulanceRequests.length === 0) {
        requestsList.innerHTML = '<p class="info">No active ambulance requests</p>';
        return;
    }
    
    requestsList.innerHTML = ambulanceRequests.map(request => `
        <div class="card ambulance-request-card">
            <div class="card-header">
                <div>
                    <div class="card-title">${request.patientName}</div>
                    <span class="request-status ${request.status}">${request.status.toUpperCase()}</span>
                </div>
            </div>
            <div class="card-body">
                <div class="card-info">
                    <div class="info-item">
                        <span class="info-label">Contact</span>
                        <span class="info-value">${request.contact}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Emergency Type</span>
                        <span class="info-value">${request.emergencyType}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Location</span>
                        <span class="info-value">${request.location}</span>
                    </div>
                    <div class="info-item">
                        <span class="info-label">Request Time</span>
                        <span class="info-value">${formatDateTime(request.requestTime)}</span>
                    </div>
                </div>
                ${request.details ? `
                    <div style="margin-top: 15px;">
                        <strong>Details:</strong>
                        <p style="color: #374151; margin-top: 5px;">${request.details}</p>
                    </div>
                ` : ''}
            </div>
            <div class="card-actions">
                <button class="btn btn-success" onclick="completeAmbulanceRequest(${request.id})">Mark as Completed</button>
                <button class="btn btn-danger" onclick="cancelAmbulanceRequest(${request.id})">Cancel Request</button>
            </div>
        </div>
    `).join('');
}

function completeAmbulanceRequest(id) {
    const request = ambulanceRequests.find(r => r.id === id);
    if (request) {
        request.status = 'completed';
        localStorage.setItem('/ambulance-requests', JSON.stringify(ambulanceRequests));
        showMessage('Ambulance request marked as completed', 'success');
        loadAmbulanceRequests();
    }
}

function cancelAmbulanceRequest(id) {
    if (confirm('Are you sure you want to cancel this ambulance request?')) {
        ambulanceRequests = ambulanceRequests.filter(r => r.id !== id);
        localStorage.setItem('/ambulance-requests', JSON.stringify(ambulanceRequests));
        showMessage('Ambulance request cancelled', 'success');
        loadAmbulanceRequests();
    }
}
