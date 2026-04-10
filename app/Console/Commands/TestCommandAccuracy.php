<?php

namespace App\Console\Commands;

use Illuminate\Console\Command;
use Illuminate\Support\Facades\Http;
use App\Models\Esp32Device;

class TestCommandAccuracy extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'test:command-accuracy {--samples=50} {--ip=}';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Test physical command accuracy and latency to the ESP32 hardware';

    /**
     * Execute the console command.
     */
    public function handle()
    {
        set_time_limit(0);
        
        $samples = (int) $this->option('samples');
        $esp32Ip = $this->option('ip');

        if (!$esp32Ip) {
            $device = Esp32Device::latest('last_seen_at')->first();
            if (!$device) {
                $this->error("No ESP32 device registered in the database. Please specify IP manually using --ip=192.168.x.x");
                return 1;
            }
            $esp32Ip = $device->ip_address;
        }

        $this->info("Starting Command Accuracy Test ($samples samples per command)");
        $this->info("Target ESP32 IP: $esp32Ip\n");

        $commands = [
            ['name' => 'START', 'payload' => ['command' => 'start']],
            ['name' => 'STOP', 'payload' => ['command' => 'stop']],
            ['name' => 'RETURN', 'payload' => ['command' => 'return_home']],
            ['name' => 'ECO', 'payload' => ['command' => 'eco', 'value' => 150]],
            ['name' => 'NORMAL', 'payload' => ['command' => 'normal', 'value' => 200]],
            ['name' => 'STRONG', 'payload' => ['command' => 'strong', 'value' => 255]],
        ];

        $results = [];

        foreach ($commands as $cmd) {
            $this->info("Testing Command: {$cmd['name']}...");
            $bar = $this->output->createProgressBar($samples);
            $bar->start();

            $durations = [];
            $successCount = 0;
            $failCount = 0;

            for ($i = 0; $i < $samples; $i++) {
                try {
                    $url = "http://{$esp32Ip}/command";
                    
                    $start = microtime(true);
                    
                    // Sending direct HTTP POST to ESP32 with 3s timeout
                    $response = Http::timeout(3)->post($url, $cmd['payload']);
                    
                    $end = microtime(true);
                    
                    if ($response->successful() && $response->json('success') === true) {
                        $successCount++;
                        $durations[] = ($end - $start) * 1000;
                    } else {
                        $failCount++;
                    }
                } catch (\Exception $e) {
                    $failCount++;
                }
                
                // Add a small delay between commands to prevent overloading the ESP32 server
                usleep(200000); 
                $bar->advance();
            }
            $bar->finish();
            $this->newLine();
            
            $accuracy = ($samples > 0) ? ($successCount / $samples) * 100 : 0;

            if (count($durations) > 0) {
                $min = min($durations);
                $max = max($durations);
                $avg = array_sum($durations) / count($durations);
                
                $results[] = [
                    'name' => $cmd['name'],
                    'tests' => $samples,
                    'success' => $successCount,
                    'fail' => $failCount,
                    'accuracy' => number_format($accuracy, 0),
                    'avg' => number_format($avg, 2, '.', ''),
                    'min' => number_format($min, 2, '.', ''),
                    'max' => number_format($max, 2, '.', '')
                ];
            } else {
                $results[] = [
                    'name' => $cmd['name'],
                    'tests' => $samples,
                    'success' => $successCount,
                    'fail' => $failCount,
                    'accuracy' => number_format($accuracy, 0),
                    'avg' => '-',
                    'min' => '-',
                    'max' => '-'
                ];
            }
        }

        $this->info("\nProcessing results...\n");

        // Create markdown table
        $this->line("| Commands | Tests | Success | Fail | Accuracy (%) | Avg (ms) | Min (ms) | Max (ms) |");
        $this->line("|---|---|---|---|---|---|---|---|");

        foreach ($results as $res) {
            $this->line(sprintf("| %s | %d | %d | %d | %s | %s | %s | %s |",
                $res['name'],
                $res['tests'],
                $res['success'],
                $res['fail'],
                $res['accuracy'],
                $res['avg'],
                $res['min'],
                $res['max']
            ));
        }

        $this->newLine();
        $this->info("Done! Copy the markdown table above for your dataset.");
        return 0;
    }
}
