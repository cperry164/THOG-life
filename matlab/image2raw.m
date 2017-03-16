function image2raw(filename_in,filename_out)

% Reads the poly.jpg image, converts it into grayscale and writes it in raw format in image_in.rimg  
[X,map] = imread(filename_in);

Gray_img=rgb2gray(X);
[R_size,C_size] = size(Gray_img);

fid = fopen(filename_out, 'w');
fwrite(fid,R_size,'integer*4');
fwrite(fid,C_size,'integer*4');
fwrite(fid,Gray_img.');

fclose(fid);


end