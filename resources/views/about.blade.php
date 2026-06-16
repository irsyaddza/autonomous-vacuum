<x-master>
    <!-- Header -->
    <div class="d-flex align-items-center justify-content-between mb-5 stagger-1">
        <div>
            <h1 class="h2 mb-1 fw-bold text-white"><i class="fas fa-info-circle me-2"></i>About</h1>
            <p class="text-secondary mb-0">Tentang sistem Autonomous Vacuum Control Panel.</p>
        </div>
    </div>

    <!-- Main About Card -->
    <div class="card mb-4 stagger-2">
        <div class="card-body p-5">
            <div class="row align-items-center g-4">
                <div class="col-md-2 text-center">
                    <div class="bg-primary bg-opacity-10 p-4 rounded-circle d-inline-block">
                        <i class="fas fa-robot fa-3x text-primary"></i>
                    </div>
                </div>
                <div class="col-md-10">
                    <h3 class="fw-bold text-white mb-2">Autonomous Vacuum Robot</h3>
                    <p class="text-secondary mb-0 lead">
                        Sistem kontrol dan monitoring robot vacuum otonom berbasis ESP32 dengan antarmuka web.
                    </p>
                </div>
            </div>
        </div>
    </div>

    <!-- Features -->
    <div class="row g-4 mb-4 stagger-3">
        <div class="col-md-4">
            <div class="card h-100">
                <div class="card-body p-4 text-center">
                    <div class="bg-info bg-opacity-10 p-3 rounded-circle d-inline-block mb-3">
                        <i class="fas fa-gamepad fa-2x text-info"></i>
                    </div>
                    <h5 class="fw-bold text-white mb-2">Kontrol Robot</h5>
                    <p class="text-secondary small mb-0">
                        Atur kecepatan motor, arah pergerakan, dan mode pembersihan langsung dari dashboard.
                    </p>
                </div>
            </div>
        </div>
        <div class="col-md-4">
            <div class="card h-100">
                <div class="card-body p-4 text-center">
                    <div class="bg-warning bg-opacity-10 p-3 rounded-circle d-inline-block mb-3">
                        <i class="fas fa-stethoscope fa-2x text-warning"></i>
                    </div>
                    <h5 class="fw-bold text-white mb-2">Sensor Diagnostic</h5>
                    <p class="text-secondary small mb-0">
                        Monitor semua sensor IR (obstacle & cliff) secara real-time. Tuning parameter timing algoritma cleaning tanpa perlu flash ulang firmware.
                    </p>
                </div>
            </div>
        </div>
        <div class="col-md-4">
            <div class="card h-100">
                <div class="card-body p-4 text-center">
                    <div class="bg-success bg-opacity-10 p-3 rounded-circle d-inline-block mb-3">
                        <i class="fas fa-battery-three-quarters fa-2x text-success"></i>
                    </div>
                    <h5 class="fw-bold text-white mb-2">Battery Monitoring</h5>
                    <p class="text-secondary small mb-0">
                        Pantau tegangan dan persentase baterai secara berkala. Sistem otomatis memperingatkan dan menghentikan robot saat baterai rendah.
                    </p>
                </div>
            </div>
        </div>
    </div>

    <!-- How it works -->
    <div class="card mb-4 stagger-4">
        <div class="card-header py-3">
            <i class="fas fa-cogs me-2 text-info"></i>
            <span class="fw-bold text-white">Cara Kerja</span>
        </div>
        <div class="card-body p-4">
            <div class="row g-4">
                <div class="col-md-6">
                    <h6 class="text-info mb-3"><i class="fas fa-microchip me-2"></i>Arsitektur Sistem</h6>
                    <ul class="list-unstyled text-secondary small">
                        <li class="mb-2">
                            <i class="fas fa-check text-success me-2"></i>
                            <strong class="text-white">ESP32</strong> sebagai otak robot — menjalankan firmware, membaca sensor, mengontrol motor, dan menjalankan algoritma cleaning.
                        </li>
                        <li class="mb-2">
                            <i class="fas fa-check text-success me-2"></i>
                            <strong class="text-white">Laravel</strong> sebagai backend — menyimpan log baterai, riwayat perintah, dan menyediakan REST API.
                        </li>
                        <li class="mb-2">
                            <i class="fas fa-check text-success me-2"></i>
                            <strong class="text-white">Browser</strong> sebagai frontend — dashboard kontrol, diagnostic live, dan tuning parameter via HTTP langsung ke ESP32.
                        </li>
                    </ul>
                </div>
                <div class="col-md-6">
                    <h6 class="text-info mb-3"><i class="fas fa-broom me-2"></i>Algoritma Cleaning</h6>
                    <ul class="list-unstyled text-secondary small">
                        <li class="mb-2">
                            <i class="fas fa-sync-alt text-warning me-2"></i>
                            <strong class="text-white">Spiral Expanding</strong> — robot bergerak spiral dari titik awal untuk membersihkan area secara merata.
                        </li>
                        <li class="mb-2">
                            <i class="fas fa-random text-warning me-2"></i>
                            <strong class="text-white">Random Bounce</strong> — setelah spiral selesai, robot berjalan lurus dan belok acak saat menabrak obstacle.
                        </li>
                        <li class="mb-2">
                            <i class="fas fa-shield-alt text-danger me-2"></i>
                            <strong class="text-white">Cliff Protection</strong> — sensor cliff menghentikan robot segera jika terdeteksi jurang/tangga.
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>

    <!-- Tech Stack -->
    <div class="card stagger-5">
        <div class="card-header py-3">
            <i class="fas fa-layer-group me-2 text-success"></i>
            <span class="fw-bold text-white">Tech Stack</span>
        </div>
        <style>
            .tech-card {
                transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
            }
            .tech-card:hover {
                transform: translateY(-4px);
                box-shadow: 0 8px 20px -6px rgba(0, 0, 0, 0.5);
                border-color: rgba(255, 255, 255, 0.15) !important;
            }
            .tech-logo-container {
                width: 48px;
                height: 48px;
                display: flex;
                align-items: center;
                justify-content: center;
                border-radius: 12px;
                background: #ffffff;
                padding: 8px;
                box-shadow: 0 4px 10px rgba(0, 0, 0, 0.15);
                transition: transform 0.3s ease;
            }
            .tech-card:hover .tech-logo-container {
                transform: scale(1.08) rotate(3deg);
            }
        </style>
        <div class="card-body p-4">
            <div class="row row-cols-1 row-cols-sm-2 row-cols-lg-4 g-3">
                <!-- ESP32 -->
                <div class="col">
                    <div class="d-flex align-items-center p-3 rounded-3 tech-card" style="background: rgba(14, 165, 233, 0.08); border: 1px solid rgba(14, 165, 233, 0.2); height: 100%;">
                        <div class="tech-logo-container me-3 flex-shrink-0">
                            <img src="{{ asset('espressif.png') }}" alt="ESP32" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                        </div>
                        <div>
                            <div class="fw-bold text-white small mb-0">ESP32</div>
                            <span class="text-secondary" style="font-size: 0.75rem;">Microcontroller</span>
                        </div>
                    </div>
                </div>
                <!-- Laravel 12 -->
                <div class="col">
                    <div class="d-flex align-items-center p-3 rounded-3 tech-card" style="background: rgba(239, 68, 68, 0.08); border: 1px solid rgba(239, 68, 68, 0.2); height: 100%;">
                        <div class="tech-logo-container me-3 flex-shrink-0">
                            <img src="{{ asset('laravel.png') }}" alt="Laravel 12" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                        </div>
                        <div>
                            <div class="fw-bold text-white small mb-0">Laravel 12</div>
                            <span class="text-secondary" style="font-size: 0.75rem;">Backend Framework</span>
                        </div>
                    </div>
                </div>
                <!-- Bootstrap 5 -->
                <div class="col">
                    <div class="d-flex align-items-center p-3 rounded-3 tech-card" style="background: rgba(13, 202, 240, 0.08); border: 1px solid rgba(13, 202, 240, 0.2); height: 100%;">
                        <div class="tech-logo-container me-3 flex-shrink-0">
                            <img src="{{ asset('bootstrap.png') }}" alt="Bootstrap 5" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                        </div>
                        <div>
                            <div class="fw-bold text-white small mb-0">Bootstrap 5</div>
                            <span class="text-secondary" style="font-size: 0.75rem;">CSS Framework</span>
                        </div>
                    </div>
                </div>
                <!-- MySQL -->
                <div class="col">
                    <div class="d-flex align-items-center p-3 rounded-3 tech-card" style="background: rgba(25, 135, 84, 0.08); border: 1px solid rgba(25, 135, 84, 0.2); height: 100%;">
                        <div class="tech-logo-container me-3 flex-shrink-0">
                            <img src="{{ asset('mysql.png') }}" alt="MySQL" style="max-width: 100%; max-height: 100%; object-fit: contain;">
                        </div>
                        <div>
                            <div class="fw-bold text-white small mb-0">MySQL</div>
                            <span class="text-secondary" style="font-size: 0.75rem;">Database</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</x-master>
