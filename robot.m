function [newxp, dPhi] = robot(difficulte, xp, u, T)
%robot : utilise la posture réelle xp et la commande u pour faire bouger
%les roues et ainsi, mettre à jour la posture réelle
%
%   xp : vecteur de posture à l'instant t (x,y,theta)
%   u : commande (u1, u2)
%   T : durée d'application de la commande
%
%   newxp : vecteur de posture à l'instant t+T (x,y,theta)
%
%   C'est ici que seront une bonne partie des approximations ajoutées pour
%   le réalisme

% --- Paramètres robot ----
r1 = 0.1; % rayon de la roue
r2 = 0.1; % rayon de la roue
L = 0.2; % rayon du robot

l1 = 0.1; % position du point P dans le repère (P0, x, y)
l2 = 0.1;

%coefFriction = 1; % à quel point 1 tour de roue implique d'avancer d'un tour de roue
% --- Fin paramètres robot ----

% ---- Calculs ----
if (difficulte > 2) % Niveau 3 : roues de tailles différentes
    r1 = 0.107; % 10 % d'incertitude
    r2 = 0.096;
end
dPhi1 = u(1)/r1 + L*u(2)/r1;
dPhi2 = u(1)/r2 - L*u(2)/r2;

if (dPhi1==0 || dPhi2 == 0 || abs(dPhi1) < abs(2*pi*3))
    dPhi1;
end
if (difficulte > 0) % niveau 1 : saturation de la vitesse de rotation des roues
    dPhiMax = 2*pi*3; % 180rpm soit vMax = 1.8 m/s
    if (abs(dPhi1) > abs(dPhiMax))
        dPhi1 = sign(dPhi1) * dPhiMax;
    end
    if (abs(dPhi2) > abs(dPhiMax))
        dPhi2 = sign(dPhi2) * dPhiMax;
    end

    if (difficulte > 1) % niveau 2 : quantification de la vitesse de rotation des roues
        dPhi1 = (dPhi1 + dPhiMax) / (2*dPhiMax); % dPhi1 passe en 0 et 1 (inclus)
        dPhi1 = dPhi1 * 512; % dPhi1 passe en 0 et 512
        dPhi1 = double(int16(dPhi1)); % dPhi1 est arrondi à l'entier le plus proche
        dPhi1 = dPhi1 / 512; % dPhi1 passe entre 0 et 1
        dPhi1 = dPhi1 * 2*dPhiMax - dPhiMax; % dPhi1 passe entre -dPhiMax et dPhiMax

        dPhi2 = (dPhi2 + dPhiMax) / (2*dPhiMax);
        dPhi2 = dPhi2 * 512;
        dPhi2 = double(int16(dPhi2));
        dPhi2 = dPhi2 / 512;
        dPhi2 = dPhi2 * 2*dPhiMax - dPhiMax;
    end
end

ureel = [(r1*dPhi1 + r2*dPhi2) / 2;
         (r1*dPhi1 - r2*dPhi2) / (2*L)];

theta = xp(3);
B = [cos(theta), -l1*sin(theta);
     sin(theta),  l1*cos(theta)];
B = B * [1, -l2;
         0 ,  1];

newxp = xp;
newxp(1:2) = newxp(1:2) + T * B * ureel;

newxp(3) = newxp(3) + T * ureel(2);
newxp(3) = mod(newxp(3), 2*pi);
dPhi = [dPhi1; dPhi2; 0];
end