volatile boolean ledOn;

//TC1 ch 0
void TC3_Handler()
{
   TC_GetStatus(TC1, 0);
   digitalWrite(13, ledOn = !ledOn);
}

void TimerStart(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t freq)
{
   pmc_set_writeprotect(false);
   pmc_enable_periph_clk(irq);
   TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |
                             TC_CMR_TCCLKS_TIMER_CLOCK4);
   uint32_t rc = VARIANT_MCK / 128 / freq;
   TC_SetRA(tc, channel, rc >> 1); // 50% duty cycle square wave
   TC_SetRC(tc, channel, rc);
   TC_Start(tc, channel);
   tc->TC_CHANNEL[channel].TC_IER=  TC_IER_CPCS | TC_IER_CPAS;
   tc->TC_CHANNEL[channel].TC_IDR=~(TC_IER_CPCS | TC_IER_CPAS);
   NVIC_EnableIRQ(irq);
}

void setup()
{
  pinMode(13, OUTPUT);
  TimerStart(TC1, 0, TC3_IRQn, 1);
}

void loop()
{

}
