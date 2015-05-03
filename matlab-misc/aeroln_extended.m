%
% Sample code for aeroelastic analysis and design
% MANE6963 - MDO
% Spring 2015
%
% This code runs the aeroelastic iteration that provides a converged
%  aeroelastic solution for a given wing.
%
% MATLAB code written by J. Martins and modified by P. LeGresley
% Last modified: Juan J. Alonso, 5/25/04
%

function [L, D, W, alpha, lift, twist, uz, maxStress, RHS, LHS, dgdx] = ...
  aeroln_extended(rho, Mach, temp, AR, sweep, span, taper, nPanel, ...
         diam, thick, aeAxis, jigtwist, loadFactor, ...
         range, sfc, graph)

%
% This function calculates one aeroelastic solution (D, W, actual twist)
% for given design variables original/jig twists, 
% flight conditions: (Mach, alpha,
% fixed variables: (sweep, AR, 
% Lift must match weight.
%
% Newton iteration to converge system

%
% Flow-related variables
%
%R = 287;        % metric
R = 1716.562845; % English
gamma = 1.4;
a = sqrt(gamma*R*temp);
Uinf = Mach * a;
q = .5 * rho * Uinf^2;

%
% Wing Planform - derived variables
%
S = span^2 / AR;

%
% Discretization variables
%
% Enforce nElem = nPanel so that no interpolations between
%  aero and structures are necessary.
%
nElem = nPanel;
nDOF = 6*nElem;

% Wing structure
%E = 70E9;       % metric
%matRho = 2.7;   % metric
E = 12E6;        % English
matRho = 0.105;  % English

%
% Spanwise discretization
%
dy = (span/2)/nPanel;
yPanel = [dy/2:dy:span/2-dy/2];
yElem = [dy:dy:span/2];
%jigtwist = yPanel' *(tiptwist/(span/2));

%
% Initialization of variables
%
% twist - actual aeroelastic twist of the wing
% f     - structural load vector (from aero)
% u     - structural displacement vector (from structures)
% Gamma - spanwise circulation distribution (from panel code)
% dfdu  - df/du sensitivities
% L     - wing total lift
%
twist = jigtwist;
f = zeros(nDOF,1);
u = zeros(nDOF,1);
Gamma = zeros(nPanel, 1);
dfdu = zeros(nDOF, nDOF);
L = 0;

%
% Initial angle of attack for entire wing
%
alpha = 0;

%
% Compute structural weight for given spar + fixed weight portion
% The fixed weight portion is added inside weightStruct
%
W = weightStruct(sweep, span*12, diam, thick, matRho, nElem);

%
% Generate matrix of Aerodynamic Influence Coefficient (AIC) for
%  the panel method.
% Compute the total lift L.
%
% lift is really SECLFT. truelift/unit span = rho*Uinf*SECLFT
% corrected to give truelift/unit span...
%
AIC = aeroAIC(AR, nPanel, Mach, taper, sweep);

%
% Get stiffness matrix for the spar, K
% Make sure units are consistent
%
K = stiffness(sweep, span*12, diam, thick, E, nElem);

%
% Calculate all derivatives needed to carry out the Newton
%  iteration that will ultimately yield the aeroelastically
%  converged wing geometry (twist) and structure (thicknesses).
%

%
% Calc dliftdGamma
%
dliftdGamma = zeros(nPanel, nPanel);
for i = 1 : nPanel
 for j = i : nPanel
   dliftdGamma(i, j) = q*span;
 end
end

%
% Calc dfdGamma (messy!)
%
dfdlift = zeros(nDOF, nPanel);
sweep = sweep*pi/180;
chordRoot = 2*S / (span*(1+taper));
aeSweep = atan( ((span/2)*tan(sweep) + chordRoot*taper*(aeAxis-.25)  -...
		 chordRoot*(aeAxis))  ...
		 / (span/2) );
delxRoot = chordRoot * (aeAxis-.25);
for i = 1 : nPanel
  delx(i) = (delxRoot+yElem(i)*tan(aeSweep)) - yElem(i)*tan(sweep);
end

for i = 1 : nPanel-1
  jFz = 6*i-3;
  dfdlift(jFz,i) = dy/2;
  dfdlift(jFz,i+1) = dy/2;
  jTy = 6*i-1;
  dfdlift(jTy,i) = dy/2 * delx(i);
  dfdlift(jTy,i+1) = dy/2 * delx(i);
end
% at i = nPanel
i = nPanel;
j = 6*i-3;
dfdlift(j, i) = dy * .5;
jTy = 6*i-1;
dfdlift(jTy,i) = dy * .5 * delx(i);
dfdlift;

% dfdGamma
dfdGamma = zeros(nDOF, nPanel);
dfdGamma = dfdlift * dliftdGamma;
sweep = sweep*180/pi;

%
% Calc dvdu
%
dvdu = zeros(nPanel, nDOF);
dvdu(1, 6-1) = .5;
for i = 2 : nPanel
  j = 6*i-1;
  dvdu(i, j) = .5;
  j = 6*(i-1)-1;
  dvdu(i, j) = .5;
end

%
%Calc dvdalpha
%
dvdalpha = zeros(nPanel, 1);
for i = 1 : nPanel
  dvdalpha(i, 1) = pi/180;
end

%  
% Calc dLdGamma
%
normWidth =  1./nElem;
dLdGamma = zeros(1, nPanel);
dLdGamma = q*S * normWidth*ones(1, nPanel) * dliftdGamma / (q*span) * AR;

dgdx = [ K                -dfdGamma      zeros(nDOF, 1);
	-dvdu              AIC          -dvdalpha;
	 zeros(1, nDOF)    dLdGamma      zeros(1, 1)];

save('foo.mat', '-mat', 'dgdx')

% Guesses
u = zeros(nDOF, 1);
dvdu(i, j) = .5;
dvdu(i, j) = .5;
Gamma = zeros(nPanel, 1);
alpha = 0;

% This is the Newton iteration: twists 
% Do this loop until twists are same as previous one...

niter = 0;
while 1

  % Update other variables
  lift = seclift(Gamma, nPanel, q, span);
  f = force(lift, aeAxis, nDOF, nPanel, AR, sweep, taper, span);
  L = totalLift(Gamma, nPanel, q, S, AR, sweep, Mach);
  
  % Calc RHS
  RHS = zeros( (nDOF+nPanel+1), 1);
  % 1
  RHS(1:nDOF, 1) = f - K*u;
  % 2
  v = zeros(nPanel, 1);
  twist(1) = jigtwist(1) + .5*u(6*1-1)*180/pi;
  v(1) = (alpha+twist(1)) * pi/180; 
  for i = 2 : nPanel
    twist(i) = jigtwist(i) + .5*180/pi*(u(6*(i-1)-1)+(u(6*(i)-1))); % degrees
    v(i) = (alpha+twist(i)) * pi/180;           % radians
  end 
  RHS(nDOF+1:nDOF+nPanel, 1) = v - AIC*Gamma;  
  % 3
  RHS(nDOF+nPanel+1, 1) = W - L; 
  
  % Calc deltas  
  deltas = dgdx \ RHS;

  du = deltas(1:nDOF, 1);
  dGamma = deltas(nDOF+1:nDOF+nPanel, 1);
  dalpha = deltas(nDOF+nPanel+1, 1);

  % Update independent variables
  u = u + du;
  Gamma = Gamma + dGamma;
  alpha = alpha + dalpha;

  niter = niter+1;
  
  error = norm(deltas, inf); 
  fprintf (1, 'niter = %d   Max delta = %15.5e \n', niter, error);
    
  % Check convergence / divergence
  if error < 10E-10
    break;
  elseif error > 10E20
    fprintf (1, 'Wing is diverging!');
    stop
  end	
  if niter > 1
    fprintf (1, 'Warning: Newton iteration takes more that one step! \n');
  end 
 
end

LHS = vertcat(u, Gamma, alpha);

disp('*************************************')
norm((K * u) - f)
disp('*************************************')

% Calc Drag
D = totalDrag(Gamma, nPanel, q, S, AR, sweep, Mach);

% Calc stresses... multiply disps by load factor to get disp at
% maneuver condition
maxStress = stress(sweep, span*12, diam, thick, E, loadFactor*u, nElem);

% print stuff ...
% if 2 < 1
%   CD = D/(q*S)
%   D
%   alpha
%   Wcruise = W
%   CL = L/(q*S)
%   LoD = L/D
%   tipDisp = u(nDOF-3)
%   tipRot = u(nDOF-1)*180/pi
%   maxStress
 %end

% Extract uz
for i = 1 : nElem
  uz(i) = u(6*i-3);
end

% Plot figures ...
if graph == 1
  figure(1)
  clf;
  for i = 1 : nElem
    uz(i) = u(6*i-5+2);
    thetay(i,1) = u(6*i-1) * 180/pi;
  end
  subplot(3,1,1), plot(yPanel, lift, 'r-+');
  grid on;
  axis([0 span/2 min(lift) 1.1*max(lift)]);
  ylabel('lift');
  subplot(3,1,2), plot(yElem, uz, 'b-+');
  grid on;
  axis([0 span/2  min(uz) max(uz)]);
  ylabel('deflection');
  subplot(3,1,3), plot(yPanel, twist, 'r-+');
  hold on;
  subplot(3,1,3), plot(yElem, thetay, 'b-*');
  subplot(3,1,3), plot(yPanel, jigtwist, 'g-o');
  axis([0 span/2  min(min([twist, thetay, jigtwist])) ...
                  max(max([twist, thetay, jigtwist]))]);
  legend('Total', 'Deflection', 'Jig') 
  grid on;
  ylabel('Twist (degrees)');
  xlabel('y');

  % Draw wing


end
% 
% L
% D
% W
% alpha
% lift
% twist
% uz
% maxStress

