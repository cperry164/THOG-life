function image2raw(filename)


%fid = fopen('image_in.rimg');
fid = fopen('image_out.rimg');

R_size=fread(fid,1,'integer*4')
C_size=fread(fid,1,'integer*4')

Gray_img=fread(fid,[C_size, R_size], '*uint8');

%[X,map] = imread(filename);
%[X,map] = imread('Edged.jpg');
%Gray_IMG=rgb2gray(X)
%[R_size,C_size] = size(Gray_IMG)

imshow(Gray_img.', [0 255])
fclose(fid);


end