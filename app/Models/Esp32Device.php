<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Esp32Device extends Model
{
    use HasFactory;

    protected $fillable = [
        'ip_address',
        'mac_address',
        'firmware_version',
        'last_seen_at',
        'is_online',
    ];

    protected $casts = [
        'last_seen_at' => 'datetime',
        'is_online' => 'boolean',
        'created_at' => 'datetime',
        'updated_at' => 'datetime',
    ];
}
