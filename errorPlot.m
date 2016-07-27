clear all; close all; clc

delete(instrfindall);
grip = mbedGripper('COM6');

grip.set('kP', 1);
pause(.01);
grip.set('kI', 0);
pause(.01);
grip.set('kD', 0);
pause(.01);
grip.set('Angle', 50);
pause(.01);

for i = 1:100
    out(i) = grip.get('error');
    pause(.01);
    in(i) = grip.get('input');
    pause(.01);
end

plot(timeseries(out, in));