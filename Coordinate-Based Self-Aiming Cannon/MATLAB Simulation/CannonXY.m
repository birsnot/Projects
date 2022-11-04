% Self Aiming Cannon Simulaton
function CannonXY()
clc
figure(1);
h = gcf;
set(h,'Color',[0.5 0.5 0.65],'Name',...
    'Self Aiming Cannon - Physics Simulaton', ...
    'NumberTitle','off','Position',[10 100 500 500]); 
ha = annotation('textbox','String',sprintf(...
    'Self Aiming Cannon\n Physics Simulaton'), ...
        'Color','w','Position',[0.23 0.98 0.9 0.01]); 
ha.FontSize=20; ha.FontWeight='bold'; ha.EdgeColor='none';
handles = generateUI(h);
xc = 0.38;  yc=0.88;  yc=yc-0.43;  wt=0.25; ht=0.075;
uicontrol(h,'Style','PushButton','String','RUN', ...
    'Units','normalized','Position',[xc yc wt ht], ...
    'FontWeight','bold','FontSize',16, ...
    'Callback',{@runSimulation,handles});
function runSimulation(hObj,event,handles)     
hOut = handles.hOut;
h_x = handles.h_x;     h_y = handles.h_y;
sX = get(h_x,'String');             x = str2double(sX);
sY = get(h_y,'String');             y = str2double(sY);
inr = (x^2 + y^2); % range
rng = 3.9939*inr^3 - 4.7126*inr^2 + 2.5891*inr + 0.0915;% approximated function
theta = -43.54261*rng^3 + 72.347453*rng^2 - 3.9841*rng - 0.2815218; % approximated function
g = 9.81; l = 0.1; V0 = 3; h = l*sind(theta);
d = y/x;
if d<0
    phi = -1*tani(-1*d);
else
    phi = tani(d);
end
if x < 0 && y >= 0
    phi = phi + 180;
elseif x < 0 && y < 0
    phi = phi - 180;
end
tmax = 1;   dt = 0.001;     tp = (0:dt:tmax);       nt=length(tp);
vx = zeros(size(tp));       vx(1) = V0*cosd(theta);
vy = zeros(size(tp));       vy(1) = V0*sind(theta);
sx = zeros(size(tp));       sx(1) = l*cosd(theta);
sy = zeros(size(tp));       sy(1) = h;
tx = zeros(size(tp));       tx(1) = sx(1)*cosd(phi);
ty = zeros(size(tp));       ty(1) = sx(1)*sind(phi);
htop=0; ttop=0; xtop=0; 
for i=2:nt
    im = i-1;
  % dvx = K1*real(vx(im)^n)*dt; drag force is negligible
    dvy = -g*dt;
    vx(i) = vx(im);
    vy(i) = vy(im) + dvy;
    sx(i) = sx(im) + 0.5*(vx(im)+vx(i))*dt; 
    sy(i) = sy(im) + 0.5*(vy(im)+vy(i))*dt;
    tx(i) = sx(i)*cosd(phi);
    ty(i) = sx(i)*sind(phi);
    if sy(i)>htop   %saves the max height values
        htop = sy(i);
        ttop = tp(i);
        xtop = sx(i);
    end
    if sy(i)<=0.0001  %check if it touches the ground
        sy(i) = 0;
        iend = i;
        break
    end
end
vx = vx(1:iend);    vy = vy(1:iend);
sx = sx(1:iend);    sy = sy(1:iend);        tp = tp(1:iend);
tx = tx(1:iend);    ty = ty(1:iend);

if (rng < 0.2) || (abs(x)>0.5) || (abs(y)>0.5)
    figure(2); oorf = gcf; clf(oorf); set (oorf,'Name','Out of Range','NumberTitle','off','Color','w','Position',[725 475 400 150]);
    oor = annotation(oorf,'textbox','String','Out of Range!', ...
        'Color','r','Position',[0.3 0.45 0.9 -2]);
    oor.FontSize=20; oor.FontWeight='bold'; oor.EdgeColor='none';
else
sOut = 'SELF AIMING CANNON - PHYSICS SIMULATION';
sOut = sprintf('%s\n\n      Input Coordinates: x = %5.2f m,  y = %5.2f m',sOut,x,y);
sOut = sprintf('%s\n\nSIMULATION RESULT',sOut);
sOut = sprintf('%s\n     Angle from the ground = %5.2f %c\n     Angle from +X axis = %5.2f %c',...
    sOut,theta,char(176),phi,char(176));
sOut = sprintf('%s\n     Final Hit Coordinates: x = %5.2f m,  y = %5.2f m',sOut,tx(iend),ty(iend));
sOut = sprintf('%s\n     Flight time = %5.3f s',sOut,tp(end));
sOut = sprintf('%s\n     Horizontal range = %6.2f m',sOut,sx(end));
sOut = sprintf('%s\n     Maximum height = %6.3f m',sOut,htop);
sOut = sprintf('%s occurs at %5.3f s when x = %5.2f m.',sOut,ttop,xtop);
set(hOut,'String',sOut);
figure(3); topView = gcf; set(topView,'Name','Top View','NumberTitle','off');
for i =0:10
    clf(topView);
    set (topView,'Color','w','Position',[700 -20 450 400]);
    axis([-0.6 0.6 -0.6 0.6]);
    ax = gca;
    ax.XAxisLocation = 'origin';
    ax.YAxisLocation = 'origin';
    ax.YTick = -0.6:0.1:0.6;
    ax.XTick = -0.6:0.1:0.6;
    ax.FontSize = 6;
    ax.FontWeight = 'bold';
    th = phi*i/10;
    tcx = 0.52; tcy = 0.52; tcdx = 0.075; tcdy = tcdx;
    annotation(topView,'line',[tcx tcx+tcdx*cosd(th)],[tcy tcy+tcdy*sind(th)],...
        'Color',[0.4 0.4 0.4],'LineWidth',7);
    annotation(topView,'ellipse',[tcx-0.02 tcy-0.02*450/400 0.04 0.04*450/400],'FaceColor',[.2 .2 .2]...
        ,'LineStyle','none');
    pause(0.001)
