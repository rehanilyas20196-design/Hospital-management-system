require('dotenv').config();
const express = require('express');
const cors = require('cors');
const { createClient } = require('@supabase/supabase-js');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');

const app = express();
const path = require('path');

app.use(cors());
app.use(express.json());

// Serve static frontend files (HTML, CSS, JS) from the current directory
app.use(express.static(__dirname));

// Default route to serve login.html if they just visit the root
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'login.html'));
});

// Initialize Supabase Client
const supabaseUrl = process.env.SUPABASE_URL;
// Use service role key to bypass RLS on the server, fallback to anon key
const supabaseKey = process.env.SUPABASE_SERVICE_ROLE_KEY || process.env.SUPABASE_ANON_KEY;
const supabase = createClient(supabaseUrl, supabaseKey);

const JWT_SECRET = process.env.JWT_SECRET || 'fallback_secret';

// Auth middleware to protect routes
const authenticateToken = (req, res, next) => {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  if (!token) return res.status(401).json({ message: 'Authentication required' });

  jwt.verify(token, JWT_SECRET, (err, user) => {
    if (err) return res.status(403).json({ message: 'Invalid or expired token' });
    req.user = user;
    next();
  });
};

// Signup Route (with Password Hashing)
app.post('/api/signup', async (req, res) => {
  try {
    const { firstname, lastname, email, phone, password, userType } = req.body;

    // Check if user already exists
    const { data: existingUsers, error: checkError } = await supabase
      .from('users')
      .select('id')
      .eq('email', email);

    if (checkError) throw checkError;

    if (existingUsers && existingUsers.length > 0) {
      return res.status(400).json({ message: 'Email already registered!' });
    }

    // Hash password
    const saltRounds = 10;
    const passwordHash = await bcrypt.hash(password, saltRounds);

    // Insert new user
    const { data: newUsers, error: insertError } = await supabase
      .from('users')
      .insert([
        {
          firstname,
          lastname,
          email,
          phone,
          password_hash: passwordHash,
          user_type: userType || 'patient'
        }
      ])
      .select('id, firstname, lastname, email, user_type');

    if (insertError) throw insertError;

    res.status(201).json({ message: 'Account created successfully!', user: newUsers[0] });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: 'Server error during signup' });
  }
});

// Login Route (with Password Verification and JWT Generation)
app.post('/api/login', async (req, res) => {
  try {
    const { username, password } = req.body; // frontend sends 'username' which is the email

    // Find user
    const { data: users, error: selectError } = await supabase
      .from('users')
      .select('*')
      .eq('email', username);

    if (selectError) throw selectError;

    if (!users || users.length === 0) {
      return res.status(401).json({ message: 'Invalid email or password!' });
    }

    const user = users[0];

    // Verify password
    const isValidPassword = await bcrypt.compare(password, user.password_hash);
    if (!isValidPassword) {
      return res.status(401).json({ message: 'Invalid email or password!' });
    }

    // Generate JWT token (Authorization)
    const token = jwt.sign(
      { 
        userId: user.id, 
        email: user.email, 
        userType: user.user_type,
        name: `${user.firstname} ${user.lastname}`
      },
      JWT_SECRET,
      { expiresIn: '24h' }
    );

    res.json({ 
      message: 'Login successful!', 
      token,
      user: {
        id: user.id,
        email: user.email,
        firstname: user.firstname,
        lastname: user.lastname,
        userType: user.user_type,
        name: `${user.firstname} ${user.lastname}`
      }
    });
  } catch (err) {
    console.error(err);
    res.status(500).json({ message: 'Server error during login' });
  }
});

// Example of a Protected Route (Requires Authorization token)
app.get('/api/profile', authenticateToken, async (req, res) => {
  try {
    const { data: users, error } = await supabase
      .from('users')
      .select('id, firstname, lastname, email, phone, user_type')
      .eq('id', req.user.userId);

    if (error) throw error;
    
    if (!users || users.length === 0) {
       return res.status(404).json({ message: 'User not found' });
    }
    
    res.json(users[0]);
  } catch (err) {
    res.status(500).json({ message: 'Server error' });
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server running on port ${PORT}`);
});
