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
        <div class="card-body p-4">
            <div class="d-flex flex-wrap gap-2">
                <span class="badge bg-primary bg-opacity-25 text-primary border border-primary border-opacity-25 px-3 py-2">ESP32</span>
                <span class="badge bg-danger bg-opacity-25 text-danger border border-danger border-opacity-25 px-3 py-2">Laravel 12</span>
                <span class="badge bg-info bg-opacity-25 text-info border border-info border-opacity-25 px-3 py-2">Bootstrap 5</span>
                <span class="badge bg-success bg-opacity-25 text-success border border-success border-opacity-25 px-3 py-2">MySQL</span>
            </div>
        </div>
    </div>
</x-master>
