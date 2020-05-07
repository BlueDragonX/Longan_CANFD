// CAN-FD Shield CAN2.0 Send example

#include <ACAN2517FD.h>
#include <SPI.h>

// FOR CAN-FD SHIELD: https://www.longan-labs.cc/1030012.html
static const byte MCP2517_CS  = 9 ; // CS input of MCP2517
static const byte MCP2517_INT =  2 ; // INT output of MCP2517

// FOR CANBed-FD: https://www.longan-labs.cc/1030009.html
//static const byte MCP2517_CS  = 17 ; // CS input of MCP2517
//static const byte MCP2517_INT =  7 ; // INT output of MCP2517

ACAN2517FD can (MCP2517_CS, SPI, MCP2517_INT) ;


void setup () {
    //--- Start serial
    Serial.begin (115200) ;
    //--- Wait for serial (blink led at 10 Hz during waiting)
    while (!Serial) {
        delay (50) ;
    }
    //----------------------------------- Begin SPI
    SPI.begin () ;
    //--- Configure ACAN2517FD
    Serial.print ("sizeof (ACAN2517FDSettings): ") ;
    Serial.print (sizeof (ACAN2517FDSettings)) ;
    Serial.println (" bytes") ;
    Serial.println ("Configure ACAN2517FD") ;

    ACAN2517FDSettings settings (ACAN2517FDSettings::OSC_20MHz, 500UL * 1000UL, DataBitRateFactor::x1) ;

    settings.mRequestedMode = ACAN2517FDSettings::NormalFD ; // Select loopback mode
    //--- Default values are too high for an Arduino Uno that contains 2048 bytes of RAM: reduce them
    settings.mDriverTransmitFIFOSize    = 1 ;
    settings.mDriverReceiveFIFOSize     = 1 ;
    //--- RAM Usage
    Serial.print ("MCP2517FD RAM Usage: ") ;
    Serial.print (settings.ramUsage ()) ;
    Serial.println (" bytes") ;
    //--- Begin
    const uint32_t errorCode = can.begin (settings, [] { can.isr () ; }) ;
    if (errorCode == 0) {
        Serial.println("CAN INIT OK!");
        Serial.print ("Bit Rate prescaler: ") ;
        Serial.println (settings.mBitRatePrescaler) ;
        Serial.print ("Arbitration Phase segment 1: ") ;
        Serial.println (settings.mArbitrationPhaseSegment1) ;
        Serial.print ("Arbitration Phase segment 2: ") ;
        Serial.println (settings.mArbitrationPhaseSegment2) ;
        Serial.print ("Arbitration SJW:") ;
        Serial.println (settings.mArbitrationSJW) ;
        Serial.print ("Actual Arbitration Bit Rate: ") ;
        Serial.print (settings.actualArbitrationBitRate ()) ;
        Serial.println (" bit/s") ;
        Serial.print ("Exact Arbitration Bit Rate ? ") ;
        Serial.println (settings.exactArbitrationBitRate () ? "yes" : "no") ;
        Serial.print ("Arbitration Sample point: ") ;
        Serial.print (settings.arbitrationSamplePointFromBitStart ()) ;
        Serial.println ("%") ;
    }else{
        Serial.print ("Configuration error 0x") ;
        Serial.println (errorCode, HEX) ;
    }
}

//——————————————————————————————————————————————————————————————————————————————
//   LOOP
//——————————————————————————————————————————————————————————————————————————————

static uint32_t gSendDate = 0 ;
static uint32_t gReceivedFrameCount = 0 ;
static uint32_t gSentFrameCount = 0 ;

CANFDMessage frame ;

void frameInit()
{
    frame.len = 64;
    for(int i=0; i<frame.len; i++)
    {
        frame.data[i] = i;
    }
}

void loop () {
    
    frameInit();

    bool ok = can.tryToSend (frame) ;
    if (ok) 
    {
        Serial.println("Sent OK") ;
    }
    else
    {
        Serial.println("Send failure") ;
    }
    
    delay(100);

}

//——————————————————————————————————————————————————————————————————————————————
