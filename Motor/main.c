
/*


*/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

//----------------------------Globals---------------------------------------

#define I2C_MASTER_FREQUENCY (400000u)
#define Freq 10
_Bool flag = false;
uint8_t Duty = 50;
cyhal_pwm_t pwm_obj;


//----------------------------Functions-------------------------------------

void Instructions(void)
{
	uint8_t i = 0;
	while(i < 2)
	{
	 cyhal_gpio_write(P12_0, 1u);
	 cyhal_system_delay_ms(150);
	 cyhal_gpio_write(P12_0, 0u);
	 cyhal_system_delay_ms(150);
	 i++;
	}
	printf("\rButton 1    = Decrease Duty Cycle.\n\r");
	printf("\rButton 2    = Increase Duty Cycle.\n\r");
	printf("\rButton 3    = Turn Right.\n\r");
	printf("\rButton 4    = Turn Left.\n\r");
	printf("\rButton PSOC = Start / Stop.\n\r");
}

void isr_button(void* handler_arg,cyhal_gpio_event_t event)
{
	 flag=!flag;

	 if(flag)
	 {
      cyhal_gpio_write(P13_7,0u);
	  cyhal_gpio_write(P10_1, 1u);
	  cyhal_gpio_write(P10_3, 0u);
	  cyhal_pwm_start(&pwm_obj);
	  printf("\rStarted\n\r");
	 }
	 else
	 {
	  cyhal_gpio_write(P13_7,1u);
	  cyhal_gpio_write(P10_1, 0u);
	  cyhal_gpio_write(P10_3, 0u);
	  cyhal_pwm_stop(&pwm_obj);
	  printf("\rStopped\n\r");
	 }

}

void isr_button1(void* handler_arg,cyhal_gpio_event_t event)
{
	if(flag)
	{
	 if(Duty > 10)
	  {
	  Duty = Duty - 10;
	  }
	 cyhal_pwm_set_duty_cycle(&pwm_obj, Duty,Freq);
	 printf("\rDuty Decreased.\n\r");
	 printf("\rDuty = %d%%\n\r",Duty);
    }
	else
	{
	 printf("\rPress The On Button First!\n\r");
	 Instructions();
	}
}

void isr_button2(void* handler_arg,cyhal_gpio_event_t event)
{
	if(flag)
	{
	  if(Duty < 100)
	  {
	   Duty = Duty + 10;
	  }
	 cyhal_pwm_set_duty_cycle(&pwm_obj, Duty,Freq);
	 printf("\rDuty Increased.\n\r");
	 printf("\rDuty = %d%%\n\r",Duty);
     }
     else
     {
      printf("\rPress The On Button First!\n\r");
      Instructions();
     }
}

void isr_button3(void* handler_arg,cyhal_gpio_event_t event)
{
	if(flag)
	{
  	 cyhal_gpio_write(P10_1, 1u);
     cyhal_gpio_write(P10_3, 0u);
     printf("\rVooruit.\n\r");
	}
	else
	{
	 printf("\rPress The On Button First!\n\r");
	 Instructions();
	}
}

void isr_button4(void* handler_arg,cyhal_gpio_event_t event)
{
	if(flag)
	{
     cyhal_gpio_write(P10_3, 1u);
     cyhal_gpio_write(P10_1, 0u);
     printf("\rAchteruit.\n\r");
    }
	else
	{
	 printf("\rPress The On Button First!\n\r");
	 Instructions();
	}
}



