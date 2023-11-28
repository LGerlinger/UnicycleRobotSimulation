function [u] = commande(difficulte, xr, dxr, xp)
%commande : Renvoie la commande u1 et u2 à envoyer aux moteurs
%       xr : position à atteindre
%       dxr : vitesse à avoir pour atteindre le point suivant
%       xp : vecteur de posture donné par les capteurs
%
%       u : commande en vitesse et en rotation

k1 = 1.1;
% k2 = k1; % Pour l'instant, on garde k2 = k1

% --- Paramètres estimés robot ----
l1 = 0.1;
l2 = 0.1;
% --- Fin paramètres robot ----

dxp = k1 * (xr - xp(1:2)) + dxr;

theta = xp(3);
B = [cos(theta), -l1*sin(theta);
     sin(theta),  l1*cos(theta)];
B = B * [1, -l2;
         0 ,  1];

u = inv(B) * dxp;

if (difficulte > 1) % Niveau 2 : ajout de bruit sur la commande
    u(1) = u(1) * (1 + 0.08*(rand()-0.5)); % 4% de bruit sur la commande
    u(2) = u(2) * (1 + 0.08*(rand()-0.5));
    if (difficulte > 2) % Niveau 3 : ajout de fort bruit sur la commande
        u(1) = u(1) * (1 + 0.16*(rand()-0.5)); % 8% de bruit sur la commande
        u(2) = u(2) * (1 + 0.16*(rand()-0.5));
    end
end
end