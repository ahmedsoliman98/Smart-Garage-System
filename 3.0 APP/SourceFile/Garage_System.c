/*
 * Garage_System.c
 *
 * This project is about to making a smart Gage system
 * it contain an Ultrasonic in each door Enter and Exit each one of them when the car is in distance one meter or leas open the door 
 * the is Servo motor that open in angle 90 degree each door is open in different direction in opposite to the car waiting Exist  
 * the garage have a three parking space each one of them has an IR sensor to detect if it empty or not
 * LCD display the number of car in the garage and the space that the driver should park on it 
 * 
 * Created: 1/25/2021 8:49:02 AM
 *  Author: ahmedsoliman
 */ 
 #include "ATmega32A_Config.h"
 #include "ATmega32A_DIO_Config.h"
 #include "Servo_Motor.h"
 #include "Ultrasonic_HCSR-04.h"
 #include "PIR_Sensor.h"
 #include "LCD.h"
 #include <util/delay.h>
 
 /* Define the Port and pin of the each IR sensor */
PIR_dataType parcking_palace1 = {GPIOB, PIN0};
PIR_dataType parcking_palace2 = {GPIOB, PIN1};
PIR_dataType parcking_palace3 = {GPIOB, PIN2};



/* Define the Port and pin of the each Ultrasonic sensor */	
ULTRASONIC_DATATYPE Enter_Gate = {GPIOD,PIN1, PIN3};
ULTRASONIC_DATATYPE Exit_Gate = {GPIOD,PIN0, PIN2};
	
  #define Car_Exsit_distance (200)
  /* Set an 8 bit variable for count the number of car */
  unsigned char Cars_Number ;
  short d;
  void Garage_System_Setup()
  {
	  /* Set up and initiate the LCD */
	  LCD_init();
	  
	  /* Set up and initiate the IR Sensor  */
	  PIR_setup(parcking_palace1);
	  PIR_setup(parcking_palace2);
	  PIR_setup(parcking_palace3);
	  
	  /* Set up and initiate the Ultrsonic Sensor */
	  ULTRASONIC_Setup(Enter_Gate);
	  ULTRASONIC_Setup(Exit_Gate);
	  /* Set the Servo at a Zero position . A refer to the Enter gate servo, B refer to the Exist gate servo*/
	  SERVO_WRITE(1.5,'A');
	  SERVO_WRITE(1.5,'B');
  }
  
  
  void Garage_System_OPERATE()
  {
	  /***************************** Default screen if there is no car want to enter ***********************************************/
	   if (ULTRASONIC_OPERATE(Enter_Gate) > Car_Exsit_distance && Cars_Number < 3)
	   {
		   /* We can not use delay because that will effect on the timer and the ultrasonic . A refer to the Enter gate servo */
		   //SERVO_WRITE(1.5, 'A');
		   LCD_Clear();
		   while(ULTRASONIC_OPERATE(Enter_Gate) > Car_Exsit_distance && Cars_Number < 3 && ULTRASONIC_OPERATE(Exit_Gate) > Car_Exsit_distance)
		   {
			   LCD_DisplayString(" Welcome ^_^");
			   LCD_Select_RowCol(1,0);
			   LCD_DisplayString("  24Hr/7weeks   ");
		   }			   
	   }
 /************************************** display screen if the garage is full **************************************************************/
	   else if ( Cars_Number == 3 )
	   {
		   /* this condition to not harm the closer cars to the gate */
		   if (ULTRASONIC_OPERATE(Enter_Gate) >= Car_Exsit_distance)
		   {
			   /* to make sure to Close the gate. A refer to the Enter gate servo */
			   SERVO_WRITE(1.5, 'A');
		   }
		   LCD_Clear();
		  while(ULTRASONIC_OPERATE(Enter_Gate) >= Car_Exsit_distance && ULTRASONIC_OPERATE(Exit_Gate) > Car_Exsit_distance)
		  {
			   LCD_DisplayString("   Sorry!!   ");
			   LCD_Select_RowCol(1,0);
			   LCD_DisplayString("Garage Full    ");
		  }			  
	   }
	   
 /******************************************************* display screen the garage is not full and there is car close to the Enter gate ******************/
 
	 else if (ULTRASONIC_OPERATE(Enter_Gate)  > 10 && ULTRASONIC_OPERATE(Enter_Gate) <= Car_Exsit_distance && Cars_Number < 3)
		  {
			/* open the Servo in angle 90 degree. A refer to the Enter gate servo */
			SERVO_WRITE(2,'A');
			  
			/* keep display on the LCD guide the driver to the packing place */
			while(ULTRASONIC_OPERATE(Enter_Gate) > 10 && ULTRASONIC_OPERATE(Enter_Gate) <= Car_Exsit_distance)
			{
				if (PIR_operate(parcking_palace1) == LOW)
				{
					/* Wait until the driver reach it is place */
					LCD_Select_RowCol(1,0);
					LCD_DisplayString("please go to P1   ");
				}
				else if (PIR_operate(parcking_palace2) == LOW)
				{
					/* Wait until the driver reach it is place */
					LCD_Select_RowCol(1,0);
					LCD_DisplayString("please go to P2   ");
				}
				else if (PIR_operate(parcking_palace3) == LOW)
				{
					/* Wait until the driver reach it is place */
					LCD_Select_RowCol(1,0);
					LCD_DisplayString("please go to P3   ");
				}
			}			
			   
		  	/* increase the number of counter car in the Garage */
		  	Cars_Number++; 
			  
			/* We can not use delay because that will effect on the timer and the ultrasonic reading so after make sure that car is far from the ultrasonic range close the Enter gate */
			SERVO_WRITE(1.5, 'A'); 
	  }
	  
 /************************************************ Car Close From the Exist Gate *********************************************************************/	
  
	  if (ULTRASONIC_OPERATE(Exit_Gate)  > 10 && ULTRASONIC_OPERATE(Exit_Gate) <= Car_Exsit_distance)
	  {
		  while(ULTRASONIC_OPERATE(Exit_Gate)  > 10 && ULTRASONIC_OPERATE(Exit_Gate) <= Car_Exsit_distance)
		  {
			  /* open the Servo in angle -90 degree. B refer to the Exit gate servo */
			  SERVO_WRITE(1,'B');
		  }	
		  
		  /* Decrease the number of car in the Garage */
		  Cars_Number--;
		  
		  /* Close the Exist gate after make sure that the car is far from the ultrasonic range */
		  SERVO_WRITE(1.5,'B');	  
	  }
	 
  }