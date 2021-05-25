/*
 * Smart_Garage_System.c
 *
 * Created: 5/11/2021 1:04:55 AM
 *  Author: ahmedsoliman
 */ 


#include "Garage_System.h"

int main(void)
{
	Garage_System_Setup();
    while(1)
    {
        Garage_System_OPERATE();
    }
}