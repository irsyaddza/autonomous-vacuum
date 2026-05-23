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
                        <span><i class="far fa-clock me-1"></i> <span id="batteryEstimate">Est: —</span></span>
                        <span><i class="fas fa-bolt me-1"></i> <span id="batteryVoltage">— V</span></span>
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

    <!-- Command History Section -->
    <div class="row g-4 mt-1 stagger-3">
        <div class="col-12">
            <div class="card">
                <div class="card-header py-3 d-flex align-items-center justify-content-between">
                    <div class="d-flex align-items-center">
                        <i class="fas fa-history me-2 text-info"></i>
                        <h6 class="m-0 fw-bold text-white">Command History</h6>
                    </div>
                    <div class="d-flex align-items-center gap-2">
                        <small class="text-secondary d-none d-md-inline" id="cmdLogUpdatedAt">Updated just now</small>
                        <button class="btn btn-sm btn-outline-secondary border-0" onclick="fetchCommandLogs()" title="Refresh" id="cmdLogRefreshBtn">
                            <i class="fas fa-sync-alt"></i>
                        </button>
                    </div>
                </div>
                <div class="card-body p-0">
                    <!-- Desktop Table View -->
                    <div class="table-responsive d-none d-md-block">
                        <table class="table table-dark table-hover mb-0 align-middle" style="background: transparent;">
                            <thead>
                                <tr style="border-bottom: 1px solid rgba(255,255,255,0.08);">
                                    <th class="ps-4 py-3 text-secondary text-uppercase" style="font-size: 0.7rem; letter-spacing: 1px;">Command</th>
                                    <th class="py-3 text-secondary text-uppercase" style="font-size: 0.7rem; letter-spacing: 1px;">Source</th>
                                    <th class="py-3 text-secondary text-uppercase" style="font-size: 0.7rem; letter-spacing: 1px;">Status</th>
                                    <th class="py-3 text-secondary text-uppercase" style="font-size: 0.7rem; letter-spacing: 1px;">Response</th>
                                    <th class="pe-4 py-3 text-secondary text-uppercase text-end" style="font-size: 0.7rem; letter-spacing: 1px;">Time</th>
                                </tr>
                            </thead>
                            <tbody id="cmdLogTableBody">
                                <tr>
                                    <td colspan="5" class="text-center text-secondary py-4">
                                        <i class="fas fa-spinner fa-spin me-2"></i>Loading command history...
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </div>

                    <!-- Mobile Card View -->
                    <div class="d-md-none" id="cmdLogMobileList">
                        <div class="text-center text-secondary py-4">
                            <i class="fas fa-spinner fa-spin me-2"></i>Loading...
                        </div>
                    </div>

                    <!-- Empty State -->
                    <div class="text-center py-5 d-none" id="cmdLogEmpty">
                        <i class="fas fa-inbox fa-3x text-secondary mb-3" style="opacity: 0.3;"></i>
                        <p class="text-secondary mb-0">No commands recorded yet</p>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Command History Styles -->
    <style>
        /* Desktop table */
        .table-dark { --bs-table-bg: transparent; }
        .table-dark > tbody > tr {
            border-bottom: 1px solid rgba(255,255,255,0.04);
            transition: background 0.2s ease;
        }
        .table-dark > tbody > tr:hover { background: rgba(255,255,255,0.03); }
        .table-dark > tbody > tr:last-child { border-bottom: none; }

        /* Command badges */
        .cmd-badge {
            display: inline-flex;
            align-items: center;
            gap: 6px;
            padding: 5px 12px;
            border-radius: 8px;
            font-size: 0.78rem;
            font-weight: 700;
            letter-spacing: 0.5px;
            text-transform: uppercase;
        }
        .cmd-badge.cmd-start   { background: rgba(16,185,129,0.15); color: #34d399; }
        .cmd-badge.cmd-stop    { background: rgba(239,68,68,0.15);  color: #f87171; }
        .cmd-badge.cmd-return  { background: rgba(245,158,11,0.15); color: #fbbf24; }
        .cmd-badge.cmd-eco     { background: rgba(16,185,129,0.12); color: #6ee7b7; }
        .cmd-badge.cmd-normal  { background: rgba(14,165,233,0.15); color: #7dd3fc; }
        .cmd-badge.cmd-strong  { background: rgba(239,68,68,0.12);  color: #fca5a5; }
        .cmd-badge.cmd-battery { background: rgba(251,191,36,0.15); color: #fde68a; }
        .cmd-badge.cmd-default { background: rgba(148,163,184,0.12); color: #cbd5e1; }

        /* Status pills */
        .status-pill {
            display: inline-flex;
            align-items: center;
            gap: 5px;
            padding: 3px 10px;
            border-radius: 50rem;
            font-size: 0.72rem;
            font-weight: 600;
        }
        .status-pill.status-success { background: rgba(16,185,129,0.15); color: #34d399; }
        .status-pill.status-failed  { background: rgba(239,68,68,0.15);  color: #f87171; }
        .status-pill.status-timeout { background: rgba(245,158,11,0.15); color: #fbbf24; }

        /* Source badge */
        .source-badge {
            display: inline-flex;
            align-items: center;
            gap: 4px;
            font-size: 0.75rem;
            color: var(--text-secondary);
        }

        /* Mobile list items */
        .cmd-mobile-item {
            padding: 14px 16px;
            border-bottom: 1px solid rgba(255,255,255,0.04);
            transition: background 0.2s ease;
        }
        .cmd-mobile-item:last-child { border-bottom: none; }
        .cmd-mobile-item:active { background: rgba(255,255,255,0.03); }

        /* Refresh spin animation */
        .spin { animation: spin 0.8s linear infinite; }
        @keyframes spin { to { transform: rotate(360deg); } }
    </style>

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
        function showNotification(type, message, persistent = false) {
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
            
            if (!persistent) {
                setTimeout(() => {
                    const alerts = container.querySelectorAll('.alert');
                    alerts.forEach(el => {
                        const alert = new bootstrap.Alert(el);
                        alert.close();
                    });
                }, 3000);
            }
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
                console.warn('⚠️ No ESP32 device registered');
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
                        reject({ status, error, responseTime: elapsed, responseJSON: xhr.responseJSON });
                    }
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

        // ===== COMMAND SENDER (Direct HTTP Only) =====
        async function sendVacuumCommand(command) {
            if (!esp32Ip) {
                showNotification('error', '❌ ESP32 not connected. Cannot send command.');
                return;
            }

            try {
                const res = await sendToEsp32('command', { command: command });
                
                // Check if command was blocked (e.g., low battery)
                if (res.blocked) {
                    showNotification('warning', `🔋 ${res.message}`, true);
                    logCommandToServer(command, 'failed', res.responseTime, esp32Ip);
                    return;
                }
                
                showNotification('success', `⚡ ${command.toUpperCase()} (${res.responseTime}ms)`);
                logCommandToServer(command, 'success', res.responseTime, esp32Ip);
                
                if (res.state) {
                    updateStatusUI({ state: res.state, power_mode: res.power_mode });
                }
            } catch (err) {
                // Check if error response contains blocked info
                if (err.responseJSON && err.responseJSON.blocked) {
                    showNotification('warning', `🔋 ${err.responseJSON.message}`, true);
                    logCommandToServer(command, 'failed', err.responseTime || 0, esp32Ip);
                    return;
                }
                showNotification('error', '❌ ESP32 unreachable. Try again.');
                logCommandToServer(command, 'failed', err.responseTime || 0, esp32Ip);
            }
        }

        async function setPowerMode(mode) {
            const powerValues = { 'eco': 150, 'normal': 200, 'strong': 255 };

            if (!esp32Ip) {
                showNotification('error', '❌ ESP32 not connected. Cannot change mode.');
                return;
            }

            try {
                const res = await sendToEsp32('command', { 
                    command: mode, 
                    value: powerValues[mode] 
                });
                showNotification('success', `⚡ ${mode.toUpperCase()} mode (${res.responseTime}ms)`);
                logCommandToServer(mode, 'success', res.responseTime, esp32Ip);
                updatePowerModeUI(mode);
            } catch (err) {
                showNotification('error', '❌ ESP32 unreachable. Try again.');
                logCommandToServer(mode, 'failed', err.responseTime || 0, esp32Ip);
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
            
            // Check if stopped due to low battery (from latest event)
            if (data.state === 'stopping' && lastBatteryEvent === 'auto_stop_low_battery') {
                infoText = '🔋 Auto-stopped: Battery depleted';
            }
            
            statusEl.innerHTML = `<span class="badge ${badgeClass}">${statusText}</span>`;
            infoEl.textContent = infoText;
            
            updatePowerModeUI(data.power_mode);
        }

        function updateBatteryUI(data) {
            const percent = data.battery_percent || data.percent;
            const estimate = data.estimated_time;
            const voltage = data.battery_voltage || data.voltage;
            
            document.getElementById('batteryPercent').textContent = `${percent}%`;
            document.getElementById('batteryEstimate').textContent = estimate ? `Est: ${estimate}` : '';
            
            // Update voltage display
            if (voltage) {
                document.getElementById('batteryVoltage').textContent = `${parseFloat(voltage).toFixed(1)} V`;
            }
            
            // Update progress bar
            const bar = document.getElementById('batteryBar');
            bar.style.width = `${percent}%`;
            
            bar.className = 'progress-bar';
            if(percent > 50) bar.classList.add('bg-success');
            else if(percent > 20) bar.classList.add('bg-warning');
            else bar.classList.add('bg-danger');
            
            // Update battery icon based on level
            const iconEl = document.querySelector('.card .fa-battery-three-quarters');
            if (iconEl) {
                iconEl.className = 'fas fa-lg';
                if (percent > 75) iconEl.classList.add('fa-battery-full');
                else if (percent > 50) iconEl.classList.add('fa-battery-three-quarters');
                else if (percent > 25) iconEl.classList.add('fa-battery-half');
                else if (percent > 10) iconEl.classList.add('fa-battery-quarter');
                else iconEl.classList.add('fa-battery-empty');
            }
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

        // ===== BATTERY EVENT POLLING =====
        let lastBatteryEvent = null;
        let lastBatteryEventId = null;

        function fetchBatteryEvents() {
            $.get(`${API_BASE_URL}/battery-events/latest`, (res) => {
                if (res.success && res.data) {
                    const event = res.data;
                    const eventKey = event.event + '_' + event.created_at;
                    
                    // Only show notification once per unique event
                    if (eventKey !== lastBatteryEventId) {
                        lastBatteryEventId = eventKey;
                        lastBatteryEvent = event.event;
                        
                        if (event.event === 'auto_stop_low_battery') {
                            showNotification('error', 
                                `🔋 Robot auto-stopped! Battery depleted (${event.battery_percent}%, ${parseFloat(event.battery_voltage).toFixed(1)}V). Please charge the battery.`, 
                                true
                            );
                            // Force refresh status
                            fetchFullStatus();
                        } else if (event.event === 'low_battery_warning') {
                            showNotification('warning', 
                                `⚠️ Low battery warning! ${event.battery_percent}% remaining (${parseFloat(event.battery_voltage).toFixed(1)}V). Robot will auto-stop at 0%.`, 
                                true
                            );
                        }
                    }
                } else {
                    lastBatteryEvent = null;
                }
            });
        }

        // ===== COMMAND LOG HISTORY =====
        let cmdLogInterval;

        function getCommandMeta(command) {
            const map = {
                'start':                   { icon: 'fa-play',              css: 'cmd-start',   label: 'Start' },
                'stop':                    { icon: 'fa-stop',              css: 'cmd-stop',    label: 'Stop' },
                'return_home':             { icon: 'fa-home',              css: 'cmd-return',  label: 'Return' },
                'eco':                     { icon: 'fa-leaf',              css: 'cmd-eco',     label: 'Eco' },
                'normal':                  { icon: 'fa-wind',              css: 'cmd-normal',  label: 'Normal' },
                'strong':                  { icon: 'fa-tornado',           css: 'cmd-strong',  label: 'Strong' },
                'low_battery_warning':     { icon: 'fa-battery-quarter',   css: 'cmd-battery', label: 'Low Battery' },
                'auto_stop_low_battery':   { icon: 'fa-battery-empty',     css: 'cmd-battery', label: 'Auto Stop' },
            };
            return map[command] || { icon: 'fa-terminal', css: 'cmd-default', label: command };
        }

        function getStatusMeta(status) {
            const map = {
                'success': { icon: 'fa-check',              css: 'status-success', label: 'Success' },
                'failed':  { icon: 'fa-times',              css: 'status-failed',  label: 'Failed' },
                'timeout': { icon: 'fa-clock',              css: 'status-timeout', label: 'Timeout' },
            };
            return map[status] || { icon: 'fa-question', css: 'status-failed', label: status };
        }

        function timeAgo(dateStr) {
            const now = new Date();
            const past = new Date(dateStr);
            const diffMs = now - past;
            const seconds = Math.floor(diffMs / 1000);
            if (seconds < 5)   return 'just now';
            if (seconds < 60)  return seconds + 's ago';
            const minutes = Math.floor(seconds / 60);
            if (minutes < 60)  return minutes + 'm ago';
            const hours = Math.floor(minutes / 60);
            if (hours < 24)    return hours + 'h ago';
            const days = Math.floor(hours / 24);
            return days + 'd ago';
        }

        function renderDesktopRow(log) {
            const cmd = getCommandMeta(log.command);
            const st  = getStatusMeta(log.status);
            const src = log.source === 'web'
                ? '<i class="fas fa-globe text-info"></i> Web'
                : '<i class="fas fa-microchip text-warning"></i> ESP32';
            const respTime = log.response_time_ms != null ? log.response_time_ms + 'ms' : '—';

            return `<tr>
                <td class="ps-4 py-3"><span class="cmd-badge ${cmd.css}"><i class="fas ${cmd.icon}"></i>${cmd.label}</span></td>
                <td class="py-3"><span class="source-badge">${src}</span></td>
                <td class="py-3"><span class="status-pill ${st.css}"><i class="fas ${st.icon}"></i>${st.label}</span></td>
                <td class="py-3"><span class="text-secondary" style="font-size:0.8rem;">${respTime}</span></td>
                <td class="pe-4 py-3 text-end"><span class="text-secondary" style="font-size:0.8rem;" title="${log.created_at}">${timeAgo(log.created_at)}</span></td>
            </tr>`;
        }

        function renderMobileItem(log) {
            const cmd = getCommandMeta(log.command);
            const st  = getStatusMeta(log.status);
            const srcIcon = log.source === 'web'
                ? '<i class="fas fa-globe text-info"></i>'
                : '<i class="fas fa-microchip text-warning"></i>';
            const respTime = log.response_time_ms != null ? log.response_time_ms + 'ms' : '';

            return `<div class="cmd-mobile-item">
                <div class="d-flex align-items-center justify-content-between mb-1">
                    <span class="cmd-badge ${cmd.css}"><i class="fas ${cmd.icon}"></i>${cmd.label}</span>
                    <span class="text-secondary" style="font-size:0.72rem;" title="${log.created_at}">${timeAgo(log.created_at)}</span>
                </div>
                <div class="d-flex align-items-center gap-3 mt-2">
                    <span class="source-badge">${srcIcon} ${log.source}</span>
                    <span class="status-pill ${st.css}"><i class="fas ${st.icon}"></i>${st.label}</span>
                    ${respTime ? `<span class="text-secondary" style="font-size:0.72rem;"><i class="fas fa-bolt me-1"></i>${respTime}</span>` : ''}
                </div>
            </div>`;
        }

        function fetchCommandLogs() {
            const btn = document.getElementById('cmdLogRefreshBtn');
            const icon = btn.querySelector('i');
            icon.classList.add('spin');

            $.get(`${API_BASE_URL}/command-logs?limit=15`, (res) => {
                icon.classList.remove('spin');

                if (!res.success || !res.data || res.data.length === 0) {
                    document.getElementById('cmdLogTableBody').innerHTML = '';
                    document.getElementById('cmdLogMobileList').innerHTML = '';
                    document.getElementById('cmdLogEmpty').classList.remove('d-none');
                    return;
                }

                document.getElementById('cmdLogEmpty').classList.add('d-none');

                // Desktop
                document.getElementById('cmdLogTableBody').innerHTML =
                    res.data.map(renderDesktopRow).join('');

                // Mobile
                document.getElementById('cmdLogMobileList').innerHTML =
                    res.data.map(renderMobileItem).join('');

                // Update timestamp
                document.getElementById('cmdLogUpdatedAt').textContent = 'Updated just now';

            }).fail(() => {
                icon.classList.remove('spin');
            });
        }

        // ===== INITIALIZATION =====
        let statusInterval, batteryInterval, batteryEventInterval;

        document.addEventListener('DOMContentLoaded', async () => {
            // 1. Discover ESP32 IP
            await discoverEsp32();
            
            // 2. Fetch initial dashboard state
            fetchFullStatus();
            
            // 3. Keep polling for dashboard sync (status from Laravel DB)
            statusInterval = setInterval(fetchFullStatus, 20000);
            batteryInterval = setInterval(fetchBatteryData, 10000);
            
            // 4. Poll for battery events (warnings & auto-stop)
            fetchBatteryEvents();
            batteryEventInterval = setInterval(fetchBatteryEvents, 10000);
            
            // 5. Re-discover ESP32 IP every 30 seconds
            setInterval(discoverEsp32, 30000);

            // 6. Fetch command logs & auto-refresh every 15s
            fetchCommandLogs();
            cmdLogInterval = setInterval(fetchCommandLogs, 15000);
        });

        window.addEventListener('beforeunload', () => {
            clearInterval(statusInterval);
            clearInterval(batteryInterval);
            clearInterval(batteryEventInterval);
            clearInterval(cmdLogInterval);
        });
    </script>
</x-master>