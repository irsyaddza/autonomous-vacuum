<x-master>
    <!-- Header -->
    <div class="d-flex align-items-center justify-content-between mb-4 stagger-1">
        <div>
            <h1 class="h2 mb-1 fw-bold text-white"><i class="fas fa-stethoscope me-2"></i>Sensor Diagnostic</h1>
            <p class="text-secondary mb-0">Real-time sensor monitoring for debugging.</p>
        </div>
        <div class="d-flex align-items-center gap-3">
            <span class="badge bg-dark border border-secondary" id="connectionBadge">
                <i class="fas fa-circle text-danger me-1" id="connectionDot"></i>
                <span id="connectionText">Disconnected</span>
            </span>
            <button class="btn btn-sm btn-outline-light" id="toggleBtn" onclick="togglePolling()">
                <i class="fas fa-play me-1" id="toggleIcon"></i>
                <span id="toggleText">Start</span>
            </button>
        </div>
    </div>

    <!-- ESP32 Status Bar -->
    <div class="card mb-4 stagger-2">
        <div class="card-body py-3">
            <div class="row text-center">
                <div class="col-md-3 col-6 mb-2 mb-md-0">
                    <small class="text-secondary d-block">State</small>
                    <span class="fw-bold text-white" id="robotState">—</span>
                </div>
                <div class="col-md-3 col-6 mb-2 mb-md-0">
                    <small class="text-secondary d-block">Direction</small>
                    <span class="fw-bold text-white" id="robotDirection">—</span>
                </div>
                <div class="col-md-3 col-6">
                    <small class="text-secondary d-block">Battery</small>
                    <span class="fw-bold text-white" id="robotBattery">—</span>
                </div>
                <div class="col-md-3 col-6">
                    <small class="text-secondary d-block">Uptime</small>
                    <span class="fw-bold text-white" id="robotUptime">—</span>
                </div>
            </div>
        </div>
    </div>

    <!-- Sensor Cards -->
    <div class="row g-4 stagger-3">

        <!-- Obstacle Avoidance Sensors -->
        <div class="col-lg-6">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-shield-alt me-2 text-warning"></i>
                    <h6 class="m-0 fw-bold text-white">Obstacle Avoidance (IR1, IR2, IR3)</h6>
                </div>
                <div class="card-body p-4">
                    <!-- Robot Visual -->
                    <div class="text-center mb-4">
                        <div class="d-flex justify-content-center align-items-center gap-4">
                            <!-- Left Sensor -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="obs-left-indicator">
                                    <i class="fas fa-wifi fa-rotate-270 fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR1 Left</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-left-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-left-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Front Sensor -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="obs-front-indicator">
                                    <i class="fas fa-wifi fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR2 Front</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-front-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-front-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Right Sensor -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="obs-right-indicator">
                                    <i class="fas fa-wifi fa-rotate-90 fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR3 Right</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-right-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="obs-right-db">DEB: —</span>
                                </div>
                            </div>
                        </div>
                    </div>

                    <!-- Legend -->
                    <div class="alert alert-dark bg-opacity-25 border-0 mb-0 small">
                        <i class="fas fa-info-circle me-1 text-info"></i>
                        <strong>RAW</strong> = Nilai mentah dari pin (0/1). <strong>DEB</strong> = Nilai setelah debounce (true/false).
                        <br><span class="text-success">● Hijau</span> = Aman (no obstacle).
                        <span class="text-danger">● Merah</span> = Terdeteksi (obstacle).
                    </div>
                </div>
            </div>
        </div>

        <!-- Cliff Detection Sensors -->
        <div class="col-lg-6">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-mountain me-2 text-danger"></i>
                    <h6 class="m-0 fw-bold text-white">Cliff Detection (IR4, IR5, IR6)</h6>
                </div>
                <div class="card-body p-4">
                    <!-- Cliff Visual -->
                    <div class="text-center mb-4">
                        <div class="d-flex justify-content-center align-items-center gap-4">
                            <!-- Left Cliff -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="clf-left-indicator">
                                    <i class="fas fa-arrow-down fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR4 Left</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-left-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-left-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Front Cliff -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="clf-front-indicator">
                                    <i class="fas fa-arrow-down fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR5 Front</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-front-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-front-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Right Cliff -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="clf-right-indicator">
                                    <i class="fas fa-arrow-down fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary">IR6 Right</small>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-right-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge bg-dark border" id="clf-right-db">DEB: —</span>
                                </div>
                            </div>
                        </div>
                    </div>

                    <!-- Legend -->
                    <div class="alert alert-dark bg-opacity-25 border-0 mb-0 small">
                        <i class="fas fa-info-circle me-1 text-info"></i>
                        <strong>RAW</strong> = Nilai mentah dari pin (0/1). <strong>DEB</strong> = Nilai setelah debounce (true/false).
                        <br><span class="text-success">● Hijau</span> = Aman (lantai terdeteksi).
                        <span class="text-danger">● Merah</span> = Bahaya (jurang/cliff).
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Raw JSON Log -->
    <div class="card mt-4 stagger-4">
        <div class="card-header py-3 d-flex align-items-center justify-content-between">
            <div>
                <i class="fas fa-terminal me-2 text-success"></i>
                <span class="fw-bold text-white">Raw JSON Log</span>
            </div>
            <button class="btn btn-sm btn-outline-secondary" onclick="clearLog()">
                <i class="fas fa-trash me-1"></i>Clear
            </button>
        </div>
        <div class="card-body p-0">
            <pre class="mb-0 p-3 text-success small" id="jsonLog" style="max-height: 300px; overflow-y: auto; background: rgba(0,0,0,0.3); font-family: 'Courier New', monospace;">Tekan "Start" untuk memulai diagnostic...</pre>
        </div>
    </div>

    <style>
        .sensor-indicator {
            width: 80px;
            height: 80px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            background: rgba(108, 117, 125, 0.2);
            border: 2px solid rgba(108, 117, 125, 0.4);
            color: #6c757d;
            transition: all 0.3s ease;
            margin: 0 auto;
        }
        .sensor-indicator.safe {
            background: rgba(25, 135, 84, 0.15);
            border-color: rgba(25, 135, 84, 0.6);
            color: #198754;
            box-shadow: 0 0 15px rgba(25, 135, 84, 0.3);
        }
        .sensor-indicator.danger {
            background: rgba(220, 53, 69, 0.2);
            border-color: rgba(220, 53, 69, 0.7);
            color: #dc3545;
            box-shadow: 0 0 20px rgba(220, 53, 69, 0.4);
            animation: pulse-danger 1s ease infinite;
        }
        @keyframes pulse-danger {
            0%, 100% { box-shadow: 0 0 15px rgba(220, 53, 69, 0.3); }
            50% { box-shadow: 0 0 25px rgba(220, 53, 69, 0.6); }
        }
    </style>

    <script>
        const API_BASE_URL = "/v1/vacuum";
        const POLL_INTERVAL = 300; // ms - poll every 300ms for real-time feel

        let esp32Ip = null;
        let pollingActive = false;
        let pollTimer = null;
        let logLines = [];
        const MAX_LOG_LINES = 50;

        // ===== ESP32 Discovery =====
        async function discoverEsp32() {
            try {
                const res = await $.get(`${API_BASE_URL}/device`);
                if (res.success && res.data) {
                    esp32Ip = res.data.ip_address;
                    updateConnection(true);
                    return true;
                }
            } catch (err) {
                console.warn('No ESP32 device registered');
            }
            esp32Ip = null;
            updateConnection(false);
            return false;
        }

        function updateConnection(connected) {
            const dot = document.getElementById('connectionDot');
            const text = document.getElementById('connectionText');
            if (connected) {
                dot.className = 'fas fa-circle text-success me-1';
                text.textContent = `Connected (${esp32Ip})`;
            } else {
                dot.className = 'fas fa-circle text-danger me-1';
                text.textContent = 'Disconnected';
            }
        }

        // ===== Polling Control =====
        function togglePolling() {
            if (pollingActive) {
                stopPolling();
            } else {
                startPolling();
            }
        }

        async function startPolling() {
            if (!esp32Ip) {
                const found = await discoverEsp32();
                if (!found) {
                    addLog('ERROR: ESP32 tidak ditemukan. Pastikan robot terhubung ke WiFi.');
                    return;
                }
            }

            pollingActive = true;
            document.getElementById('toggleIcon').className = 'fas fa-pause me-1';
            document.getElementById('toggleText').textContent = 'Pause';
            document.getElementById('toggleBtn').className = 'btn btn-sm btn-outline-warning';
            addLog('>>> Diagnostic polling STARTED <<<');
            pollDiagnostic();
        }

        function stopPolling() {
            pollingActive = false;
            if (pollTimer) clearTimeout(pollTimer);
            document.getElementById('toggleIcon').className = 'fas fa-play me-1';
            document.getElementById('toggleText').textContent = 'Start';
            document.getElementById('toggleBtn').className = 'btn btn-sm btn-outline-light';
            addLog('>>> Diagnostic polling PAUSED <<<');
        }

        // ===== Fetch Diagnostic Data =====
        async function pollDiagnostic() {
            if (!pollingActive || !esp32Ip) return;

            try {
                const res = await $.ajax({
                    url: `http://${esp32Ip}/diagnostic`,
                    type: 'GET',
                    timeout: 2000
                });

                if (res.success) {
                    updateSensorUI(res);
                    updateStatusBar(res);
                    addLog(JSON.stringify(res));
                }
            } catch (err) {
                addLog('ERROR: Gagal mengambil data dari ESP32');
                updateConnection(false);
            }

            if (pollingActive) {
                pollTimer = setTimeout(pollDiagnostic, POLL_INTERVAL);
            }
        }

        // ===== Update Sensor UI =====
        function updateSensorUI(data) {
            // Obstacle sensors
            updateSensorCard('obs-left', data.obstacle_raw.left, data.obstacle_debounced.left, 'obstacle');
            updateSensorCard('obs-front', data.obstacle_raw.front, data.obstacle_debounced.front, 'obstacle');
            updateSensorCard('obs-right', data.obstacle_raw.right, data.obstacle_debounced.right, 'obstacle');

            // Cliff sensors
            updateSensorCard('clf-left', data.cliff_raw.left, data.cliff_debounced.left, 'cliff');
            updateSensorCard('clf-front', data.cliff_raw.front, data.cliff_debounced.front, 'cliff');
            updateSensorCard('clf-right', data.cliff_raw.right, data.cliff_debounced.right, 'cliff');
        }

        function updateSensorCard(prefix, rawVal, debouncedVal, type) {
            const indicator = document.getElementById(`${prefix}-indicator`);
            const rawBadge = document.getElementById(`${prefix}-raw`);
            const dbBadge = document.getElementById(`${prefix}-db`);

            // Display raw value
            rawBadge.textContent = `RAW: ${rawVal}`;
            rawBadge.className = `badge ${rawVal === 1 ? 'bg-warning text-dark' : 'bg-dark'} border`;

            // Display debounced value
            dbBadge.textContent = `DEB: ${debouncedVal ? 'true' : 'false'}`;
            dbBadge.className = `badge ${debouncedVal ? 'bg-danger' : 'bg-success'} border-0`;

            // Update indicator visual
            // For obstacle: debounced true = danger (obstacle detected)
            // For cliff: debounced true = danger (cliff detected)
            if (debouncedVal) {
                indicator.className = 'sensor-indicator danger';
            } else {
                indicator.className = 'sensor-indicator safe';
            }
        }

        function updateStatusBar(data) {
            document.getElementById('robotState').textContent = data.state || '—';
            document.getElementById('robotDirection').textContent = data.direction || '—';

            if (data.battery) {
                document.getElementById('robotBattery').textContent =
                    `${data.battery.percent}% (${data.battery.voltage.toFixed(1)}V)`;
            }

            if (data.uptime_ms) {
                const sec = Math.floor(data.uptime_ms / 1000);
                const min = Math.floor(sec / 60);
                const hr = Math.floor(min / 60);
                document.getElementById('robotUptime').textContent =
                    `${hr}h ${min % 60}m ${sec % 60}s`;
            }
        }

        // ===== Log =====
        function addLog(text) {
            const now = new Date().toLocaleTimeString('en-US', { hour12: false });
            logLines.push(`[${now}] ${text}`);
            if (logLines.length > MAX_LOG_LINES) logLines.shift();

            const logEl = document.getElementById('jsonLog');
            logEl.textContent = logLines.join('\n');
            logEl.scrollTop = logEl.scrollHeight;
        }

        function clearLog() {
            logLines = [];
            document.getElementById('jsonLog').textContent = 'Log cleared.';
        }

        // ===== Init =====
        document.addEventListener('DOMContentLoaded', async () => {
            await discoverEsp32();
        });
    </script>
</x-master>
