function [Gray_img] = int32Raw2image(filename,fp_precision)

fid = fopen(filename);

R_size=fread(fid,1,'integer*4')
C_size=fread(fid,1,'integer*4')

Gray_img=fread(fid,[C_size, R_size], 'int32=>float');

Gray_img = Gray_img./(2^fp_precision);

imshow(Gray_img.', [0 max(max(Gray_img))]);
fclose(fid);

end