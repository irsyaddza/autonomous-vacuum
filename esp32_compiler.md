###Need arduino-cli in your computer!###

1. **Compile only (outputs .bin to firmware/)**
```
php artisan firmware:compile
```

2. **Compile + Flash to ESP32 via USB**
```
php artisan firmware:compile --upload --port=COM3
```

3. **Custom sketch name (if not VacuumRobot)**
```
php artisan firmware:compile --sketch=MyOtherSketch
```
*D:\laragon\www\autonomous_vacuum\app\Console\Commands\CompileFirmware.php* //script location