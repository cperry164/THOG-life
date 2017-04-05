%Création du vecteur de poid de la SVM
clc;
clear all;

input_dir_pos='../database/window/pos';
input_dir_neg='../database/window/neg';
svm_name_out='../database/thog2.svm';

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
fprintf(1,'Reading negatives samples...\n');
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
%options.MaxIter = 10000000;
svmStruct = svmtrain(data,type,'autoscale',true);%,'Options', options);

%A vector of 3780 dimensions that will multiply the hog windows
vector=svmStruct.Alpha'*svmStruct.SupportVectors;

%Bias of the svm
bias = svmStruct.Bias;

biasFactor = (2^22)/bias;

%Write the data to a file
fid = fopen(svm_name_out, 'w');
fwrite(fid,round(biasFactor*bias,0),'int32');
fwrite(fid,round(biasFactor.*vector,0),'int32');

fprintf(1,'Done\n');

pos = pos_samples*vector';
neg = neg_samples*vector';

subplot(1,2,1);
histogram(pos);
subplot(1,2,2);
histogram(neg);