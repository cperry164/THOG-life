function image2raw(filename)

% Reads the poly.jpg image, converts it into grayscale and writes it in raw format in image_in.rimg  
[X,map] = imread(filename);
%[X,map] = imread('poly.jpg');
Gray_img=rgb2gray(X)
[R_size,C_size] = size(Gray_img)

fid = fopen('image_in.rimg', 'w');
fwrite(fid,R_size,'integer*4');
fwrite(fid,C_size,'integer*4');
fwrite(fid,Gray_img.');

fclose(fid);


end