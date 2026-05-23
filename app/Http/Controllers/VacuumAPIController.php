<?php

namespace App\Http\Controllers;

use App\Models\CommandLog;
use App\Models\Esp32Device;
use App\Models\VacuumStatus;
use App\Models\BatteryLog;
use Illuminate\Http\Request;

class VacuumAPIController extends Controller
{

    /**
     * POST /api/vacuum/battery
     * ESP32 mengirim data battery ke server
     */
    public function updateBattery(Request $request)
    {
        try {
            $validated = $request->validate([
                'battery_percent' => 'required|integer|between:0,100',
                'battery_voltage' => 'nullable|numeric',
                'estimated_time' => 'nullable|string'
            ]);

            // Simpan ke battery log
            $batteryLog = BatteryLog::create([
                'battery_percent' => $validated['battery_percent'],
                'battery_voltage' => $validated['battery_voltage'] ?? null,
                'estimated_time' => $validated['estimated_time'] ?? null
            ]);

            return response()->json([
                'success' => true,
                'message' => 'Battery data received',
                'data' => [
                    'battery_percent' => $batteryLog->battery_percent,
                    'battery_voltage' => $batteryLog->battery_voltage,
                    'received_at' => $batteryLog->created_at
                ]
            ], 201);

        } catch (\Illuminate\Validation\ValidationException $e) {
            return response()->json([
                'success' => false,
                'message' => 'Validasi gagal',
                'errors' => $e->errors()
            ], 422);
        }
    }

    /**
     * GET /api/vacuum/battery/latest
     * Mengambil battery data terbaru (untuk menampilkan di web)
     */
    public function getLatestBattery()
    {
        try {
            $batteryLog = BatteryLog::latest()->first();

            if (!$batteryLog) {
                return response()->json([
                    'success' => false,
                    'message' => 'Data tidak ditemukan'
                ], 404);
            }

            return response()->json([
                'success' => true,
                'data' => [
                    'battery_percent' => $batteryLog->battery_percent,
                    'battery_voltage' => $batteryLog->battery_voltage,
                    'estimated_time' => $batteryLog->estimated_time,
                    'updated_at' => $batteryLog->created_at
                ]
            ], 200);

        } catch (\Exception $e) {
            return response()->json([
                'success' => false,
                'message' => 'Error: ' . $e->getMessage()
            ], 500);
        }
    }



    /**
     * GET /api/vacuum/full-status
     * Mengambil semua status vacuum sekaligus (state + power + battery)
     * Useful untuk dashboard
     */
    public function getFullStatus()
    {
        try {
            $vacuumStatus = VacuumStatus::first();
            $batteryLog = BatteryLog::latest()->first();

            return response()->json([
                'success' => true,
                'vacuum' => [
                    'state' => $vacuumStatus->state ?? 'standby',
                    'power_mode' => $vacuumStatus->power_mode ?? 'normal',
                    'power_value' => $vacuumStatus->power_value ?? 200,
                    'updated_at' => $vacuumStatus->updated_at ?? now()
                ],
                'battery' => [
                    'percent' => $batteryLog->battery_percent ?? 0,
                    'voltage' => $batteryLog->battery_voltage ?? null,
                    'estimated_time' => $batteryLog->estimated_time ?? null,
                    'updated_at' => $batteryLog->created_at ?? now()
                ]
            ], 200);

        } catch (\Exception $e) {
            return response()->json([
                'success' => false,
                'message' => 'Error: ' . $e->getMessage()
            ], 500);
        }
    }

    /**
     * POST /v1/vacuum/register-device
     * ESP32 registers its IP and MAC on boot
     */
    public function registerDevice(Request $request)
    {
        try {
            $validated = $request->validate([
                'ip_address' => 'required|ip',
                'mac_address' => 'required|string',
                'firmware_version' => 'nullable|string',
            ]);

            $device = Esp32Device::updateOrCreate(
                ['mac_address' => $validated['mac_address']],
                [
                    'ip_address' => $validated['ip_address'],
                    'firmware_version' => $validated['firmware_version'] ?? null,
                    'last_seen_at' => now(),
                    'is_online' => true,
                ]
            );

            return response()->json([
                'success' => true,
                'message' => 'Device registered',
                'data' => [
                    'id' => $device->id,
                    'ip_address' => $device->ip_address,
                    'mac_address' => $device->mac_address,
                    'registered_at' => $device->last_seen_at,
                ]
            ], 200);

        } catch (\Illuminate\Validation\ValidationException $e) {
            return response()->json([
                'success' => false,
                'message' => 'Validation failed',
                'errors' => $e->errors()
            ], 422);
        }
    }

    /**
     * GET /v1/vacuum/device
     * Browser fetches the current ESP32 IP address
     */
    public function getDevice()
    {
        try {
            $device = Esp32Device::where('is_online', true)
                ->orderBy('last_seen_at', 'desc')
                ->first();

            if (!$device) {
                return response()->json([
                    'success' => false,
                    'message' => 'No ESP32 device registered'
                ], 404);
            }

            return response()->json([
                'success' => true,
                'data' => [
                    'ip_address' => $device->ip_address,
                    'mac_address' => $device->mac_address,
                    'firmware_version' => $device->firmware_version,
                    'last_seen_at' => $device->last_seen_at,
                    'is_online' => $device->is_online,
                ]
            ], 200);

        } catch (\Exception $e) {
            return response()->json([
                'success' => false,
                'message' => 'Error: ' . $e->getMessage()
            ], 500);
        }
    }

