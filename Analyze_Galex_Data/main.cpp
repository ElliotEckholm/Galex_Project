#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <boost/tokenizer.hpp>
#include <boost/range/iterator_range.hpp>
#include <algorithm>
//#include <malloc.h>

//Local includes
#include "main.h"

//Function Prototypes
//First Step
void load_HDNumbers(char *fn);
void load_RA_DEC_Numbers(char *fn);
float convertRA(float h, float m, float s);
float convertDEC(float d, float m, float s);
void writeOutputFile(char *fn);
//Second Step
void load_GalexData(char *fn);
void load_RosatData(char *fn);
static bool sortByHDNumber(const point &a, const point &b);
float calculateBolometricCorrection(char *fn, float findB_V);


//Globals
dataPointList list;
long num_dataPoints = 0;
int hdNum_toFind;

//using
using namespace std;


//Vectors
vector<point> dataPointVector;
vector<point> secondStepVector;

int main(int argc, char * argv[]){
    
    
//---------First Step: Gather 980 data points with HD number, RA, and Dec
   /*
     load_HDNumbers("HDNumbers.txt");
     load_RA_DEC_Numbers("HD_RA_DEC.txt");
     writeOutputFile("galexInputFile1.txt");
    
    */
    //Upload outputFile.txt into Galex View

    
    
//---------Second Step: Gather 339 data points with HD number, RA, DEC, Far UV Magnitude, Apparent V Magnitude, B-V, Distance, and X-Ray Luminousity

  
    //Read in galex data and Rosat data to compile data points with corresponding attributes^^
    load_GalexData("CleanGalexAverage.txt");
    load_RosatData("S_Stars.txt");
    
    

   
    //sort the list of data points by HD number smallest to greatest
    sort(secondStepVector.begin(),secondStepVector.end(), sortByHDNumber);

    
    //write new output file
    writeOutputFile("finalOutFileTest.txt");
  
    return 0;
    
}
float calculateBolometricCorrection(char *fn, float findB_V){
    float b_v;
    float correction;
    float returnValue = 0;
    
    
    ifstream bolometriCorrectionFile(fn);
    
    while (bolometriCorrectionFile >> b_v >> correction){
        
        if (findB_V == b_v){
            returnValue = correction;
        }
       
    
    }
    
    return returnValue;


}

static bool sortByHDNumber(const point &a, const point &b){
    return a.hdNumber < b.hdNumber;
}

void load_RosatData(char *fn){
    float vmag, b_v, dist, xray;
    int hdNum;
    int num = 0;
    string binary_Flag;

    ifstream rosatFile(fn);
    
    if (!rosatFile){
        printf("Cannot find Rosat File");
    }
    
    
    
    while (rosatFile >> hdNum >> vmag >> b_v >> dist >> binary_Flag >>xray){
        
        
        for (int i = 0; i < secondStepVector.size(); i++){

            
            if (secondStepVector[i].hdNumber == hdNum ){
               
             
                secondStepVector[i].apparent_V_Mag = vmag;
                secondStepVector[i].b_v = b_v;
                secondStepVector[i].distance = dist;
                secondStepVector[i].x_ray_lum = xray;
                
                //calculate bolometric correction
                secondStepVector[i].correction = calculateBolometricCorrection("BolometricCorrection.txt", secondStepVector[i].b_v);
                        
                        num++;
                
               
            }
           
           
        }
 
    }
    
    
    printf("\nSize of array %d\n", secondStepVector.size());
    printf("\nLoading of Rosat Data complete: %d\n", num);


}

void load_GalexData(char *fn){
    float RA, DEC, fuv_Mag, fuv_Mag_Error;
    int hdNum, band;
    int num = 0;
    
    
    ifstream galexFile(fn);
    
    if (!galexFile){
        printf("Cannot find Galex File");
    }
    
    
    
    while (galexFile >> hdNum >> RA >> DEC >> band >> fuv_Mag >> fuv_Mag_Error){
        
        
        
        
                point dataPoint;
        
                
                dataPoint.hdNumber = hdNum;
                dataPoint.RA = RA;
                dataPoint.DEC = DEC;
                dataPoint.band = band;
                dataPoint.far_UV_Mag = fuv_Mag;
                dataPoint.far_UV_Mag_Error = fuv_Mag_Error;
        
                secondStepVector.push_back(dataPoint);
                num++;
                
                
            
            
            
        
        
    }
    
    
    printf("\nSize of array %d\n", secondStepVector.size());
    printf("\nLoading of Galex Data complete: %d\n", num);
    
    
}



void load_GalexData2(char *fn){
    
    int lineNum = 0;
    int num = 0;

    ifstream galexFile(fn);
  
    if (galexFile.is_open())
    {
        std::string line;
        while ((std::getline(galexFile, line)))
        {
            typedef boost::escaped_list_separator<char> Separator;
            typedef boost::tokenizer<Separator> Tokenizer;
            
            std::vector<std::string> tokens;
            Tokenizer tokenizer(line);
            for (Tokenizer::iterator iter = tokenizer.begin(); iter != tokenizer.end(); ++iter)
            {
                tokens.push_back(*iter);
            }
            
            if (lineNum != 0){
                int hdNum = atoi(tokens[0].c_str());
                float RA = atof(tokens[1].c_str());
                float DEC = atof(tokens[2].c_str());
                float band = atof(tokens[12].c_str());
                
                float fuv_mag = atof(tokens[15].c_str());
                float fuv_mag_error = atof(tokens[16].c_str());
                
                if (fuv_mag != -999 && fuv_mag != 0 && band != 1) {
                    point dataPoint;
                    dataPoint.hdNumber = hdNum;
                    dataPoint.RA = RA;
                    dataPoint.DEC = DEC;
                    dataPoint.band = band;
                    dataPoint.far_UV_Mag = fuv_mag;
                    dataPoint.far_UV_Mag_Error = fuv_mag_error;
                    secondStepVector.push_back(dataPoint);
                    
                    num++;
                    
                
                }
                
            }
            

            lineNum++;
        }
    }
     printf("\nLoading of Galex Data complete: number of points Galex Points: %d\n", num);
}

