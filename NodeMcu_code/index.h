/*
 * This file contains the HTML, CSS, and JavaScript for the 
 * driver drowsiness dashboard. It is stored in PROGMEM (flash)
 * to save RAM on the ESP8266.
 */

// C++ raw string literal (R"=====(...)=====") lets us paste HTML directly
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Driver Drowsiness Monitor</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #1e3c72 0%, #2a5298 50%, #7e22ce 100%);
      min-height: 100vh;
      padding: 20px;
    }
    
    .dashboard {
      max-width: 1200px;
      margin: 0 auto;
    }
    
    .header {
      text-align: center;
      color: white;
      margin-bottom: 30px;
      animation: fadeIn 0.6s ease-out;
    }
    
    .header h1 {
      font-size: 2.5em;
      margin-bottom: 10px;
      text-shadow: 0 2px 10px rgba(0, 0, 0, 0.3);
    }
    
    .header p {
      font-size: 1.1em;
      opacity: 0.9;
    }
    
    @keyframes fadeIn {
      from {
        opacity: 0;
        transform: translateY(-20px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }
    
    .driver-card {
      background: linear-gradient(135deg, rgba(255, 255, 255, 0.95) 0%, rgba(255, 255, 255, 0.9) 100%);
      backdrop-filter: blur(10px);
      border-radius: 25px;
      padding: 35px;
      box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);
      animation: slideUp 0.8s ease-out;
      position: relative;
      overflow: hidden;
    }
    
    @keyframes slideUp {
      from {
        opacity: 0;
        transform: translateY(40px);
      }
      to {
        opacity: 1;
        transform: translateY(0);
      }
    }
    
    .driver-card::before {
      content: '';
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 8px;
      background: linear-gradient(90deg, #8b5cf6, #ec4899, #f59e0b);
      border-radius: 25px 25px 0 0;
    }
    
    .card-header {
      display: flex;
      align-items: center;
      gap: 25px;
      margin-bottom: 30px;
      padding-bottom: 25px;
      border-bottom: 2px solid #e5e7eb;
    }
    
    .driver-avatar {
      width: 120px;
      height: 120px;
      border-radius: 50%;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 50px;
      box-shadow: 0 8px 25px rgba(102, 126, 234, 0.3);
      flex-shrink: 0;
    }
    
    .driver-info {
      flex: 1;
    }
    
    .driver-name {
      font-size: 2em;
      color: #1e293b;
      margin-bottom: 5px;
      font-weight: 700;
    }
    
    .driver-id {
      color: #64748b;
      font-size: 1em;
      margin-bottom: 5px;
    }
    
    .driver-license {
      color: #94a3b8;
      font-size: 0.9em;
    }
    
    .status-badge {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      padding: 8px 20px;
      border-radius: 50px;
      font-weight: 600;
      font-size: 0.85em;
      text-transform: uppercase;
      letter-spacing: 1px;
      margin-top: 10px;
    }
    
    .badge-awake {
      background: #d1fae5;
      color: #065f46;
      border: 2px solid #10b981;
    }
    
    .badge-sleeping {
      background: #fee2e2;
      color: #991b1b;
      border: 2px solid #ef4444;
      animation: blink 1s ease-in-out infinite;
    }
    
    @keyframes blink {
      0%, 100% { opacity: 1; }
      50% { opacity: 0.7; }
    }
    
    .stats-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 20px;
      margin-bottom: 30px;
    }
    
    .stat-card {
      background: white;
      border-radius: 15px;
      padding: 20px;
      box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
      transition: transform 0.3s ease;
    }
    
    .stat-card:hover {
      transform: translateY(-5px);
    }
    
    .stat-icon {
      font-size: 2.5em;
      margin-bottom: 10px;
    }
    
    .stat-label {
      color: #64748b;
      font-size: 0.85em;
      text-transform: uppercase;
      letter-spacing: 1px;
      margin-bottom: 8px;
    }
    
    .stat-value {
      color: #1e293b;
      font-size: 1.8em;
      font-weight: 700;
    }
    
    .status-display {
      background: linear-gradient(135deg, #f8fafc 0%, #f1f5f9 100%);
      border-radius: 20px;
      padding: 35px;
      text-align: center;
      margin-bottom: 25px;
      border: 3px solid transparent;
      transition: all 0.4s ease;
    }
    
    .status-display.awake {
      border-color: #10b981;
      background: linear-gradient(135deg, #ecfdf5 0%, #d1fae5 100%);
    }
    
    .status-display.sleeping {
      border-color: #ef4444;
      background: linear-gradient(135deg, #fef2f2 0%, #fee2e2 100%);
      animation: alertPulse 2s ease-in-out infinite;
    }
    
    @keyframes alertPulse {
      0%, 100% {
        box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.4);
      }
      50% {
        box-shadow: 0 0 0 20px rgba(239, 68, 68, 0);
      }
    }
    
    .status-icon-large {
      font-size: 100px;
      margin-bottom: 15px;
      display: inline-block;
    }
    
    .status-display.awake .status-icon-large {
      animation: float 3s ease-in-out infinite;
    }
    
    .status-display.sleeping .status-icon-large {
      animation: shake 0.6s ease-in-out infinite;
    }
    
    @keyframes float {
      0%, 100% {
        transform: translateY(0);
      }
      50% {
        transform: translateY(-10px);
      }
    }
    
    @keyframes shake {
      0%, 100% { transform: translateX(0) rotate(0deg); }
      25% { transform: translateX(-8px) rotate(-5deg); }
      75% { transform: translateX(8px) rotate(5deg); }
    }
    
    .status-text-large {
      font-size: 3em;
      font-weight: 800;
      margin-bottom: 10px;
      text-transform: uppercase;
      letter-spacing: 2px;
    }
    
    .status-display.awake .status-text-large {
      color: #059669;
      text-shadow: 2px 2px 4px rgba(5, 150, 105, 0.2);
    }
    
    .status-display.sleeping .status-text-large {
      color: #dc2626;
      text-shadow: 2px 2px 4px rgba(220, 38, 38, 0.2);
    }
    
    .status-subtitle {
      color: #64748b;
      font-size: 1.1em;
    }
    
    .alert-message {
      background: linear-gradient(135deg, #fef3c7 0%, #fde68a 100%);
      border-left: 5px solid #f59e0b;
      padding: 20px;
      border-radius: 12px;
      margin-bottom: 25px;
      display: none;
    }
    
    .alert-message.show {
      display: block;
      animation: slideInRight 0.5s ease-out;
    }
    
    @keyframes slideInRight {
      from {
        opacity: 0;
        transform: translateX(50px);
      }
      to {
        opacity: 1;
        transform: translateX(0);
      }
    }
    
    .alert-message strong {
      color: #92400e;
      font-size: 1.1em;
      display: block;
      margin-bottom: 5px;
    }
    
    .alert-message p {
      color: #78350f;
      margin: 0;
    }
    
    .footer-info {
      text-align: center;
      margin-top: 25px;
      padding-top: 20px;
      border-top: 2px solid #e5e7eb;
      color: #64748b;
      font-size: 0.9em;
    }
    
    @media (max-width: 768px) {
      .driver-name {
        font-size: 1.5em;
      }
      
      .driver-avatar {
        width: 80px;
        height: 80px;
        font-size: 35px;
      }
      
      .status-text-large {
        font-size: 2em;
      }
      
      .status-icon-large {
        font-size: 70px;
      }
      
      .stats-grid {
        grid-template-columns: 1fr 1fr;
      }
    }
  </style>
</head>
<body>
  <div class="dashboard">
    <div class="header">
      <h1>🚗 Smart Driver Monitoring System</h1>
      <p>Real-time AI-Powered Drowsiness Detection Dashboard</p>
    </div>

    <div class="driver-card">
      <div class="card-header">
        <div class="driver-avatar" id="driverAvatar">👨‍✈</div>
        <div class="driver-info">
          <div class="driver-name" id="driverName">John Anderson</div>
          <div class="driver-id">Driver ID: <strong>DRV-2024-8731</strong></div>
          <div class="driver-license">License: <strong>DL-45289-CA</strong></div>
          <span class="status-badge badge-awake" id="statusBadge">
            <span id="badgeIcon">✓</span>
            <span id="badgeText">AWAKE</span>
          </span>
        </div>
      </div>

      <div class="alert-message" id="alertMessage">
        <strong>⚠ DROWSINESS ALERT!</strong>
        <p>Driver showing signs of fatigue. Immediate attention required!</p>
      </div>

      <div class="status-display awake" id="statusDisplay">
        <div class="status-icon-large" id="statusIcon">👁</div>
        <div class="status-text-large" id="statusText">AWAKE</div>
        <div class="status-subtitle" id="statusSubtitle">Driver is alert and focused</div>
      </div>

      <div class="stats-grid">
        <div class="stat-card">
          <div class="stat-icon">⏱</div>
          <div class="stat-label">Current Duration</div>
          <div class="stat-value" id="duration">0s</div>
        </div>
        
        <div class="stat-card">
          <div class="stat-icon">🕐</div>
          <div class="stat-label">Session Time</div>
          <div class="stat-value" id="sessionTime">0m 0s</div>
        </div>
        
        <div class="stat-card">
          <div class="stat-icon">📊</div>
          <div class="stat-label">Status Changes</div>
          <div class="stat-value" id="statusChanges">0</div>
        </div>
        
        <div class="stat-card">
          <div class="stat-icon">🔔</div>
          <div class="stat-label">Alerts Sent</div>
          <div class="stat-value" id="alertsCount">0</div>
        </div>
      </div>

      <div class="footer-info">
        <strong>🔒 Secure Connection</strong> | ESP8266 NodeMCU | Last Update: <span id="lastUpdate">Connecting...</span>
      </div>
    </div>
  </div>

  <script>
    let currentLocalState = 'AWAKE'; // Used to check if the UI needs updating
    
    // Driver profile data (can be customized)
    const driverProfile = {
      name: 'John Anderson',
      id: 'DRV-2024-8731',
      license: 'DL-45289-CA',
      avatar: '👨‍✈'
    };
    
    // Initialize driver info
    document.getElementById('driverName').textContent = driverProfile.name;
    document.getElementById('driverAvatar').textContent = driverProfile.avatar;

    /**
     * Formats a duration in total seconds into a h/m/s string
     */
    function formatDuration(totalSeconds) {
      const hours = Math.floor(totalSeconds / 3600);
      const minutes = Math.floor((totalSeconds % 3600) / 60);
      const seconds = totalSeconds % 60;
      
      let durationText = '';
      if (hours > 0) {
        durationText = hours + 'h ' + minutes + 'm';
      } else if (minutes > 0) {
        durationText = minutes + 'm ' + seconds + 's';
      } else {
        durationText = seconds + 's';
      }
      return durationText;
    }

    /**
     * Updates the entire dashboard UI based on the new status
     */
    function updateDashboardUI(newStatus) {
      // If state hasn't changed, do nothing
      if (newStatus === currentLocalState) {
        return;
      }
      
      currentLocalState = newStatus; // Update the local state
      
      const statusDisplay = document.getElementById('statusDisplay');
      const statusIcon = document.getElementById('statusIcon');
      const statusText = document.getElementById('statusText');
      const statusSubtitle = document.getElementById('statusSubtitle');
      const statusBadge = document.getElementById('statusBadge');
      const badgeIcon = document.getElementById('badgeIcon');
      const badgeText = document.getElementById('badgeText');
      const alertMessage = document.getElementById('alertMessage');
      
      if (newStatus === 'SLEEPING') {
        // Status Display
        statusDisplay.className = 'status-display sleeping';
        statusIcon.textContent = '😴';
        statusText.textContent = 'SLEEPING';
        statusSubtitle.textContent = '⚠ Driver requires immediate attention!';
        
        // Badge
        statusBadge.className = 'status-badge badge-sleeping';
        badgeIcon.textContent = '⚠';
        badgeText.textContent = 'SLEEPING';
        
        // Show alert
        alertMessage.classList.add('show');
        
        console.log('⚠ UI Update: Driver is SLEEPING!');
      } else {
        // Status Display
        statusDisplay.className = 'status-display awake';
        statusIcon.textContent = '👁';
        statusText.textContent = 'AWAKE';
        statusSubtitle.textContent = 'Driver is alert and focused';
        
        // Badge
        statusBadge.className = 'status-badge badge-awake';
        badgeIcon.textContent = '✓';
        badgeText.textContent = 'AWAKE';
        
        // Hide alert
        alertMessage.classList.remove('show');
        
        console.log('✓ UI Update: Driver is AWAKE');
      }
    }
    
    /**
     * Fetches the latest data from the ESP8266 /data endpoint
     */
    async function fetchData() {
      try {
        const response = await fetch('/data');
        if (!response.ok) {
          throw new Error('Network response was not ok');
        }
        const data = await response.json();
        
        // 1. Update the main status display (if needed)
        updateDashboardUI(data.status);
        
        // 2. Update all the stat cards
        document.getElementById('duration').textContent = formatDuration(data.stateDuration);
        document.getElementById('sessionTime').textContent = formatDuration(data.sessionDuration);
        document.getElementById('statusChanges').textContent = data.changes;
        document.getElementById('alertsCount').textContent = data.alerts;
        
        // 3. Update the footer
        // *** THIS NOW WORKS ***
        document.getElementById('lastUpdate').textContent = data.time;
        
      } catch (error) {
        console.error('Error fetching data:', error);
        document.getElementById('lastUpdate').textContent = 'Connection Lost';
      }
    }
    
    // Fetch data every 2 seconds (2000 milliseconds)
    setInterval(fetchData, 2000);
    
    // Fetch data immediately on page load
    document.addEventListener('DOMContentLoaded', fetchData);
    
  </script>
</body>
</html>
)=====";