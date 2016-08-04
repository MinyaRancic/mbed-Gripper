clear all; close all; clc

delete(instrfindall);
pause(.01);
grip = mbedGripper('COM6');

grip.set('kP', 5);
pause(.1);
grip.set('kI', 0);
pause(.1);
grip.set('kD', 0);
pause(.1);
grip.set('Angle', 25);
pause(.1);

for i = 1:100
    out(i) = grip.get('error');
    pause(.01);
    %in(i) = grip.get('input');
    %pause(.01);
end

plot(timeseries(out, 1:100));