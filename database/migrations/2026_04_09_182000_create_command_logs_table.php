<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    public function up(): void
    {
        Schema::create('command_logs', function (Blueprint $table) {
            $table->id();
            $table->string('command');          // start, stop, return_home, eco, normal, strong
            $table->string('source');           // web, esp32
            $table->string('status');           // success, failed, timeout
            $table->integer('response_time_ms')->nullable(); // round-trip time
            $table->string('esp32_ip')->nullable();
            $table->json('payload')->nullable(); // raw request/response data
            $table->timestamps();
        });
    }

    public function down(): void
    {
        Schema::dropIfExists('command_logs');
    }
};
