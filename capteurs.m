function [xpmes] = capteurs(difficulte, xpreel)
%capteurs : Définit la posture mesurée à partir de la posture réelle
%   xp : posture mesurée
xpmes = xpreel;

varBruitCart = 0;
varBruitAngl = 0;
biaiBruitCart = 0;
biaiBruitAngl = 0;

if (difficulte > 0) % niveau 1 : mesures légèrement bruitées, pas de biai
    varBruitCart = 0.2; % 0.1m d'erreur de mesure
    varBruitAngl = 3.6 * pi/180; % 1.8 deg d'erreur d'angle

    if (difficulte > 1) % niveau 2 : mesures bruitées, biai
        varBruitCart = 1; % 0.5m d'erreur de mesure
        varBruitAngl = 18 * pi/180; % 9 deg d'erreur d'angle
        biaiBruitCart = 1; % 1 m de décalage
        biaiBruitAngl = 3.6 * pi/180; % 3.6 deg de décalage

        if (difficulte > 3) % niveau 4 : mesures très bruitées, fort biai
            varBruitCart = 5; % 2.5m d'erreur de mesure
            varBruitAngl = 40 * pi/180; % 20 deg d'erreur d'angle
            biaiBruitCart = 2; % 2 m de décalage
            biaiBruitAngl = 9 * pi/180; % 9 deg de décalage
        end
    end
end

xpmes(1) = xpreel(1) + varBruitCart*(rand()-0.5) + biaiBruitCart;
xpmes(2) = xpreel(2) + varBruitCart*(rand()-0.5) + biaiBruitCart;

xpmes(3) = xpreel(3) + varBruitAngl*(rand()-0.5) + biaiBruitAngl;
end