int main(void)
{
//----------------------------Local Declarations----------------------------
	//Callback
	cyhal_gpio_callback_data_t cb_data =
	{
	    .callback = isr_button,
	};
	cyhal_gpio_callback_data_t cb_data1 =
	{
	    .callback = isr_button1,
	};
	cyhal_gpio_callback_data_t cb_data2 =
	{
	    .callback = isr_button2,
	};
	cyhal_gpio_callback_data_t cb_data3 =
	{
	    .callback = isr_button3,
	};
	cyhal_gpio_callback_data_t cb_data4 =
	{
	    .callback = isr_button4,
	};

	//I2C
	cyhal_i2c_t         i2c_master_obj;

	cyhal_i2c_cfg_t i2c_master_config =
	    { CYHAL_I2C_MODE_MASTER ,0 ,I2C_MASTER_FREQUENCY };

//----------------------------Initializations-------------------------------
    cybsp_init();

    //Outputs
    cyhal_gpio_init(P13_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1u); //Led voor aangeven dat de motor aan staat.
    cyhal_gpio_init(P12_1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u); //Niet gebruikt
    cyhal_gpio_init(P12_0, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u); //Buzzer
    cyhal_gpio_init(P12_3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u); //Niet gebruikt

    //Inputs
    cyhal_gpio_init(P0_4, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);   //Button Start/Stop
    cyhal_gpio_init(P9_1, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);   //BUtton 1
    cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);   //Button 2
    cyhal_gpio_init(P9_7, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);   //Button 3
    cyhal_gpio_init(P9_6, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);   //Button 4

    //Callback
    cyhal_gpio_register_callback(P0_4, &cb_data);
    cyhal_gpio_enable_event(P0_4, CYHAL_GPIO_IRQ_FALL, 3, true);
    cyhal_gpio_register_callback(P9_1, &cb_data1);
    cyhal_gpio_enable_event(P9_1, CYHAL_GPIO_IRQ_FALL, 3, true);
    cyhal_gpio_register_callback(P9_4, &cb_data2);
    cyhal_gpio_enable_event(P9_4, CYHAL_GPIO_IRQ_FALL, 3, true);
    cyhal_gpio_register_callback(P9_7, &cb_data3);
    cyhal_gpio_enable_event(P9_7, CYHAL_GPIO_IRQ_FALL, 3, true);
    cyhal_gpio_register_callback(P9_6, &cb_data4);
    cyhal_gpio_enable_event(P9_6, CYHAL_GPIO_IRQ_FALL, 3, true);

    //Motor Pinnen
    cyhal_gpio_init(P10_1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1u);
    cyhal_gpio_init(P10_3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1u);

    //PWM
    cyhal_pwm_init(&pwm_obj, P10_0, NULL);
    cyhal_pwm_set_duty_cycle(&pwm_obj, Duty,Freq);

    //I2C
    uint8_t PrintTeller = 0;               // Teller voor printf (anders printen we teveel)
    uint16_t Data;                         // 16Bit Data van sensor.
    uint8_t VCNL4010_PROXIMITYDATA = 0x87; // Register for proximity measurement.
    uint8_t VCNL4010_M[2];
    VCNL4010_M[0] = 0x80;                  // Command register.
    VCNL4010_M[1] = 0x08;                  // Demand  proximity measurement.
    cyhal_i2c_init(&i2c_master_obj, P6_1, P6_0, NULL);
    cyhal_i2c_configure(&i2c_master_obj,&i2c_master_config);

    //Printf
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    __enable_irq();

//--------------------------------------------------------------------------

    printf("\rWelcome\n\r");

    Instructions();

    for (;;)
    {
    	if(flag)
    	{
    	 cyhal_i2c_master_write(&i2c_master_obj, 0x13, &VCNL4010_M, 2, 0, true);
    	 cyhal_i2c_master_write(&i2c_master_obj, 0x13, &VCNL4010_PROXIMITYDATA, 1u, 0, true);
         cyhal_i2c_master_read(&i2c_master_obj,0x13,&Data, 2, 0, true);

    	 Data = Data - 25352;  //gaf 25352 bij niks. Geeft nu een waarde tussen 0 en ongeveer 65.535.
         if(PrintTeller == 40) //Na 40 * 100ms = 4000ms = 4Sec een printf.
         {
        	 printf("\r                                      Proximity_data = %u\n\r",Data);
        	 PrintTeller = 0;
         }

    	 if( (Data > 45000) && (Data < 60000) )
    	 {
    		 flag=!flag;
    		 cyhal_gpio_write(P10_1, 0u);
    		 cyhal_gpio_write(P10_3, 0u);
    		 cyhal_pwm_stop(&pwm_obj);
    		 printf("\rStopped\n\r");
    	 }
    	 PrintTeller++;
    	 cyhal_system_delay_ms(100);
    	}

    	if(flag == false)
    	{
    		cyhal_gpio_write(P13_7,1u);
    		printf("\rIn Sleepmode\n\r");
    		cyhal_syspm_sleep();          //Sleepmode wnr de motorsturing niet actief is.
    	}

    }
}

