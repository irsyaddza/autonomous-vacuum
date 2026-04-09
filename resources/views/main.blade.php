<x-master>
    <!-- Header Section (Desktop) -->
    <div class="d-none d-md-flex align-items-center justify-content-between mb-5 stagger-1">
        <div>
            <h1 class="h2 mb-1 fw-bold text-white">Dashboard Overview</h1>
            <p class="text-secondary mb-0">Monitor and control your autonomous vacuum.</p>
        </div>
        <div class="mt-3 mt-md-0">
        </div>
    </div>

    <!-- Header Section (Mobile Clock) -->
    <div class="d-md-none text-center mb-3 stagger-1">
        <h2 class="fw-bold text-white mb-0" id="mobile-clock-time" style="font-family: 'Courier New', monospace;">00:00:00</h2>
        <small class="text-secondary small text-uppercase tracking-widest" id="mobile-clock-date" style="font-size: 0.7rem;">Loading...</small>
    </div>

    <!-- Notification Container -->
    <div id="notification-container" class="mb-4"></div>

    <!-- Stats Cards -->
    <div class="row g-4 mb-5 stagger-2">
        <!-- Status Robot Card -->
        <div class="col-md-6 col-lg-6">
            <div class="card h-100">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center mb-3">
                        <div class="bg-primary bg-opacity-10 p-3 rounded-circle text-primary me-3">
                            <i class="fas fa-robot fa-lg"></i>
                        </div>
                        <div>
                            <h6 class="text-uppercase text-secondary fw-bold mb-1" style="font-size: 0.75rem; letter-spacing: 1px;">Robot Status</h6>
                            <h3 class="mb-0 fw-bold" id="statusRobot">Standby</h3>
                        </div>
                    </div>
                    <div class="alert alert-dark bg-opacity-25 border-0 mb-0 d-flex align-items-center" role="alert">
                        <i class="fas fa-info-circle me-2 text-info"></i>
                        <span id="statusInfo" class="small text-secondary">Ready for operation</span>
                    </div>
                </div>
            </div>
        </div>

        <!-- Battery Monitoring Card -->
        <div class="col-md-6 col-lg-6">
            <div class="card h-100">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center justify-content-between mb-3">
                        <div class="d-flex align-items-center">
                            <div class="bg-success bg-opacity-10 p-3 rounded-circle text-success me-3">
                                <i class="fas fa-battery-three-quarters fa-lg"></i>
                            </div>
                            <div>
                                <h6 class="text-uppercase text-secondary fw-bold mb-1" style="font-size: 0.75rem; letter-spacing: 1px;">Battery Level</h6>
                                <h3 class="mb-0 fw-bold" id="batteryPercent">85%</h3>
                            </div>
                        </div>
                        <div class="text-end">
                            <i class="fas fa-bolt text-warning fa-lg"></i>
                        </div>
                    </div>
                    
                    <div class="progress mb-3" style="height: 8px;">
                        <div class="progress-bar bg-gradient-success" id="batteryBar" role="progressbar" style="width: 85%"></div>
                    </div>
                    
                    <div class="d-flex justify-content-between align-items-center small text-secondary">
                        <span><i class="far fa-clock me-1"></i> <span id="batteryEstimate">Est: 3h 45m</span></span>
                        <span>12.5 V</span>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Main Control Section -->
    <div class="row g-4 stagger-3">
        <!-- Main Controls -->
        <div class="col-lg-8">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-gamepad me-2 text-primary"></i>
                    <h6 class="m-0 fw-bold text-white">Main Controls</h6>
                </div>
                <div class="card-body p-4">
                    <div class="row g-3">
                        <div class="col-md-4">
                            <button class="btn btn-primary-glow w-100 py-4 rounded-3 d-flex flex-column align-items-center justify-content-center" onclick="startVacuum()">
                                <i class="fas fa-play fa-2x mb-3"></i>
                                <span class="fw-bold tracking-wide">START</span>
                            </button>
                        </div>
                        <div class="col-md-4">
                            <button class="btn btn-danger-glow w-100 py-4 rounded-3 d-flex flex-column align-items-center justify-content-center" onclick="stopVacuum()">
                                <i class="fas fa-stop fa-2x mb-3"></i>
                                <span class="fw-bold tracking-wide">STOP</span>
                            </button>
                        </div>
                        <div class="col-md-4">
                            <button class="btn btn-warning-glow w-100 py-4 rounded-3 d-flex flex-column align-items-center justify-content-center" onclick="returnToBase()">
                                <i class="fas fa-home fa-2x mb-3"></i>
                                <span class="fw-bold tracking-wide">RETURN</span>
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Power Regulation -->
        <div class="col-lg-4">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-fan me-2 text-info"></i>
                    <h6 class="m-0 fw-bold text-white">Suction Power</h6>
                </div>
                <div class="card-body p-0">
                    <div class="list-group list-group-flush rounded-bottom">
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white border-bottom border-light border-opacity-10 py-3" onclick="setPowerMode('eco')" id="ecoBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-leaf text-success me-3 fa-lg"></i>
                                <div>
                                    <div class="fw-bold">ECO Mode</div>
                                    <small class="text-secondary">Energy saving, quiet operation</small>
                                </div>
                            </div>
                        </button>
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white border-bottom border-light border-opacity-10 py-3 active-mode" onclick="setPowerMode('normal')" id="normalBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-wind text-primary me-3 fa-lg"></i>
                                <div>
                                    <div class="fw-bold">NORMAL Mode</div>
                                    <small class="text-secondary">Standard suction power</small>
                                </div>
                            </div>
                        </button>
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white py-3" onclick="setPowerMode('strong')" id="strongBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-tornado text-danger me-3 fa-lg"></i>
                                <div>
                                    <div class="fw-bold">STRONG Mode</div>
                                    <small class="text-secondary">Maximum power for deep cleaning</small>
                                </div>
                            </div>
                        </button>
                    </div>
                    
                    <div class="p-3 bg-dark bg-opacity-50">
                        <small class="text-secondary d-block text-center" id="powerInfo">Current: <span class="text-white fw-bold">NORMAL</span></small>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Inline script for page specific logic -->
    <script>
        // ===== CONFIGURATION =====
        const API_BASE_URL = "/v1/vacuum";
        const ESP32_TIMEOUT = 3000; // 3 seconds timeout for direct ESP32 communication

        // ===== GLOBAL STATE =====
        let esp32Ip = null; // Will be fetched from Laravel on page load
        let vacuumState = {
            state: 'standby',
            powerMode: 'normal',
            batteryPercent: 85
        };

        // ===== UI HELPERS =====
        function showNotification(type, message) {
            const icon = type === 'success' ? 'fa-check-circle' : type === 'warning' ? 'fa-exclamation-triangle' : 'fa-exclamation-circle';
            const colorClass = type === 'success' ? 'text-success' : type === 'warning' ? 'text-warning' : 'text-danger';
            const borderClass = type === 'success' ? 'border-success' : type === 'warning' ? 'border-warning' : 'border-danger';
            
            const html = `
                <div class="alert alert-dark border-start ${borderClass} border-4 shadow-lg fade show" role="alert" style="background: rgba(30, 41, 59, 0.95);">
                    <div class="d-flex align-items-center">
                        <i class="fas ${icon} ${colorClass} fa-lg me-3"></i>
                        <div>${message}</div>
                        <button type="button" class="btn-close btn-close-white ms-auto" data-bs-dismiss="alert" aria-label="Close"></button>
                    </div>
                </div>
            `;
            
            const container = document.getElementById('notification-container');
            container.innerHTML = html;
            
            setTimeout(() => {
                const alerts = container.querySelectorAll('.alert');
                alerts.forEach(el => {
                    const alert = new bootstrap.Alert(el);
                    alert.close();
                });
            }, 3000);
        }

        // ===== ESP32 IP DISCOVERY =====
        async function discoverEsp32() {
            try {
                const res = await $.get(`${API_BASE_URL}/device`);
                if (res.success && res.data) {
                    esp32Ip = res.data.ip_address;
                    console.log(`✅ ESP32 discovered at: ${esp32Ip}`);
                    return true;
                }
            } catch (err) {
                console.warn('⚠️ No ESP32 device registered. Using fallback mode.');
            }
            esp32Ip = null;
            return false;
        }

        // ===== DIRECT HTTP TO ESP32 =====
        function sendToEsp32(endpoint, payload) {
            return new Promise((resolve, reject) => {
                if (!esp32Ip) {
                    reject(new Error('No ESP32 IP'));
                    return;
                }

                const startTime = Date.now();

                $.ajax({
                    url: `http://${esp32Ip}/${endpoint}`,
                    type: 'POST',
                    contentType: 'application/json',
                    data: JSON.stringify(payload),
                    timeout: ESP32_TIMEOUT,
                    success: (res) => {
                        const elapsed = Date.now() - startTime;
                        resolve({ ...res, responseTime: elapsed });
                    },
                    error: (xhr, status, error) => {
                        const elapsed = Date.now() - startTime;
                        reject({ status, error, responseTime: elapsed });
                    }
                });
            });
        }

        // ===== FALLBACK: Send via Laravel DB (old flow) =====
        function sendViaLaravel(command, type = 'command') {
            const csrfToken = document.querySelector('meta[name="csrf-token"]').getAttribute('content');
            const powerValues = { 'eco': 150, 'normal': 200, 'strong': 255 };

            let url, data;
            if (type === 'power') {
                url = `${API_BASE_URL}/power-mode`;
                data = { mode: command, value: powerValues[command] };
            } else {
                url = `${API_BASE_URL}/command`;
                data = { command: command };
            }

            return new Promise((resolve, reject) => {
                $.ajax({
                    url: url,
                    type: 'POST',
                    headers: { 'X-CSRF-TOKEN': csrfToken, 'Content-Type': 'application/json' },
                    data: JSON.stringify(data),
                    success: (res) => resolve(res),
                    error: (err) => reject(err)
                });
            });
        }

        // ===== LOG COMMAND TO LARAVEL (history) =====
        function logCommandToServer(command, status, responseTimeMs, esp32Ip) {
            const csrfToken = document.querySelector('meta[name="csrf-token"]').getAttribute('content');
            
            $.ajax({
                url: `${API_BASE_URL}/command-log`,
                type: 'POST',
                headers: { 'X-CSRF-TOKEN': csrfToken, 'Content-Type': 'application/json' },
                data: JSON.stringify({
                    command: command,
                    source: 'web',
                    status: status,
                    response_time_ms: responseTimeMs,
                    esp32_ip: esp32Ip
                }),
                error: (err) => console.error('Failed to log command:', err)
            });
        }

        // ===== MAIN COMMAND SENDER (Direct → Fallback) =====
        async function sendVacuumCommand(command) {
            // Try Direct HTTP to ESP32 first
            if (esp32Ip) {
                try {
                    const res = await sendToEsp32('command', { command: command });
                    showNotification('success', `⚡ Direct: ${command.toUpperCase()} (${res.responseTime}ms)`);
                    
                    // Log to history
                    logCommandToServer(command, 'success', res.responseTime, esp32Ip);
                    
                    // Update UI from ESP32 response if available
                    if (res.state) {
                        updateStatusUI({ state: res.state, power_mode: res.power_mode });
                    }
                    return;
                } catch (err) {
                    console.warn(`⚠️ ESP32 unreachable (${err.responseTime}ms), falling back to Laravel...`);
                    logCommandToServer(command, 'timeout', err.responseTime, esp32Ip);
                }
            }

            // Fallback: Send via Laravel DB
            try {
                await sendViaLaravel(command, 'command');
                showNotification('warning', `📡 Fallback: ${command.toUpperCase()} sent via server`);
                setTimeout(fetchVacuumStatus, 500);
            } catch (err) {
                showNotification('error', `Failed to send command: ${command}`);
                logCommandToServer(command, 'failed', 0, null);
            }
        }

        async function setPowerMode(mode) {
            const powerValues = { 'eco': 150, 'normal': 200, 'strong': 255 };

            // Try Direct HTTP to ESP32 first
            if (esp32Ip) {
                try {
                    const res = await sendToEsp32('command', { 
                        command: mode, 
                        value: powerValues[mode] 
                    });
                    showNotification('success', `⚡ Direct: ${mode.toUpperCase()} mode (${res.responseTime}ms)`);
                    logCommandToServer(mode, 'success', res.responseTime, esp32Ip);
                    updatePowerModeUI(mode);
                    return;
                } catch (err) {
                    console.warn(`⚠️ ESP32 unreachable, falling back...`);
                    logCommandToServer(mode, 'timeout', err.responseTime, esp32Ip);
                }
            }

            // Fallback: Send via Laravel DB
            try {
                await sendViaLaravel(mode, 'power');
                showNotification('warning', `📡 Fallback: ${mode.toUpperCase()} mode via server`);
                updatePowerModeUI(mode);
            } catch (err) {
                showNotification('error', `Failed to change power mode`);
                logCommandToServer(mode, 'failed', 0, null);
            }
        }

        // ===== BUTTON LISTENERS =====
        window.startVacuum = () => sendVacuumCommand('start');
        window.stopVacuum = () => sendVacuumCommand('stop');
        window.returnToBase = () => sendVacuumCommand('return_home');
        window.setPowerMode = setPowerMode;

        // ===== FETCHING DATA (still from Laravel for dashboard sync) =====
        function fetchFullStatus() {
            $.get(`${API_BASE_URL}/full-status`, (res) => {
                if(res.success) {
                    updateUI(res.vacuum, res.battery);
                }
            });
        }

        function fetchVacuumStatus() {
            $.get(`${API_BASE_URL}/status`, (res) => {
                if(res.success && res.data) {
                    updateStatusUI(res.data);
                }
            });
        }

        function fetchBatteryData() {
            $.get(`${API_BASE_URL}/battery/latest`, (res) => {
                if(res.success && res.data) {
                    updateBatteryUI(res.data);
                }
            });
        }

        // ===== UI UPDATE FUNCTIONS =====
        function updateUI(vacuum, battery) {
            updateStatusUI(vacuum);
            updateBatteryUI(battery);
        }

        function updateStatusUI(data) {
            const statusEl = document.getElementById('statusRobot');
            const infoEl = document.getElementById('statusInfo');
            
            let badgeClass = 'bg-secondary';
            let statusText = 'Unknown';
            let infoText = '...';
            
            switch(data.state) {
                case 'working': 
                    badgeClass = 'bg-success'; statusText = 'Cleaning'; infoText = 'Robot is actively cleaning'; 
                    break;
                case 'standby': 
                    badgeClass = 'bg-secondary'; statusText = 'Standby'; infoText = 'Ready for commands'; 
                    break;
                case 'returning': 
                    badgeClass = 'bg-warning text-dark'; statusText = 'Returning'; infoText = 'Going back to dock'; 
                    break;
                case 'charging': 
                    badgeClass = 'bg-info text-dark'; statusText = 'Charging'; infoText = 'Battery is charging'; 
                    break;
                case 'stopping': 
                    badgeClass = 'bg-danger'; statusText = 'Stopped'; infoText = 'Operation halted'; 
                    break;
            }
            
            statusEl.innerHTML = `<span class="badge ${badgeClass}">${statusText}</span>`;
            infoEl.textContent = infoText;
            
            updatePowerModeUI(data.power_mode);
        }

        function updateBatteryUI(data) {
            const percent = data.battery_percent || data.percent;
            const estimate = data.estimated_time;
            
            document.getElementById('batteryPercent').textContent = `${percent}%`;
            document.getElementById('batteryEstimate').textContent = estimate ? `Est: ${estimate}` : '';
            
            const bar = document.getElementById('batteryBar');
            bar.style.width = `${percent}%`;
            
            bar.className = 'progress-bar';
            if(percent > 50) bar.classList.add('bg-success');
            else if(percent > 20) bar.classList.add('bg-warning');
            else bar.classList.add('bg-danger');
        }

        function updatePowerModeUI(mode) {
            const modes = ['eco', 'normal', 'strong'];
            modes.forEach(m => {
                const btn = document.getElementById(`${m}Btn`);
                if(m === mode) {
                    btn.classList.add('bg-primary', 'bg-opacity-25');
                    btn.classList.remove('bg-transparent');
                } else {
                    btn.classList.remove('bg-primary', 'bg-opacity-25');
                    btn.classList.add('bg-transparent');
                }
            });
            document.getElementById('powerInfo').innerHTML = `Current: <span class="text-white fw-bold uppercase">${mode.toUpperCase()}</span>`;
        }

        // ===== INITIALIZATION =====
        let statusInterval, batteryInterval;

        document.addEventListener('DOMContentLoaded', async () => {
            // 1. Discover ESP32 IP
            await discoverEsp32();
            
            // 2. Fetch initial dashboard state
            fetchFullStatus();
            
            // 3. Keep polling for dashboard sync (status from Laravel DB)
            statusInterval = setInterval(fetchVacuumStatus, 20000);
            batteryInterval = setInterval(fetchBatteryData, 10000);
            
            // 4. Re-discover ESP32 IP every 30 seconds
            setInterval(discoverEsp32, 30000);
        });

        window.addEventListener('beforeunload', () => {
            clearInterval(statusInterval);
            clearInterval(batteryInterval);
        });
    </script>
</x-master>