function [xr,dxr] = consigne(difficulte, traj, date, N, T)
%consigne : Utilise la trajectoire et la date pour en déduire une consigne
%       xr, dxr = [x_point_voulu, x_vitesse_voule]
%                 [y_point_voulu, y_vitesse_voule]

if (difficulte > 3) % Niveau 4 : la trajectoire est  envoyée en directe avec du délai et du bruit
    date = max(1, int32(date-2/T)); % 2s de délai
    if (date >= N)
        date = N;
    end
    if (rand() < 0.1)
        traj(1, date) = 10000 * (rand()-0.5);
        traj(2, date) = 10000 * (rand()-0.5);
        traj(3, date) = 10000 * (rand()-0.5);
    end
end

if (date >= N) % L'instant du dernier point de la trajectoire est atteint (ou dépassé)
    xr = [traj(1, N); traj(2, N)]; % La consigne à tout moment après la fin est d'etre sur le dernier point de la trajectoire
    dxr = [0; 0]; % On s'arrete sur le dernier point a priori
else
    xr = traj(1:2, date);
    dxr = traj(1:2, date+1);
    delta_t = (traj(3, date+1) - traj(3, date));
    if (delta_t == 0)
        delta_t
        date
        dxr = [0; 0];
    else
        dxr = (dxr - xr)/delta_t;
    end
end  
end