<?php

use App\Http\Controllers\VacuumAPIController;
use Illuminate\Support\Facades\Route;

Route::get('/', function () {
    return view('main');
});
Route::get('about', function () {
    return view('about');
});
Route::get('diagnostic', function () {
    return view('diagnostic');
});

// ===== VACUUM API v1 Routes =====
Route::prefix('v1')->name('api.')->group(function () {
    Route::prefix('vacuum')->group(function () {
        
        // ===== GET Endpoints (Polling from ESP32 & Web App) =====
        Route::get('status', [VacuumAPIController::class, 'getStatus'])->name('vacuum.status');
        Route::get('battery/latest', [VacuumAPIController::class, 'getLatestBattery'])->name('vacuum.battery.latest');
        Route::get('battery/history', [VacuumAPIController::class, 'getBatteryHistory'])->name('vacuum.battery.history');
        Route::get('full-status', [VacuumAPIController::class, 'getFullStatus'])->name('vacuum.full-status');
        Route::get('command-logs', [VacuumAPIController::class, 'getCommandLogs'])->name('vacuum.command-logs');
        
        // ===== POST Endpoints (Commands from Web App & ESP32) =====
        Route::post('command', [VacuumAPIController::class, 'sendCommand'])->name('vacuum.command');
        Route::post('battery', [VacuumAPIController::class, 'updateBattery'])->name('vacuum.battery');

        // ===== Direct HTTP Architecture Endpoints =====
        Route::post('register-device', [VacuumAPIController::class, 'registerDevice'])->name('vacuum.register-device');
        Route::get('device', [VacuumAPIController::class, 'getDevice'])->name('vacuum.device');
        Route::post('command-log', [VacuumAPIController::class, 'logCommand'])->name('vacuum.command-log');

        // ===== Battery Protection Events =====
        Route::post('battery-event', [VacuumAPIController::class, 'batteryEvent'])->name('vacuum.battery-event');
        Route::get('battery-events/latest', [VacuumAPIController::class, 'getLatestBatteryEvent'])->name('vacuum.battery-events.latest');
    });
});