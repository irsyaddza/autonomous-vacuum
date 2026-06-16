<!DOCTYPE html>
<html lang="en" data-bs-theme="dark">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Monitor and control your autonomous vacuum robot — real-time dashboard, sensor diagnostic, and firmware tuning.">
    <meta name="author" content="Autonomous Vacuum">
    <meta name="csrf-token" content="{{ csrf_token() }}">

    <!-- Open Graph -->
    <meta property="og:title" content="Autonomous Vacuum — Control Panel">
    <meta property="og:description" content="Real-time dashboard and control for ESP32-based autonomous vacuum robot.">
    <meta property="og:type" content="website">

    <title>@yield('title', 'Control Panel') — Autonomous Vacuum</title>

    <!-- FontAwesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">

    <!-- Google Fonts: Outfit -->
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700;800&display=swap" rel="stylesheet">

    <!-- Bootstrap 5.3 CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom Premium Dark Styles -->
    <link href="{{ asset('css/custom-dark.css') }}" rel="stylesheet">
</head>

<body id="page-top">

    <!-- Skip to Content -->
    <a href="#main-content" class="skip-link">Skip to content</a>

    <!-- Main Content Wrapper -->
    <div class="d-flex flex-column min-vh-100">

        <!-- Topbar -->
        <x-navbar></x-navbar>

        <!-- Main Content -->
        <main id="main-content" class="flex-grow-1 py-4 fade-in">
            <div class="container" style="max-width: 1320px;">
                {{$slot}}
            </div>
        </main>

        <!-- Footer -->
        <x-footer></x-footer>

    </div>

    <!-- Scroll to Top Button-->
    <a class="scroll-to-top rounded" href="#page-top" style="display: none;">
        <i class="fas fa-angle-up"></i>
    </a>

    <!-- Bootstrap 5 Bundle with Popper -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>

</body>

</html>
