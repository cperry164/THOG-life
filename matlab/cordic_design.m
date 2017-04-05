clc;
clear all;

max_iter=24;
n = 0:1:max_iter;

twiddle = atan(2.^-n);

fixed_point=10;

fixed_point_one=2^fixed_point;

twiddle = round(fixed_point_one*twiddle,0);

fprintf(1,'\n');
for i=1:length(n)
    fprintf(1,'0x%08X, ',twiddle(i));
end
fprintf(1,'\n');

fprintf(1,'one = 0x%08X \n',fixed_point_one);
fprintf(1,'K = 0x%08X \n',round(fixed_point_one*0.6072529350088812561694));
fprintf(1,'pi = 0x%08X \n',round(fixed_point_one*pi));
fprintf(1,'half_pi = 0x%08X \n',round(fixed_point_one*pi/2));
fprintf(1,'pi/9 = 0x%08X \n',round(fixed_point_one*pi/9));

fprintf(1,'binsAngle=[');
for i=0:8
    fprintf(1,'0x%08X, ',round(i*fixed_point_one*pi/9));
end
fprintf(1,']\n');
fprintf(1,'255 = 0x%08X \n',round(fixed_point_one*255));