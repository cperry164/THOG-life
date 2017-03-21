
imgPath='../database/test/test2.png';
rectPath='../database/test/test2.rect';

I = imread(imgPath);
imshow(I);

rect = readRect(rectPath);
for i=1:size(rect,1)
    rectangle('Position',[rect(i,2) rect(i,1) 64 128]);
    %I = insertShape(I,'rectangle',rectangle,'LineWidth',3);
end
