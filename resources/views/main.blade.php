<x-master>
    <style>
        /* Toast notification popup styling */
        #notification-container {
            position: fixed;
            top: 24px;
            right: 24px;
            z-index: 9999;
            max-width: 400px;
            width: calc(100% - 48px);
            pointer-events: none;
        }
        #notification-container .alert {
            pointer-events: auto;
            margin-bottom: 12px;
            border-left: 4px solid;
            background: rgba(21, 30, 48, 0.95);
            backdrop-filter: blur(8px);
            border-radius: 12px;
            box-shadow: var(--shadow-lg);
            animation: slideIn 0.3s cubic-bezier(0.16, 1, 0.3, 1) forwards;
            position: relative;
        }
        @keyframes slideIn {
            from {
                opacity: 0;
                transform: translateX(100px);
            }
            to {
                opacity: 1;
                transform: translateX(0);
            }
        }
        @media (max-width: 576px) {
            #notification-container {
                top: 16px;
                right: 16px;
                left: 16px;
                width: calc(100% - 32px);
                max-width: none;
            }
            @keyframes slideIn {
                from {
                    opacity: 0;
                    transform: translateY(-50px);
                }
                to {
                    opacity: 1;
                    transform: translateY(0);
                }
            }
        }

        /* Command History list styling */
        .history-table-container {
            max-height: 275px;
            overflow-y: auto;
            position: relative;
        }
        .history-mobile-container {
            max-height: 320px;
            overflow-y: auto;
        }
        /* Style for smooth rotation of collapse icon */
        .collapse-icon {
            transition: transform 0.2s ease-in-out;
        }
        [aria-expanded="false"] .collapse-icon {
            transform: rotate(180deg);
        }
    </style>

    <!-- Header Section (Desktop) -->
    <div class="d-none d-md-flex align-items-center justify-content-between mb-5 stagger-1">
        <div>
            <p class="text-secondary mb-1 small fw-medium tracking-wide text-uppercase" style="font-size: 0.72rem; color: var(--accent);">Control Panel</p>
            <h1 class="h2 mb-0 fw-bold" style="letter-spacing: -0.025em;">Dashboard overview</h1>
        </div>
        <div class="mt-3 mt-md-0">
            <button class="btn btn-outline-danger btn-sm shadow-sm" onclick="resetEsp32Connection()" style="border-radius: 8px; font-weight: 500;">
                <i class="fas fa-wifi me-2"></i>Clear connection
            </button>
        </div>
    </div>

    <!-- Header Section (Mobile Clock) -->
    <div class="d-md-none text-center mb-4 stagger-1">
        <h2 class="fw-bold mb-0" id="mobile-clock-time" style="font-family: 'Outfit', monospace; letter-spacing: 0.08em; color: var(--accent);">00:00:00</h2>
        <small class="text-secondary fw-medium" id="mobile-clock-date" style="font-size: 0.72rem;">Loading...</small>
    </div>

    <!-- Notification Container -->
    <div id="notification-container"></div>

    <!-- Stats Cards -->
    <div class="row g-4 mb-4 stagger-2">
        <!-- Status Robot Card -->
        <div class="col-md-6">
            <div class="card h-100">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center mb-3">
                        <div class="d-flex align-items-center justify-content-center rounded-3 me-3" style="width: 48px; height: 48px; flex-shrink: 0; background: var(--accent-dim); border: 1px solid rgba(34, 211, 238, 0.15);">
                            <img src="{{ asset('robot.png') }}" alt="Robot status indicator" style="width: 26px; height: 26px; object-fit: contain; filter: drop-shadow(0 1px 3px rgba(0,0,0,0.4));">
                        </div>
                        <div>
                            <p class="text-uppercase text-secondary fw-semibold mb-1" style="font-size: 0.68rem; letter-spacing: 0.1em;">Robot status</p>
                            <h3 class="mb-0 fw-bold" id="statusRobot">Standby</h3>
                        </div>
                    </div>
                    <div class="rounded-3 p-3 d-flex align-items-center" style="background: rgba(255,255,255,0.025); border: 1px solid var(--surface-border);" role="alert">
                        <i class="fas fa-info-circle me-2" style="color: var(--accent);"></i>
                        <span id="statusInfo" class="small text-secondary fw-medium">Ready for operation</span>
                    </div>
                </div>
            </div>
        </div>

        <!-- Battery Monitoring Card -->
        <div class="col-md-6">
            <div class="card h-100">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center justify-content-between mb-3">
                        <div class="d-flex align-items-center">
                            <div class="p-3 rounded-3 text-success me-3" style="background: rgba(52, 211, 153, 0.1); border: 1px solid rgba(52, 211, 153, 0.15);">
                                <i class="fas fa-battery-three-quarters fa-lg" id="batteryIcon"></i>
                            </div>
                            <div>
                                <p class="text-uppercase text-secondary fw-semibold mb-1" style="font-size: 0.68rem; letter-spacing: 0.1em;">Battery level</p>
                                <h3 class="mb-0 fw-bold" id="batteryPercent">85%</h3>
                            </div>
                        </div>
                        <div class="text-end">
                            <i class="fas fa-bolt fa-lg" style="color: var(--warning);"></i>
                        </div>
                    </div>

                    <div class="progress mb-3" style="height: 6px;">
                        <div class="progress-bar bg-success" id="batteryBar" role="progressbar" style="width: 85%"></div>
                    </div>

                    <div class="d-flex justify-content-between align-items-center small text-secondary fw-medium">
                        <span><i class="far fa-clock me-1"></i> <span id="batteryEstimate">Est: —</span></span>
                        <span><i class="fas fa-bolt me-1"></i> <span id="batteryVoltage">— V</span></span>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Main Control Section -->
    <div class="row g-4 mb-4 stagger-3">
        <!-- Main Controls -->
        <div class="col-lg-8">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-gamepad me-2" style="color: var(--accent);"></i>
                    <h6 class="m-0 fw-semibold text-white" style="font-size: 0.9rem;">Main controls</h6>
                </div>
                <div class="card-body p-4">
                    <div class="row g-3">
                        <div class="col-6">
                            <button class="btn btn-primary-glow w-100 py-4 rounded-3 d-flex flex-column align-items-center justify-content-center" style="border-radius: var(--radius-md);" onclick="startVacuum()">
                                <i class="fas fa-play fa-2x mb-2"></i>
                                <span class="fw-semibold tracking-wide" style="font-size: 0.85rem;">START</span>
                            </button>
                        </div>
                        <div class="col-6">
                            <button class="btn btn-danger-glow w-100 py-4 rounded-3 d-flex flex-column align-items-center justify-content-center" style="border-radius: var(--radius-md);" onclick="stopVacuum()">
                                <i class="fas fa-stop fa-2x mb-2"></i>
                                <span class="fw-semibold tracking-wide" style="font-size: 0.85rem;">STOP</span>
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
                    <i class="fas fa-fan me-2" style="color: var(--accent);"></i>
                    <h6 class="m-0 fw-semibold text-white" style="font-size: 0.9rem;">Suction power</h6>
                </div>
                <div class="card-body p-0">
                    <div class="list-group list-group-flush">
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white border-bottom border-light border-opacity-10 py-3" onclick="setPowerMode('eco')" id="ecoBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-leaf text-success me-3 fa-lg"></i>
                                <div>
                                    <div class="fw-semibold" style="font-size: 0.9rem;">Eco mode</div>
                                    <small class="text-secondary">Energy saving, quiet</small>
                                </div>
                            </div>
                        </button>
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white border-bottom border-light border-opacity-10 py-3 active-mode" onclick="setPowerMode('normal')" id="normalBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-wind me-3 fa-lg" style="color: var(--accent);"></i>
                                <div>
                                    <div class="fw-semibold" style="font-size: 0.9rem;">Normal mode</div>
                                    <small class="text-secondary">Standard suction</small>
                                </div>
                            </div>
                        </button>
                        <button type="button" class="list-group-item list-group-item-action bg-transparent text-white py-3" onclick="setPowerMode('strong')" id="strongBtn">
                            <div class="d-flex align-items-center">
                                <i class="fas fa-tornado text-danger me-3 fa-lg"></i>
                                <div>
                                    <div class="fw-semibold" style="font-size: 0.9rem;">Strong mode</div>
                                    <small class="text-secondary">Maximum deep cleaning</small>
                                </div>
                            </div>
                        </button>
                    </div>

                    <div class="p-3" style="background: rgba(0,0,0,0.2);">
                        <small class="text-secondary d-block text-center fw-medium" id="powerInfo">Current: <span class="text-white fw-semibold">NORMAL</span></small>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Command History Section -->
    <div class="row g-4 stagger-4">
        <div class="col-12">
            <div class="card">
                <div class="card-header py-3 d-flex align-items-center justify-content-between">
                    <div class="d-flex align-items-center">
                        <i class="fas fa-history me-2" style="color: var(--accent);"></i>
                        <h6 class="m-0 fw-semibold text-white" style="font-size: 0.9rem;">Command history</h6>
                    </div>
                    <div class="d-flex align-items-center gap-2">
                        <small class="text-secondary d-none d-md-inline fw-medium" id="cmdLogUpdatedAt">Updated just now</small>
                        <button class="btn btn-sm btn-outline-secondary border-0" onclick="fetchCommandLogs()" title="Refresh" id="cmdLogRefreshBtn" style="border-radius: 6px;">
                            <i class="fas fa-sync-alt"></i>
                        </button>
                        <button class="btn btn-sm btn-outline-secondary border-0" data-bs-toggle="collapse" data-bs-target="#cmdHistoryCollapse" aria-expanded="true" aria-controls="cmdHistoryCollapse" title="Minimize" style="border-radius: 6px;">
                            <i class="fas fa-chevron-up collapse-icon"></i>
                        </button>
                    </div>
                </div>
                <div class="collapse show" id="cmdHistoryCollapse">
                    <div class="card-body p-0">
                        <!-- Desktop Table View -->
                        <div class="table-responsive d-none d-md-block history-table-container">
                            <table class="table table-dark table-hover mb-0 align-middle" style="background: transparent;">
                                <thead>
                                    <tr style="border-bottom: 1px solid rgba(255,255,255,0.06);">
                                        <th class="ps-4 py-3 text-secondary text-uppercase fw-semibold" style="font-size: 0.68rem; letter-spacing: 0.1em;">Command</th>
                                        <th class="py-3 text-secondary text-uppercase fw-semibold" style="font-size: 0.68rem; letter-spacing: 0.1em;">Source</th>
                                        <th class="py-3 text-secondary text-uppercase fw-semibold" style="font-size: 0.68rem; letter-spacing: 0.1em;">Status</th>
                                        <th class="py-3 text-secondary text-uppercase fw-semibold" style="font-size: 0.68rem; letter-spacing: 0.1em;">Response</th>
                                        <th class="pe-4 py-3 text-secondary text-uppercase text-end fw-semibold" style="font-size: 0.68rem; letter-spacing: 0.1em;">Time</th>
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
                        <div class="d-md-none history-mobile-container" id="cmdLogMobileList">
                            <div class="text-center text-secondary py-4">
                                <i class="fas fa-spinner fa-spin me-2"></i>Loading...
                            </div>
                        </div>

                        <!-- Empty State -->
                        <div class="text-center py-5 d-none" id="cmdLogEmpty">
                            <i class="fas fa-inbox fa-3x text-secondary mb-3" style="opacity: 0.25;"></i>
                            <p class="text-secondary mb-0 fw-medium">No commands recorded yet</p>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Inline script for page specific logic -->
    <script>
        // ===== CONFIGURATION =====
        const API_BASE_URL = "/v1/vacuum";
        const ESP32_TIMEOUT = 3000;

        // ===== GLOBAL STATE =====
        let esp32Ip = null;
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
                <div class="alert alert-dark border-start ${borderClass} border-4 shadow-lg fade show" role="alert" style="background: rgba(21, 30, 48, 0.95); border-radius: 12px;">
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
                const res = await fetch(`${API_BASE_URL}/device`).then(r => r.json());
                if (res.success && res.data) {
                    esp32Ip = res.data.ip_address;
                    console.log(`ESP32 discovered at: ${esp32Ip}`);
                    return true;
                }
            } catch (err) {
                console.warn('No ESP32 device registered');
            }
            esp32Ip = null;
            return false;
        }

        // ===== DIRECT HTTP TO ESP32 =====
        async function sendToEsp32(endpoint, payload) {
            if (!esp32Ip) throw new Error('No ESP32 IP');

            const startTime = Date.now();
            try {
                const controller = new AbortController();
                const timeoutId = setTimeout(() => controller.abort(), ESP32_TIMEOUT);
                const res = await fetch(`http://${esp32Ip}/${endpoint}`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(payload),
                    signal: controller.signal
                });
                clearTimeout(timeoutId);
                const data = await res.json();
                const elapsed = Date.now() - startTime;
                if (!res.ok) {
                    const err = new Error('HTTP ' + res.status);
                    err.responseTime = elapsed;
                    err.responseJSON = data;
                    throw err;
                }
                return { ...data, responseTime: elapsed };
            } catch (err) {
                if (!err.responseTime) err.responseTime = Date.now() - startTime;
                throw err;
            }
        }

        // ===== LOG COMMAND TO LARAVEL (history) =====
        function logCommandToServer(command, status, responseTimeMs, esp32Ip) {
            const csrfToken = document.querySelector('meta[name="csrf-token"]').getAttribute('content');
            fetch(`${API_BASE_URL}/command-log`, {
                method: 'POST',
                headers: { 'X-CSRF-TOKEN': csrfToken, 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    command, source: 'web', status,
                    response_time_ms: responseTimeMs, esp32_ip: esp32Ip
                })
            }).catch(err => console.error('Failed to log command:', err));
        }

        // ===== COMMAND SENDER (Direct HTTP Only) =====
        async function sendVacuumCommand(command) {
            if (!esp32Ip) {
                showNotification('error', 'ESP32 not connected. Cannot send command.');
                return;
            }

            try {
                const res = await sendToEsp32('command', { command: command });

                if (res.blocked) {
                    showNotification('warning', `Battery: ${res.message}`, true);
                    logCommandToServer(command, 'failed', res.responseTime, esp32Ip);
                    return;
                }

                showNotification('success', `${command.toUpperCase()} sent (${res.responseTime}ms)`);
                logCommandToServer(command, 'success', res.responseTime, esp32Ip);

                if (res.state) {
                    updateStatusUI({ state: res.state, power_mode: res.power_mode });
                }
            } catch (err) {
                if (err.responseJSON && err.responseJSON.blocked) {
                    showNotification('warning', `Battery: ${err.responseJSON.message}`, true);
                    logCommandToServer(command, 'failed', err.responseTime || 0, esp32Ip);
                    return;
                }
                showNotification('error', 'ESP32 unreachable. Try again.');
                logCommandToServer(command, 'failed', err.responseTime || 0, esp32Ip);
                discoverEsp32();
            }
        }

        async function setPowerMode(mode) {
            const powerValues = { 'eco': 150, 'normal': 200, 'strong': 255 };

            if (!esp32Ip) {
                showNotification('error', 'ESP32 not connected. Cannot change mode.');
                return;
            }

            try {
                const res = await sendToEsp32('command', {
                    command: mode,
                    value: powerValues[mode]
                });
                showNotification('success', `${mode.toUpperCase()} mode set (${res.responseTime}ms)`);
                logCommandToServer(mode, 'success', res.responseTime, esp32Ip);
                updatePowerModeUI(mode);
            } catch (err) {
                showNotification('error', 'ESP32 unreachable. Try again.');
                logCommandToServer(mode, 'failed', err.responseTime || 0, esp32Ip);
                discoverEsp32();
            }
        }

        // ===== RESET CONNECTION =====
        function resetEsp32Connection() {
            if (confirm("Are you sure you want to reset the ESP32 connection? This will clear the saved IP from the database. You will need to reset the WiFi on the ESP32 to register the new IP.")) {
                const csrfToken = document.querySelector('meta[name="csrf-token"]').getAttribute('content');
                fetch(`${API_BASE_URL}/reset-devices`, {
                    method: 'POST',
                    headers: { 'X-CSRF-TOKEN': csrfToken }
                })
                .then(r => r.json())
                .then(res => {
                    if (res.success) {
                        showNotification('success', res.message, true);
                        esp32Ip = null;
                        updateStatusUI({ state: 'standby', power_mode: 'normal' });
                    }
                })
                .catch(() => showNotification('error', 'Failed to reset connection.'));
            }
        }

        // ===== BUTTON LISTENERS =====
        window.startVacuum = () => sendVacuumCommand('start');
        window.stopVacuum = () => sendVacuumCommand('stop');
        window.returnToBase = () => sendVacuumCommand('return_home');
        window.setPowerMode = setPowerMode;

        // ===== FETCHING DATA =====
        function fetchFullStatus() {
            fetch(`${API_BASE_URL}/full-status`)
                .then(r => r.json())
                .then(res => {
                    if (res.success) updateUI(res.vacuum, res.battery);
                })
                .catch(() => {});
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

            if (data.state === 'stopping' && lastBatteryEvent === 'auto_stop_low_battery') {
                infoText = 'Auto-stopped: Battery depleted';
            }

            statusEl.innerHTML = `<span class="badge ${badgeClass}" style="font-size: 0.85rem; padding: 0.45em 0.9em; border-radius: 6px; font-weight: 600;">${statusText}</span>`;
            infoEl.textContent = infoText;

            updatePowerModeUI(data.power_mode);
        }

        function updateBatteryUI(data) {
            const percent = data.battery_percent || data.percent;
            const estimate = data.estimated_time;
            const voltage = data.battery_voltage || data.voltage;

            document.getElementById('batteryPercent').textContent = `${percent}%`;
            document.getElementById('batteryEstimate').textContent = estimate ? `Est: ${estimate}` : '';

            if (voltage) {
                document.getElementById('batteryVoltage').textContent = `${parseFloat(voltage).toFixed(1)} V`;
            }

            const bar = document.getElementById('batteryBar');
            bar.style.width = `${percent}%`;

            bar.className = 'progress-bar';
            if(percent > 50) bar.classList.add('bg-success');
            else if(percent > 20) bar.classList.add('bg-warning');
            else bar.classList.add('bg-danger');

            const iconEl = document.getElementById('batteryIcon');
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
                    btn.classList.add('active-mode');
                    btn.classList.remove('bg-transparent');
                } else {
                    btn.classList.remove('active-mode');
                    btn.classList.add('bg-transparent');
                }
            });
            document.getElementById('powerInfo').innerHTML = `Current: <span class="text-white fw-semibold">${mode.toUpperCase()}</span>`;
        }

        // ===== BATTERY EVENT POLLING =====
        let lastBatteryEvent = null;
        let lastBatteryEventId = null;

        function fetchBatteryEvents() {
            fetch(`${API_BASE_URL}/battery-events/latest`)
                .then(r => r.json())
                .then(res => {
                    if (res.success && res.data) {
                        const event = res.data;
                        const eventKey = event.event + '_' + event.created_at;
                        if (eventKey !== lastBatteryEventId) {
                            lastBatteryEventId = eventKey;
                            lastBatteryEvent = event.event;
                            if (event.event === 'auto_stop_low_battery') {
                                showNotification('error',
                                    `Robot auto-stopped. Battery depleted (${event.battery_percent}%, ${parseFloat(event.battery_voltage).toFixed(1)}V). Please charge.`, true);
                                fetchFullStatus();
                            } else if (event.event === 'low_battery_warning') {
                                showNotification('warning',
                                    `Low battery: ${event.battery_percent}% remaining (${parseFloat(event.battery_voltage).toFixed(1)}V). Robot will auto-stop at 0%.`, true);
                            }
                        }
                    } else {
                        lastBatteryEvent = null;
                    }
                })
                .catch(() => {});
        }

        // ===== COMMAND LOG HISTORY =====
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
                ? '<i class="fas fa-globe" style="color: var(--accent);"></i> Web'
                : '<i class="fas fa-microchip" style="color: var(--warning);"></i> ESP32';
            const respTime = log.response_time_ms != null ? log.response_time_ms + 'ms' : '—';

            return `<tr>
                <td class="ps-4 py-3"><span class="cmd-badge ${cmd.css}"><i class="fas ${cmd.icon}"></i>${cmd.label}</span></td>
                <td class="py-3"><span class="source-badge">${src}</span></td>
                <td class="py-3"><span class="status-pill ${st.css}"><i class="fas ${st.icon}"></i>${st.label}</span></td>
                <td class="py-3"><span class="text-secondary fw-medium" style="font-size:0.8rem;">${respTime}</span></td>
                <td class="pe-4 py-3 text-end"><span class="text-secondary fw-medium" style="font-size:0.8rem;" title="${log.created_at}">${timeAgo(log.created_at)}</span></td>
            </tr>`;
        }

        function renderMobileItem(log) {
            const cmd = getCommandMeta(log.command);
            const st  = getStatusMeta(log.status);
            const srcIcon = log.source === 'web'
                ? '<i class="fas fa-globe" style="color: var(--accent);"></i>'
                : '<i class="fas fa-microchip" style="color: var(--warning);"></i>';
            const respTime = log.response_time_ms != null ? log.response_time_ms + 'ms' : '';

            return `<div class="cmd-mobile-item">
                <div class="d-flex align-items-center justify-content-between mb-1">
                    <span class="cmd-badge ${cmd.css}"><i class="fas ${cmd.icon}"></i>${cmd.label}</span>
                    <span class="text-secondary fw-medium" style="font-size:0.72rem;" title="${log.created_at}">${timeAgo(log.created_at)}</span>
                </div>
                <div class="d-flex align-items-center gap-3 mt-2">
                    <span class="source-badge">${srcIcon} ${log.source}</span>
                    <span class="status-pill ${st.css}"><i class="fas ${st.icon}"></i>${st.label}</span>
                    ${respTime ? `<span class="text-secondary fw-medium" style="font-size:0.72rem;"><i class="fas fa-bolt me-1"></i>${respTime}</span>` : ''}
                </div>
            </div>`;
        }

        function fetchCommandLogs() {
            const btn = document.getElementById('cmdLogRefreshBtn');
            const icon = btn.querySelector('i');
            icon.classList.add('spin');

            fetch(`${API_BASE_URL}/command-logs?limit=15`)
                .then(r => r.json())
                .then(res => {
                    icon.classList.remove('spin');
                    if (!res.success || !res.data || res.data.length === 0) {
                        document.getElementById('cmdLogTableBody').innerHTML = '';
                        document.getElementById('cmdLogMobileList').innerHTML = '';
                        document.getElementById('cmdLogEmpty').classList.remove('d-none');
                        return;
                    }
                    document.getElementById('cmdLogEmpty').classList.add('d-none');
                    document.getElementById('cmdLogTableBody').innerHTML = res.data.map(renderDesktopRow).join('');
                    document.getElementById('cmdLogMobileList').innerHTML = res.data.map(renderMobileItem).join('');
                    document.getElementById('cmdLogUpdatedAt').textContent = 'Updated just now';
                })
                .catch(() => icon.classList.remove('spin'));
        }

        // ===== INITIALIZATION =====
        let statusInterval, batteryEventInterval, cmdLogInterval;
        let isTabVisible = true;

        document.addEventListener('DOMContentLoaded', async () => {
            await discoverEsp32();
            fetchFullStatus();
            statusInterval = setInterval(fetchFullStatus, 20000);
            fetchBatteryEvents();
            batteryEventInterval = setInterval(fetchBatteryEvents, 10000);
            fetchCommandLogs();
            cmdLogInterval = setInterval(fetchCommandLogs, 15000);
        });

        document.addEventListener('visibilitychange', () => {
            if (document.hidden) {
                isTabVisible = false;
                clearInterval(statusInterval);
                clearInterval(batteryEventInterval);
                clearInterval(cmdLogInterval);
                statusInterval = setInterval(fetchFullStatus, 120000);
                batteryEventInterval = setInterval(fetchBatteryEvents, 60000);
                cmdLogInterval = setInterval(fetchCommandLogs, 60000);
            } else {
                isTabVisible = true;
                clearInterval(statusInterval);
                clearInterval(batteryEventInterval);
                clearInterval(cmdLogInterval);
                fetchFullStatus();
                fetchBatteryEvents();
                fetchCommandLogs();
                statusInterval = setInterval(fetchFullStatus, 20000);
                batteryEventInterval = setInterval(fetchBatteryEvents, 10000);
                cmdLogInterval = setInterval(fetchCommandLogs, 15000);
            }
        });

        window.addEventListener('beforeunload', () => {
            clearInterval(statusInterval);
            clearInterval(batteryEventInterval);
            clearInterval(cmdLogInterval);
        });
    </script>
</x-master>
