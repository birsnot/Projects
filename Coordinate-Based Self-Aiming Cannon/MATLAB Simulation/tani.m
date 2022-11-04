function x = tani(d)
tnd = [0 0.5 1 2 4 8 20 40 100];
atnd = [0 26.57 45 63.43 75.96 82.87 87.14 88.57 89.43];
for q=1:9
        if tnd(q) > d 
            break;
        end
end
    if(d>=100)
        x = 90;
    else
    x = (d-tnd(q-1))/(tnd(q)-tnd(q-1))*(atnd(q)-atnd(q-1))+atnd(q-1);
    return;
    end
end