//
//  main.h
//  searchGalexData
//
//  Created by Elliot Eckholm on 12/12/16.
//  Copyright © 2016 Elliot Eckholm. All rights reserved.
//

#include <string>

using namespace std;

typedef struct {
    int hdNumber;
    float RA;
    float DEC;
    float band;
    float far_UV_Mag;
    float far_UV_Mag_Error;
    float apparent_V_Mag;
    float b_v;
    float distance;
    string binary_Flag;
    float x_ray_lum;
    float correction;
    
    
} point;

typedef struct {
    point *dataPoints;
    int num_dataPoints;
    
}dataPointList;
