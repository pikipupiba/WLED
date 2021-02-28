// #pragma once

// #include "FX.h"
// #include "./matrix/Matrix.h"
// #include "./matrix/D2.h"
// #include "./matrix/Particle.h"

// Matrix neo(32, 32, "snake", 0);
// std::vector<Particle *> particles;

// bool started = false;
// bool started4 = false;

// // uint16_t WS2812FX::mode_fountain(void)
// // {
// //     if(!started4)
// //     {
// //         started4 = true;

// //         for(int i = 0; i < 10; i++)
// //         {
// //             heap_caps_check_integrity_all(true);
// //             CHSV color = CHSV(uint8_t(255.0/20.0*i), 255, 255);

// //             // All start in the middle
// //             D2 position = { 1, i*3+0.5 };
// //             D2 velocity = { 2*i+15 , 0 };
// //             D2 acceleration = { 0 , 0 };

// //             std::vector<D2> params;

// //             params.push_back(position);
// //             params.push_back(velocity);
// //             params.push_back(acceleration);

// //             particles.push_back(new Particle(neo, params, color));
// //         }
// //     }

// //     // blackout
// //     for (uint16_t i = 0; i < 1024; i++)
// //     {
// //         setPixelColor(i, BLACK);
// //     }

// //     neo.clear();

// //     for(auto& p : particles)
// //     {
// //         p->run(FRAMETIME);
// //         p->draw();
// //     }

// //     draw_matrix_3(neo);
// // }

// uint16_t WS2812FX::spokes_base(
//           float num_spokes,
//           float radius,
//           float rpm,
//           float bpm)
// {

// }

// uint16_t WS2812FX::mode_fireflies(void)
// {
//     // blackout
//     for (uint16_t i = 0; i < 1024; i++)
//     {
//         setPixelColor(i, BLACK);
//     }
//     neo.fade(abs((SEGMENT.speed-127)/1.3));

//     int bpm = SEGMENT.intensity;

//     int num_spokes = 3 ;//beatsin16(1, 3, 5);
//     float rpm = (SEGMENT.speed-127) * 24;
//     float min = 12;
//     float max = 28;
//     float circle_radius = 15;
//     float radius_factor = 65536 / circle_radius;
//     int color_offset = 255/(num_spokes);
//     int slope_offset = 65536/(num_spokes+2);

//     CHSV color = CHSV((SEGENV.aux1), 255, 255);

//     EVERY_N_SECONDS(15)
//     {
//         SEGENV.step++;

//         if(SEGENV.step > 3) SEGENV.step = 0;
//     }

//     if(SEGENV.step == 0)
//     {
//         neo.spokes1(num_spokes, circle_radius, SEGENV.aux0, rpm, color, color_offset);
//     }
//     if(SEGENV.step == 1)
//     {
//         circle_radius = beatsin8(bpm*2,min/3, max*0.20) + beatsin8(bpm,min/3, max*0.4) + beatsin8(bpm/2,min/3, max*0.4);
//         neo.spokes2(num_spokes, circle_radius, SEGENV.aux0, rpm, color, color_offset);
//     }
//     if(SEGENV.step == 2)
//     {
//         neo.spokes3(num_spokes, circle_radius, SEGENV.aux0, rpm, color, color_offset);
//     }
//     if(SEGENV.step == 3)
//     {
//         num_spokes = 5;
//         circle_radius = beatsin8(bpm*2,min/3, max*0.25) + beatsin8(bpm,min/3, max*0.375) + beatsin8(bpm/2,min/3, max*0.375);
//         neo.spokes2(num_spokes, circle_radius, SEGENV.aux0, rpm, color, color_offset);
//     }
    
//     draw_matrix(neo);

//     SEGENV.aux0+=rpm;
//     SEGENV.aux1++;

//     if(SEGENV.aux1 > 255) SEGENV.aux1 = 0;

//     return FRAMETIME;
// }

// uint16_t WS2812FX::mode_fountain(void)
// {
//     if (!started4)
//     {
//         started4 = true;

//         int num_balls = 24;
//         int mult = 1;

//         for (int i = 0; i < num_balls; i++)
//         {
//             heap_caps_check_integrity_all(true);
//             CHSV color = CHSV(uint8_t(i), 255, 255);

//             // All start in the middle
//             D2 position = {(float(neo.w) - 1.0) / 2.0, (float(neo.h) - 1.0) / 2.0};
//             D2 velocity = {float(cos16((65535.0 / (num_balls*mult-1)) * i)) / 2500, float(sin16((65535.0 / (num_balls*mult-1)) * i)) / 2500};
//             // D2 acceleration = { float(sin16((65535.0/32.0)*i))/5000 , float(cos16((65535.0/32.0)*i))/5000 };
//             D2 acceleration = {0, 0};

//             std::vector<D2> params;

//             params.push_back(position);
//             params.push_back(velocity);
//             params.push_back(acceleration);

//             particles.push_back(new Particle(neo, params, color));
//         }
//     }

//     // blackout
//     for (uint16_t i = 0; i < 1024; i++)
//     {
//         setPixelColor(i, BLACK);
//     }

//     neo.fade(1);

//     for (auto &p : particles)
//     {
//         p->run(FRAMETIME);
//         p->draw();
//     }

//     draw_matrix(neo);

//     return FRAMETIME;
// }

// void WS2812FX::draw_matrix(Matrix &m)
// {
//     //log();

//     if (m.shape == "snake")
//     {
//         for (int i = m.first_led; i < m.first_led + m.length; i++)
//         {
//             // CONTAIN THE MEMORY LEAK!!!
//             if (m.w != 32 || m.h != 32)
//             {
//                 // Serial.print("m.w = ");
//                 // Serial.println(m.w);
//                 // Serial.print("m.h = ");
//                 // Serial.println(m.h);

//                 // Serial.print("i: ");
//                 // Serial.print(i);
//                 // Serial.print(" (");
//                 // Serial.print(m.first_led);
//                 // Serial.print(",");
//                 // Serial.print(m.length);
//                 // Serial.println(")");

//                 m.w = 32;
//                 m.h = 32;
//             }

//             int y = int(i / m.w);
//             int x = int(i % m.w);

//             // if(x > 31 || y >31 || i >1023 || i < 0 || x < 0 || y <0){
//             //             Serial.print("i: ");
//             //             Serial.print(i);
//             //             Serial.print(" (");
//             //             Serial.print(x);
//             //             Serial.print(",");
//             //             Serial.print(y);
//             //             Serial.print(") = ");
//             //             Serial.println(our_matrix.pixels[y][x]);
//             // }

//             if (y % 2 == 0)
//             {
//                 CRGB color_rgb = m.pixels[y][x];
//                 // if(m.pixels[y][x] > 0) //setPixelColor(i, RED);
//                 //     setPixelColor(i, color_from_palette(128, true, true, 0, m.pixels[y][x].v));
//                 setPixelColor(i, crgb_to_col(color_rgb));
//             }
//             else
//             {
//                 CRGB color_rgb = m.pixels[y][31 - x];
//                 // if(m.pixels[y][x] > 0) //setPixelColor(i, RED);
//                 //     setPixelColor(i, color_from_palette(128, true, true, 0, m.pixels[y][x].v));
//                 setPixelColor(i, crgb_to_col(color_rgb));
//             }
//         }
//     }
// }