    /**
     * POST /v1/vacuum/command-log
     * ESP32 logs an executed command to history
     */
    public function logCommand(Request $request)
    {
        try {
            $validated = $request->validate([
                'command' => 'required|string',
                'source' => 'required|in:web,esp32',
                'status' => 'required|in:success,failed,timeout',
                'response_time_ms' => 'nullable|integer',
                'esp32_ip' => 'nullable|string',
                'payload' => 'nullable|array',
            ]);

            $log = CommandLog::create($validated);

            // Also update vacuum_statuses so dashboard stays in sync
            $stateMap = [
                'start' => 'working',
                'stop' => 'stopping',
                'return_home' => 'returning',
                'auto_stop_low_battery' => 'stopping',
            ];
            $powerMap = [
                'eco' => ['mode' => 'eco', 'value' => 150],
                'normal' => ['mode' => 'normal', 'value' => 200],
                'strong' => ['mode' => 'strong', 'value' => 255],
            ];

            if ($validated['status'] === 'success') {
                $vacuumStatus = VacuumStatus::first() ?? new VacuumStatus();

                if (isset($stateMap[$validated['command']])) {
                    $vacuumStatus->state = $stateMap[$validated['command']];
                }
                if (isset($powerMap[$validated['command']])) {
                    $vacuumStatus->power_mode = $powerMap[$validated['command']]['mode'];
                    $vacuumStatus->power_value = $powerMap[$validated['command']]['value'];
                }

                $vacuumStatus->save();
            }

            return response()->json([
                'success' => true,
                'message' => 'Command logged',
                'data' => [
                    'id' => $log->id,
                    'command' => $log->command,
                    'status' => $log->status,
                    'logged_at' => $log->created_at,
                ]
            ], 201);

        } catch (\Illuminate\Validation\ValidationException $e) {
            return response()->json([
                'success' => false,
                'message' => 'Validation failed',
                'errors' => $e->errors()
            ], 422);
        }
    }

    /**
     * POST /v1/vacuum/battery-event
     * ESP32 sends battery events (low_battery_warning, auto_stop_low_battery)
     */
    public function batteryEvent(Request $request)
    {
        try {
            $validated = $request->validate([
                'event' => 'required|in:low_battery_warning,auto_stop_low_battery',
                'battery_percent' => 'required|integer|between:0,100',
                'battery_voltage' => 'nullable|numeric',
                'power_mode' => 'nullable|string',
            ]);

            // Log as command for history
            $log = CommandLog::create([
                'command' => $validated['event'],
                'source' => 'esp32',
                'status' => 'success',
                'response_time_ms' => 0,
                'payload' => [
                    'battery_percent' => $validated['battery_percent'],
                    'battery_voltage' => $validated['battery_voltage'],
                    'power_mode' => $validated['power_mode'] ?? null,
                ],
            ]);

            // If auto-stop, update vacuum state to stopping
            if ($validated['event'] === 'auto_stop_low_battery') {
                $vacuumStatus = VacuumStatus::first();
                if ($vacuumStatus) {
                    $vacuumStatus->state = 'stopping';
                    $vacuumStatus->save();
                }
            }

            return response()->json([
                'success' => true,
                'message' => 'Battery event logged: ' . $validated['event'],
                'data' => [
                    'id' => $log->id,
                    'event' => $validated['event'],
                    'battery_percent' => $validated['battery_percent'],
                    'logged_at' => $log->created_at,
                ]
            ], 201);

        } catch (\Illuminate\Validation\ValidationException $e) {
            return response()->json([
                'success' => false,
                'message' => 'Validation failed',
                'errors' => $e->errors()
            ], 422);
        }
    }

    /**
     * GET /v1/vacuum/battery-events/latest
     * Get the latest battery event (for dashboard notification)
     */
    public function getLatestBatteryEvent()
    {
        try {
            $event = CommandLog::whereIn('command', ['low_battery_warning', 'auto_stop_low_battery'])
                ->where('created_at', '>=', now()->subMinutes(5))
                ->orderBy('created_at', 'desc')
                ->first();

            if (!$event) {
                return response()->json([
                    'success' => true,
                    'data' => null
                ], 200);
            }

            return response()->json([
                'success' => true,
                'data' => [
                    'event' => $event->command,
                    'battery_percent' => $event->payload['battery_percent'] ?? null,
                    'battery_voltage' => $event->payload['battery_voltage'] ?? null,
                    'power_mode' => $event->payload['power_mode'] ?? null,
                    'created_at' => $event->created_at,
                ]
            ], 200);

        } catch (\Exception $e) {
            return response()->json([
                'success' => false,
                'message' => 'Error: ' . $e->getMessage()
            ], 500);
        }
    }

    /**
     * GET /v1/vacuum/command-logs
     * Fetch recent command logs for dashboard display
     */
    public function getCommandLogs(Request $request)
    {
        try {
            $limit = min($request->input('limit', 20), 100);

            $logs = CommandLog::orderBy('created_at', 'desc')
                ->limit($limit)
                ->get(['id', 'command', 'source', 'status', 'response_time_ms', 'esp32_ip', 'created_at']);

            return response()->json([
                'success' => true,
                'data' => $logs
            ], 200);

        } catch (\Exception $e) {
            return response()->json([
                'success' => false,
                'message' => 'Error: ' . $e->getMessage()
            ], 500);
        }
    }
}