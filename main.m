% niveau : choisir le niveau de réalisme (et donc de difficulté) de
% l'asservissement

clear all;

niveau = 0 % niveau de difficulté (diminue la précision des capteurs, ajoute des frottements, etc...)

duree = 50; % en secondes
T = 0.02; % période d'asservisement
if (niveau > 2)
    T = 0.1;

    if (niveau > 3)
        T = 0.2;
    end
end
N = duree / T; % nombre de points dans la trajectoire.

postureRelle = zeros(3,N+1); % Vecteur de posture réelle
postureRelle(:,1) = [0; 1; 0];

u = zeros(2,N+1); % Commandes
dPhi = zeros(3,N+1); % Vitesse des roues

postureMes = zeros(3,N+1); % Vecteur de posture mesurée
postureMes(:,1) = capteurs(niveau, postureRelle(:,1));

trajectoire = planification('rectangle', N, T, postureMes(1,1), postureMes(2,1));

erreur = zeros(2,N+1); % Erreur de position
erreur(:,1) = trajectoire(1:2,1) - postureRelle(1:2,1);
erreurAbsolue = zeros(1,N+1); % Erreur absolue de position
erreurAbsolue(1) = sqrt(erreur(1,1) * erreur(1,1) + erreur(2,1) * erreur(2,1));
erreurMax = 5.1;

nbIte = 100;
postureRelleTemp = zeros(3,1);

n = 1;
t = 1;
%erreurAbsolue(t) > erreurMax
while((erreurAbsolue(t) >= erreurMax || t <= N) && t <= 2*N)
    [xr, dxr] = consigne(niveau, trajectoire, t, N, T);
    u(:,t) = commande(niveau, xr, dxr, postureMes(:, t));

    postureRelleTemp = postureRelle(:,t);
    for i = 0:nbIte
        [postureRelleTemp, dPhi(:, t)] = robot(niveau, postureRelleTemp, u(:,t), T/nbIte);
    end
    postureRelle(:,t+1) = environnement(niveau, postureRelleTemp, T);

    if (t>N)
        n = N;
    else
        n = t;
    end
    erreur(:,t+1) = trajectoire(1:2,n) - postureRelle(1:2,t+1);
    erreurAbsolue(t+1) = sqrt(erreur(1,t+1) * erreur(1,t+1) + erreur(2,t+1) * erreur(2,t+1));
    
    postureMes(:,t+1) = capteurs(niveau, postureRelle(:,t+1));

    t = t+1;
end


erreur_totale = 0; % Permet de juger du niveau d'asservissement
l = size(erreurAbsolue);
for t = 1:1:l(2)
    erreur_totale = erreur_totale + T*erreurAbsolue(t);
end
erreur_totale

% Sauvegarde de la trajectoire pour un affichage dynamique (enfin je vais essayer de
% le faire)
%savetrajectory(niveau, trajectoire, postureRelle);






%% ---- affichage des résultats ----

close all;

% Trajectoires
figure("Name", 'trajectoires');
plot(trajectoire(1,:), trajectoire(2,:), 'g',...
    'LineWidth', 2);
hold on;
plot(postureRelle(1,:), postureRelle(2,:), 'b');
hold on;
plot(postureMes(1,:), postureMes(2,:), 'r');
title("Trajectoires");
legend('trajectoire', 'position réelle', 'position mesurée');
axis equal;

% Erreur
figure("Name", 'Erreur');
plot(0:t-1, erreurAbsolue);
xlim([0 1.05*t]);
title("Erreur absolue en position en fonction du temps");
legend('Erreur absolue');


% Posture
figure(3);

subplot(5,1,1);
plot(1:t, postureRelle(1, :), 'r', 1:t, postureMes(1, :), 'b');
xlim([0 1.05*t]);
title('xp(t)');
legend('Posture réelle', 'Posture mesurée');

subplot(5,1,2)
plot(1:t, postureRelle(2, :), 'r', 1:t, postureMes(2, :), 'b');
xlim([0 1.05*t]);
title('yp(t)');
legend('Posture réelle', 'Posture mesurée');

subplot(5,1,3);
plot(1:t, postureRelle(3, :), 'r', 1:t, postureMes(3, :), 'b');
xlim([0 1.05*t]);
title('theta(t)');
legend('Posture réelle', 'Posture mesurée');

l = size(dPhi);
subplot(5,1,4);
plot(1:l(2), dPhi(1, :));
xlim([0 1.05*t]);
title('dPhi1(t)');

subplot(5,1,5);
plot(1:l(2), dPhi(2, :));
xlim([0 1.05*t]);
title('dPhi2(t)');

% Commandes
figure("Name", 'Commandes');

subplot(2,1,1);
plot(1:l(2), u(1,:));
xlim([0 1.05*t]);
title('Commandes u1(t)');

subplot(2,1,2);
plot(1:l(2), u(2,:));
xlim([0 1.05*t]);
title('Commandes u2(t)');
