-- ============================================================
-- MediCare Hospital Management System - Full Database Schema
-- Paste ALL of the following into Neon SQL Editor and run.
-- Project ID: calm-wind-53947071
-- ============================================================


-- ============================================================
-- 1. USERS TABLE (Authentication)
--    Stores login accounts for doctors, patients, admin
-- ============================================================
CREATE TABLE IF NOT EXISTS users (
    id          SERIAL PRIMARY KEY,
    firstname   VARCHAR(100)        NOT NULL,
    lastname    VARCHAR(100)        NOT NULL,
    email       VARCHAR(255) UNIQUE NOT NULL,
    phone       VARCHAR(20),
    password_hash VARCHAR(255)      NOT NULL,
    user_type   VARCHAR(50)         NOT NULL DEFAULT 'patient',
    created_at  TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Index for fast login lookups
CREATE INDEX IF NOT EXISTS idx_users_email ON users(email);


-- ============================================================
-- 2. PATIENTS TABLE
--    Core patient records - name, age, gender, contact, etc.
-- ============================================================
CREATE TABLE IF NOT EXISTS patients (
    id          SERIAL PRIMARY KEY,
    name        VARCHAR(150)  NOT NULL,
    age         INTEGER       NOT NULL CHECK (age >= 0 AND age <= 150),
    gender      VARCHAR(20)   NOT NULL,
    contact     VARCHAR(30)   NOT NULL,
    address     TEXT          NOT NULL,
    is_emergency BOOLEAN      NOT NULL DEFAULT FALSE,
    created_at  TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Index for searching patients by name or contact
CREATE INDEX IF NOT EXISTS idx_patients_name    ON patients(name);
CREATE INDEX IF NOT EXISTS idx_patients_contact ON patients(contact);
-- Emergency patients can be filtered quickly
CREATE INDEX IF NOT EXISTS idx_patients_emergency ON patients(is_emergency);


-- ============================================================
-- 3. DOCTORS TABLE
--    Doctor profiles - name, specialization, availability
-- ============================================================
CREATE TABLE IF NOT EXISTS doctors (
    id              SERIAL PRIMARY KEY,
    name            VARCHAR(150) NOT NULL,
    specialization  VARCHAR(100) NOT NULL,
    contact         VARCHAR(30)  NOT NULL,
    is_available    BOOLEAN      NOT NULL DEFAULT TRUE,
    created_at      TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Index for looking up doctors by specialization
CREATE INDEX IF NOT EXISTS idx_doctors_specialization ON doctors(specialization);


-- ============================================================
-- 4. APPOINTMENTS TABLE
--    Links patients to doctors with date/time and status
-- ============================================================
CREATE TABLE IF NOT EXISTS appointments (
    id                  SERIAL PRIMARY KEY,
    patient_id          INTEGER NOT NULL REFERENCES patients(id) ON DELETE CASCADE,
    doctor_id           INTEGER NOT NULL REFERENCES doctors(id)  ON DELETE CASCADE,
    appointment_date    TIMESTAMP WITH TIME ZONE NOT NULL,
    status              VARCHAR(30) NOT NULL DEFAULT 'scheduled',
                        -- Values: 'scheduled', 'completed', 'cancelled'
    created_at          TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Indexes for common joins and filters
CREATE INDEX IF NOT EXISTS idx_appointments_patient ON appointments(patient_id);
CREATE INDEX IF NOT EXISTS idx_appointments_doctor  ON appointments(doctor_id);
CREATE INDEX IF NOT EXISTS idx_appointments_status  ON appointments(status);
CREATE INDEX IF NOT EXISTS idx_appointments_date    ON appointments(appointment_date);


-- ============================================================
-- 5. PRESCRIPTIONS TABLE
--    Medical prescriptions linked to appointment records
-- ============================================================
CREATE TABLE IF NOT EXISTS prescriptions (
    id              SERIAL PRIMARY KEY,
    patient_id      INTEGER NOT NULL REFERENCES patients(id)      ON DELETE CASCADE,
    doctor_id       INTEGER NOT NULL REFERENCES doctors(id)       ON DELETE CASCADE,
    appointment_id  INTEGER          REFERENCES appointments(id)  ON DELETE SET NULL,
    diagnosis       TEXT    NOT NULL,
    medicines       TEXT    NOT NULL,
    notes           TEXT,
    created_at      TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Indexes for prescription lookups by patient and doctor
CREATE INDEX IF NOT EXISTS idx_prescriptions_patient     ON prescriptions(patient_id);
CREATE INDEX IF NOT EXISTS idx_prescriptions_doctor      ON prescriptions(doctor_id);
CREATE INDEX IF NOT EXISTS idx_prescriptions_appointment ON prescriptions(appointment_id);


-- ============================================================
-- 6. AMBULANCE REQUESTS TABLE
--    Tracks emergency ambulance dispatch requests
-- ============================================================
CREATE TABLE IF NOT EXISTS ambulance_requests (
    id              SERIAL PRIMARY KEY,
    patient_name    VARCHAR(150) NOT NULL,
    contact         VARCHAR(30)  NOT NULL,
    location        TEXT         NOT NULL,
    emergency_type  VARCHAR(50)  NOT NULL,
                    -- Values: 'cardiac','accident','stroke','breathing','trauma','other'
    details         TEXT,
    status          VARCHAR(30)  NOT NULL DEFAULT 'active',
                    -- Values: 'active', 'completed', 'cancelled'
    request_time    TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Index for filtering active requests
CREATE INDEX IF NOT EXISTS idx_ambulance_status ON ambulance_requests(status);


-- ============================================================
-- SAMPLE DATA (Optional - good for testing)
-- ============================================================

-- Insert 4 default doctors (matches the app's default doctor list)
INSERT INTO doctors (name, specialization, contact, is_available)
VALUES
    ('Dr. Sarah Johnson',  'Cardiologist',  '+1-555-0101', TRUE),
    ('Dr. Michael Chen',   'Neurologist',   '+1-555-0102', TRUE),
    ('Dr. Emily Williams', 'Pediatrician',  '+1-555-0103', TRUE),
    ('Dr. James Brown',    'Orthopedic',    '+1-555-0104', TRUE)
ON CONFLICT DO NOTHING;


-- ============================================================
-- USEFUL QUERY EXAMPLES (for reference / testing)
-- ============================================================

-- View all patients (emergency first)
-- SELECT * FROM patients ORDER BY is_emergency DESC, created_at DESC;

-- View all upcoming appointments with patient and doctor names
-- SELECT
--     a.id,
--     p.name AS patient_name,
--     d.name AS doctor_name,
--     d.specialization,
--     a.appointment_date,
--     a.status
-- FROM appointments a
-- JOIN patients p ON a.patient_id = p.id
-- JOIN doctors  d ON a.doctor_id  = d.id
-- WHERE a.status = 'scheduled'
-- ORDER BY a.appointment_date ASC;

-- View all prescriptions for a specific patient (replace 1 with actual patient id)
-- SELECT
--     pr.id,
--     p.name  AS patient_name,
--     d.name  AS doctor_name,
--     pr.diagnosis,
--     pr.medicines,
--     pr.notes,
--     pr.created_at
-- FROM prescriptions pr
-- JOIN patients p ON pr.patient_id = p.id
-- JOIN doctors  d ON pr.doctor_id  = d.id
-- WHERE pr.patient_id = 1
-- ORDER BY pr.created_at DESC;

-- View all active ambulance requests
-- SELECT * FROM ambulance_requests WHERE status = 'active' ORDER BY request_time DESC;
