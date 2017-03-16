function [window] = readHogWindow(filename)

fid = fopen(filename);

size=fread(fid,1,'integer*4');

for i=1:size
    window(i,:)=fread(fid,3780, '*float');
end

fclose(fid);

end