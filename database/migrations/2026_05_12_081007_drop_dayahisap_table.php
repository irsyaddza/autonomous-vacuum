<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::dropIfExists('dayahisap');
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        Schema::create('dayahisap', function (Blueprint $table) {
            $table->id();
            $table->integer('value');
            $table->enum('mode', ['eco', 'normal', 'strong'])->nullable();
            $table->timestamps();
            $table->index('created_at');
        });
    }
};
