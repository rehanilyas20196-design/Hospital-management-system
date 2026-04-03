// Authentication JavaScript

function showMessage(message, type) {
    const existingMessage = document.querySelector('.message');
    if (existingMessage) {
        existingMessage.remove();
    }

    const messageDiv = document.createElement('div');
    messageDiv.className = `message message-${type}`;
    messageDiv.innerHTML = `
        <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            ${type === 'success' 
                ? '<polyline points="20 6 9 17 4 12"></polyline>' 
                : '<circle cx="12" cy="12" r="10"></circle><line x1="12" y1="8" x2="12" y2="12"></line><line x1="12" y1="16" x2="12.01" y2="16"></line>'}
        </svg>
        <span>${message}</span>
    `;

    const form = document.querySelector('.auth-form');
    form.insertBefore(messageDiv, form.firstChild);

    setTimeout(() => {
        messageDiv.remove();
    }, 5000);
}

function handleLogin(event) {
    event.preventDefault();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    const remember = document.getElementById('remember').checked;

    // Get stored users
    const users = JSON.parse(localStorage.getItem('hospital_users') || '[]');

    // Find user
    const user = users.find(u => 
        (u.email === username || u.username === username) && u.password === password
    );

    if (user) {
        // Store session
        const session = {
            userId: user.id,
            username: user.username || user.email,
            userType: user.userType,
            name: `${user.firstname} ${user.lastname}`,
            loginTime: new Date().toISOString()
        };

        if (remember) {
            localStorage.setItem('hospital_session', JSON.stringify(session));
        } else {
            sessionStorage.setItem('hospital_session', JSON.stringify(session));
        }

        showMessage('Login successful! Redirecting...', 'success');

        setTimeout(() => {
            window.location.href = 'index.html';
        }, 1500);
    } else {
        showMessage('Invalid username or password!', 'error');
    }
}

function handleSignup(event) {
    event.preventDefault();

    const firstname = document.getElementById('firstname').value;
    const lastname = document.getElementById('lastname').value;
    const email = document.getElementById('email').value;
    const phone = document.getElementById('phone').value;
    const password = document.getElementById('signup-password').value;
    const confirmPassword = document.getElementById('confirm-password').value;
    const userType = document.getElementById('user-type').value;
    const terms = document.getElementById('terms').checked;

    // Validation
    if (password !== confirmPassword) {
        showMessage('Passwords do not match!', 'error');
        return;
    }

    if (password.length < 6) {
        showMessage('Password must be at least 6 characters long!', 'error');
        return;
    }

    if (!terms) {
        showMessage('Please accept the terms and conditions!', 'error');
        return;
    }

    // Get existing users
    const users = JSON.parse(localStorage.getItem('hospital_users') || '[]');

    // Check if email already exists
    if (users.some(u => u.email === email)) {
        showMessage('Email already registered!', 'error');
        return;
    }

    // Create new user
    const newUser = {
        id: Date.now(),
        firstname,
        lastname,
        email,
        phone,
        password, // In production, this should be hashed
        userType,
        createdAt: new Date().toISOString()
    };

    users.push(newUser);
    localStorage.setItem('hospital_users', JSON.stringify(users));

    showMessage('Account created successfully! Redirecting to login...', 'success');

    setTimeout(() => {
        window.location.href = 'login.html';
    }, 2000);
}

// Check if user is already logged in
function checkAuth() {
    const session = localStorage.getItem('hospital_session') || sessionStorage.getItem('hospital_session');
    
    if (session && (window.location.pathname.includes('login.html') || window.location.pathname.includes('signup.html'))) {
        window.location.href = 'index.html';
    }
}

// Social login handlers (placeholder)
document.addEventListener('DOMContentLoaded', function() {
    checkAuth();

    const googleBtn = document.querySelector('.btn-google');
    if (googleBtn) {
        googleBtn.addEventListener('click', function(e) {
            e.preventDefault();
            showMessage('Google Sign-In coming soon!', 'error');
        });
    }
});

// Password strength indicator
const passwordInput = document.getElementById('signup-password');
if (passwordInput) {
    passwordInput.addEventListener('input', function() {
        const password = this.value;
        let strength = 0;

        if (password.length >= 6) strength++;
        if (password.length >= 10) strength++;
        if (/[a-z]/.test(password) && /[A-Z]/.test(password)) strength++;
        if (/\d/.test(password)) strength++;
        if (/[^a-zA-Z\d]/.test(password)) strength++;

        // You can add a visual indicator here
    });
}
