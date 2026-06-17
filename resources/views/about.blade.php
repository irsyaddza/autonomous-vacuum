<x-master>
    <!-- Header -->
    <div class="mb-5 stagger-1">
        <p class="text-secondary mb-1 small fw-medium tracking-wide text-uppercase" style="font-size: 0.72rem; color: var(--accent);">About the system</p>
        <h1 class="h2 mb-2 fw-bold" style="letter-spacing: -0.025em;">Autonomous vacuum robot</h1>
        <p class="text-secondary mb-0" style="max-width: 55ch;">
            ESP32-powered autonomous vacuum with real-time web dashboard, sensor diagnostic, and live firmware tuning.
        </p>
    </div>

    <!-- Hero About Card -->
    <div class="card mb-5 stagger-2" style="overflow: hidden; position: relative;">
        <div class="card-body p-5 position-relative" style="z-index: 1;">
            <div class="row align-items-center g-5">
                <div class="col-md-3 text-center">
                    <div class="d-inline-flex align-items-center justify-content-center rounded-3" style="width: 96px; height: 96px; background: var(--accent-dim); border: 1px solid rgba(34, 211, 238, 0.15);">
                        <i class="fas fa-robot fa-2x" style="color: var(--accent);"></i>
                    </div>
                </div>
                <div class="col-md-9">
                    <h3 class="fw-bold mb-2" style="letter-spacing: -0.02em;">Control panel for your robot</h3>
                    <p class="text-secondary mb-0 lead fw-medium" style="font-size: 1.05rem; max-width: 60ch;">
                        Monitor sensors, control movement, adjust suction power, and tune cleaning parameters — all from a single web interface.
                    </p>
                </div>
            </div>
        </div>
    </div>

    <!-- Features: 2-column asymmetric -->
    <div class="row g-4 mb-5 stagger-3">
        <!-- Large feature card -->
        <div class="col-md-7">
            <div class="card h-100">
                <div class="card-body p-5">
                    <div class="d-flex align-items-center mb-4">
                        <div class="d-inline-flex align-items-center justify-content-center rounded-3 me-3" style="width: 44px; height: 44px; background: var(--accent-dim);">
                            <i class="fas fa-gamepad fa-lg" style="color: var(--accent);"></i>
                        </div>
                        <h5 class="fw-bold mb-0" style="letter-spacing: -0.01em;">Robot control</h5>
                    </div>
                    <p class="text-secondary mb-0 small" style="max-width: 50ch;">
                        Start and stop the robot, switch between eco, normal, and strong suction modes. All commands are sent directly to the ESP32 over HTTP for minimal latency.
                    </p>
                </div>
            </div>
        </div>
        <!-- Smaller stacked cards -->
        <div class="col-md-5 d-flex flex-column gap-4">
            <div class="card flex-grow-1">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center mb-3">
                        <div class="d-inline-flex align-items-center justify-content-center rounded-3 me-3" style="width: 40px; height: 40px; background: rgba(251, 191, 36, 0.1);">
                            <i class="fas fa-stethoscope" style="color: var(--warning);"></i>
                        </div>
                        <h6 class="fw-bold mb-0">Sensor diagnostic</h6>
                    </div>
                    <p class="text-secondary mb-0 small">
                        Live IR sensor monitoring with debounce values. Tune timing parameters without reflashing firmware.
                    </p>
                </div>
            </div>
            <div class="card flex-grow-1">
                <div class="card-body p-4">
                    <div class="d-flex align-items-center mb-3">
                        <div class="d-inline-flex align-items-center justify-content-center rounded-3 me-3" style="width: 40px; height: 40px; background: rgba(52, 211, 153, 0.1);">
                            <i class="fas fa-battery-three-quarters" style="color: var(--success);"></i>
                        </div>
                        <h6 class="fw-bold mb-0">Battery monitoring</h6>
                    </div>
                    <p class="text-secondary mb-0 small">
                        Track voltage and percentage in real time. Auto-stop protection when battery depletes.
                    </p>
                </div>
            </div>
        </div>
    </div>

    <!-- How it works -->
    <div class="card mb-5 stagger-4">
        <div class="card-header py-3 d-flex align-items-center">
            <i class="fas fa-cogs me-2" style="color: var(--accent);"></i>
            <span class="fw-semibold text-white" style="font-size: 0.9rem;">How it works</span>
        </div>
        <div class="card-body p-5">
            <div class="row g-5">
                <div class="col-md-6">
                    <h6 class="fw-semibold mb-3" style="color: var(--accent);"><i class="fas fa-microchip me-2"></i>System architecture</h6>
                    <ul class="list-unstyled text-secondary small">
                        <li class="mb-3 d-flex align-items-start">
                            <i class="fas fa-check me-3 mt-1" style="color: var(--success); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">ESP32</strong> — runs the firmware, reads sensors, controls motors, and executes the cleaning algorithm.</span>
                        </li>
                        <li class="mb-3 d-flex align-items-start">
                            <i class="fas fa-check me-3 mt-1" style="color: var(--success); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">Laravel</strong> — backend API that stores battery logs, command history, and manages device registration.</span>
                        </li>
                        <li class="d-flex align-items-start">
                            <i class="fas fa-check me-3 mt-1" style="color: var(--success); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">Browser</strong> — dashboard for control, live diagnostic, and parameter tuning via direct HTTP to ESP32.</span>
                        </li>
                    </ul>
                </div>
                <div class="col-md-6">
                    <h6 class="fw-semibold mb-3" style="color: var(--accent);"><i class="fas fa-broom me-2"></i>Cleaning algorithm</h6>
                    <ul class="list-unstyled text-secondary small">
                        <li class="mb-3 d-flex align-items-start">
                            <i class="fas fa-sync-alt me-3 mt-1" style="color: var(--warning); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">Spiral expanding</strong> — robot moves in an expanding spiral from the start point to cover open areas evenly.</span>
                        </li>
                        <li class="mb-3 d-flex align-items-start">
                            <i class="fas fa-random me-3 mt-1" style="color: var(--warning); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">Random bounce</strong> — after spiral completes, the robot drives straight and turns randomly on obstacle hits.</span>
                        </li>
                        <li class="d-flex align-items-start">
                            <i class="fas fa-shield-alt me-3 mt-1" style="color: var(--danger); font-size: 0.7rem;"></i>
                            <span><strong class="text-white">Cliff protection</strong> — cliff sensors halt the robot immediately if a drop or staircase is detected.</span>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>

    <!-- Tech Stack -->
    <div class="stagger-5">
        <p class="text-secondary mb-3 small fw-medium tracking-wide text-uppercase" style="font-size: 0.68rem; letter-spacing: 0.1em;">Built with</p>
        <style>
            .tech-card {
                transition: border-color 0.3s ease, box-shadow 0.3s ease;
            }
            .tech-card:hover {
                border-color: rgba(255, 255, 255, 0.12) !important;
                box-shadow: var(--shadow-md);
            }
            .tech-logo-container {
                width: 44px;
                height: 44px;
                display: flex;
                align-items: center;
                justify-content: center;
                border-radius: 10px;
                background: #ffffff;
                padding: 7px;
                box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
                flex-shrink: 0;
                transition: transform 0.3s ease;
            }
            .tech-card:hover .tech-logo-container {
                transform: scale(1.06) rotate(2deg);
            }
        </style>
        <div class="row row-cols-1 row-cols-sm-2 row-cols-lg-4 g-3">
            <!-- ESP32 -->
            <div class="col">
                <div class="d-flex align-items-center p-3 rounded-3 tech-card h-100" style="background: rgba(34, 211, 238, 0.06); border: 1px solid rgba(34, 211, 238, 0.12);">
                    <div class="tech-logo-container me-3">
                        <img src="{{ asset('espressif.png') }}" alt="ESP32 microcontroller logo" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                    </div>
                    <div>
                        <div class="fw-semibold text-white small mb-0">ESP32</div>
                        <span class="text-secondary" style="font-size: 0.72rem;">Microcontroller</span>
                    </div>
                </div>
            </div>
            <!-- Laravel 12 -->
            <div class="col">
                <div class="d-flex align-items-center p-3 rounded-3 tech-card h-100" style="background: rgba(239, 68, 68, 0.06); border: 1px solid rgba(239, 68, 68, 0.12);">
                    <div class="tech-logo-container me-3">
                        <img src="{{ asset('laravel.png') }}" alt="Laravel 12 framework logo" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                    </div>
                    <div>
                        <div class="fw-semibold text-white small mb-0">Laravel 12</div>
                        <span class="text-secondary" style="font-size: 0.72rem;">Backend framework</span>
                    </div>
                </div>
            </div>
            <!-- Bootstrap 5 -->
            <div class="col">
                <div class="d-flex align-items-center p-3 rounded-3 tech-card h-100" style="background: rgba(13, 202, 240, 0.06); border: 1px solid rgba(13, 202, 240, 0.12);">
                    <div class="tech-logo-container me-3">
                        <img src="{{ asset('bootstrap.png') }}" alt="Bootstrap 5 CSS framework logo" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                    </div>
                    <div>
                        <div class="fw-semibold text-white small mb-0">Bootstrap 5</div>
                        <span class="text-secondary" style="font-size: 0.72rem;">CSS framework</span>
                    </div>
                </div>
            </div>
            <!-- MySQL -->
            <div class="col">
                <div class="d-flex align-items-center p-3 rounded-3 tech-card h-100" style="background: rgba(25, 135, 84, 0.06); border: 1px solid rgba(25, 135, 84, 0.12);">
                    <div class="tech-logo-container me-3">
                        <img src="{{ asset('mysql.png') }}" alt="MySQL database logo" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                    </div>
                    <div>
                        <div class="fw-semibold text-white small mb-0">MySQL</div>
                        <span class="text-secondary" style="font-size: 0.72rem;">Database</span>
                    </div>
                </div>
            </div>
        </div>
    </div>
</x-master>
