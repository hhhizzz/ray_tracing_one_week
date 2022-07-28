for i in {1..100}
    do
        aperture=$((i * 0.1 + 0.1))
        v_fov=$((90 - i * 0.8))
        ./sphere > a$i.ppm
    done
