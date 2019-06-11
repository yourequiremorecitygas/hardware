


byte ReadRegisterValue(int RegisterAddress)
{
  byte data = 0;
  
  Wire.beginTransmission(OV7670_I2C_ADDRESS);         
  Wire.write(RegisterAddress);                        
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_ADDRESS, 1);            
  while(Wire.available() < 1);              
  data = Wire.read(); 

  return data;  
}

void ReadRegisters()
{
  byte data = 0;
  
  data = ReadRegisterValue(CLKRC);
  Serial.print(F("CLKRC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM7);
  Serial.print(F("COM7 = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM3);
  Serial.print(F("COM3 = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM14);
  Serial.print(F("COM14 = "));
  Serial.println(data,HEX);
   
  data = ReadRegisterValue(SCALING_XSC);
  Serial.print(F("SCALING_XSC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(SCALING_YSC);
  Serial.print(F("SCALING_YSC = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(SCALING_DCWCTR);
  Serial.print(F("SCALING_DCWCTR = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(SCALING_PCLK_DIV);
  Serial.print(F("SCALING_PCLK_DIV = "));
  Serial.println(data,HEX);
   
  data = ReadRegisterValue(SCALING_PCLK_DELAY);
  Serial.print(F("SCALING_PCLK_DELAY = "));
  Serial.println(data,HEX);
  
  //data = ReadRegisterValue(COM10);
  //Serial.print(F("COM10 (Vsync Polarity) = "));
  //Serial.println(data,HEX);
  
  // default value D
  data = ReadRegisterValue(TSLB);
  Serial.print(F("TSLB (YUV Order- Higher Bit, Bit[3]) = "));
  Serial.println(data,HEX);
  
  // default value 88
  data = ReadRegisterValue(COM13);
  Serial.print(F("COM13 (YUV Order - Lower Bit, Bit[1]) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM17);
  Serial.print(F("COM17 (DSP Color Bar Selection) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM4);
  Serial.print(F("COM4 (works with COM 17) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM15);
  Serial.print(F("COM15 (COLOR FORMAT SELECTION) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM11);
  Serial.print(F("COM11 (Night Mode) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(COM8);
  Serial.print(F("COM8 (Color Control, AWB) = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(HAECC7);
  Serial.print(F("HAECC7 (AEC Algorithm Selection) = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(GFIX);
  Serial.print(F("GFIX = "));
  Serial.println(data,HEX);
  
   
  // Window Output
  data = ReadRegisterValue(HSTART);
  Serial.print(F("HSTART = "));
  Serial.println(data,HEX);
  //Serial.print(F(", "));
  //Serial.println(data, DEC);
  
  data = ReadRegisterValue(HSTOP);
  Serial.print(F("HSTOP = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(HREF);
  Serial.print(F("HREF = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(VSTRT);
  Serial.print(F("VSTRT = "));
  Serial.println(data,HEX);
  
  data = ReadRegisterValue(VSTOP);
  Serial.print(F("VSTOP = "));
  Serial.println(data,HEX);
 
  data = ReadRegisterValue(VREF);
  Serial.print(F("VREF = "));
  Serial.println(data,HEX);
}


void ResetCameraRegisters()
{
  // Reset Camera Registers
  // Reading needed to prevent error
  byte data = ReadRegisterValue(COM7);
  
  int result = OV7670WriteReg(COM7, COM7_VALUE_RESET );
  String sresult = ParseI2CResult(result);
  Serial.println("RESETTING ALL REGISTERS BY SETTING COM7 REGISTER to 0x80: " + sresult);

  // Delay at least 500ms 
  delay(500);
}



// Main Call to Setup the ov7670 Camera
void SetupCamera()
{
  Serial.println(F("In SetupCamera() ..."));
  InitializeOV7670Camera();
}


void InitializeOV7670Camera()
{
  Serial.println(F("Initializing OV7670 Camera ..."));
  
  //Set WRST to 0 and RRST to 0 , 0.1ms after power on.
  int DurationMicroSecs =  1;
  
  // Set mode for pins wither input or output
  pinMode(WRST , OUTPUT);
  pinMode(RRST , OUTPUT);
  pinMode(WEN  , OUTPUT);
  pinMode(VSYNC, INPUT);
  pinMode(RCLK , OUTPUT);
  
  // FIFO Ram output pins
  pinMode(DO7 , INPUT);
  pinMode(DO6 , INPUT);
  pinMode(DO5 , INPUT);
  pinMode(DO4 , INPUT);
  pinMode(DO3 , INPUT);
  pinMode(DO2 , INPUT);
  pinMode(DO1 , INPUT);
  pinMode(DO0 , INPUT);
  
  // Delay 1 ms 
  delay(1); 
  
  PulseLowEnabledPin(WRST, DurationMicroSecs); 
  
  //PulseLowEnabledPin(RRST, DurationMicroSecs); 
  // Need to clock the fifo manually to get it to reset
  digitalWrite(RRST, LOW);
  PulsePin(RCLK, DurationMicroSecs); 
  PulsePin(RCLK, DurationMicroSecs); 
  digitalWrite(RRST, HIGH);  
}

void SetupCameraAdvancedAutoWhiteBalanceConfig()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Advanced Auto White Balance Configs ........"));
  
   result = OV7670WriteReg(AWBC1, AWBC1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC1: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC2, AWBC2_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC2: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC3, AWBC3_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC3: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC4, AWBC4_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC4: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC5, AWBC5_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC5: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC6, AWBC6_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC6: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(AWBC7, AWBC7_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC7: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBC8, AWBC8_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC8: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBC9, AWBC9_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC9: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBC10, AWBC10_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC10: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBC11, AWBC11_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC11: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBC12, AWBC12_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBC12: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBCTR3, AWBCTR3_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBCTR3: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBCTR2, AWBCTR2_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBCTR2: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(AWBCTR1, AWBCTR1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBCTR1: "));
   Serial.println(sresult);
}

void SetupCameraUndocumentedRegisters()
{ 
   // Write(0xb0,0x84); //adding this improve the color a little bit
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Undocumented Registers ........"));
   result = OV7670WriteReg(0xB0, 0x84);
   sresult = ParseI2CResult(result);
   Serial.print(F("Setting B0 UNDOCUMENTED register to 0x84:= "));
   Serial.println(sresult);
}

void SetupCameraFor30FPS()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera to 30 FPS ........"));
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);

   result = OV7670WriteReg(DBLV, DBLV_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DBLV: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(EXHCH, EXHCH_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("EXHCH: "));
   Serial.println(sresult);

   result = OV7670WriteReg(EXHCL, EXHCL_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("EXHCL: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(DM_LNL, DM_LNL_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DM_LNL: "));
   Serial.println(sresult);

   result = OV7670WriteReg(DM_LNH, DM_LNH_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("DM_LNH: "));
   Serial.println(sresult);

   result = OV7670WriteReg(COM11, COM11_VALUE_30FPS);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM11: "));
   Serial.println(sresult);
   
}

void SetupCameraABLC()
{
   int result = 0;
   String sresult = "";
   
   // If ABLC is off then return otherwise
   // turn on ABLC.
   if (ABLCParam == "AblcOFF")
   {
     return;
   }
   
   Serial.println(F("........ Setting Camera ABLC ......."));
   
   result = OV7670WriteReg(ABLC1, ABLC1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ABLC1: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(THL_ST, THL_ST_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("THL_ST: "));
   Serial.println(sresult);
}

void SetupOV7670ForVGARawRGB()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("--------------------------- Setting Camera for VGA (Raw RGB) ---------------------------"));
  
   PHOTO_WIDTH  = 640;
   PHOTO_HEIGHT = 480; 
   PHOTO_BYTES_PER_PIXEL = 1;

   Serial.print(F("Photo Width = "));
   Serial.println(PHOTO_WIDTH);
   
   Serial.print(F("Photo Height = "));
   Serial.println(PHOTO_HEIGHT);
   
   Serial.print(F("Bytes Per Pixel = "));
   Serial.println(PHOTO_BYTES_PER_PIXEL);
   
   
   // Basic Registers
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_VGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(COM7, COM7_VALUE_VGA );
   //result = OV7670WriteReg(COM7, COM7_VALUE_VGA_COLOR_BAR );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM7: "));
   Serial.println(sresult);

   result = OV7670WriteReg(COM3, COM3_VALUE_VGA);   
   sresult = ParseI2CResult(result);
   Serial.print(F("COM3: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(COM14, COM14_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM14: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_XSC,SCALING_XSC_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_XSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_YSC,SCALING_YSC_VALUE_VGA );    
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_YSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_DCWCTR, SCALING_DCWCTR_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_DCWCTR: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DIV, SCALING_PCLK_DIV_VALUE_VGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DIV: "));
   Serial.println (sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DELAY,SCALING_PCLK_DELAY_VALUE_VGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DELAY: "));
   Serial.println(sresult);
 
   // COM17 - DSP Color Bar Enable/Disable
   // 0000 1000 => to Enable
   // 0x08  
   // COM17_VALUE  0x08 // Activate Color Bar for DSP
   //result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_COLOR_BAR);
   result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM17: "));
   Serial.println(sresult);
  
 
   // Set Additional Parameters
   
   // Set Camera Frames per second
   SetCameraFPSMode();
    
   // Set Camera Automatic Exposure Control
   SetCameraAEC();
   
   // Needed Color Correction, green to red
   result = OV7670WriteReg(0xB0, 0x8c);
   sresult = ParseI2CResult(result);
   Serial.print(F("Setting B0 UNDOCUMENTED register to 0x84:= "));
   Serial.println(sresult);
   
   // Set Camera Saturation
   SetCameraSaturationControl();
     
   // Setup Camera Array Control
   SetupCameraArrayControl();
   
    // Set ADC Control
   SetupCameraADCControl();
   
   // Set Automatic Black Level Calibration
   SetupCameraABLC();
    
 
 
   Serial.println(F("........... Setting Camera Window Output Parameters  ........"));
 
   // Change Window Output parameters after custom scaling
   result = OV7670WriteReg(HSTART, HSTART_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTART: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HSTOP, HSTOP_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HREF, HREF_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HREF: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTRT, VSTRT_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTRT: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTOP, VSTOP_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VREF, VREF_VALUE_VGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VREF: "));
   Serial.println(sresult);
}

void SetupOV7670ForVGAProcessedBayerRGB()
{
   int result = 0;
   String sresult = "";
   
   // Call Base for VGA Raw Bayer RGB Mode
   SetupOV7670ForVGARawRGB();
  
   Serial.println(F("------------- Setting Camera for VGA (Processed Bayer RGB) ----------------"));
    
   // Set key register for selecting processed bayer rgb output
   result = OV7670WriteReg(COM7, COM7_VALUE_VGA_PROCESSED_BAYER );
   //result = OV7670WriteReg(COM7, COM7_VALUE_VGA_COLOR_BAR );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM7: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(TSLB, 0x04);
   sresult = ParseI2CResult(result);
   Serial.print(F("Initializing TSLB register result = "));
   Serial.println(sresult);
   
   // Needed Color Correction, green to red
   result = OV7670WriteReg(0xB0, 0x8c);
   sresult = ParseI2CResult(result);
   Serial.print(F("Setting B0 UNDOCUMENTED register to 0x84:= "));
   Serial.println(sresult);
  
   // Set Camera Automatic White Balance
   SetupCameraAWB();
   
   // Denoise and Edge Enhancement
   SetupCameraDenoiseEdgeEnhancement();
}

void SetupCameraAverageBasedAECAGC()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("-------------- Setting Camera Average Based AEC/AGC Registers ---------------"));
  
   result = OV7670WriteReg(AEW, AEW_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AEW: "));
   Serial.println(sresult);

   result = OV7670WriteReg(AEB, AEB_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AEB: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(VPT, VPT_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("VPT: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(HAECC7, HAECC7_VALUE_AVERAGE_AEC_ON);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC7: "));
   Serial.println(sresult);   
}

void SetCameraHistogramBasedAECAGC()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("-------------- Setting Camera Histogram Based AEC/AGC Registers ---------------"));
  
   result = OV7670WriteReg(AEW, AEW_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AEW: "));
   Serial.println(sresult);

   result = OV7670WriteReg(AEB, AEB_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("AEB: "));
   Serial.println(sresult);
    
   result = OV7670WriteReg(HAECC1, HAECC1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC1: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC2, HAECC2_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC2: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC3, HAECC3_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC3: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC4, HAECC4_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC4: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC5, HAECC5_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC5: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC6, HAECC6_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC6: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HAECC7, HAECC7_VALUE_HISTOGRAM_AEC_ON);
   sresult = ParseI2CResult(result);
   Serial.print(F("HAECC7: "));
   Serial.println(sresult);  
}

void SetupOV7670ForQVGAYUV()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("--------------------------- Setting Camera for QVGA (YUV) ---------------------------"));
  
   PHOTO_WIDTH  = 320;
   PHOTO_HEIGHT = 240; 
   PHOTO_BYTES_PER_PIXEL = 2;

   Serial.print(F("Photo Width = "));
   Serial.println(PHOTO_WIDTH);
   
   Serial.print(F("Photo Height = "));
   Serial.println(PHOTO_HEIGHT);
   
   Serial.print(F("Bytes Per Pixel = "));
   Serial.println(PHOTO_BYTES_PER_PIXEL);
   
   
   // Basic Registers
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_QVGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(COM7, COM7_VALUE_QVGA );
   //result = OV7670WriteReg(COM7, COM7_VALUE_QVGA_COLOR_BAR );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM7: "));
   Serial.println(sresult);

   result = OV7670WriteReg(COM3, COM3_VALUE_QVGA);  
   sresult = ParseI2CResult(result);
   Serial.print(F("COM3: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(COM14, COM14_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM14: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_XSC,SCALING_XSC_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_XSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_YSC,SCALING_YSC_VALUE_QVGA );    
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_YSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_DCWCTR, SCALING_DCWCTR_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_DCWCTR: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DIV, SCALING_PCLK_DIV_VALUE_QVGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DIV: "));
   Serial.println (sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DELAY,SCALING_PCLK_DELAY_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DELAY: "));
   Serial.println(sresult);
   
   // YUV order control change from default use with COM13
   //result = OV7670WriteReg(TSLB, TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_ENABLED);
   //result = OV7670WriteReg(TSLB, TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED); // Works
   //result = OV7670WriteReg(TSLB, TSLB_VALUE_UYVY_AUTO_OUTPUT_WINDOW_DISABLED);
   //result = OV7670WriteReg(TSLB, TSLB_VALUE_TESTVALUE); 
   result = OV7670WriteReg(TSLB, 0x04);
   sresult = ParseI2CResult(result);
   Serial.print(F("TSLB: "));
   Serial.println(sresult);
  
   //COM13
   //result = OV7670WriteReg(COM13, COM13_VALUE_GAMMA_YUYV);
   //result = OV7670WriteReg(COM13, COM13_VALUE_DEFAULT);
   //result = OV7670WriteReg(COM13, COM13_VALUE_GAMMA_YVYU);
   //result = OV7670WriteReg(COM13, COM13_VALUE_NOGAMMA_YUYV);
   //result = OV7670WriteReg(COM13, COM13_VALUE_YUYV_UVSATAUTOADJ_ON); 
   // { REG_COM13, /*0xc3*/0x48 } // error in datasheet bit 3 controls YUV order
   //result = OV7670WriteReg(COM13, 0x48);
   //result = OV7670WriteReg(COM13, 0xC8); // needed for correct color bar
   result = OV7670WriteReg(COM13, 0xC2);   // from YCbCr reference specs 
   //result = OV7670WriteReg(COM13, 0x82);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM13: "));
   Serial.println(sresult);
  
   // COM17 - DSP Color Bar Enable/Disable
   // 0000 1000 => to Enable
   // 0x08  
   // COM17_VALUE  0x08 // Activate Color Bar for DSP
   //result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_COLOR_BAR);
   result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM17: "));
   Serial.println(sresult);
  
 
 
   // Set Additional Parameters
   
   // Set Camera Frames per second
   SetCameraFPSMode();
    
   // Set Camera Automatic Exposure Control
   SetCameraAEC();
   
   // Set Camera Automatic White Balance
   SetupCameraAWB();
   
   // Setup Undocumented Registers - Needed Minimum
   SetupCameraUndocumentedRegisters();
   
   // Set Color Matrix for YUV
   if (YUVMatrixParam == "YUVMatrixOn")
   {
     SetCameraColorMatrixYUV();
   }
  
   // Set Camera Saturation
   SetCameraSaturationControl();
   
    // Denoise and Edge Enhancement
   SetupCameraDenoiseEdgeEnhancement();
   
   
   // Set up Camera Array Control
   SetupCameraArrayControl();
   
   
   // Set ADC Control
   SetupCameraADCControl();
   
   
   // Set Automatic Black Level Calibration
   SetupCameraABLC();
    
  
   Serial.println(F("........... Setting Camera Window Output Parameters  ........"));
 
   // Change Window Output parameters after custom scaling
   result = OV7670WriteReg(HSTART, HSTART_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTART: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HSTOP, HSTOP_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HREF, HREF_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HREF: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTRT, VSTRT_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTRT: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTOP, VSTOP_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VREF, VREF_VALUE_QVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VREF: "));
   Serial.println(sresult);
}

void SetupCameraNightMode()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("......... Turning NIGHT MODE ON ........"));
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_NIGHTMODE_AUTO);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(COM11, COM11_VALUE_NIGHTMODE_AUTO);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM11: "));
   Serial.println(sresult); 
}


void SetupCameraSimpleAutomaticWhiteBalance()
{
 /*
   i2c_salve_Address = 0x42;
   write_i2c(0x13, 0xe7); //AWB on
   write_i2c(0x6f, 0x9f); // Simple AWB
 */
 
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera to Simple AWB ........"));
  
   // COM8
   //result = OV7670WriteReg(0x13, 0xE7);
   result = OV7670WriteReg(COM8, COM8_VALUE_AWB_ON);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM8(0x13): "));
   Serial.println(sresult);
 
   // AWBCTR0
   //result = OV7670WriteReg(0x6f, 0x9f);
   result = OV7670WriteReg(AWBCTR0, AWBCTR0_VALUE_NORMAL);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWBCTR0 Control Register 0(0x6F): "));
   Serial.println(sresult);
}

void SetupCameraAdvancedAutomaticWhiteBalance()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera to Advanced AWB ........"));
  
   // AGC, AWB, and AEC Enable
   result = OV7670WriteReg(0x13, 0xE7);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM8(0x13): "));
   Serial.println(sresult);
 
   // AWBCTR0 
   result = OV7670WriteReg(0x6f, 0x9E);
   sresult = ParseI2CResult(result);
   Serial.print(F("AWB Control Register 0(0x6F): "));
   Serial.println(sresult);
}

void SetupCameraGain()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Gain ........"));
   
   // Set Maximum Gain
   //result = OV7670WriteReg(COM9, COM9_VALUE_MAX_GAIN_128X);
   result = OV7670WriteReg(COM9, COM9_VALUE_4XGAIN);
   //result = OV7670WriteReg(COM9, 0x18);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM9: "));
   Serial.println(sresult);
   
   // Set Blue Gain
   //{ REG_BLUE, 0x40 },
   result = OV7670WriteReg(BLUE, BLUE_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("BLUE GAIN: "));
   Serial.println(sresult);
   
   // Set Red Gain
   //{ REG_RED, 0x60 },
   result = OV7670WriteReg(RED, RED_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("RED GAIN: "));
   Serial.println(sresult);
   
   
   // Set Green Gain
   //{ 0x6a, 0x40 }, 
   result = OV7670WriteReg(GGAIN, GGAIN_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GREEN GAIN: "));
   Serial.println(sresult);
   
   
   // Enable AWB Gain
   // REG_COM16 0x41  /* Control 16 */
   // COM16_AWBGAIN   0x08    /* AWB gain enable */
   // { REG_COM16, COM16_AWBGAIN }, 
   result = OV7670WriteReg(COM16, COM16_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM16(ENABLE GAIN): "));
   Serial.println(sresult);
   
}

void SetCameraSaturationControl()
{
  int result = 0;
  String sresult = "";
  
  Serial.println(F("........... Setting Camera Saturation Level ........"));
  result = OV7670WriteReg(SATCTR, SATCTR_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("SATCTR: "));
  Serial.println(sresult);
}

void SetCameraColorMatrixYUV()
{
  int result = 0;
  String sresult = "";
  
  Serial.println(F("........... Setting Camera Color Matrix for YUV ........"));
  
  result = OV7670WriteReg(MTX1, MTX1_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX1: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTX2, MTX2_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX2: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTX3, MTX3_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX3: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTX4, MTX4_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX4: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTX5, MTX5_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX5: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTX6, MTX6_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTX6: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(CONTRAS, CONTRAS_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("CONTRAS: "));
  Serial.println(sresult);
 
  result = OV7670WriteReg(MTXS, MTXS_VALUE);
  sresult = ParseI2CResult(result);
  Serial.print(F("MTXS: "));
  Serial.println(sresult);  
}

void SetCameraFPSMode()
{
   // Set FPS for Camera
   if (FPSParam == "ThirtyFPS")
   {
     SetupCameraFor30FPS();
   }    
   else
   if (FPSParam == "NightMode")
   {
     SetupCameraNightMode();
   } 
}

void SetCameraAEC()
{
    // Process AEC
   if (AECParam == "AveAEC")
   {
     // Set Camera's Average AEC/AGC Parameters  
     SetupCameraAverageBasedAECAGC();  
   }
   else
   if (AECParam == "HistAEC")
   { 
     // Set Camera AEC algorithim to Histogram
     SetCameraHistogramBasedAECAGC();
   }
}

void SetupCameraAWB()
{
   // Set AWB Mode
   if (AWBParam == "SAWB")
   {
     // Set Simple Automatic White Balance
     SetupCameraSimpleAutomaticWhiteBalance(); // OK
      
     // Set Gain Config
     SetupCameraGain();
   }
   else
   if (AWBParam == "AAWB")
   {
     // Set Advanced Automatic White Balance
     SetupCameraAdvancedAutomaticWhiteBalance(); // ok
   
     // Set Camera Automatic White Balance Configuration
     SetupCameraAdvancedAutoWhiteBalanceConfig(); // ok
     
     // Set Gain Config
     SetupCameraGain();
   }
}


void SetupCameraDenoise()
{  
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Denoise  ........"));
  
   result = OV7670WriteReg(DNSTH, DNSTH_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("DNSTH: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(REG77, REG77_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("REG77: "));
   Serial.println(sresult);
}

void SetupCameraEdgeEnhancement()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Edge Enhancement  ........"));
  
   result = OV7670WriteReg(EDGE, EDGE_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("EDGE: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(REG75, REG75_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("REG75: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(REG76, REG76_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("REG76: "));
   Serial.println(sresult);
}

void SetupCameraDenoiseEdgeEnhancement()
{
   int result = 0;
   String sresult = "";
   
   if ((DenoiseParam == "DenoiseYes")&& 
       (EdgeParam == "EdgeYes"))
      {
        SetupCameraDenoise();
        SetupCameraEdgeEnhancement();
        result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);
        sresult = ParseI2CResult(result);
        Serial.print(F("COM16: "));
        Serial.println(sresult);
      }
      else
      if ((DenoiseParam == "DenoiseYes")&& 
          (EdgeParam == "EdgeNo"))
       {
         SetupCameraDenoise();
         result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_ON__EDGE_ENHANCEMENT_OFF__AWBGAIN_ON);
         sresult = ParseI2CResult(result);
         Serial.print(F("COM16: "));
         Serial.println(sresult);
       }
       else
       if ((DenoiseParam == "DenoiseNo")&& 
          (EdgeParam == "EdgeYes"))
          {
            SetupCameraEdgeEnhancement();
            result = OV7670WriteReg(COM16, COM16_VALUE_DENOISE_OFF__EDGE_ENHANCEMENT_ON__AWBGAIN_ON);
            sresult = ParseI2CResult(result);
            Serial.print(F("COM16: "));
            Serial.println(sresult);
          }
}


/*

void SetCameraGamma()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Gamma  ........"));
  
   result = OV7670WriteReg(SLOP, SLOP_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("SLOP: "));
   Serial.println(sresult);

   result = OV7670WriteReg(GAM1, GAM1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM1: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(GAM2, GAM2_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM2: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(GAM3, GAM3_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM3: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM4, GAM4_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM4: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM5, GAM5_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM5: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM6, GAM6_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM6: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM7, GAM7_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM7: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM8, GAM8_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM8: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM9, GAM9_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM9: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM10, GAM10_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM10: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM11, GAM11_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM11: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM12, GAM12_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM12: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM13, GAM13_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM13: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM14, GAM14_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM14: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(GAM15, GAM15_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("GAM15: "));
   Serial.println(sresult);
}
*/


void SetupCameraArrayControl()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera Array Control  ........"));
  
   result = OV7670WriteReg(CHLF, CHLF_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("CHLF: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(ARBLM, ARBLM_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ARBLM: "));
   Serial.println(sresult);
}


void SetupCameraADCControl()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("........... Setting Camera ADC Control  ........"));
  
   result = OV7670WriteReg(ADCCTR1, ADCCTR1_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ADCCTR1: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(ADCCTR2, ADCCTR2_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ADCCTR2: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(ADC, ADC_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ADC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(ACOM, ACOM_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("ACOM: "));
   Serial.println(sresult);

   result = OV7670WriteReg(OFON, OFON_VALUE);
   sresult = ParseI2CResult(result);
   Serial.print(F("OFON: "));
   Serial.println(sresult);  
}

void SetupOV7670ForQQVGAYUV()
{
   int result = 0;
   String sresult = "";
   
   Serial.println(F("--------------------------- Setting Camera for QQVGA YUV ---------------------------"));
  
   PHOTO_WIDTH  = 160;
   PHOTO_HEIGHT = 120; 
   PHOTO_BYTES_PER_PIXEL = 2;

   Serial.print(F("Photo Width = "));
   Serial.println(PHOTO_WIDTH);
   
   Serial.print(F("Photo Height = "));
   Serial.println(PHOTO_HEIGHT);
   
   Serial.print(F("Bytes Per Pixel = "));
   Serial.println(PHOTO_BYTES_PER_PIXEL);
   
    
   Serial.println(F("........... Setting Basic QQVGA Parameters  ........"));
 
   result = OV7670WriteReg(CLKRC, CLKRC_VALUE_QQVGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("CLKRC: "));
   Serial.println(sresult);
 
   result = OV7670WriteReg(COM7, COM7_VALUE_QQVGA );
   //result = OV7670WriteReg(COM7, COM7_VALUE_QQVGA_COLOR_BAR );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM7: "));
   Serial.println(sresult);

   result = OV7670WriteReg(COM3, COM3_VALUE_QQVGA); 
   sresult = ParseI2CResult(result);
   Serial.print(F("COM3: "));
   Serial.println(sresult);
   
   result = OV7670WriteReg(COM14, COM14_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("COM14: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_XSC,SCALING_XSC_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_XSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_YSC,SCALING_YSC_VALUE_QQVGA );    
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_YSC: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_DCWCTR, SCALING_DCWCTR_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_DCWCTR: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DIV, SCALING_PCLK_DIV_VALUE_QQVGA);
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DIV: "));
   Serial.println (sresult);
  
   result = OV7670WriteReg(SCALING_PCLK_DELAY,SCALING_PCLK_DELAY_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("SCALING_PCLK_DELAY: "));
   Serial.println(sresult);
   
   // YUV order control change from default use with COM13
   result = OV7670WriteReg(TSLB, TSLB_VALUE_YUYV_AUTO_OUTPUT_WINDOW_DISABLED); // Works
   sresult = ParseI2CResult(result);
   Serial.print(F("TSLB: "));
   Serial.println(sresult);
  
   //COM13
   //result = OV7670WriteReg(COM13, COM13_VALUE_GAMMA_YUYV);
   //result = OV7670WriteReg(COM13, COM13_VALUE_DEFAULT);
   //result = OV7670WriteReg(COM13, COM13_VALUE_GAMMA_YVYU);
   //result = OV7670WriteReg(COM13, COM13_VALUE_NOGAMMA_YUYV);
   //result = OV7670WriteReg(COM13, COM13_VALUE_YUYV_UVSATAUTOADJ_ON); 
   // { REG_COM13, /*0xc3*/0x48 } // error in datasheet bit 3 controls YUV order
   //result = OV7670WriteReg(COM13, 0x48);
   result = OV7670WriteReg(COM13, 0xC8);  // Gamma Enabled, UV Auto Adj On
   sresult = ParseI2CResult(result);
   Serial.print(F("COM13: "));
   Serial.println(sresult);
  
   // COM17 - DSP Color Bar Enable/Disable
   // 0000 1000 => to Enable
   // 0x08  
   // COM17_VALUE  0x08 // Activate Color Bar for DSP
   //result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_COLOR_BAR);
   result = OV7670WriteReg(COM17, COM17_VALUE_AEC_NORMAL_NO_COLOR_BAR);
   sresult = ParseI2CResult(result);
   Serial.print(F("COM17: "));
   Serial.println(sresult);
  
  
   // Set Additional Parameters
   // Set Camera Frames per second
   SetCameraFPSMode();
    
   // Set Camera Automatic Exposure Control
   SetCameraAEC();
   
   // Set Camera Automatic White Balance
   SetupCameraAWB();
   
   // Setup Undocumented Registers - Needed Minimum
   SetupCameraUndocumentedRegisters();
  
 
   // Set Color Matrix for YUV
   if (YUVMatrixParam == "YUVMatrixOn")
   {
     SetCameraColorMatrixYUV();
   }
  
   // Set Camera Saturation
   SetCameraSaturationControl();
   
   // Denoise and Edge Enhancement
   SetupCameraDenoiseEdgeEnhancement();
   
   // Set New Gamma Values
   //SetCameraGamma();
   
   // Set Array Control
   SetupCameraArrayControl();
   
   // Set ADC Control
   SetupCameraADCControl();

   // Set Automatic Black Level Calibration
   SetupCameraABLC();
    

   Serial.println(F("........... Setting Camera Window Output Parameters  ........"));
 
   // Change Window Output parameters after custom scaling
   result = OV7670WriteReg(HSTART, HSTART_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTART: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HSTOP, HSTOP_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(HREF, HREF_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("HREF: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTRT, VSTRT_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTRT: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VSTOP, VSTOP_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VSTOP: "));
   Serial.println(sresult);
  
   result = OV7670WriteReg(VREF, VREF_VALUE_QQVGA );
   sresult = ParseI2CResult(result);
   Serial.print(F("VREF: "));
   Serial.println(sresult);
}

void CaptureOV7670Frame()
{
   unsigned long DurationStart = 0;
   unsigned long DurationStop = 0;
   unsigned long TimeForCaptureStart = 0;
   unsigned long TimeForCaptureEnd = 0;
   unsigned long ElapsedTime = 0;
   
   //Capture one frame into FIFO memory
   
   // 0. Initialization. 
   Serial.println();
   Serial.println(F("Starting Capture of Photo ..."));
   TimeForCaptureStart = millis();
 
   // 1. Wait for VSync to pulse to indicate the start of the image
   DurationStart = pulseIn(VSYNC, HIGH);
 
   // 2. Reset Write Pointer to 0. Which is the beginning of frame
   PulseLowEnabledPin(WRST, 6); // 3 microseconds + 3 microseconds for error factor on Arduino

   // 3. Set FIFO Write Enable to active (high) so that image can be written to ram
   digitalWrite(WEN, HIGH);
  
   // 4. Wait for VSync to pulse again to indicate the end of the frame capture
   DurationStop = pulseIn(VSYNC, HIGH);
  
   // 5. Set FIFO Write Enable to nonactive (low) so that no more images can be written to the ram
   digitalWrite(WEN, LOW);
     
   // 6. Print out Stats
   TimeForCaptureEnd = millis();
   ElapsedTime = TimeForCaptureEnd - TimeForCaptureStart;
   
   Serial.print(F("Time for Frame Capture (milliseconds) = "));
   Serial.println(ElapsedTime);
   
   Serial.print(F("VSync beginning duration (microseconds) = "));
   Serial.println(DurationStart);
   
   Serial.print(F("VSync end duration (microseconds) = "));
   Serial.println(DurationStop);
   
   // 7. WAIT so that new data can appear on output pins Read new data.
   delay(2);
}

String CreatePhotoFilename()
{
  String Filename = "";
  String Ext = "";
  
  // Creates filename that the photo will be saved under
  
  // Create file extension
  // If Command = QQVGA or QVGA then extension is .yuv
  if ((Command == "QQVGA") || (Command == "QVGA"))
  {
     Ext = ".yuv";    
  }
  else
  if ((Command == "VGA") || (Command == "VGAP"))
  {
    Ext = ".raw"; 
  }
  
  // Create Filename from
  // Resolution + PhotoNumber + Extension
  Filename = Command + PhotoTakenCount + Ext; 
 
  return Filename;
}

// Converts pin HIGH/LOW values on pins at positions 0-7 to a corresponding byte value
byte ConvertPinValueToByteValue(int PinValue, int PinPosition)
{
  byte ByteValue = 0;
  if (PinValue == HIGH)
  {
    ByteValue = 1 << PinPosition;
  }
  
  return ByteValue;
}

void ReadTransmitCapturedFrame()
{
   // Read captured frame from FIFO memory and send each byte as it is read to the Android controller
   // via bluetooth
    
   // Set Output Enable OE to active (low).
   // * Make sure to connect the OE output to ground.
   
   // Reset the FIFO read pointer by setting RRST to active (low) then bringing it back to high.
   // *Done from previous CaptureOV7670Frame() assuming WRST and RRST are tied together.
   
   // Read in the QQVGA image that is captured in the camera buffer by reading in the 38400 bytes that make up the 
   //   YUV photo 
   
   byte PixelData = 0;
   byte PinVal7 = 0;
   byte PinVal6 = 0;
   byte PinVal5 = 0;
   byte PinVal4 = 0;
   byte PinVal3 = 0;
   byte PinVal2 = 0;
   byte PinVal1 = 0;
   byte PinVal0 = 0;
   
   //Serial.println(F("Starting Transmission of Photo To SDCard ..."));
   Serial.println(F("저장 테스트 ..."));
  
   // Set Read Buffer Pointer to start of frame
   digitalWrite(RRST, LOW);
   PulsePin(RCLK, 1); 
   PulsePin(RCLK, 1);
   PulsePin(RCLK, 1);
   digitalWrite(RRST, HIGH);
   
   unsigned long  ByteCounter = 0;
   //String test = "";
   byte buff[1460];
   
   for (int height = 0; height < PHOTO_HEIGHT; height++)
   {
     for (int width = 0; width < PHOTO_WIDTH; width++)
     {
       for (int bytenumber = 0; bytenumber < PHOTO_BYTES_PER_PIXEL; bytenumber++)
       {
         // Pulse the read clock RCLK to bring in new byte of data.
         // 7ns for RCLK High Pulse Width and Low Pulse Width .007 micro secs
         PulsePin(RCLK, 1); 
             
         // Convert Pin values to byte values for pins 0-7 of incoming pixel byte
         PinVal7 = ConvertPinValueToByteValue(digitalRead(DO7), 7);
         PinVal6 = ConvertPinValueToByteValue(digitalRead(DO6), 6);
         PinVal5 = ConvertPinValueToByteValue(digitalRead(DO5), 5);
         PinVal4 = ConvertPinValueToByteValue(digitalRead(DO4), 4);
         PinVal3 = ConvertPinValueToByteValue(digitalRead(DO3), 3);
         PinVal2 = ConvertPinValueToByteValue(digitalRead(DO2), 2);
         PinVal1 = ConvertPinValueToByteValue(digitalRead(DO1), 1);
         PinVal0 = ConvertPinValueToByteValue(digitalRead(DO0), 0);
     
         // Combine individual data from each pin into composite data in the form of a single byte
         PixelData = PinVal7 | PinVal6 | PinVal5 | PinVal4 | PinVal3 | PinVal2 | PinVal1 | PinVal0;
        
         /////////////////////////////  SD Card ////////////////////////////////
         //ByteCounter = ByteCounter + ImageOutputFile.write(PixelData);

         if(ByteCounter == 0){
          client.print("AT+IMGSTART;;Seoul/Dongjak/3;;");
          isEnd = false;
         }
         else if(ByteCounter % 1460 == 0){
          client.write(buff, sizeof(buff));
          delay(1);
         }
         
         buff[ByteCounter % 1460] = PixelData;
         
         ByteCounter = ByteCounter + 1;     
         ///////////////////////////////////////////////////////////////////////
       }
     }
   }

   client.print("AT+IMGEND");
   isEnd = true;
   
   client.flush();
   client.stop();
   delay(1000);
   
   Serial.println("");
}

void TakePhoto()
{
  unsigned long StartTime   = 0;
  unsigned long EndTime     = 0;
  unsigned long ElapsedTime = 0;
  
  StartTime = millis();
  
  CaptureOV7670Frame(); 
  ReadTransmitCapturedFrame();
 
  EndTime = millis(); 
  ElapsedTime = (EndTime - StartTime)/1000; // Convert to seconds
  
  Serial.print(F("Elapsed Time for Taking and Sending Photo(secs) = "));
  Serial.println(ElapsedTime);
}

void PulseLowEnabledPin(int PinNumber, int DurationMicroSecs)
{
  // For Low Enabled Pins , 0 = on and 1 = off
  digitalWrite(PinNumber, LOW);            // Sets the pin on
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds      
  
  digitalWrite(PinNumber, HIGH);            // Sets the pin off
  delayMicroseconds(DurationMicroSecs);     // Pauses for DurationMicroSecs microseconds  
}

void PulsePin(int PinNumber, int DurationMicroSecs)
{
  digitalWrite(PinNumber, HIGH);           // Sets the pin on
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds      
  
  digitalWrite(PinNumber, LOW);            // Sets the pin off
  delayMicroseconds(DurationMicroSecs);    // Pauses for DurationMicroSecs microseconds  
}

String ParseI2CResult(int result)
{
  String sresult = "";
  switch(result)
  {
    case 0:
     sresult = "I2C Operation OK ...";
    break;
    
    case  I2C_ERROR_WRITING_START_ADDRESS:
     sresult = "I2C_ERROR_WRITING_START_ADDRESS";
    break;
    
    case I2C_ERROR_WRITING_DATA:
     sresult = "I2C_ERROR_WRITING_DATA";
    break;
      
    case DATA_TOO_LONG:
     sresult = "DATA_TOO_LONG";
    break;   
    
    case NACK_ON_TRANSMIT_OF_ADDRESS:
     sresult = "NACK_ON_TRANSMIT_OF_ADDRESS";
    break;
    
    case NACK_ON_TRANSMIT_OF_DATA:
     sresult = "NACK_ON_TRANSMIT_OF_DATA";
    break;
    
    case OTHER_ERROR:
     sresult = "OTHER_ERROR";
    break;
       
    default:
     sresult = "I2C ERROR TYPE NOT FOUND...";
    break;
  }
 
  return sresult;
}


// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
int OV7670Write(int start, const byte *pData, int size)
{
  int n, error;

  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
  {
    return (I2C_ERROR_WRITING_START_ADDRESS);
  }
  
  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
  {
    return (I2C_ERROR_WRITING_DATA);
  }
  
  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
  {
    return (error);
  }
  
  return 0;         // return : no error
}


//
// A function to write a single register
//
int OV7670WriteReg(int reg, byte data)
{
  int error;

  error = OV7670Write(reg, &data, 1);

  return (error);
}

//
// This is a common function to read multiple bytes 
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus. 
// This is implemented in Arduino 1.0.1.
//
int OV7670Read(int start, byte *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
  {
    return (I2C_READ_START_ADDRESS_ERROR);
  }
  
  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
  {
    return (n);
  }
  
  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(OV7670_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++] = Wire.read();
  }
  if ( i != size)
  {
    return (I2C_READ_DATA_SIZE_MISMATCH_ERROR);
  }
  
  return (0);  // return no error
}

//
// A function to read a single register
//
int OV7670ReadReg(int reg, byte *data)
{
  int error;

  error = OV7670Read(reg, data, 1);

  return (error);
}

void ExecuteCommand(String Command)
{
  // Set up Camera for VGA, QVGA, or QQVGA Modes
  if (Command == "VGA")
  {
     Serial.println(F("Taking a VGA Photo...")); 
     if (Resolution != VGA)
     {
       // If current resolution is not QQVGA then set camera for QQVGA
       ResetCameraRegisters();
       
       Resolution = VGA;
       SetupOV7670ForVGARawRGB();
       Serial.println(F("----------------------------- Camera Registers ----------------------------"));
       ReadRegisters();
       Serial.println(F("---------------------------------------------------------------------------"));
     }
  }
  else
  if (Command == "VGAP")
  {
     Serial.println(F("Taking a VGAP Photo...")); 
     if (Resolution != VGAP)
     {
       // If current resolution is not VGAP then set camera for VGAP
       ResetCameraRegisters();
       
       Resolution = VGAP;
       SetupOV7670ForVGAProcessedBayerRGB();
       Serial.println(F("----------------------------- Camera Registers ----------------------------"));
       ReadRegisters();
       Serial.println(F("---------------------------------------------------------------------------"));
     }
  }
  else if (Command == "QVGA")
  {
     Serial.println(F("Taking a QVGA Photo...")); 
     if (Resolution != QVGA)
     {
       // If current resolution is not QQVGA then set camera for QQVGA
       Resolution = QVGA;
       SetupOV7670ForQVGAYUV();
       Serial.println(F("----------------------------- Camera Registers ----------------------------"));
       ReadRegisters();
       Serial.println(F("---------------------------------------------------------------------------"));
     }
  }
  else if (Command == "QQVGA")
  {
     Serial.println(F("Taking a QQVGA Photo...")); 
     if (Resolution != QQVGA)
     {
       // If current resolution is not QQVGA then set camera for QQVGA
       Resolution = QQVGA;
       SetupOV7670ForQQVGAYUV();
       Serial.println(F("----------------------------- Camera Registers ----------------------------"));
       ReadRegisters();
       Serial.println(F("---------------------------------------------------------------------------"));
     }
  }
  else
  {
     Serial.print(F("The command ")); 
     Serial.print(Command);
     Serial.println(F(" is not recognized ..."));
  }
  
  
  // Delay for registers to settle
  delay(100);
  
  // Take Photo 
  turnLED(1);
  TakePhoto();
  turnLED(0);
}

boolean ProcessRawCommandElement(String Element)
{
  boolean result = false;
  
  Element.toLowerCase();
  
  if ((Element == "vga") || 
      (Element == "vgap") ||
      (Element == "qvga")||
      (Element == "qqvga"))
  {
    Element.toUpperCase(); 
    Command = Element;
    result = true;
  }
  else
  if (Element == "thirtyfps")
  { 
    FPSParam = "ThirtyFPS";
    result = true;
  }
  else
  if (Element == "nightmode")
  { 
    FPSParam = "NightMode";
    result = true;
  } 
  else
  if (Element == "sawb")
  { 
    AWBParam = "SAWB";
    result = true;
  }
  else
  if (Element == "aawb")
  { 
    AWBParam = "AAWB";
    result = true;
  }
  else
  if (Element == "aveaec")
  { 
    AECParam = "AveAEC";
    result = true;
  }
  else
  if (Element == "histaec")
  { 
    AECParam = "HistAEC";
    result = true;
  }
  else
  if (Element == "yuvmatrixon")
  { 
    YUVMatrixParam = "YUVMatrixOn";
    result = true;
  }
  else
  if (Element == "yuvmatrixoff")
  { 
    YUVMatrixParam = "YUVMatrixOff";
    result = true;
  }
  else
  if (Element == "denoiseyes")
  { 
    DenoiseParam = "DenoiseYes";
    result = true;
  }
  else
  if (Element == "denoiseno")
  { 
    DenoiseParam = "DenoiseNo";
    result = true;
  }
  else
  if (Element == "edgeyes")
  { 
    EdgeParam = "EdgeYes";
    result = true;
  }
  else
  if (Element == "edgeno")
  { 
    EdgeParam = "EdgeNo";
    result = true;
  }
  else
  if (Element == "ablcon")
  { 
    ABLCParam = "AblcON";
    result = true;
  }
  else
  if (Element == "ablcoff")
  { 
    ABLCParam = "AblcOFF";
    result = true;
  }
 
  return result;
}

void ParseRawCommand(String RawCommandLine)
{
   String Entries[10];
   boolean success = false;
     
   // Parse into command and parameters
   int NumberElements = ParseCommand(RawCommandLine.c_str(), ' ', Entries);
   
   for (int i = 0 ; i < NumberElements; i++)
   {
     boolean success = ProcessRawCommandElement(Entries[i]);
     if (!success)
     {
       Serial.print(F("Invalid Command or Parameter: "));
       Serial.println(Entries[i]);
     }
     else
     {
       Serial.print(F("Command or parameter "));
       Serial.print(Entries[i]);
       Serial.println(F(" sucessfully set ..."));
     }
   }
   
   
   // Assume parameter change since user is setting parameters on command line manually
   // Tells the camera to re-initialize and set up camera according to new parameters
   Resolution = None; // Reset and reload registers
   ResetCameraRegisters();
   
}


void DisplayHelpCommandsParams()
{
   Serial.println(F("....... Help Menu Camera Commands/Params .........."));
 
   Serial.println(F("Resolution Change Commands: VGA,VGAP,QVGA,QQVGA"));
   Serial.println(F("FPS Parameters: ThirtyFPS, NightMode"));
   Serial.println(F("AWB Parameters: SAWB, AAWB"));
   Serial.println(F("AEC Parameters: AveAEC, HistAEC"));  
   Serial.println(F("YUV Matrix Parameters: YUVMatrixOn, YUVMatrixOff"));
   Serial.println(F("Denoise Parameters: DenoiseYes, DenoiseNo"));
   Serial.println(F("Edge Enchancement: EdgeYes, EdgeNo"));
   Serial.println(F("Android Storage Parameters: StoreYes, StoreNo"));
   Serial.println(F("Automatic Black Level Calibration: AblcON, AblcOFF"));
   Serial.println();
   Serial.println();  
}

void  DisplayHelpMenu()
{
    Serial.println(F("................. Help Menu .................."));
    Serial.println(F("d - Display Current Camera Command"));
    Serial.println(F("t - Take Photograph using current Command and Parameters"));
    Serial.println(F("testread - Tests reading files from the SDCard by reading and printig the contents of test.txt"));
    Serial.println(F("testwrite - Tests writing files to SDCard"));
    
    //Serial.println(F("sdinfo - Display SD Card information including files present"));
    Serial.println(F("help camera - Displays Camera's Commands and Parameters"));
    Serial.println();
    Serial.println();
    
}

void DisplayCurrentCommand()
{
     // Print out Command and Parameters
     Serial.println(F("Current Command:"));
     Serial.print(F("Command: "));
     Serial.println(Command);
     
     Serial.print(F("FPSParam: "));
     Serial.println(FPSParam);
     
     Serial.print(F("AWBParam: "));
     Serial.println(AWBParam);
     
     Serial.print(F("AECParam: "));
     Serial.println(AECParam);   
     
     Serial.print(F("YUVMatrixParam: "));
     Serial.println(YUVMatrixParam);   
     
     Serial.print(F("DenoiseParam: "));
     Serial.println(DenoiseParam);   
    
     Serial.print(F("EdgeParam: "));
     Serial.println(EdgeParam);  
    
     Serial.print(F("ABLCParam: "));
     Serial.println(ABLCParam);  
    
     Serial.println();
}

int ParseCommand(const char* commandline, char splitcharacter, String* Result)
{ 
  int ResultIndex = 0;
  int length = strlen(commandline);
  String temp = "";
  
  for (int i = 0; i < length ; i++)
  {
   char tempchar = commandline[i];
   if (tempchar == splitcharacter)
   {
       Result[ResultIndex] += temp;
       ResultIndex++;
       temp = "";
   }
   else
   {
     temp += tempchar;
   } 
  }
  
  // Put in end part of string
  Result[ResultIndex] = temp;
  
  return (ResultIndex + 1);
}
