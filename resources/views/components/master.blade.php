<!DOCTYPE html>
<html lang="en" data-bs-theme="dark">

<head>

    <meta name="csrf-token" content="{{ csrf_token() }}">
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Autonomous Vacuum Control Panel">
    <meta name="author" content="">

    <title>Autonomous Vacuum - Control Panel</title>

    <!-- FontAwesome -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css?family=Nunito:400,400i,600,600i,700,700i,800,800i" rel="stylesheet">

    <!-- Bootstrap 5.3 CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom Premium Dark Styles -->
    <link href="{{ asset('css/custom-dark.css') }}" rel="stylesheet">

</head>

<body id="page-top">

    <!-- Main Content Wrapper -->
    <div class="d-flex flex-column min-vh-100">

        <!-- Topbar -->
        <x-navbar></x-navbar>

        <!-- Main Content -->
        <main class="flex-grow-1 py-4 fade-in">
            <div class="container">
                {{$slot}}
            </div>
        </main>

        <!-- Footer -->
        <x-footer></x-footer>

    </div>
    <!-- End of Page Wrapper -->

    <!-- Scroll to Top Button-->
    <a class="scroll-to-top rounded" href="#page-top" style="display: none;">
        <i class="fas fa-angle-up"></i>
    </a>

    <!-- Bootstrap 5 Bundle with Popper -->
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js"></script>

</body>

</html>
