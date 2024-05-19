
/*


*/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

//Functions
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


//Globals

#define Freq 10
_Bool flag = false;
uint8_t Duty = 50;
cyhal_pwm_t pwm_obj;


void isr_button(void* handler_arg,cyhal_gpio_event_t event)
{
	 flag=!flag;

	 if(flag)
	 {
	  cyhal_gpio_write(P10_1, 1u);
	  cyhal_gpio_write(P10_3, 0u);
	  cyhal_pwm_start(&pwm_obj);
	  printf("\rStarted\n\r");
	 }
	 else
	 {
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

//----------------------------Initializations-------------------------------
    cybsp_init();
    //Outputs
    cyhal_gpio_init(P13_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1u);
    cyhal_gpio_init(P12_1, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u);
    cyhal_gpio_init(P12_0, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u);
    cyhal_gpio_init(P12_3, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0u);
    //Inputs
    cyhal_gpio_init(P0_4, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);
    cyhal_gpio_init(P9_1, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);
    cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);
    cyhal_gpio_init(P9_7, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);
    cyhal_gpio_init(P9_6, CYHAL_GPIO_DIR_INPUT	, CYHAL_GPIO_DRIVE_NONE, 1u);
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

    //Printf
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    __enable_irq();
//--------------------------------------------------------------------------

    printf("\rWelcome\n\r");

    for (;;)
    {
    	if(flag == false)
    	{
    		printf("\rIn Sleepmode\n\r");
    		cyhal_syspm_sleep();
    	}

    }
}

