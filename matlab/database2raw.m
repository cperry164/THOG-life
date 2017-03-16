%Convertion de l'ensemble de la database en format raw

input_dir_pos='../database/MIT/pedestrians128x64';
output_dir_pos='../database/raw/pos';

input_dir_neg='../database/INRIA/neg';
output_dir_neg='../database/raw/neg';

%Conversion des échantillons positifs
files=dir(input_dir_pos);

for i=1:length(files)
    if files(i).isdir==0
        splitted_name=strsplit(files(i).name,'.');
        image2raw(strcat(input_dir_pos,'/',files(i).name),...
                  strcat(output_dir_pos,'/',splitted_name{1},'.raw'));
        fprintf(1,'Converted image %d/%d\n',i,length(files));
    end
end

%Conversion des échantillons négatifs
files=dir(input_dir_neg);

for i=1:length(files)
    if files(i).isdir==0
        splitted_name=strsplit(files(i).name,'.');
        image2raw(strcat(input_dir_neg,'/',files(i).name),...
                  strcat(output_dir_neg,'/',splitted_name{1},'.raw'));
        fprintf(1,'Converted image %d/%d\n',i,length(files));
    end
end