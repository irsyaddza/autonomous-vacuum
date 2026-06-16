<x-master>
    <!-- Header -->
    <div class="d-flex align-items-center justify-content-between mb-4 stagger-1">
        <div>
            <p class="text-secondary mb-1 small fw-medium tracking-wide text-uppercase" style="font-size: 0.72rem; color: var(--accent);">Live monitoring</p>
            <h1 class="h2 mb-0 fw-bold" style="letter-spacing: -0.025em;">Sensor diagnostic</h1>
        </div>
        <div class="d-flex align-items-center gap-3">
            <span class="badge rounded-pill px-3 py-2" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-weight: 500; font-size: 0.78rem;" id="connectionBadge">
                <i class="fas fa-circle me-1" style="font-size: 0.5rem; color: var(--danger);" id="connectionDot"></i>
                <span id="connectionText" class="fw-medium">Disconnected</span>
            </span>
            <button class="btn btn-sm btn-outline-light" id="toggleBtn" onclick="togglePolling()" style="border-radius: 8px; font-weight: 500;">
                <i class="fas fa-play me-1" id="toggleIcon"></i>
                <span id="toggleText">Start</span>
            </button>
        </div>
    </div>

    <!-- ESP32 Quick Links -->
    <div class="d-flex flex-wrap gap-2 mb-4 stagger-1">
        <button class="btn btn-sm btn-outline-info" onclick="openEsp32Page('/firmware')" id="btnFirmware" disabled style="border-radius: 8px; font-weight: 500;">
            <i class="fas fa-upload me-1"></i>Firmware upload
        </button>
        <button class="btn btn-sm btn-outline-warning" onclick="openEsp32Page('/power')" id="btnPower" disabled style="border-radius: 8px; font-weight: 500;">
            <i class="fas fa-battery-half me-1"></i>Battery diagnostic
        </button>
    </div>

    <!-- ESP32 Status Bar -->
    <div class="card mb-4 stagger-2">
        <div class="card-body py-3">
            <div class="row text-center">
                <div class="col-md-3 col-6 mb-2 mb-md-0">
                    <small class="text-secondary d-block fw-medium" style="font-size: 0.7rem; letter-spacing: 0.06em; text-transform: uppercase;">State</small>
                    <span class="fw-semibold text-white" id="robotState" style="font-size: 0.95rem;">—</span>
                </div>
                <div class="col-md-3 col-6 mb-2 mb-md-0">
                    <small class="text-secondary d-block fw-medium" style="font-size: 0.7rem; letter-spacing: 0.06em; text-transform: uppercase;">Direction</small>
                    <span class="fw-semibold text-white" id="robotDirection" style="font-size: 0.95rem;">—</span>
                </div>
                <div class="col-md-3 col-6">
                    <small class="text-secondary d-block fw-medium" style="font-size: 0.7rem; letter-spacing: 0.06em; text-transform: uppercase;">Battery</small>
                    <span class="fw-semibold text-white" id="robotBattery" style="font-size: 0.95rem;">—</span>
                </div>
                <div class="col-md-3 col-6">
                    <small class="text-secondary d-block fw-medium" style="font-size: 0.7rem; letter-spacing: 0.06em; text-transform: uppercase;">Uptime</small>
                    <span class="fw-semibold text-white" id="robotUptime" style="font-size: 0.95rem;">—</span>
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
                    <i class="fas fa-shield-alt me-2" style="color: var(--warning);"></i>
                    <h6 class="m-0 fw-semibold text-white" style="font-size: 0.9rem;">Obstacle avoidance (IR1, IR2, IR3)</h6>
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
                                <small class="d-block mt-2 text-secondary fw-medium">IR1 Left</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-left-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-left-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Front Sensor -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="obs-front-indicator">
                                    <i class="fas fa-wifi fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary fw-medium">IR2 Front</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-front-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-front-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Right Sensor -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="obs-right-indicator">
                                    <i class="fas fa-wifi fa-rotate-90 fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary fw-medium">IR3 Right</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-right-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="obs-right-db">DEB: —</span>
                                </div>
                            </div>
                        </div>
                    </div>

                    <!-- Legend -->
                    <div class="rounded-3 p-3 small" style="background: rgba(255,255,255,0.025); border: 1px solid var(--surface-border);">
                        <i class="fas fa-info-circle me-1" style="color: var(--accent);"></i>
                        <strong class="text-white">RAW</strong> = raw pin value (0/1). <strong class="text-white">DEB</strong> = debounced value (true/false).
                        <br><span style="color: var(--success);">● Green</span> = safe (no obstacle).
                        <span style="color: var(--danger);">● Red</span> = detected (obstacle).
                    </div>
                </div>
            </div>
        </div>

        <!-- Cliff Detection Sensors -->
        <div class="col-lg-6">
            <div class="card h-100">
                <div class="card-header py-3 d-flex align-items-center">
                    <i class="fas fa-mountain me-2" style="color: var(--danger);"></i>
                    <h6 class="m-0 fw-semibold text-white" style="font-size: 0.9rem;">Cliff detection (IR4, IR5, IR6)</h6>
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
                                <small class="d-block mt-2 text-secondary fw-medium">IR4 Left</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-left-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-left-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Front Cliff -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="clf-front-indicator">
                                    <i class="fas fa-arrow-down fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary fw-medium">IR5 Front</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-front-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-front-db">DEB: —</span>
                                </div>
                            </div>
                            <!-- Right Cliff -->
                            <div class="text-center">
                                <div class="sensor-indicator" id="clf-right-indicator">
                                    <i class="fas fa-arrow-down fa-2x"></i>
                                </div>
                                <small class="d-block mt-2 text-secondary fw-medium">IR6 Right</small>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-right-raw">RAW: —</span>
                                </div>
                                <div class="mt-1">
                                    <span class="badge rounded-pill" style="background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;" id="clf-right-db">DEB: —</span>
                                </div>
                            </div>
                        </div>
                    </div>

                    <!-- Legend -->
                    <div class="rounded-3 p-3 small" style="background: rgba(255,255,255,0.025); border: 1px solid var(--surface-border);">
                        <i class="fas fa-info-circle me-1" style="color: var(--accent);"></i>
                        <strong class="text-white">RAW</strong> = raw pin value (0/1). <strong class="text-white">DEB</strong> = debounced value (true/false).
                        <br><span style="color: var(--success);">● Green</span> = safe (floor detected).
                        <span style="color: var(--danger);">● Red</span> = danger (cliff/stairs).
                    </div>
                </div>
            </div>
        </div>
    </div>

    <!-- Timing Settings Panel -->
    <div class="card mt-4 stagger-4">
        <div class="card-header py-3 d-flex align-items-center justify-content-between">
            <div class="d-flex align-items-center">
                <i class="fas fa-sliders-h me-2" style="color: var(--accent);"></i>
                <span class="fw-semibold text-white" style="font-size: 0.9rem;">Timing settings</span>
                <small class="text-secondary ms-2 fw-medium">(live tuning without reflashing)</small>
            </div>
            <div class="d-flex gap-2">
                <button class="btn btn-sm btn-outline-secondary" onclick="resetTimingDefaults()" style="border-radius: 8px; font-weight: 500;">
                    <i class="fas fa-undo me-1"></i>Reset
                </button>
                <button class="btn btn-sm btn-primary" onclick="saveTimingSettings()" style="border-radius: 8px; font-weight: 500; background: var(--accent); border-color: var(--accent); color: var(--bg-dark);">
                    <i class="fas fa-save me-1"></i>Save to ESP32
                </button>
            </div>
        </div>
        <div class="card-body p-4">
            <div class="row g-4">
                <!-- Obstacle Avoidance -->
                <div class="col-md-4">
                    <h6 class="fw-semibold mb-3" style="color: var(--warning); font-size: 0.85rem;"><i class="fas fa-shield-alt me-1"></i> Obstacle avoidance</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Backup duration (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_backupDuration" min="100" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Turn min (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationMin" min="100" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Turn max (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationMax" min="200" max="3000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Small turn (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_turnDurationSmall" min="50" max="1000" step="50">
                    </div>
                </div>
                <!-- Cliff Avoidance -->
                <div class="col-md-4">
                    <h6 class="fw-semibold mb-3" style="color: var(--danger); font-size: 0.85rem;"><i class="fas fa-mountain me-1"></i> Cliff avoidance</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Cliff backup (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_cliffBackupDuration" min="200" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">180° turn (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_cliffTurnDuration" min="300" max="3000" step="50">
                    </div>
                    <h6 class="fw-semibold mb-3 mt-4" style="color: var(--accent); font-size: 0.85rem;"><i class="fas fa-exclamation-triangle me-1"></i> Stuck detection</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Obstacle count threshold</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_stuckObstacleCount" min="2" max="10" step="1">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Time window (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_stuckTimeWindow" min="1000" max="10000" step="500">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Escape turn (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_escapeTurnDuration" min="500" max="3000" step="100">
                    </div>
                </div>
                <!-- Spiral Pattern -->
                <div class="col-md-4">
                    <h6 class="fw-semibold mb-3" style="color: var(--success); font-size: 0.85rem;"><i class="fas fa-sync-alt me-1"></i> Spiral pattern</h6>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Initial duration (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralInitialDuration" min="200" max="2000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Increment (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralIncrement" min="50" max="500" step="25">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Max duration (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralMaxDuration" min="1000" max="10000" step="500">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Spiral turn (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_spiralTurnDuration" min="100" max="1000" step="50">
                    </div>
                    <div class="mb-3">
                        <label class="form-label small text-secondary fw-medium">Re-spiral idle time (ms)</label>
                        <input type="number" class="form-control form-control-sm bg-dark text-white border-secondary" id="t_respiralIdleTime" min="5000" max="60000" step="1000">
                        <small class="text-muted">Idle duration in open area before re-triggering spiral</small>
                    </div>
                </div>
            </div>
            <div class="rounded-3 p-3 mt-3 small" style="background: rgba(255,255,255,0.025); border: 1px solid var(--surface-border);">
                <i class="fas fa-info-circle me-1" style="color: var(--accent);"></i>
                Changes apply directly to ESP32 <strong class="text-white">without reflashing</strong>. Values reset to default on ESP32 restart.
                Click <strong class="text-white">Reset</strong> to restore all values from <code>config.h</code>.
            </div>
        </div>
    </div>

    <!-- Raw JSON Log -->
    <div class="card mt-4 stagger-5">
        <div class="card-header py-3 d-flex align-items-center justify-content-between">
            <div class="d-flex align-items-center">
                <i class="fas fa-terminal me-2" style="color: var(--success);"></i>
                <span class="fw-semibold text-white" style="font-size: 0.9rem;">Raw JSON log</span>
            </div>
            <div class="d-flex align-items-center gap-3">
                <div class="form-check form-switch mb-0">
                    <input class="form-check-input" type="checkbox" id="logToggle" checked onchange="logEnabled = this.checked">
                    <label class="form-check-label small text-secondary fw-medium" for="logToggle">Log</label>
                </div>
                <button class="btn btn-sm btn-outline-secondary" onclick="clearLog()" style="border-radius: 8px; font-weight: 500;">
                    <i class="fas fa-trash me-1"></i>Clear
                </button>
            </div>
        </div>
        <div class="card-body p-0">
            <pre class="mb-0 p-3 small" id="jsonLog" style="max-height: 300px; overflow-y: auto; background: rgba(0,0,0,0.3); font-family: 'JetBrains Mono', 'Fira Code', 'Courier New', monospace; font-size: 0.78rem; color: var(--success); line-height: 1.5; border-radius: 0 0 var(--radius-lg) var(--radius-lg);">Press "Start" to begin diagnostic...</pre>
        </div>
    </div>

    <script>
        const API_BASE_URL = "/v1/vacuum";
        const POLL_INTERVAL = 300;

        let esp32Ip = null;
        let pollingActive = false;
        let pollTimer = null;
        let logLines = [];
        let logEnabled = true;
        const MAX_LOG_LINES = 50;

        // ===== ESP32 Discovery =====
        async function discoverEsp32() {
            try {
                const res = await fetch(`${API_BASE_URL}/device`).then(r => r.json());
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
            const btnFw = document.getElementById('btnFirmware');
            const btnPw = document.getElementById('btnPower');
            if (connected) {
                dot.style.color = 'var(--success)';
                dot.className = 'fas fa-circle me-1';
                dot.style.fontSize = '0.5rem';
                text.textContent = `Connected (${esp32Ip})`;
                btnFw.disabled = false;
                btnPw.disabled = false;
            } else {
                dot.style.color = 'var(--danger)';
                dot.className = 'fas fa-circle me-1';
                dot.style.fontSize = '0.5rem';
                text.textContent = 'Disconnected';
                btnFw.disabled = true;
                btnPw.disabled = true;
            }
        }

        function openEsp32Page(path) {
            if (!esp32Ip) {
                addLog('ERROR: ESP32 not connected. Run discovery first.');
                return;
            }
            window.open(`http://${esp32Ip}${path}`, '_blank');
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
                    addLog('ERROR: ESP32 not found. Make sure the robot is connected to WiFi.');
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
                const controller = new AbortController();
                const timeoutId = setTimeout(() => controller.abort(), 2000);
                const res = await fetch(`http://${esp32Ip}/diagnostic`, {
                    signal: controller.signal
                }).then(r => r.json());
                clearTimeout(timeoutId);

                if (res.success) {
                    updateSensorUI(res);
                    updateStatusBar(res);
                    if (res.timing && !timingLoaded) {
                        populateTimingFields(res.timing);
                        timingLoaded = true;
                    }
                    if (logEnabled) addLog(JSON.stringify(res));
                }
            } catch (err) {
                addLog('ERROR: Failed to fetch data from ESP32');
                updateConnection(false);
            }

            if (pollingActive) {
                pollTimer = setTimeout(pollDiagnostic, POLL_INTERVAL);
            }
        }

        // ===== Update Sensor UI =====
        function updateSensorUI(data) {
            updateSensorCard('obs-left', data.obstacle_raw.left, data.obstacle_debounced.left, 'obstacle');
            updateSensorCard('obs-front', data.obstacle_raw.front, data.obstacle_debounced.front, 'obstacle');
            updateSensorCard('obs-right', data.obstacle_raw.right, data.obstacle_debounced.right, 'obstacle');

            updateSensorCard('clf-left', data.cliff_raw.left, data.cliff_debounced.left, 'cliff');
            updateSensorCard('clf-front', data.cliff_raw.front, data.cliff_debounced.front, 'cliff');
            updateSensorCard('clf-right', data.cliff_raw.right, data.cliff_debounced.right, 'cliff');
        }

        function updateSensorCard(prefix, rawVal, debouncedVal, type) {
            const indicator = document.getElementById(`${prefix}-indicator`);
            const rawBadge = document.getElementById(`${prefix}-raw`);
            const dbBadge = document.getElementById(`${prefix}-db`);

            rawBadge.textContent = `RAW: ${rawVal}`;
            rawBadge.className = `badge rounded-pill ${rawVal === 1 ? 'bg-warning text-dark' : ''}`;
            if (rawVal !== 1) {
                rawBadge.style.cssText = `background: var(--card-bg); border: 1px solid var(--surface-border); font-size: 0.72rem;`;
            } else {
                rawBadge.style.cssText = '';
            }

            dbBadge.textContent = `DEB: ${debouncedVal ? 'true' : 'false'}`;
            dbBadge.className = `badge rounded-pill ${debouncedVal ? 'bg-danger' : 'bg-success'} border-0`;
            dbBadge.style.cssText = debouncedVal ? '' : '';

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
            'respiralIdleTime',
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
                addLog('ERROR: ESP32 not connected');
                return;
            }

            const settings = {};
            timingKeys.forEach(key => {
                const el = document.getElementById(`t_${key}`);
                if (el && el.value) settings[key] = parseInt(el.value);
            });

            try {
                const controller = new AbortController();
                const timeoutId = setTimeout(() => controller.abort(), 3000);
                await fetch(`http://${esp32Ip}/settings`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(settings),
                    signal: controller.signal
                });
                clearTimeout(timeoutId);
                addLog('Settings SAVED: ' + JSON.stringify(settings));
                timingLoaded = false;
            } catch (err) {
                addLog('ERROR: Failed to save settings');
            }
        }

        async function resetTimingDefaults() {
            if (!esp32Ip) {
                addLog('ERROR: ESP32 not connected');
                return;
            }

            try {
                const controller = new AbortController();
                const timeoutId = setTimeout(() => controller.abort(), 3000);
                await fetch(`http://${esp32Ip}/settings`, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ resetDefaults: true }),
                    signal: controller.signal
                });
                clearTimeout(timeoutId);
                addLog('Settings RESET to defaults');
                timingLoaded = false;
            } catch (err) {
                addLog('ERROR: Failed to reset settings');
            }
        }

        // ===== Init =====
        document.addEventListener('DOMContentLoaded', async () => {
            await discoverEsp32();
        });
    </script>
</x-master>