void load_HDNumbers(char *fn){
    
    
    ifstream hdFile(fn);
    
    
    while (hdFile >> hdNum_toFind){
        
        point dataPoint;
        dataPoint.hdNumber = hdNum_toFind;
        dataPointVector.push_back(dataPoint);
    }
    
}

void load_RA_DEC_Numbers(char *fn){
    float h, m, s;
    float d, md, sd;
    int hdNum;
    int j = 1;
    
    
    ifstream hdFile(fn);
    
    while (hdFile >> h >> m >> s >> d >> md >> sd >> hdNum){
        
       
        for (int i = 0; i < dataPointVector.size(); i++){
            if (dataPointVector[i].hdNumber == j) {
                dataPointVector[i].hdNumber = j;
                dataPointVector[i].RA = convertRA(h, m, s);
                dataPointVector[i].DEC = convertDEC(d, md, sd);
            }
            
        }
        
        j++;
        
    }
    
    printf("\nLoading RA DEC and HD complete\n");
    
    
}

void writeOutputFile(char *fn){
    int num = 0;
    
    ofstream outputFile(fn);
    /*
    outputFile << "HD" << ", " << "RA" << ", "<< "DEC" << ", " <<
    "Band" << ", " <<
    "Far UV Calibrated Magnitude" << ", " <<
    "Far UV Error" << endl;

    
    for (int i = 0; i < secondStepVector.size(); i++){
       
        
            outputFile << secondStepVector[i].hdNumber << ", " <<
            secondStepVector[i].RA << ", " <<
            secondStepVector[i].DEC << ", " <<
            secondStepVector[i].band << ", " <<
            secondStepVector[i].far_UV_Mag << ", " <<
            secondStepVector[i].far_UV_Mag_Error << endl;
            
            num++;
            
            // printf("\nHD: %d BV: %f", secondStepVector[i].hdNumber, secondStepVector[i].b_v);
            //  }
            
        
    }
*/
    
  
    outputFile << "HD" << ", " << "RA" << ", "<< "DEC" << ", " <<
                "Band" << ", " <<
                "Far UV Calibrated Magnitude" << ", " <<
                "Far UV Error" << ", " <<
                "Apparent V Magnitude" << ", " <<
                "B-V Color Index" << ", " <<
                "Distance" << ", " <<
                "X-Ray Luminosity" << ", " <<
                "Bolometric Correction" << endl;
    
    
    for (int i = 0; i < secondStepVector.size(); i++){
        if (secondStepVector[i].distance != 0){
          //  if (secondStepVector[i].b_v > 0.4){
        
            outputFile << secondStepVector[i].hdNumber << ", " <<
            secondStepVector[i].RA << ", " <<
            secondStepVector[i].DEC << ", " <<
            secondStepVector[i].band << ", " <<
            secondStepVector[i].far_UV_Mag << ", " <<
            secondStepVector[i].far_UV_Mag_Error << ", " <<
            secondStepVector[i].apparent_V_Mag << ", " <<
            secondStepVector[i].b_v << ", " <<
            secondStepVector[i].distance << ", " <<
            secondStepVector[i].x_ray_lum << ", " <<
            secondStepVector[i].correction <<endl;
            
            num++;
                
           // printf("\nHD: %d BV: %f", secondStepVector[i].hdNumber, secondStepVector[i].b_v);
          //  }
        
        }
    }
   
     /*
    
    for (int i = 0; i < dataPointVector.size(); i++){
        
          
                
                outputFile << dataPointVector[i].hdNumber << ", " <<
                dataPointVector[i].RA << ", " <<
                dataPointVector[i].DEC << endl;
                
                num++;
            
            
        }
       */

        
       
        
        
        
        
        //Find which datapoint to split the outpfile into two outputfiles to upload to galex view
    
        //printf("\nHD 500: %d", dataPointVector[500].hdNumber);
    
    
    
    //printf("\nWrite complete. Number of data points: %d\n", num);
    
    outputFile.close();
    printf("\nTotal Data Points: %d\n", num);
    
    printf("\n\nWrite complete\n");
}

float convertRA(float h, float m, float s){
    float degrees = 0;
    
    degrees = ((h * 15.) + (m / 4.) + (s / 240.));
    
    return degrees;
}

float convertDEC(float d, float m, float s){
    float degrees = 0;
    
    if (d < 0){
        degrees = d + ((-1 * m) / 60.) + ((-1 * s) / 3600.);
    }
    else{
        degrees = d + (m / 60.) + (s / 3600.);
    }
    
    
    
    return degrees;
}




