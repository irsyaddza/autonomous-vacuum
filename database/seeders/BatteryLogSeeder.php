<?php
// database/seeders/BatteryLogSeeder.php

namespace Database\Seeders;

use App\Models\BatteryLog;
use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class BatteryLogSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        // Create initial battery log records
        // Simulate battery from 100% to 85% over time
        
        $baseTime = now()->subMinutes(60); // Start 60 minutes ago
        
        $batteryData = [
            ['percent' => 100, 'voltage' => 12.60, 'time' => $baseTime->copy()->addMinutes(0)],
        ];
        
        foreach ($batteryData as $data) {
            BatteryLog::create([
                'battery_percent' => $data['percent'],
                'battery_voltage' => $data['voltage'],
                'estimated_time' => $this->calculateEstimatedTime($data['percent']),
                'created_at' => $data['time'],
                'updated_at' => $data['time']
            ]);
        }

        echo "✓ Battery logs seeded: " . count($batteryData) . " records created\n";
    }

    /**
     * Calculate estimated remaining time based on battery percentage
     */
    private function calculateEstimatedTime($batteryPercent)
    {
        $estimatedMinutes = ($batteryPercent * 225) / 100; // 225 menit = 3.75 jam max
        $hours = intdiv($estimatedMinutes, 60);
        $minutes = $estimatedMinutes % 60;
        
        if ($hours > 0) {
            return $hours . 'h ' . round($minutes) . 'm';
        } else {
            return round($minutes) . 'm';
        }
    }
}