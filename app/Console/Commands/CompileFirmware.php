<?php

namespace App\Console\Commands;

use Illuminate\Console\Command;

class CompileFirmware extends Command
{
    protected $signature = 'firmware:compile
                            {--sketch=VacuumRobot : Sketch folder name inside firmware/}
                            {--fqbn=esp32:esp32:esp32 : Fully Qualified Board Name}
                            {--cli-path= : Full path to arduino-cli.exe}
                            {--upload : Flash to ESP32 via USB after compile}
                            {--port= : COM port for upload (e.g. COM3)}';

    protected $description = 'Compile ESP32 firmware using arduino-cli and output .bin to firmware/';

    /**
     * Resolve arduino-cli executable path.
     * Priority: --cli-path option > .env ARDUINO_CLI_PATH > PATH > common locations
     */
    private function resolveCli(): ?string
    {
        // 1. Explicit --cli-path option
        $opt = $this->option('cli-path');
        if ($opt && file_exists($opt)) {
            return $opt;
        }

        // 2. .env variable
        $envPath = env('ARDUINO_CLI_PATH');
        if ($envPath && file_exists($envPath)) {
            return $envPath;
        }

        // 3. Check system PATH
        $pathCheck = trim(shell_exec('where.exe arduino-cli 2>NUL') ?? '');
        if (!empty($pathCheck)) {
            return trim(explode("\n", $pathCheck)[0]);
        }

        // 4. Common install locations
        $commonPaths = [
            'C:\\arduino_cli\\arduino-cli.exe',
            'C:\\arduino-cli\\arduino-cli.exe',
            'C:\\Program Files\\arduino-cli\\arduino-cli.exe',
            'C:\\Program Files (x86)\\arduino-cli\\arduino-cli.exe',
            'C:\\tools\\arduino-cli.exe',
            'C:\\ProgramData\\chocolatey\\bin\\arduino-cli.exe',
            'D:\\arduino-cli\\arduino-cli.exe',
        ];

        // Also scan user home directories
        $home = getenv('USERPROFILE') ?: getenv('HOME');
        if ($home) {
            $commonPaths[] = "{$home}\\arduino-cli\\arduino-cli.exe";
            $commonPaths[] = "{$home}\\AppData\\Local\\Arduino15\\arduino-cli.exe";
            $commonPaths[] = "{$home}\\scoop\\shims\\arduino-cli.exe";
        }

        foreach ($commonPaths as $p) {
            if (file_exists($p)) {
                return $p;
            }
        }

        return null;
    }

