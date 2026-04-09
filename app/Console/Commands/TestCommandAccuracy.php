<?php

namespace App\Console\Commands;

use Illuminate\Console\Command;
use Illuminate\Support\Facades\Http;

class TestCommandAccuracy extends Command
{
    protected $signature = 'test:command-accuracy 
                            {--count=50 : Number of times to send each command}
                            {--base-url=http://127.0.0.1:8000 : Base URL of the server}
                            {--delay=100 : Delay between requests in ms}';

    protected $description = 'Test command accuracy by sending each vacuum command N times and tracking results';

    public function handle()
    {
        $count = (int) $this->option('count');
        $baseUrl = rtrim($this->option('base-url'), '/');
        $delayMs = (int) $this->option('delay');
        $apiBase = $baseUrl . '/v1/vacuum';

        $this->newLine();
        $this->info('╔══════════════════════════════════════════════════════╗');
        $this->info('║       VACUUM COMMAND ACCURACY TEST                  ║');
        $this->info('╚══════════════════════════════════════════════════════╝');
        $this->newLine();
        $this->info("Base URL     : {$apiBase}");
        $this->info("Per command  : {$count} requests");
        $this->info("Delay        : {$delayMs}ms between requests");
        $this->info("Total planned: " . ($count * 6) . " requests");
        $this->newLine();

        // Get CSRF cookie first by visiting the main page
        $this->info('⏳ Fetching CSRF token...');
        try {
            $initResponse = Http::get($baseUrl);
            // Extract CSRF token from meta tag
            preg_match('/meta name="csrf-token" content="([^"]+)"/', $initResponse->body(), $matches);
            $csrfToken = $matches[1] ?? null;

            if (!$csrfToken) {
                $this->error('❌ Could not extract CSRF token from page.');
                return 1;
            }
            $this->info("✅ CSRF token obtained: " . substr($csrfToken, 0, 15) . '...');

            // Get cookies from response
            $cookies = $initResponse->cookies();
        } catch (\Exception $e) {
            $this->error('❌ Cannot connect to server: ' . $e->getMessage());
            $this->error('   Make sure `php artisan serve` is running.');
            return 1;
        }

        $this->newLine();

        // Define test commands
        $tests = [
            // Command endpoint tests
            ['label' => 'START',       'url' => "{$apiBase}/command",    'payload' => ['command' => 'start']],
            ['label' => 'STOP',        'url' => "{$apiBase}/command",    'payload' => ['command' => 'stop']],
            ['label' => 'RETURN_HOME', 'url' => "{$apiBase}/command",    'payload' => ['command' => 'return_home']],
            // Power mode endpoint tests
            ['label' => 'ECO',         'url' => "{$apiBase}/power-mode", 'payload' => ['mode' => 'eco',    'value' => 150]],
            ['label' => 'NORMAL',      'url' => "{$apiBase}/power-mode", 'payload' => ['mode' => 'normal', 'value' => 200]],
            ['label' => 'STRONG',      'url' => "{$apiBase}/power-mode", 'payload' => ['mode' => 'strong', 'value' => 255]],
        ];

        $allResults = [];

        foreach ($tests as $test) {
            $this->info("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
            $this->info("🧪 Testing: {$test['label']}");
            $this->info("   Endpoint: {$test['url']}");
            $this->info("   Payload : " . json_encode($test['payload']));
            $this->newLine();

            $success = 0;
            $fail = 0;
            $responseTimes = [];
            $errors = [];

            $bar = $this->output->createProgressBar($count);
            $bar->setFormat(' %current%/%max% [%bar%] %percent:3s%% | %elapsed:6s% ');
            $bar->start();

            for ($i = 1; $i <= $count; $i++) {
                try {
                    $startTime = microtime(true);

                    $response = Http::withHeaders([
                        'X-CSRF-TOKEN' => $csrfToken,
                        'Content-Type' => 'application/json',
                        'Accept' => 'application/json',
                    ])->withOptions([
                        'cookies' => $cookies,
                    ])->post($test['url'], $test['payload']);

                    $elapsed = round((microtime(true) - $startTime) * 1000, 2); // ms
                    $responseTimes[] = $elapsed;

                    $body = $response->json();

                    if ($response->successful() && ($body['success'] ?? false) === true) {
                        $success++;
                    } else {
                        $fail++;
                        $errors[] = "#{$i}: HTTP {$response->status()} - " . ($body['message'] ?? 'Unknown error');
                    }
                } catch (\Exception $e) {
                    $fail++;
                    $elapsed = round((microtime(true) - $startTime) * 1000, 2);
                    $responseTimes[] = $elapsed;
                    $errors[] = "#{$i}: Exception - " . $e->getMessage();
                }

                $bar->advance();

                // Small delay between requests
                if ($delayMs > 0 && $i < $count) {
                    usleep($delayMs * 1000);
                }
            }

            $bar->finish();
            $this->newLine(2);

            // Calculate stats
            $avgTime = count($responseTimes) > 0 ? round(array_sum($responseTimes) / count($responseTimes), 2) : 0;
            $minTime = count($responseTimes) > 0 ? round(min($responseTimes), 2) : 0;
            $maxTime = count($responseTimes) > 0 ? round(max($responseTimes), 2) : 0;
            $accuracy = $count > 0 ? round(($success / $count) * 100, 2) : 0;

            $allResults[] = [
                'command'    => $test['label'],
                'total'      => $count,
                'success'    => $success,
                'fail'       => $fail,
                'accuracy'   => $accuracy,
                'avg_ms'     => $avgTime,
                'min_ms'     => $minTime,
                'max_ms'     => $maxTime,
            ];

            // Print per-command result
            $this->info("   ✅ Success  : {$success}/{$count}");
            if ($fail > 0) {
                $this->error("   ❌ Failed   : {$fail}/{$count}");
            }
            $this->info("   📊 Accuracy : {$accuracy}%");
            $this->info("   ⏱  Avg Time : {$avgTime}ms (min: {$minTime}ms, max: {$maxTime}ms)");

            if (count($errors) > 0) {
                $this->newLine();
                $this->warn("   Errors:");
                foreach (array_slice($errors, 0, 5) as $err) {
                    $this->warn("     · {$err}");
                }
                if (count($errors) > 5) {
                    $this->warn("     · ...and " . (count($errors) - 5) . " more");
                }
            }

            $this->newLine();
        }

        // ========= SUMMARY TABLE =========
        $this->newLine();
        $this->info('╔══════════════════════════════════════════════════════════════════════════════╗');
        $this->info('║                         FINAL SUMMARY                                      ║');
        $this->info('╚══════════════════════════════════════════════════════════════════════════════╝');
        $this->newLine();

        $this->table(
            ['Command', 'Total', 'Success', 'Fail', 'Accuracy %', 'Avg (ms)', 'Min (ms)', 'Max (ms)'],
            array_map(fn($r) => [
                $r['command'],
                $r['total'],
                $r['success'],
                $r['fail'],
                $r['accuracy'] . '%',
                $r['avg_ms'],
                $r['min_ms'],
                $r['max_ms'],
            ], $allResults)
        );

        // Overall stats
        $totalRequests = array_sum(array_column($allResults, 'total'));
        $totalSuccess  = array_sum(array_column($allResults, 'success'));
        $totalFail     = array_sum(array_column($allResults, 'fail'));
        $overallAccuracy = $totalRequests > 0 ? round(($totalSuccess / $totalRequests) * 100, 2) : 0;
        $overallAvg    = round(array_sum(array_column($allResults, 'avg_ms')) / count($allResults), 2);

        $this->newLine();
        $this->info("📋 Overall Results:");
        $this->info("   Total Requests  : {$totalRequests}");
        $this->info("   Total Success   : {$totalSuccess}");
        $this->info("   Total Failed    : {$totalFail}");
        $this->info("   Overall Accuracy: {$overallAccuracy}%");
        $this->info("   Avg Response    : {$overallAvg}ms");
        $this->newLine();

        if ($overallAccuracy >= 100) {
            $this->info('🎉 PERFECT! All commands delivered successfully!');
        } elseif ($overallAccuracy >= 95) {
            $this->warn('⚠️  Some commands failed. Check error logs above.');
        } else {
            $this->error('❌ Significant failures detected. Review server logs.');
        }

        $this->newLine();
        return 0;
    }
}
