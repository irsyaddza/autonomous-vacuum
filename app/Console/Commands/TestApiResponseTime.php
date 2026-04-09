<?php

namespace App\Console\Commands;

use Illuminate\Console\Command;
use Illuminate\Support\Facades\Http;
use Illuminate\Support\Facades\DB;

class TestApiResponseTime extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'test:api-response-time';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Test API response times and fetch results using Laravel Telescope';

    /**
     * Execute the console command.
     */
    public function handle()
    {
        set_time_limit(0);
        
        $endpoints = [
            ['uri' => 'v1/vacuum/status', 'method' => 'GET', 'payload' => []],
            ['uri' => 'v1/vacuum/battery/latest', 'method' => 'GET', 'payload' => []],
            ['uri' => 'v1/vacuum/device', 'method' => 'GET', 'payload' => []],
            ['uri' => 'v1/vacuum/full-status', 'method' => 'GET', 'payload' => []],
            ['uri' => 'v1/vacuum/command-log', 'method' => 'POST', 'payload' => [
                'command' => 'test_start',
                'source' => 'web',
                'status' => 'success',
                'response_time_ms' => 120,
                'esp32_ip' => '192.168.1.100'
            ]],
            ['uri' => 'v1/vacuum/register-device', 'method' => 'POST', 'payload' => [
                'ip_address' => '192.168.1.100',
                'mac_address' => '00:11:22:33:44:55',
                'firmware_version' => '2.0.0'
            ]],
            ['uri' => 'v1/vacuum/battery/history', 'method' => 'GET', 'payload' => []],
            ['uri' => 'v1/vacuum/command', 'method' => 'POST', 'payload' => [
                'command' => 'start'
            ]],
            ['uri' => 'v1/vacuum/power-mode', 'method' => 'POST', 'payload' => [
                'mode' => 'eco'
            ]],
            ['uri' => 'v1/vacuum/battery', 'method' => 'POST', 'payload' => [
                'level' => 85,
                'is_charging' => false
            ]]
        ];

        $samples = 50;
        $baseUrl = 'http://autonomous_vacuum.test/';

        $this->info("Starting API Response Time tests with $samples samples per endpoint...\n");

        // Clear previous telescope request records
        DB::table('telescope_entries')->where('type', 'request')->delete();

        $results = [];

        foreach ($endpoints as $ep) {
            $this->info("Testing /{$ep['uri']} ({$ep['method']})...");
            $bar = $this->output->createProgressBar($samples);
            $bar->start();

            $durations = [];
            for ($i = 0; $i < $samples; $i++) {
                try {
                    $url = rtrim(config('app.url', 'http://localhost'), '/') . '/' . ltrim($ep['uri'], '/');
                    $request = \Illuminate\Http\Request::create($url, $ep['method'], $ep['payload'] ?? []);
                    
                    $start = microtime(true);
                    $response = app()->handle($request);
                    $end = microtime(true);
                    
                    $durations[] = ($end - $start) * 1000; // convert to ms
                } catch (\Exception $e) {
                    // Ignore
                }
                $bar->advance();
            }
            $bar->finish();
            $this->newLine();
            
            if (count($durations) > 0) {
                $min = min($durations);
                $max = max($durations);
                $avg = array_sum($durations) / count($durations);
                
                $results[] = [
                    'endpoint' => '/' . $ep['uri'],
                    'samples' => count($durations),
                    'method' => $ep['method'],
                    'min' => round($min),
                    'max' => round($max),
                    'avg' => number_format($avg, 2, ',', '')
                ];
            } else {
                $results[] = [
                    'endpoint' => '/' . $ep['uri'],
                    'samples' => 0,
                    'method' => $ep['method'],
                    'min' => '-',
                    'max' => '-',
                    'avg' => '-'
                ];
            }
        }

        $this->info("\nProcessing results...\n");

        // Create markdown table header
        $this->line("| Endpoint | Total Sample | Method | Min(ms) | Max(ms) | Avg(ms) |");
        $this->line("|---|---|---|---|---|---|");

        foreach ($results as $res) {
            $this->line(sprintf("| %s | %s | %s | %s | %s | %s |",
                $res['endpoint'],
                $res['samples'],
                $res['method'],
                $res['min'],
                $res['max'],
                $res['avg']
            ));
        }

        $this->newLine();
        $this->info("Done! You can copy the markdown table above to Excel/Word if needed.");
    }
}