    public function handle()
    {
        $sketch = $this->option('sketch');
        $fqbn = $this->option('fqbn');
        $basePath = base_path('firmware');
        $sketchPath = "{$basePath}\\{$sketch}";
        $outputBin = "{$basePath}\\{$sketch}.bin";

        // Verify sketch exists
        if (!is_dir($sketchPath)) {
            $this->error("Sketch not found: {$sketchPath}");
            return 1;
        }

        $inoFile = "{$sketchPath}\\{$sketch}.ino";
        if (!file_exists($inoFile)) {
            $this->error("INO file not found: {$inoFile}");
            return 1;
        }

        // Resolve arduino-cli path
        $cli = $this->resolveCli();
        if (!$cli) {
            $this->error('arduino-cli not found.');
            $this->line('  Fix: Add arduino-cli to PATH, or set ARDUINO_CLI_PATH in .env, or use --cli-path option.');
            $this->line('  Example: ARDUINO_CLI_PATH=C:\\arduino_cli\\arduino-cli.exe');
            $this->line('  Download: https://arduino.github.io/arduino-cli/latest/installation/');
            return 1;
        }
        $cliVersion = trim(explode("\n", shell_exec("\"{$cli}\" version 2>&1") ?? '')[0]);
        $this->info("Using: {$cliVersion}");
        $this->line("  Path : {$cli}");

        $this->newLine();
        $this->line("╔══════════════════════════════════════════╗");
        $this->line("║     ESP32 Firmware Compiler              ║");
        $this->line("╚══════════════════════════════════════════╝");
        $this->newLine();
        $this->line("  Sketch : {$sketch}");
        $this->line("  Board  : {$fqbn}");
        $this->line("  Source : {$sketchPath}");
        $this->line("  Output : {$outputBin}");
        $this->newLine();

        // Compile — output directly to firmware/
        $this->info('>>> Compiling...');
        $this->newLine();

        $compileCmd = sprintf(
            '"%s" compile --clean --fqbn %s --output-dir "%s" "%s" 2>&1',
            $cli,
            $fqbn,
            $basePath,
            $sketchPath
        );

        $startTime = microtime(true);

        // Run compile and stream output
        $process = popen($compileCmd, 'r');
        $output = '';
        while (!feof($process)) {
            $line = fgets($process);
            if ($line !== false) {
                $output .= $line;
                // Show warnings/errors in real-time
                $trimmed = trim($line);
                if (!empty($trimmed)) {
                    if (stripos($line, 'error') !== false) {
                        $this->error($trimmed);
                    } elseif (stripos($line, 'warning') !== false) {
                        $this->warn($trimmed);
                    } elseif (stripos($line, 'Used:') !== false || stripos($line, 'Global') !== false) {
                        $this->line("  {$trimmed}");
                    }
                }
            }
        }
        $exitCode = pclose($process);
        $elapsed = round(microtime(true) - $startTime, 1);

        $this->newLine();

        if ($exitCode !== 0) {
            $this->error("Compilation FAILED (exit code: {$exitCode})");
            $this->newLine();
            // Show full error output
            $this->line($output);
            return 1;
        }

        // Find the compiled .bin
        $compiledBin = "{$basePath}\\{$sketch}.ino.bin";
        if (!file_exists($compiledBin)) {
            $this->error("Compiled .bin not found: {$compiledBin}");
            return 1;
        }

        // Rename .ino.bin to clean name
        if ($compiledBin !== $outputBin) {
            // Remove old output if exists, then rename
            if (file_exists($outputBin)) unlink($outputBin);
            rename($compiledBin, $outputBin);
        }

        // Clean up intermediate build artifacts from firmware/
        $artifacts = ["{$sketch}.ino.elf", "{$sketch}.ino.map", "{$sketch}.ino.bootloader.bin",
                       "{$sketch}.ino.partitions.bin", "{$sketch}.ino.merged.bin"];
        foreach ($artifacts as $file) {
            $path = "{$basePath}\\{$file}";
            if (file_exists($path)) unlink($path);
        }

        $binSize = filesize($outputBin);
        $sizeKb = round($binSize / 1024, 1);
        $sizeMb = round($binSize / 1048576, 2);

        $this->info(">>> Compilation SUCCESSFUL ({$elapsed}s)");
        $this->newLine();
        $this->line("╔══════════════════════════════════════════╗");
        $this->line("║  Output Files                            ║");
        $this->line("╚══════════════════════════════════════════╝");
        $this->line("  .bin : {$outputBin}");
        $this->line("  Size : {$sizeKb} KB ({$sizeMb} MB)");
        $this->newLine();

        // Flash if --upload flag
        if ($this->option('upload')) {
            $port = $this->option('port');
            if (!$port) {
                // Try to auto-detect
                $boardList = shell_exec("\"{$cli}\" board list 2>&1");
                $this->line("Detected boards:");
                $this->line($boardList);
                $port = $this->ask('Enter COM port (e.g. COM3)');
                if (!$port) {
                    $this->warn('Upload skipped — no port specified.');
                    return 0;
                }
            }

            $this->info(">>> Flashing to {$port}...");
            $uploadCmd = sprintf(
                '"%s" upload --fqbn %s --port %s "%s" 2>&1',
                $cli,
                $fqbn,
                $port,
                $sketchPath
            );

            $uploadOutput = shell_exec($uploadCmd);
            if (stripos($uploadOutput, 'error') !== false || stripos($uploadOutput, 'failed') !== false) {
                $this->error("Upload FAILED:");
                $this->line($uploadOutput);
                return 1;
            }

            $this->info(">>> Flash SUCCESSFUL!");
            $this->line($uploadOutput);
        }

        return 0;
    }
}
