<nav class="navbar navbar-expand-lg navbar-dark navbar-custom fixed-top shadow-sm">
    <div class="container" style="max-width: 1320px;">
        <a class="navbar-brand" href="/">
            <i class="fas fa-robot"></i>
            <span>Vacuum Control</span>
        </a>
        <button class="navbar-toggler border-0" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav ms-auto align-items-center gap-1">
                <li class="nav-item">
                    <a class="nav-link {{ request()->is('/') ? 'active' : '' }}" href="/">
                        <i class="fas fa-tachometer-alt me-1"></i> Dashboard
                    </a>
                </li>
                <li class="nav-item">
                    <a class="nav-link {{ request()->is('diagnostic') ? 'active' : '' }}" href="/diagnostic">
                        <i class="fas fa-stethoscope me-1"></i> Diagnostic
                    </a>
                </li>
                <li class="nav-item">
                    <a class="nav-link {{ request()->is('about') ? 'active' : '' }}" href="/about">
                        <i class="fas fa-info-circle me-1"></i> About
                    </a>
                </li>
                <li class="nav-item ms-lg-3 mt-3 mt-lg-0 d-none d-lg-block">
                    <div class="d-flex flex-column align-items-end justify-content-center">
                        <div id="navbar-clock-time" class="fw-semibold text-white" style="font-family: 'Outfit', monospace; font-size: 0.9rem; letter-spacing: 0.08em; color: var(--accent);">00:00:00</div>
                        <div id="navbar-clock-date" class="text-secondary" style="font-size: 0.7rem; font-weight: 500;">Loading...</div>
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
        const dateString = now.toLocaleDateString('en-US', { weekday: 'short', day: 'numeric', month: 'short', year: 'numeric' });

        const formattedTime = timeString.replace(/\./g, ':');

        const navTime = document.getElementById('navbar-clock-time');
        const navDate = document.getElementById('navbar-clock-date');
        if(navTime) navTime.textContent = formattedTime;
        if(navDate) navDate.textContent = dateString;

        const mobTime = document.getElementById('mobile-clock-time');
        const mobDate = document.getElementById('mobile-clock-date');
        if(mobTime) mobTime.textContent = formattedTime;
        if(mobDate) mobDate.textContent = dateString;
    }

    updateClock();
    setInterval(updateClock, 1000);
</script>
<!-- Spacer for fixed navbar -->
<div style="height: 72px;"></div>
