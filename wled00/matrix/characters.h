#pragma once
#include "FX.h"

struct image
{
    int width;
    int height;
    int pixel_array[35];
    bool is_color;
};

image images[59] =
    {    //chars
        {// SPACE
         1,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// !
         5,
         7,
         {0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 1, 0, 0}},
        {// "
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// #
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// $
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// %
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// &
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// '
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// (
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// )
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// *
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// +
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// ,
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 1, 0, 0, 0,
          1, 0, 0, 0, 0}},
        {// -
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// .
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// /
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {//0
         5,
         7,
         {0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 1, 1, 0,
          1, 1, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 } },
        {//1
         5,
         7,
         {0, 1, 0,  0, 0,
          1, 1, 0,  0, 0,
          0, 1, 0,  0, 0,
          0, 1, 0,  0, 0,
          0, 1, 0,  0, 0,
          0, 1, 0,  0, 0,
          1, 1, 1  ,0 ,0 }},
        {//2
         5,
         7,
         {//2
          0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 1, 0, 0,
          0, 1, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 1 ,0 }},
        {//3
         5,
         7,
         {//3
          0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          0, 1, 1, 0, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {//4
         5,
         7,
         {//4
          1, 0, 1, 0, 0,
          1, 0, 1, 0, 0,
          1, 0, 1, 0, 0,
          1, 1, 1, 1, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0 ,0 }},
        {//5
         5,
         7,
         {//5
          1, 1, 1, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 0, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {//6
         5,
         7,
         {//6
          0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {//7
         5,
         7,
         {//7
          1, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 1, 0, 0,
          0, 1, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0 ,0 }},
        {//8
         5,
         7,
         {//8
          0, 1, 1, 0, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          0, 1, 1, 0, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          0, 1, 1, 0 ,0  }},
        {//9
         5,
         7,
         {//9
          0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }
        
        },
        {//:
         5,
         7,
         {//:
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}
        
        },
        {// ;
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 1, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 1, 0, 0, 0,
          1, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// <
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// =
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// >
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// ?
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0}},
        {// @
         5,
         7,
         {0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0, 0,
          0, 0, 0, 0 ,0 }},
        {// A
         5,
         7,
         {0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0}},
        {// B
         5,
         7,
         {1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 0, 0}},
        {// C
         5,
         7,
         {0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0, 0}},
        {// D
         5,
         7,
         {1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 0, 0}},
        {// E
         5,
         7,
         {1, 1, 1, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 1, 0}},
        {// F
         5,
         7,
         {1, 1, 1, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0}},
        {// G
         5,
         7,
         {1, 1, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1, 0}},
        {// H
         5,
         7,
         {1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0}},
        {// I
         5,
         7,
         {1, 1, 1, 0, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 0, 0, 0, 
          0, 1, 0, 0, 0, 
          1, 1, 1 ,0 ,0 }},
        {// J
         5,
         7,
         {1, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {// K
         5,
         7,
         {1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 1, 0, 0, 
          1, 1, 0, 0, 0, 
          1, 0, 1, 0, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1 ,0  }},
        {// L
         5,
         7,
         {1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1 ,0 }},
        {// M
         5,
         7,
         {1, 0, 0, 0, 1,
          1, 1, 0, 1, 1,
          1, 0, 1, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1}},
        {// N
         5,
         7,
         {1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 0, 1, 0,
          1, 0, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1 ,0 }},
        {// O
         5,
         7,
         {0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {// P
         5,
         7,
         {1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 0, 0,
          1, 0, 0, 0 ,0 }},
        {// Q
         5,
         7,
         {0, 1, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 1, 1, 0,
          0, 1, 0, 1, 1}},
        {// R
         5,
         7,
         {1, 1, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1, 0,
          1, 0, 1, 0, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1 ,0 }},
        {// S
         5,
         7,
         {1, 1, 1, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 0, 0,
          1, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 0 ,0 }},
        {// T
         5,
         7,
         {1, 1, 1, 1, 1,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0,
          0, 0, 1, 0, 0}},
        {// U
         5,
         7,
         {1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          0, 1, 1, 0 ,0 }},
        {// V
         5,
         7,
         {1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          0, 1, 0, 1, 0,
          0, 0, 1, 0, 0}},
        {// W
         5,
         7,
         {1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 0, 0, 1,
          1, 0, 1, 0, 1,
          1, 0, 1, 0, 1,
          1, 0, 1, 0, 1,
          0, 1, 0, 1, 0}},
        {// X
         5,
         7,
         {1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          0, 1, 1, 0, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1, 0, 
          1, 0, 0, 1 ,0  }},
        {// Y
         5,
         7,
         {1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1, 0,
          0, 0, 0, 1, 0,
          1, 0, 0, 1, 0,
          1, 1, 1, 1 ,0 }},
        {// Z
         5,
         7,
         {1, 1, 1, 1, 0, 
          0, 0, 0, 1, 0, 
          0, 0, 1, 0, 0, 
          0, 1, 1, 0, 0, 
          0, 1, 0, 0, 0, 
          1, 0, 0, 0, 0, 
          1, 1, 1, 0 ,0  }}
    };