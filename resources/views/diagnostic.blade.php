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

    <!-- Timing Settings Panel -->
    <div class="card mt-4 stagger-4">
        <div class="card-header py-3 d-flex align-items-center justify-content-between">
            <div>
                <i class="fas fa-sliders-h me-2 text-info"></i>
                <span class="fw-bold text-white">Timing Settings</span>
                <small class="text-secondary ms-2">(Live tuning tanpa flash ulang)</small>
            </div>
            <div class="d-flex gap-2">
                <button class="btn btn-sm btn-outline-secondary" onclick="resetTimingDefaults()">
                    <i class="fas fa-undo me-1"></i>Reset Default
                </button>
                <button class="btn btn-sm btn-primary" onclick="saveTimingSettings()">
                    <i class="fas fa-save me-1"></i>Save to ESP32
                </button>
            </div>
        </div>
        <div class="card-body p-4">
            <div class="row g-4">
                <!-- Obstacle Avoidance -->
                <div class="col-md-4">
                    <h6 class="text-warning mb-3"><i class="fas fa-shield-alt me-1"></i> Obstacle Avoidance</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Mundur (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_backupDuration" min="100" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Belok Min (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationMin" min="100" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Belok Max (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationMax" min="200" max="3000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Belok Kecil (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationSmall" min="50" max="1000" step="50">
                    </div>
                </div>
                <!-- Cliff Avoidance -->
                <div class="col-md-4">
                    <h6 class="text-danger mb-3"><i class="fas fa-mountain me-1"></i> Cliff Avoidance</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Mundur Cliff (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_cliffBackupDuration" min="200" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Putar 180° (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_cliffTurnDuration" min="300" max="3000" step="50">
                    </div>
                    <h6 class="text-primary mb-3 mt-4"><i class="fas fa-exclamation-triangle me-1"></i> Stuck Detection</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Jumlah Obstacle</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_stuckObstacleCount" min="2" max="10" step="1">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Jendela Waktu (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_stuckTimeWindow" min="1000" max="10000" step="500">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Escape Turn (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_escapeTurnDuration" min="500" max="3000" step="100">
                    </div>
                </div>
                <!-- Spiral Pattern -->
                <div class="col-md-4">
                    <h6 class="text-success mb-3"><i class="fas fa-sync-alt me-1"></i> Spiral Pattern</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Durasi Awal (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralInitialDuration" min="200" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Increment (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralIncrement" min="50" max="500" step="25">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Max Durasi (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralMaxDuration" min="1000" max="10000" step="500">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary">Belok Spiral (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralTurnDuration" min="100" max="1000" step="50">
                    </div>
                </div>
            </div>
            <div class="alert alert-dark bg-opacity-25 border-0 mt-3 mb-0 small">
                <i class="fas fa-info-circle me-1 text-info"></i>
                Perubahan langsung berlaku di ESP32 <strong>tanpa flash ulang</strong>. Nilai akan kembali ke default setelah ESP32 di-restart.
                Klik <strong>Reset Default</strong> untuk mengembalikan semua ke nilai di <code>config.h</code>.
            </div>
        </div>
    </div>

    <!-- Raw JSON Log -->
    <div class="card mt-4 stagger-5">
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
                    // Load timing settings on first successful poll
                    if (res.timing && !timingLoaded) {
                        populateTimingFields(res.timing);
                        timingLoaded = true;
                    }
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

        // ===== Timing Settings =====
        let timingLoaded = false;
        const timingKeys = [
            'backupDuration', 'turnDurationMin', 'turnDurationMax', 'turnDurationSmall',
            'cliffBackupDuration', 'cliffTurnDuration',
            'spiralInitialDuration', 'spiralIncrement', 'spiralMaxDuration', 'spiralTurnDuration',
            'stuckObstacleCount', 'stuckTimeWindow', 'escapeTurnDuration'
        ];

        function populateTimingFields(timing) {
            timingKeys.forEach(key => {
                const el = document.getElementById(`t_${key}`);
                if (el && timing[key] !== undefined) {
                    el.value = timing[key];
                }
            });
        }

        async function saveTimingSettings() {
            if (!esp32Ip) {
                addLog('ERROR: ESP32 tidak terhubung');
                return;
            }

            const settings = {};
            timingKeys.forEach(key => {
                const el = document.getElementById(`t_${key}`);
                if (el && el.value) {
                    settings[key] = parseInt(el.value);
                }
            });

            try {
                const res = await $.ajax({
                    url: `http://${esp32Ip}/settings`,
                    type: 'POST',
                    contentType: 'application/json',
                    data: JSON.stringify(settings),
                    timeout: 3000
                });
                addLog('Settings SAVED: ' + JSON.stringify(settings));
                timingLoaded = false; // Reload on next poll
            } catch (err) {
                addLog('ERROR: Gagal menyimpan settings');
            }
        }

        async function resetTimingDefaults() {
            if (!esp32Ip) {
                addLog('ERROR: ESP32 tidak terhubung');
                return;
            }

            try {
                await $.ajax({
                    url: `http://${esp32Ip}/settings`,
                    type: 'POST',
                    contentType: 'application/json',
                    data: JSON.stringify({ resetDefaults: true }),
                    timeout: 3000
                });
                addLog('Settings RESET to defaults');
                timingLoaded = false; // Reload on next poll
            } catch (err) {
                addLog('ERROR: Gagal reset settings');
            }
        }

        // ===== Init =====
        document.addEventListener('DOMContentLoaded', async () => {
            await discoverEsp32();
        });
    </script>
</x-master>
