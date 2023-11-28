function [] = savetrajectory(difficulte, trajectoire, parcours)
%savetrajectory : sauvegarde la trajectoire et le parcours
%   Sauvegarde dans des fichiers binaires


fileName = ['binSave/traj_x_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,trajectoire(1,:),'double');
fclose(fileID);

fileName = ['binSave/traj_y_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,trajectoire(2,:),'double');
fclose(fileID);

fileName = ['binSave/traj_theta_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,trajectoire(3,:),'double');
fclose(fileID);



fileName = ['binSave/parc_x_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,parcours(1,:),'double');
fclose(fileID);

fileName = ['binSave/parc_y_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,parcours(2,:),'double');
fclose(fileID);

fileName = ['binSave/parc_theta_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,parcours(3,:),'double');
fclose(fileID);


% Saving parameters (t)
l = size(parcours);
fileName = ['binSave/param_N' num2str(difficulte) '.bin'];
fileID = fopen(fileName,'w');
fwrite(fileID,l(2),'int32');
fclose(fileID);

end