<x-master>
    <div class="container-fluid">
        <!-- Page Heading -->
        <div class="d-sm-flex align-items-center justify-content-between mb-4">
            <h1 class="h3 mb-0 text-gray-800">Autonomous Vacuum Control Panel</h1>
            <span class="badge badge-success badge-lg">Online</span>
        </div>

        <!-- Status Robot Card -->
        <div class="row mb-4">
            <div class="col-lg-6">
                <div class="card border-left-primary shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-primary font-weight-bold text-uppercase mb-1">
                            Status Robot
                        </div>
                        <div class="h3 mb-0">
                            <span class="badge" id="statusRobot">Standby</span>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Battery Monitoring Card -->
            <div class="col-lg-6">
                <div class="card border-left-info shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-info font-weight-bold text-uppercase mb-1">
                            Status Baterai
                        </div>
                        <div class="d-flex align-items-center">
                            <div class="h3 mb-0 mr-3" id="batteryPercent">85%</div>
                            <div class="progress" style="flex: 1; height: 25px;">
                                <div class="progress-bar bg-success" id="batteryBar" role="progressbar" 
                                     style="width: 85%" aria-valuenow="85" aria-valuemin="0" aria-valuemax="100">
                                </div>
                            </div>
                        </div>
                        <small class="text-muted mt-2 d-block">Estimasi: 3 jam 45 menit</small>
                    </div>
                </div>
            </div>
        </div>

        <!-- Control Section -->
        <div class="row mb-4">
            <!-- Kontrol Utama -->
            <div class="col-lg-8">
                <div class="card shadow mb-4">
                    <div class="card-header py-3 bg-primary">
                        <h6 class="m-0 font-weight-bold text-white">Kontrol Utama</h6>
                    </div>
                    <div class="card-body">
                        <div class="row text-center mb-4">
                            <div class="col-md-4 mb-3">
                                <button class="btn btn-success btn-lg btn-block py-4" id="startBtn" onclick="startVacuum()">
                                    <i class="fas fa-play fa-2x mb-2 d-block"></i>
                                    <strong>MULAI</strong>
                                </button>
                            </div>
                            <div class="col-md-4 mb-3">
                                <button class="btn btn-danger btn-lg btn-block py-4" id="stopBtn" onclick="stopVacuum()">
                                    <i class="fas fa-stop fa-2x mb-2 d-block"></i>
                                    <strong>BERHENTI</strong>
                                </button>
                            </div>
                            <div class="col-md-4 mb-3">
                                <button class="btn btn-warning btn-lg btn-block py-4" id="homeBtn" onclick="returnToBase()">
                                    <i class="fas fa-home fa-2x mb-2 d-block"></i>
                                    <strong>KEMBALI</strong>
                                </button>
                            </div>
                        </div>
                        
                        <!-- Status Informasi -->
                        <div class="alert alert-info" id="statusInfo" role="alert">
                            <strong>Status:</strong> Siap digunakan
                        </div>
                    </div>
                </div>
            </div>

            <!-- Pengaturan Daya Hisap -->
            <div class="col-lg-4">
                <div class="card shadow mb-4">
                    <div class="card-header py-3 bg-info">
                        <h6 class="m-0 font-weight-bold text-white">Daya Hisap</h6>
                    </div>
                    <div class="card-body">
                        <div class="btn-group-vertical w-100" role="group">
                            <button type="button" class="btn btn-outline-primary text-left py-3 border-bottom" 
                                    onclick="setPowerMode('eco')" id="ecoBtn">
                                <div class="font-weight-bold">ECO</div>
                                <small>Hemat Energi</small>
                            </button>
                            <button type="button" class="btn btn-outline-primary text-left py-3 border-bottom active" 
                                    onclick="setPowerMode('normal')" id="normalBtn">
                                <div class="font-weight-bold">NORMAL</div>
                                <small>Penggunaan Standar</small>
                            </button>
                            <button type="button" class="btn btn-outline-primary text-left py-3" 
                                    onclick="setPowerMode('strong')" id="strongBtn">
                                <div class="font-weight-bold">STRONG</div>
                                <small>Daya Maksimal</small>
                            </button>
                        </div>
                        
                        <div class="mt-3 alert alert-light" id="powerInfo">
                            <strong>Mode Saat Ini:</strong> NORMAL
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Statistics Section -->
        {{-- <div class="row">
            <div class="col-md-3 mb-4">
                <div class="card border-left-success shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-success font-weight-bold text-uppercase mb-1">
                            Area Bersih
                        </div>
                        <div class="h3 mb-0">45 m²</div>
                    </div>
                </div>
            </div>
            <div class="col-md-3 mb-4">
                <div class="card border-left-warning shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-warning font-weight-bold text-uppercase mb-1">
                            Waktu Berjalan
                        </div>
                        <div class="h3 mb-0">2h 15m</div>
                    </div>
                </div>
            </div>
            <div class="col-md-3 mb-4">
                <div class="card border-left-info shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-info font-weight-bold text-uppercase mb-1">
                            Total Siklus
                        </div>
                        <div class="h3 mb-0">24</div>
                    </div>
                </div>
            </div>
            <div class="col-md-3 mb-4">
                <div class="card border-left-danger shadow h-100 py-2">
                    <div class="card-body">
                        <div class="text-danger font-weight-bold text-uppercase mb-1">
                            Error Log
                        </div>
                        <div class="h3 mb-0">0</div>
                    </div>
                </div>
            </div>
        </div> --}}

    </div>

    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>

    <script>
        // State Management
        let vacuumState = {
            status: 'standby',
            battery: 85,
            powerMode: 'normal',
            isRunning: false
        };

        // Fungsi Kontrol Vacuum
        function startVacuum() {
            vacuumState.status = 'working';
            vacuumState.isRunning = true;
            updateUI();
            updateStatusInfo('Vacuum sedang berjalan...');
            
            // Simulasi batasan berkurang
            simulateBatteryDecrease();
        }

        function stopVacuum() {
            vacuumState.status = 'stop';
            vacuumState.isRunning = false;
            updateUI();
            updateStatusInfo('Vacuum telah dihentikan');
        }

        function returnToBase() {
            vacuumState.status = 'returning';
            updateUI();
            updateStatusInfo('Vacuum kembali ke charging base...');
            
            setTimeout(() => {
                vacuumState.status = 'charging';
                updateUI();
                updateStatusInfo('Vacuum sedang charging di base');
            }, 5000);
        }

        // Fungsi untuk mengirim data daya hisap ke database
        function sendPowerModeToDatabase(mode, value) {
            $.ajaxSetup({
                headers: {
                    'X-CSRF-TOKEN': $('meta[name="csrf-token"]').attr('content')
                }
            });

            $.ajax({
                url: "{{ route('dayahisap.store') }}", 
                type: 'POST',
                dataType: 'json',
                data: {
                    power_mode: mode,
                    value: value
                },
                success: function(response) {
                    console.log('Data berhasil dikirim:', response);
                    // Tampilkan notifikasi berhasil
                    showNotification('success', 'Data daya hisap berhasil disimpan');
                },
                error: function(xhr, status, error) {
                    console.error('Error:', error);
                    console.error('Response:', xhr.responseText);
                    showNotification('error', 'Gagal mengirim data');
                }
            });
        }


        // Update fungsi setPowerMode untuk mengirim ke database
        function setPowerMode(mode) {
            vacuumState.powerMode = mode;
            updateUI();
            
            // Tentukan nilai berdasarkan mode
            const powerValues = {
                'eco': 150,
                'normal': 200,
                'strong': 255
            };
            
            const value = powerValues[mode];
            
            // Kirim ke database
            sendPowerModeToDatabase(mode, value);
            
            const modeInfo = {
                'eco': 'Hemat Energi - Daya rendah untuk pembersihan ringan',
                'normal': 'Mode Standar - Keseimbangan daya dan efisiensi',
                'strong': 'Daya Maksimal - Pembersihan intensif'
            };
            
            updatePowerInfo(mode, modeInfo[mode]);
        }

        // Update UI
        function updateUI() {
            // Update Status
            const statusMap = {
                'standby': '<span class="badge badge-secondary">Standby</span>',
                'working': '<span class="badge badge-success">Berjalan</span>',
                'stop': '<span class="badge badge-danger">Berhenti</span>',
                'returning': '<span class="badge badge-warning">Kembali ke Base</span>',
                'charging': '<span class="badge badge-info">Charging</span>'
            };
            
            document.getElementById('statusRobot').innerHTML = statusMap[vacuumState.status];
            
            // Update Power Mode Buttons
            document.getElementById('ecoBtn').classList.toggle('active', vacuumState.powerMode === 'eco');
            document.getElementById('normalBtn').classList.toggle('active', vacuumState.powerMode === 'normal');
            document.getElementById('strongBtn').classList.toggle('active', vacuumState.powerMode === 'strong');
            
            // Update Battery
            document.getElementById('batteryPercent').textContent = vacuumState.battery + '%';
            document.getElementById('batteryBar').style.width = vacuumState.battery + '%';
            
            // Update Battery Color
            const batteryBar = document.getElementById('batteryBar');
            if (vacuumState.battery > 50) {
                batteryBar.classList.remove('bg-warning', 'bg-danger');
                batteryBar.classList.add('bg-success');
            } else if (vacuumState.battery > 20) {
                batteryBar.classList.remove('bg-success', 'bg-danger');
                batteryBar.classList.add('bg-warning');
            } else {
                batteryBar.classList.remove('bg-success', 'bg-warning');
                batteryBar.classList.add('bg-danger');
            }
        }

        function updateStatusInfo(message) {
            document.getElementById('statusInfo').innerHTML = '<strong>Status:</strong> ' + message;
        }

        function updatePowerInfo(mode, description) {
            const modeText = {
                'eco': 'ECO',
                'normal': 'NORMAL',
                'strong': 'STRONG'
            };
            document.getElementById('powerInfo').innerHTML = `
                <strong>Mode Saat Ini:</strong> ${modeText[mode]}<br>
                <small>${description}</small>
            `;
        }

        function simulateBatteryDecrease() {
            if (vacuumState.isRunning && vacuumState.battery > 0) {
                const decreaseRate = vacuumState.powerMode === 'eco' ? 0.5 : 
                                    vacuumState.powerMode === 'normal' ? 1 : 1.5;
                
                vacuumState.battery = Math.max(0, vacuumState.battery - decreaseRate);
                updateUI();
                
                setTimeout(simulateBatteryDecrease, 1000);
            }
        }

        // Initialize
        document.addEventListener('DOMContentLoaded', function() {
            updateUI();
        });
    </script>

    <style>
        .btn-lg {
            font-size: 1.1rem;
            padding: 1rem !important;
        }

        .btn-group-vertical .btn {
            border-radius: 0;
            padding: 1rem;
        }

        .btn-group-vertical .btn:first-child {
            border-top-left-radius: 0.25rem;
            border-top-right-radius: 0.25rem;
        }

        .btn-group-vertical .btn:last-child {
            border-bottom-left-radius: 0.25rem;
            border-bottom-right-radius: 0.25rem;
        }

        .btn-group-vertical .btn.active {
            background-color: #007bff;
            color: white;
            border-color: #007bff;
        }

        .card-header.bg-primary {
            background-color: #4e73df !important;
        }

        .card-header.bg-info {
            background-color: #17a2b8 !important;
        }

        .border-left-primary {
            border-left: 0.25rem solid #4e73df !important;
        }

        .border-left-info {
            border-left: 0.25rem solid #17a2b8 !important;
        }

        .border-left-success {
            border-left: 0.25rem solid #1cc88a !important;
        }

        .border-left-warning {
            border-left: 0.25rem solid #f6c23e !important;
        }

        .border-left-danger {
            border-left: 0.25rem solid #e74c3c !important;
        }

        .progress {
            background-color: #e3e6f0;
        }

        .badge-lg {
            padding: 0.5rem 1rem;
            font-size: 1rem;
        }
    </style>
</x-master>