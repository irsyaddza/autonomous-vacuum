<?php

namespace Database\Seeders;

use Illuminate\Database\Seeder;
use App\Models\Esp32Device;

class Esp32DeviceSeeder extends Seeder
{
    /**
     * Run the database seeds.
     */
    public function run(): void
    {
        Esp32Device::firstOrCreate(
            ['mac_address' => '00:11:22:33:44:55'],
            [
                'ip_address' => '192.168.1.9',
                'firmware_version' => 'v1.0.0',
                'last_seen_at' => now(),
                'is_online' => true,
            ]
        );

        echo "✓ Default ESP32 Device created: 192.168.1.5\n";
    }
}
