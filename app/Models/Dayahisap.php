<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Dayahisap extends Model
{
    use HasFactory;

    protected $table = 'dayahisap';

    protected $fillable = [
        'value'
    ];

    protected $casts = [
        'created_at' => 'datetime',
        'updated_at' => 'datetime'
    ];
}