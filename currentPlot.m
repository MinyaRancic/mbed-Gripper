clear all; close all; clc

delete(instrfindall);
grip = mbedGripper('COM6');

grip.set('Angle', 50)
pause(.01);

for i = 1:100
    out(i) = grip.get('Current');
    pause(.01);
end

plot(timeseries(out, 1:100));