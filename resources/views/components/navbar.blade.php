<nav class="navbar navbar-expand-lg navbar-dark navbar-custom fixed-top mb-5 shadow-sm">
    <div class="container">
        <a class="navbar-brand" href="/">
            <i class="fas fa-robot mr-2"></i> Control Panel
        </a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav ms-auto align-items-center">
                <li class="nav-item">
                    <a class="nav-link active" aria-current="page" href="/"><i class="fas fa-tachometer-alt me-1"></i> Dashboard</a>
                </li>
                <li class="nav-item">
                    <a class="nav-link" href="about"><i class="fas fa-info-circle me-1"></i> About</a>
                </li>
                <li class="nav-item ms-lg-3 mt-3 mt-lg-0 d-none d-lg-block">
                    <div class="d-flex flex-column align-items-center align-items-lg-end justify-content-center text-center text-lg-end">
                        <div id="navbar-clock-time" class="fw-bold text-white" style="font-family: 'Courier New', monospace; letter-spacing: 1px;">00:00:00</div>
                        <div id="navbar-clock-date" class="small text-secondary" style="font-size: 0.75rem;">Loading...</div>
                    </div>
                </li>
            </ul>
        </div>
    </div>
</nav>

<script>
    function updateClock() {
        const now = new Date();
        const timeString = now.toLocaleTimeString('en-US', { hour12: false, hour: '2-digit', minute: '2-digit', second: '2-digit' });
        const dateString = now.toLocaleDateString('en-US', { weekday: 'long', day: 'numeric', month: 'long', year: 'numeric' });
        
        const formattedTime = timeString.replace(/\./g, ':');

        // Update Navbar Clock (Desktop)
        const navTime = document.getElementById('navbar-clock-time');
        const navDate = document.getElementById('navbar-clock-date');
        if(navTime) navTime.textContent = formattedTime;
        if(navDate) navDate.textContent = dateString;

        // Update Header Clock (Mobile)
        const mobTime = document.getElementById('mobile-clock-time');
        const mobDate = document.getElementById('mobile-clock-date');
        if(mobTime) mobTime.textContent = formattedTime;
        if(mobDate) mobDate.textContent = dateString;
    }
    
    // Update immediately and then every second
    updateClock();
    setInterval(updateClock, 1000);
</script>
<!-- Spacer for fixed navbar -->
<div style="height: 80px;"></div>