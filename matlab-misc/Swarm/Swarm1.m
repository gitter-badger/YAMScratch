function [x_star] = Swarm1(obj, xlb, xub)
%Purpose:
    assert(isvector(xlb));
    assert(isvector(xub));
    SIZE_X = length(xub);
    assert(SIZE_X == length(xlb));

    npart = 30;     % The number of particles.
    niter = 30;    % The number of iterations.
    cbi = 2.5;      % Initial value of the individual-best acceleration factor.
    cbf = 0.5;      % Final value of the individual-best acceleration factor.
    cgi = 0.5;      % Initial value of the global-best acceleration factor.
    cgf = 2.5;      % Final value of the global-best acceleration factor.
    wi = 0.9;       % Initial value of the inertia factor.
    wf = 0.4;       % Final value of the inertia factor.
    vspaninit = 1;  % The initial velocity span. Initial 
    vmax = Inf;     % Absolute speed limit. It is the primary

    Y = zeros(npart,1);
    %Initialize X with random variables
    X_range = xub - xlb;
    X = rand(SIZE_X,npart);
    X = bsxfun(@times, X_range, X);
    X = bsxfun(@plus, xlb, X);

    %check for best conditions
    tic
    Y(1) = obj(X(:,1));
    tend = toc
    reg_time = tend;
    try
        gx0 = gpuArray(X);
        tic
        Y(1) = obj(gx0(:,1));
        gpu_time = toc
    catch ME
        disp('GPU disabled')
        gpu_time = Inf; 
    end

    %==============================================
    %           INITIALIZATION ROUTINE
    %==============================================
    %test if parfor is any good at for first run
    try
        tic;
        %we have already found the first part
        parfor index = 2:npart
            Y(index) = obj(X(:,index));
        end
        tend = toc;
        par_time = tend/npart
    catch ME
        par_time = Inf;
        %compute the native method
        for index = 2:npart
            Y(index) = obj(X(:,index));
        end 
    end

    Ybest = Y; % The best individual score for each particle
    Xbest = X; % The best individual position for each particle
    [GYbest, gbest] = min(Ybest);% GYbest is the best score within the entire swarm.
                                 % gbest is the index of particle that achived YGbest.
    gbest = gbest(1);% In case when more than one particle achieved the best
                     % score, we choose the one with the lowest index as the
                     % best one.
    V = (rand(SIZE_X, npart)-0.5)*2.*repmat(npart, vspaninit.', 1);

    if par_time < gpu_time
        if par_time < reg_time
            STATE = 3;
        else
            STATE = 1;
        end
    elseif gpu_time < reg_time
        %STATE = 2;
        %GPU DOES NOT WORK
        STATE = 1;
    else
        STATE = 1;
    end

    if STATE == 3
        disp('Parallel Mode')

        for iter = 1:niter
            disp(iter)
            w = wi + ((wf-wi)/(niter))*(niter-iter);
            cp = cbi + ((cbf-cbi)/(niter))*(niter-iter);
            cg = cgi + ((cgf-cgi)/(niter))*(niter-iter);

            % For later calculations only
            GXbest = repmat(Xbest(:, gbest), 1, npart);

            % Calculating speeds
            V = w*V + cp*rand(size(V)).*(Xbest-X) + cg*rand(size(V)).*(GXbest-X);
            V = min(vmax, abs(V)).*sign(V);

            % Population is moving
            X = X + V;
            tic
            parfor eval_index = 1:npart
                Y(eval_index) = obj(X(:,eval_index));
            end
            toc

            % Calculating new individually best values
            mask = Y<Ybest;
            mask = mask.';
            mask = repmat(mask, SIZE_X, 1);
            Xbest = mask.*X +(~mask).*Xbest;
            Ybest = min(Y,Ybest);
            
            % Calculating new globally best value
            [GYbest, gbest] = min(Ybest);
            disp(GYbest)
            gbest = gbest(1);
        end
    elseif STATE == 2
        disp('GPU Mode')
        X = gpuArray(X);
        Y = gpuArray(Y);
        V = gpuArray(V);
        Xbest = gpuArray(Xbest);
        Ybest = gpuArray(Ybest);
        mask = gpuArray(zeros(size(Y)));

        for iter = 1:niter
            disp(iter)
            w = wi + ((wf-wi)/(niter))*(niter-iter);
            cp = cbi + ((cbf-cbi)/(niter))*(niter-iter);
            cg = cgi + ((cgf-cgi)/(niter))*(niter-iter);

            % For later calculations only
            GXbest = repmat(Xbest(:, gbest), 1, npart);

            % Calculating speeds
            V = w*V + cp*rand(size(V)).*(Xbest-X) + cg*rand(size(V)).*(GXbest-X);
            V = min(vmax, abs(V)).*sign(V);

            % Population is moving
            X = X + V;
            for eval_index = 1:npart
                Y(eval_index) = obj(X(:,eval_index));
            end
            % Calculating new individually best values
            mask = Y<Ybest;
            mask = mask.';
            mask = repmat(mask, SIZE_X, 1);
            Xbest = mask.*X +(~mask).*Xbest;
            Ybest = min(Y,Ybest);
            
            % Calculating new globally best value
            [GYbest, gbest] = min(Ybest);
            gbest = gbest(1);
        end
    elseif STATE == 1
        disp('Vanilla Mode')
        for iter = 1:niter
            disp(iter)
            w = wi + ((wf-wi)/(niter))*(niter-iter);
            cp = cbi + ((cbf-cbi)/(niter))*(niter-iter);
            cg = cgi + ((cgf-cgi)/(niter))*(niter-iter);

            % For later calculations only
            GXbest = repmat(Xbest(:, gbest), 1, npart);

            % Calculating speeds
            V = w*V + cp*rand(size(V)).*(Xbest-X) + cg*rand(size(V)).*(GXbest-X);
            V = min(vmax, abs(V)).*sign(V);

            % Population is moving
            X = X + V;
            for eval_index = 1:npart
                Y(eval_index) = obj(X(:,eval_index));
            end
            % Calculating new individually best values
            mask = Y<Ybest;
            mask = mask.';
            mask = repmat(mask, SIZE_X, 1);
            Xbest = mask.*X +(~mask).*Xbest;
            Ybest = min(Y,Ybest);
            
            % Calculating new globally best value
            [GYbest, gbest] = min(Ybest);
            gbest = gbest(1);
        end
    end

    x_star = Xbest(:,gbest);
     return
end
