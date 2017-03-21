function [rect] = readRect(filename)

fid = fopen(filename);
rect=[];

while ~feof(fid)
    rectangle=fread(fid,2,'integer*4');
    if ~isempty(rectangle)
        rect=[rect;(8.*rectangle)'];
    end
end

fclose(fid);

end