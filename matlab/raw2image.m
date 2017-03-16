function image2raw(filename)

fid = fopen(filename);

R_size=fread(fid,1,'integer*4')
C_size=fread(fid,1,'integer*4')

Gray_img=fread(fid,[C_size, R_size], '*uint8');

imshow(Gray_img.', [0 255])
fclose(fid);

end