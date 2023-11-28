function [newxp] = environnement(difficulte,xp, T)
%environnement : modifie xp à cause de glissements et autres
%   
newxp = xp;
if (difficulte > 2) % Niveau 3 : ajout de glissement constant par dérive
    newxp(1) = xp(1) - 0.1 * T; % 0.1 m/s de dérive
    newxp(2) = xp(2) - 0.1 * T; % 0.1 m/s de dérive

    if (difficulte > 3) % Niveau 4 : ajout de glissement constant par dérive
        newxp(1) = xp(1) - 0.4 * T; % 0.1 m/s de dérive
        newxp(2) = xp(2) - 0.2 * T; % 0.1 m/s de dérive
    end
end
end