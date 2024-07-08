
#include "stm32f3xx.h"

void CreateTask(int id, int priority)
{
    NVIC_SetPriority(id, priority);
    NVIC_EnableIRQ(id);
}

void WakeUpTask(int id)
{
   NVIC_SetPendingIRQ(id); 
}

void SleepTask(int id)
{
    NVIC_ClearPendingIRQ(id);   
}
