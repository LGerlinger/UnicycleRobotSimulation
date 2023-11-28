function [trajectoire] = planification(type, N, T, x0, y0)
%planification : Génère la trajectoire à suivre en N points
%   type : string indiquant le type de trajectoire à indiquer
%
%   trajectoire est un array de taille Nx3 [x(0)...x(N)]
%                                          [y(0)...y(N)]
%                                          [t(0)...t(N)]
trajectoire = zeros(3,N);

switch(type)
    case 'rectangle'
        x = x0;
        y = y0+10;
        
        for i = 1:1:N
            trajectoire(3, i) = T*(i-1);
        end
        
        % La trajectoire sera un rectangle
        
        for i = 1:1:int16(N/4)
            trajectoire(1, i) = x;
            trajectoire(2, i) = y;
            x = x + 1*T;
        end

        for i = int16(N/4)+1:1:int16(N/2)
            trajectoire(1, i) = x;
            trajectoire(2, i) = y;
            y = y + 2*T;
        end
        
        for i = int16(N/2)+1:1:int16(3*N/4)
            trajectoire(1, i) = x;
            trajectoire(2, i) = y;
            x = x - 1*T;
        end
        
        for i = int16(3*N/4)+1:1:N
            trajectoire(1, i) = x;
            trajectoire(2, i) = y;
            y = y - 2*T;
        end

    case 'sinus'
        for i = 1:1:N
            trajectoire(1, i) = T*i;
            trajectoire(2, i) = 2 * sin(T*i * 0.5);
            trajectoire(3, i) = T*(i-1);
        end
    case 'fleur'
        for i = 1:1:N
            theta = 2*pi*i/N;
            ray = 20*(2+sin(3*theta));

            x = ray * cos(theta);
            y = ray * sin(theta);
            trajectoire(1, i) = x;
            trajectoire(2, i) = y;
            trajectoire(3, i) = T*(i-1);
        end
end
end