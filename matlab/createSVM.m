%Création du vecteur de poid de la SVM
clc;
clear all;

input_dir_pos='../database/window/pos';
input_dir_neg='../database/window/neg';
svm_name_out='../database/thog.svm';

pos_samples=[];
neg_samples=[];

%Read positives samples
fprintf(1,'Reading positives samples...\n');
files=dir(input_dir_pos);

filecount=0;
for i=1:length(files)
    if files(i).isdir==0
        filecount=filecount+1;
        sample = readHogWindow(strcat(input_dir_pos,'/',files(i).name));
        pos_samples = [pos_samples;sample];
    end
end

fprintf(1,'%d positives samples on %d files found\n',size(pos_samples,1),filecount);

%Read negatives samples
fprintf(1,'Reading positives samples...\n');
files=dir(input_dir_neg);

filecount=0;
for i=1:length(files)
    if files(i).isdir==0
        filecount=filecount+1;
        sample = readHogWindow(strcat(input_dir_neg,'/',files(i).name));
        neg_samples = [neg_samples;sample];
    end
end

fprintf(1,'%d negatives samples on %d files found\n',size(neg_samples,1),filecount);

%build the svm
fprintf(1,'Building the SVM...\n');

data=[pos_samples;
      neg_samples];
type=[true(size(pos_samples,1),1);
      false(size(neg_samples,1),1)];
  
%SVM train algorithm of Matlab. Disable scaling, otherwise we would have to
%do scaling in the C code
svmStruct = svmtrain(data,type,'autoscale',false);

%A vector of 3780 dimensions that will multiply the hog windows
vector=svmStruct.Alpha'*svmStruct.SupportVectors;

%Bias of the svm
bias = svmStruct.Bias;

%Write the data to a file
fid = fopen(svm_name_out, 'w');
fwrite(fid,bias,'*float');
fwrite(fid,vector,'*float');

fprintf(1,'Done\n');