for i in {100..120}
    do
       v_fov=$((20+i*0.5))  step=$((-i*1.8)) ./sphere > output2/b$i.ppm
    done