end
figure(2); sideView = gcf; set(sideView,'Name','Side View','NumberTitle','off'); set (sideView,'Color','w','Position',[510 465 850 225]);
axis([0 0.8 0 0.2]);
for i = 0:10
clf(sideView);
th = theta*i/10;
cx = 0.13; cy = 0.18; cdx = 0.1; cdy = cdx*850/225; cd = 0.2;
annotation(sideView,'line',[cx cx+cdx*cosd(th)],[cy cy+cdy*sind(th)],...
    'Color',[0.4 0.4 0.4],'LineWidth',10);
annotation(sideView,'ellipse',[cx-0.025 cy-0.1 0.05 0.05*850/225],'FaceColor',[.2 .2 .2]...
    ,'LineStyle','none');
annotation(sideView,'ellipse',[cx-0.012 cy-0.05 0.023 0.023*850/225],'FaceColor',[.6 .6 .6]...
    ,'LineStyle','none');
axis([0 0.8 0 0.2]);
xlabel('x, m'); ylabel('y, m');
pause(0.001)
end
pause(0.5)
for j=1:10:length(tp)+10
    i = j;
    if i > length(tp)
        i = length(tp);
    end
    figure(2);
    clf(sideView);
    annotation(sideView,'line',[cx cx+cdx*cosd(theta)],[cy cy+cdy*sind(theta)],...
    'Color',[0.4 0.4 0.4],'LineWidth',10);
    annotation(sideView,'ellipse',[cx-0.025 cy-0.1 0.05 0.05*850/225],'FaceColor',[.2 .2 .2]...
    ,'LineStyle','none');
    annotation(sideView,'ellipse',[cx-0.012 cy-0.05 0.023 0.023*850/225],'FaceColor',[.6 .6 .6]...
    ,'LineStyle','none');
%     plot(sx,sy,'b-'); hold on %on/off trajectary
    plot(sx(i),sy(i),'r.','MarkerSize',30);
    axis([0 0.8 0 0.2]);
    xlabel('Range(m)'); ylabel('y(m)');
    title(sprintf('SIDE VIEW: t =%6.3f s,  Range =%6.2f m, Height =%6.2f m',tp(i),sx(i),sy(i)));
    figure(3);
    clf(topView);
    annotation(topView,'line',[tcx tcx+tcdx*cosd(phi)],[tcy tcy+tcdy*sind(phi)],...
        'Color',[0.4 0.4 0.4],'LineWidth',7);
    annotation(topView,'ellipse',[tcx-0.02 tcy-0.02*450/400 0.04 0.04*450/400],'FaceColor',[.2 .2 .2]...
        ,'LineStyle','none');
%     plot(tx,ty,'b-'); hold on; %on/off trajectary
    plot(tx(i),ty(i),'r.','MarkerSize',20);
    axis([-0.6 0.6 -0.6 0.6]);
    ax = gca;
    ax.XAxisLocation = 'origin';
    ax.YAxisLocation = 'origin';
    ax.YTick = -0.6:0.1:0.6;
    ax.XTick = -0.6:0.1:0.6;
    ax.FontSize = 6;
    ax.FontWeight = 'bold';
    xlabel('x(m)'); ylabel('y(m)');
    title(sprintf('TOP VIEW: t =%6.3f s,  x =%6.2f m, y =%6.2f m'...
        ,tp(i),tx(i),ty(i))); 
    pause(0.001)
end
grid
end
function handles = generateUI(h)
xc = 0.25;  yc=0.75;  wt=0.5; ht=0.075;
uicontrol(h,'Style','text','String','Coordinates', ...
    'FontWeight','bold','FontSize',20, ...
    'Units','normalized','Position',[0.25 yc wt ht]);
% V0
xc = 0.25;  yc=yc-0.075;  wt=0.25; ht=0.075;
uicontrol(h,'Style','text','String','x (m)', ...
    'FontWeight','bold','FontSize',20, ...
    'Units','normalized','Position',[xc yc wt+0.002 ht]);
handles.h_x = uicontrol(h,'Style','edit','String','0.5', ...
    'FontWeight','bold','FontSize',9, ...
    'Units','normalized','Position',[xc+wt+0.002 yc-0.001 wt ht+0.003]);
xc = 0.25;  yc=yc-0.075;  wt=0.25; ht=0.075;
uicontrol(h,'Style','text','String','y (m)', ...
    'FontWeight','bold','FontSize',20, ...
    'Units','normalized','Position',[xc yc wt+0.002 ht]);
handles.h_y = uicontrol(h,'Style','edit','String','0.5', ...
    'FontWeight','bold','FontSize',9, ...
    'Units','normalized','Position',[xc+wt+0.002 yc-0.001 wt ht+0.003]);
handles.hOut = annotation('TextBox',[0.1 0.08 0.8 0.35], ...
        'Interpreter','tex', ...
        'Background',[0.95 0.95 0.95], ...
        'String',' ','FitBoxToText','on');
