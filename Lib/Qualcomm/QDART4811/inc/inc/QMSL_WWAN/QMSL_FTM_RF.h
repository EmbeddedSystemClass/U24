#if !defined(_QMSL_FTM_RF_H)
#define _QMSL_FTM_RF_H

#if !defined(_WINDOWS_)
#include "windows.h"
#endif

/**
	Establish whether the library is being compiled into a DLL (exporting),
	or being included from a client (importing)

	When the DLL is built, then QLIB_EXPORTS should be defined
*/
#ifdef QLIB_API
#undef QLIB_API
#endif

#ifdef QLIB_EXPORTS
#define QLIB_API __declspec(dllexport)
#else
#define QLIB_API __declspec(dllimport)
#endif

#if defined(QLIB_STATIC)
#undef QLIB_API
#define QLIB_API
//#define QLIB_API __cdecl
#endif


#ifdef __cplusplus
extern "C" {
#endif

  /******************************************************************************
                     Commands for multiple FTM modes
   *******************************************************************************/

   /******************************************************************************/
   /**

   Factory Test Mode WCDMA Commands 3.2.1.9, Set mode

   Factory Test Mode GSM Commands 3.5, Set mode

   Factory Test Mode 1x RF Commands 3.2.11 (CL93-V5419-1)Set mode

   Factory Test Mode RF Commands 3.3.1.10 (CL93-V4168-1) Set mode

   This function sets the RF operating mode of the mobile. It initializes all of the necessary
         hardware to place the mobile in the requested RF mode.



   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  _eNewMode  = specifies the mode that the phone is to enter.
             FTM_RF_Mode_Enum is the definitive list of available modes.

             The following modes are defined by the enumeration, "ftm_mode_id_type" in the embedded software
           file services\ftm\ftm.h.
   \code
        PHONE_MODE_FM        = 1      (FM)
        PHONE_MODE_GPS       = 3      (GPS)
        PHONE_MODE_GPS_SINAD = 4      (GPS SINAD)
        PHONE_MODE_CDMA_800  = 5      (CDMA 800)
        PHONE_MODE_CDMA_1900 = 6      (CDMA 1900)
        PHONE_MODE_CDMA_1800 = 8      (CDMA 1800)
        PHONE_MODE_J_CDMA    = 14     (JCDMA)
        PHONE_MODE_CDMA_450  = 17     (CDMA 450)
        PHONE_MODE_CDMA_IMT  = 19     (CDMA IMT)

        PHONE_MODE_WCDMA_IMT   =9      (WCDMA IMT, Band I)
        PHONE_MODE_GSM_900     =10     (GSM 900)
        PHONE_MODE_GSM_1800    =11     (GSM 1800)
        PHONE_MODE_GSM_1900    =12,    (GSM 1900)
        PHONE_MODE_WCDMA_1900A =15,    (WCDMA 1900 A, Band II Add)
        PHONE_MODE_WCDMA_1900B =16,    (WCDMA 1900 B, Band II Gen)
        PHONE_MODE_GSM_850     =18,    (GSM 850)
        PHONE_MODE_WCDMA_800   =22,    (WCDMA 800, Band V Gen)
        PHONE_MODE_WCDMA_800A  =23,    (WCDMA 800, Band V Add)
        PHONE_MODE_WCDMA_1800  =25,    (WCDMA 1800, Band III)
        PHONE_MODE_WCDMA_BC4   =28,    (WCDMA BC4-used for both Band IV Gen and Band IV Add)
        PHONE_MODE_WCDMA_BC8   =29,    (WCDMA BC8, Band VIII)
        PHONE_MODE_MF_700      =30,    (MediaFLO)
        PHONE_MODE_WCDMA_BC9   =31,    (WCDMA BC9 (1750MHz & 1845MHz), Band IX)
        PHONE_MODE_CDMA_BC15   =32     (CDMA Band Class 15)

      PHONE_MODE_LTE_B1	   =34,	    (LTE Band Class 1)
      PHONE_MODE_LTE_B7     =35,		(LTE Band Class 7)
      PHONE_MODE_LTE_B13    =36, 	(LTE Band Class 13)
      PHONE_MODE_LTE_B17    =37,  	(LTE Band Class 17)
      PHONE_MODE_LTE_B38    =38, 	(LTE Band Class 38)
      PHONE_MODE_LTE_B40    =39, 	(LTE Band Class 40)
      PHONE_MODE_WCDMA_1500=40,		//!<' WCDMA BC11 (1500MHz) Band XI

      PHONE_MODE_LTE_B2=43,
      PHONE_MODE_LTE_B3=44,
      PHONE_MODE_LTE_B5=45,
      PHONE_MODE_LTE_B6=46,
      PHONE_MODE_LTE_B8=47,
      PHONE_MODE_LTE_B9=48,
      PHONE_MODE_LTE_B10=49,
      PHONE_MODE_LTE_B12=50,
      PHONE_MODE_LTE_B14=51,
      PHONE_MODE_LTE_B15=52,
      PHONE_MODE_LTE_B16=53,
      PHONE_MODE_LTE_B18=54,
      PHONE_MODE_LTE_B19=55,
      PHONE_MODE_LTE_B20=56,
      PHONE_MODE_LTE_B21=57,
      PHONE_MODE_LTE_B22=58,
      PHONE_MODE_LTE_B23=59,
      PHONE_MODE_LTE_B24=60,
      PHONE_MODE_LTE_B25=61,
      PHONE_MODE_LTE_B26=62,
      PHONE_MODE_LTE_B27=63,
      PHONE_MODE_LTE_B28=64,
      PHONE_MODE_LTE_B29=65,
      PHONE_MODE_LTE_B30=66,
      PHONE_MODE_LTE_B31=67,
      PHONE_MODE_LTE_B32=68,
      PHONE_MODE_LTE_B33=69,
      PHONE_MODE_LTE_B34=70,
      PHONE_MODE_LTE_B35=71,
      PHONE_MODE_LTE_B36=72,
      PHONE_MODE_LTE_B37=73,
      PHONE_MODE_LTE_B39=74,

      PHONE_MODE_WCDMA_BC19=75,     //!<' WCDMA BC19 (subset of 800MHz) Band XIX
      PHONE_MODE_LTE_B41=76,

       //TDSCDMA reserves 90 - 99
      PHONE_MODE_TDSCDMA_B34=90,
      PHONE_MODE_TDSCDMA_B39=91,
      PHONE_MODE_TDSCDMA_B40=92,
      PHONE_MODE_MAX         =255    (Last possible value, not a valid mode)

   \endcode

   \return true if successful, false if fail.

   \warning Must be called before any other FTM RF commands.
   \warning,  If the new mode is a GSM RF mode, a delay will be issued after the command
   is successfully sent to the mobile.  The duration of the delay is based on the
   value of delay type "QMSL_Timeout_Delay_GSM_RF_Mode" which can be controled by
   QLIB_ConfigureTimeOut() and read by QLIB_GetTimeOut()

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_MODE( HANDLE hResourceContext, short eNewMode );

   /******************************************************************************/
   /**

   This function sets the RF sub block mask of the mobile , for the RF mode set in the QLIB_FTM_SET_MODE()
   command

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  _eIndex  = Specifies the block NV or parent NV to be written to.

   \code
      _eIndex = -1 ( Writing to the Parent Band Class NV )
     _eIndex = 0  ( Pointing to the Band Class and Block mask corresponding to the Block 0 NV )
     _eIndex = 1  (  Pointing to the Band Class and Block mask corresponding to the Block 1 NV )

   \endcode
   \param  _eBlockMask  = Specifies the block mask to be set for the RF mode set in QLIB_FTM_SET_MODE().
   The block mask should be a mask where Block A = bit 0 , Block B = bit 1 and so on.

   \return true if successful, false if fail.

   \warning QLIB_FTM_SET_MODE should be called before this command.
   \warning This command should be called before any other RF commands.


   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_BLOCK_MASK( HANDLE hResourceContext, short eIndex , unsigned long eBlockMask );

   /******************************************************************************/
   /**

   This function returns the command mode (59, 75 or -1 (invalid));

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iCommandMode     = Detected command mode.  The value can be 59 (used in MSM6000/6025/6050),
                                          75, or -1 (invalid)

   \return true if successful, false if fail.

   *******************************************************************************/


   QLIB_API unsigned char QLIB_FTM_DETECT_COMMAND_CODE( HANDLE hResourceContext, short* iCommandMode );

   /******************************************************************************/
   /**

   This function sets the command code (59 or 75);

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param eCommandMode	    = Command code (59 or 75)

   Note: The library uses command code 75 by default. MSM6000/6025/6050 uses commmand code 59.
   For MSM6000/6025/6050, this function should be called prior to other FTM functions

   \return true if successful, false otherwise

   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SET_COMMAND_CODE( HANDLE hResourceContext, short eCommandCode );

   /******************************************************************************/
   /**

   This function gets the command code (59 or 75) used by the handle

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param eCommandMode	    = Command code (59 or 75)

   \return true if successful, false otherwise

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_COMMAND_CODE( HANDLE hResourceContext, short* eCommandCode );

   /******************************************************************************/
   /**

   This function sets the system mode ID used by subsequent FTM API calls.
   It does not send diagnostic command to the mobile

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param eModeId = Mode ID

   The Mode IDs are from the FTM_Mode_Id_Enum in QLib_Defines.h
   \code
   FTM_MODE_ID_CDMA_1X     = 0,		//!<' RF CDMA 1X mode - RX0
   FTM_MODE_ID_WCDMA       = 1,		//!<' RF WCDMA mode
   FTM_MODE_ID_GSM         = 2,		//!<' RF GSM Mode
   FTM_MODE_ID_CDMA_1X_RX1 = 3,		//!<' RF CDMA 1X mode - RX1
   FTM_MODE_ID_BLUETOOTH   = 4,		//!<' Bluetooth
   FTM_MODE_ID_CDMA_1X_CALL= 7,		//!<' CALL CDMA 1X mode
   FTM_MODE_ID_LOGGING     = 9,		//!<' FTM Logging
   FTM_MODE_ID_AGPS        = 10,		//!<' Async GPS
   FTM_MODE_ID_PMIC        = 11,		//!<' PMIC FTM Command
   FTM_MODE_GSM_BER        = 13,		//!<' GSM BER
   FTM_MODE_ID_AUDIO       = 14,		//!<' Audio FTM
   FTM_MODE_ID_CAMERA      = 15,		//!<' Camera
   FTM_MODE_WCDMA_BER      = 16,		//!<' WCDMA BER
   FTM_MODE_ID_GSM_EXTENDED_C = 17,	//!<' GSM Extended commands
   FTM_MODE_CDMA_API_V2    = 18,		//!<' CDMA RF Cal API v2
   FTM_MODE_ID_MF_C        = 19,		//!<' MediaFLO
   FTM_MODE_RF_COMMON      = 20,		//!<' RF Common
   FTM_MODE_WCDMA_RX1      = 21,		//!<' WCDMA Diversity Rx (RX1)
   FTM_MODE_CDMA_C2        = 32,		//!<' CDMA C2
   FTM_MODE_CDMA_C3        = 40,		//!<' CDMA C3
   FTM_MODE_CDMA_C4        = 45,		//!<' CDMA C4
   FTM_MODE_ID_PRODUCTION  = 0x8000,	//!<' Production FTM
   FTM_MODE_ID_LTM         = 0x8001	//!<' LTM
   \endcode
   Note: This function sets

   \return true if successful, false otherwise

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SET_MODE_ID( HANDLE hResourceContext, short eModeId );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.1 Set PDM

   Factory Test Mode 1x RF Commands 3.2.2 (CL93-V5419-1)Set mode

   Factory Test Mode RF Commands 3.3.1.1 (CL93-V4168-1) Set mode

   This command sets the value of the PDM, based on PDM_ID.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iPDMtype  = represents the PDM to be set.
      \code
            WCDMA mode values
              2 - Tx AGC Adjust PDM
              4 - Trk Lo Adjust PDM

            GSM mode values
              0 - Trk Lo Adjust PDM

            CDMA mode values
              2 - Tx AGC Adjust PDM
              4 - Trk Lo Adjust PDM
      \endcode

   \param	iPDMvalue =
      \code
            For WCDMA mode Tx AGC Adjust PDM:
              PDM values from 0 to 511
                 0   - lowest AGC voltage
                 511 - highest AGC voltage

            For GSM mode values:
               PDM values from 0 to 4096

            For CDMA mode values:
               PDM values from 0 to 511
               0   - highest AGC voltage
               511 - lowest AGC voltage

            For FM mode values:
               PDM values from 0 to 511
               0   - lowest AGC voltage
              511 - highest AGC voltage
      \endcode

   \return true if successful, false if fail.

   \note This function can be used in QMSL FTM Sequencer operation
   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SET_PDM
   ( HANDLE hResourceContext, unsigned short iPDMtype, unsigned short iPDMvalue );

   /******************************************************************************/
   /**
   This function is the same as QLIB_FTM_SET_PDM(), except the iPDMvalue is signed
   for this version of the function.  This is applicable to the GSM VCTCXO PDM control.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iPDMtype  = represents the PDM to be set.
      \code
            WCDMA mode values
              2 - Tx AGC Adjust PDM
              4 - Trk Lo Adjust PDM

            GSM mode values
              0 - Trk Lo Adjust PDM

            CDMA mode values
              2 - Tx AGC Adjust PDM
      \endcode

   \param	iPDMvalue =
      \code
            For WCDMA mode Tx AGC Adjust PDM:
              PDM values from 0 to 511
                 0   - lowest AGC voltage
                 511 - highest AGC voltage

            For GSM mode values, the range depends upon the embedded RF software.  The typical ranges are be
                    PDM values from -4096 to 4095  or  PDM values from -2048 to 2047

            For CDMA mode values:
               PDM values from 0 to 511
               0   - highest AGC voltage
               511 - lowest AGC voltage

            For FM mode values:
               PDM values from 0 to 511
               0   - lowest AGC voltage
              511 - highest AGC voltage
      \endcode

   \return true if successful, false if fail.

   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SET_PDM_signed
   ( HANDLE hResourceContext, unsigned short iPDMtype, short iPDMvalue );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands x.x.x.x

   Set SMPS PA Bias override mode

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param iOnOff = Override mode on/off

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_SMPS_PA_BIAS_OVERRIDE
   ( HANDLE hResourceContext, unsigned char iOnOff );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands x.x.x.x

   Set SMPS PA Bias value.  QLIB_SET_SET_SMPS_PA_BIAS_OVERRIDE (on) should be called first

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param iPA_Bias_Value = Value of the PA Bias

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_SMPS_PA_BIAS_VAL
   ( HANDLE hResourceContext, unsigned short iPA_Bias_Value);


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.12 Set TX on

   Factory Test Mode GSM Commands 3.3 Set Tx ON

   Factory Test Mode 1x RF Commands 3.2.14 (CL93-V5419-1)Set Tx on

   Factory Test Mode RF Commands 3.3.1.13 (CL93-V4168-1) Set Tx on

      CDMA:  This command turns on the phone.s transmit chain (including the PA) and causes a CDMA
      reverse-link waveform to be generated on the current channel. All necessary encoder hardware
      initialization for transmitting is handled by this function.

      WCDMA: This command turns on the phoneҳ transmit chain (including the PA) and causes a WCDMA
      uplink waveform or CW to be generated on the current channel. All necessary encoder hardware
      initialization for transmitting is handled by this function.


      GSM:This command turns on the phoneҳ transmit chain (including the PA) and causes a GSM uplink
      burst to be generated on the current channel. The data that is transmitted and other parameters of
      transmission are set up by the Set Transmit Burst API (see Section 3.2.1.10).

      Hence, the Set Transmit Burst API must be called at least once after a mode change (to one
      of the GSM modes) and prior to the execution of the Set TX ON API.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \warning GSM: Set Transmit Burst must be setup before calling this function.


   \return true if successful, false if fail.

   \note This function can be used in QMSL FTM Sequencer operation
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_TX_ON( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.11 Set TX off

   Factory Test Mode GSM Commands 3.4 Set Tx OFF

   Factory Test Mode 1x RF Commands 3.2.13 (CL93-V5419-1)Set Tx off

   Factory Test Mode RF Commands 3.3.1.12 (CL93-V4168-1) Set Tx off

   This command turns off the phoneҳ transmit chain.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_TX_OFF( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.8 Set LNA range

   Factory Test Mode GSM Commands 3.7 Set LNA range

   Factory Test Mode 1x RF Commands 3.2.10 (CL93-V5419-1)Set LNA range

   Factory Test Mode RF Commands 3.3.1.9 (CL93-V4168-1) Set LNA range

   This command sets the LNA range state machine to the specified state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iLNArange =

            CDMA: the binary state of the LNA range state machine:
                     0 - LNA_R0 - highest gain,
                     1 - LNA_R1,
                     2 - LNA_R2,
                     3 - LNA_R3
                     4 - LNA_R4 - lowest gain

            The number of possible gain states varies in different RF chipsets
            Please refer to relevant NV documents for the number of possible gain states.

            WCDMA: the binary state of the LNA range state machine:
                     0 - LNA_R0 - highest gain,
                     1 - LNA_R1,
                     2 - LNA_R2,
                     3 - LNA_R3 - lowest gain

            GSM: iLNArange = desired LNA range (0-3), 0 is highest gain, 3 is lowest gain

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_LNA_RANGE( HANDLE hResourceContext, unsigned char iLNArange);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.6 Set RF channel

   Factory Test Mode GSM Commands 3.6 Set channel

   Factory Test Mode 1x RF Commands 3.2.7 (CL93-V5419-1)Set channel

   Factory Test Mode RF Commands 3.3.1.6 (CL93-V4168-1) Set channel

    CDMA/WCDMA: This command tunes the RF synthesizers to the frequency defined by the channel parameter,
         which is based on the band class from the Set Mode command. Set Mode must be called first.

         GSM: This command tunes the RF synthesizers to the frequency defined by the ARFCN parameter,
         which is based on the band class from the Set Mode command. Set mode must be called first.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iChannel =
      \code
           CDMA: desired channel that is used
               PHONE_MODE_CDMA_800 : 1-799, 991-1023
               PHONE_MODE_CDMA_1900: 0 - 1199
               PHONE_MODE_CDMA_1800: 0 - 599
               PHONE_MODE_J_CDMA:    1-799, 801-1039, 1041-1199, 1201-1600

           WCDMA: desired channel that is used

           GSM: desired ARFCN that is used:
               PHONE_MODE_GSM_900 : 128 to 251 for GSM 850 MHz,
               PHONE_MODE_GSM_1900 : 0 to 124 and 975 to 1023 for GSM 900 MHz,
               PHONE_MODE_GSM_1800 : 512 to 885 for GSM 1800 MHz,
               PHONE_MODE_GSM_850 : 512 to 810 for GSM 1900 MHz
      \endcode
   \return true if successful, false if fail.

   \warning Set Mode must be called before this.  This function must be called before
          any Tx or Rx specific functions.

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_CHAN( HANDLE hResourceContext, unsigned short iChannel);

   /******************************************************************************/
   /**
        Factory Test Mode WCDMA Commands

        This command is used to set multiple carrier channels in multi carrier operation

        \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

        \param iChannel0 = desired first channel

        \param iChannel1 = desired second channel

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_MULTI_CHAN( HANDLE hResourceContext, unsigned short iChannel0,
         unsigned short iChannel1 );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.2 Get ADC

   Factory Test Mode 1x RF Commands 3.2.3 (CL93-V5419-1) Get ADC

   Factory Test Mode RF Commands 3.3.1.2 (CL93-V4168-1)  Get ADC

   This command returns the value of the specified analog-to-digital converter (ADC) channels.

   \param  iADC_type  = the ADC channel to be read
   \param  iADC_value = ADC value of the desired channel

   \return true if successful, false if fail.

   \note This function can be used in QMSL FTM Sequencer operation
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_ADC_VAL
   ( HANDLE hResourceContext, unsigned short iADC_type, unsigned short* piADC_value );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.3 Get RX AGC

   Factory Test Mode 1x RF Commands 3.2.4 (CL93-V5419-1) Get Rx AGC

   Factory Test Mode RF Commands 3.3.1.3 (CL93-V4168-1)  Get Rx AGC

   This command returns the 10-bit twos complement output of the RX AGC loop.

   NOTE:  This number is normally represented as a signed number.  In this
          case, the sign is on the 10th bit, so if a signed number is
         desired, the user of this function will have to perform the
         conversion to a 16-bit signed value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iRX_AGC = For CDMA/WCDMA mode, AGC return value is from -512 to 511

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_RX_AGC( HANDLE hResourceContext, unsigned short* piRX_AGC );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands

   This command returns the 10-bit twos complement output of the RX AGC loop.
   The output corresponds to carrier 1 in a dual carrier setup.

   NOTE:  This number is normally represented as a signed number.  In this
          case, the sign is on the 10th bit, so if a signed number is
          desired, the user of this function will have to perform the
          conversion to a 16-bit signed value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  iRX_AGC = For CDMA/WCDMA mode, AGC return value is from -512 to 511

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_CAGC_DUAL_CARRIER_RX_AGC( HANDLE hResourceContext, short* piRX_AGC );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.4 Get synthesizer lock state

   Factory Test Mode 1x RF Commands 3.2.5 (CL93-V5419-1) Get synthesizer lock state

   Factory Test Mode RF Commands 3.3.1.4 (CL93-V4168-1)  Get synthesizer lock state

   This command returns the combined (ANDed) lock state of the RF synthesizers.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  bLockState = 0 = if one or more is out of lock
                         1 = if the synthesizers are all in lock

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_SYNTH_STATE( HANDLE hResourceContext, unsigned char* pbLockState);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.5 Set uplink waveform to CW

   Factory Test Mode 1x RF Commands 3.2.6 (CL93-V5419-1) Set  CDMA wave CW

   Factory Test Mode RF Commands 3.3.1.5 (CL93-V4168-1)  Set  CDMA wave CW


      This command turns off the spreading on the uplink in WCDMA mode. The TX chain and all
      clocks are active, but there is no modulating signal applied to the carrier; therefore a CW is
      generated.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  bSelectCW = 0 - OFF/LO/FALSE deselect CW
                  1 - ON/HI/TRUE select CW

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_CDMA_CW_WAVEFORM( HANDLE hResourceContext, unsigned char bSelectCW);

   /******************************************************************************/
   /**
   !! Document Reference TBD


      This command sets an offset tone at the given frequency offset in WCDMA mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iOffsetFreq = The offset frequency at which to generate the tone in Hz.

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_OFFSET_CW( HANDLE hResourceContext, int iOffsetFreq);


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.7 Set LNA offset

   Factory Test Mode 1x RF Commands 3.2.9 (CL93-V5419-1) Set LNA offset

   Factory Test Mode RF Commands 3.3.1.8 (CL93-V4168-1)  Set LNA offset

   This command sets the LNA offset for a given index value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLNAindex  = LNA offset index ID (0 is highest gain)

   The number of possible LNA offset varies with RF chipsets
         Please refer to applicable RF NV documents for the number of possible gain state

   \param iLNAoffset = LNA offset value in 1/12 dB resolution for the index ID

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_LNA_OFFSET
   ( HANDLE hResourceContext, unsigned short iLNAindex, unsigned short iLNAoffset);


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.10 Set PA range

   Factory Test Mode 1x RF Commands 3.2.12 (CL93-V5419-1) Set PA range

   Factory Test Mode RF Commands 3.3.1.11 (CL93-V4168-1)  Set PA range

   This command sets the PA range state machine to the specified state.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iPArange = the binary state of the PA range state:
   \code
       0 - R0 = 0, R1 = 0,
       1 - R0 = 1, R1 = 0,
       2 - R0 = 0, R1 = 1,
       3 - R0 = 1, R1 = 1
   \endcode
   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_PA_RANGE( HANDLE hResourceContext, unsigned short iPArange );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.13 Get WCDMA IM2

   Factory Test Mode 1x RF Commands 3.2.15 (CL93-V5419-1) Get CDMA IM2

   Factory Test Mode RF Commands (cdma2000) 3.3.1.14 (CL93-V4168-1)  Get CDMA IM2

   FTM GSM RF Commands 3.8(CL93-V5370-1)  Get CDMA IM2

         This command returns the optimum IM2 value for the current mode and channel of the mobile.
         RX signal inputs are required to be set before issuing this command or the results will not be
         accurate.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iI_Value = The I value
   \param  iQ_Value = The Q value
   \param  iTransConductance_Value = The transconductance value.

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_CDMA_IM2
   (
      HANDLE hResourceContext, unsigned char* piI_Value, unsigned char* piQ_Value,
      unsigned char* piTransConductance_Value
   );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.14 Set DVGA offset

   Factory Test Mode 1x RF Commands 3.2.16 (CL93-V5419-1) Set DVGA offset

   Factory Test Mode RF Commands 3.3.1.15 (CL93-V4168-1)  Set DVGA offset

   This function sets the DVGA offset register for WCDMA mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iDVGAoffset = offset value, in 1/120ths of dB

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_DVGA_OFFSET( HANDLE hResourceContext, unsigned short iDVGAoffset );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands (FTM document details)

   This function sets the carrier 1 DVGA offset register for dual carrier WCDMA mode.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iDVGAoffset = offset value, in 1/10th dB resolution

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_DUAL_CARRIER_DVGA_OFFSET( HANDLE hResourceContext, short iDVGAoffset );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.15 TX calibration sweep

   Factory Test Mode 1x RF Commands 3.2.18 (CL93-V5419-1) Tx Sweep Cal

   Factory Test Mode RF Commands 3.3.1.17 (CL93-V4168-1)  Tx Sweep Cal

   This command sets up one or more complete TX sweeps from high power to low power with
   a predefined TX adjust PDM step size. Figure 2 illustrates one TX sweep.
   The start power is determined by the last TX adjust PDM value set by the user before the
   sweep. To prevent users from setting the TX power too high, the start power is also limited by
   TX_SWEEP_CAL_MAX.

   NOTE By default, TX_SWEEP_CAL_MAX is 450, while TX_SWEEP_CAL_MIN is 100. The
   TX_SWEEP_CAL_ STEP is set to 10.

   The stop power is governed by TX_SWEEP_CAL_MIN. The current implementation has two full
   sweeps to ensure a spectrum analyzer will catch at least one full sweep with the video trigger
   option. It could be reduced to one sweep for an automated test.

   This command sets up one or more complete TX sweeps from high power to low power with
         a predefined TX adjust PDM step size.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  bOnOff = 0 - no effect, 1 - to trigger TX calibration sweep

   \return true if successful, false if fail.
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_TX_SWEEP_CAL( HANDLE hResourceContext, unsigned char bOnOff );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.16 Get DVGA offset

   Factory Test Mode 1x RF Commands 3.2.18 (CL93-V5419-1) Get DVGA offset

   Factory Test Mode RF Commands 3.3.1.17 (CL93-V4168-1)  Get DVGA offset


   For MSM6000/6025/6050 targets, iExpectedRX_AGC is ignored.

   For MSM6100 (DMSS version: ??), iExpectedRX_AGC is ignored.
   For	MSM6300 (DMSS version: ??), iExpectedRX_AGC is ignored.
   For	MSM6500 (DMSS version: ??), iExpectedRX_AGC is ignored.

   For MSM6550 (DMSS version: ??), iExpectedRX_AGC is used.
   For MSM6500 (DMSS version: ??), iExpectedRX_AGC is used.

   For MSM6200 targets, this command assumes the user is inputting a mobile RX signal at the
   level equivalent to the NV item WCDMA_LNA_RANGE_FALL_I (all external losses, including
   fixture must be accounted for). The return value is in 1/12 dB resolution.

   For MSM6250 phase II targets, this command takes DVGA gain offset as an input instead of
   reading RFNV WCDMA_LNA_RANGE_FALL_I value. The input value needs to be converted from
   dBm to an RX AGC value.

   This command calibrates and returns the DVGA offset for the current channel of operation.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iExpectedRX_AGC = power level in RX AGC units, at the input to the phone
   \param  iDVGAoffset = return value of the measured DVGA offset

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_DVGA_OFFSET( HANDLE hResourceContext, short iExpectedRX_AGC, short* piDVGAoffset );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands (FTM document details)

   This command calibrates and returns the WCDMA dual carrier DVGA offset for the current channels of operation.
        The offset returned corresponds to carrier 1 in a dual carrier setup.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iExpectedRX_AGC = power level in RX AGC units, at the input to the phone
   \param  iDVGAoffset = return value of the measured DVGA offset

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_DUAL_CARRIER_DVGA_OFFSET( HANDLE hResourceContext, short iExpectedRX_AGC, short* piDVGAoffset );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 3.2.1.17 Get LNA offset

   Factory Test Mode 1x RF Commands 3.2.20 (CL93-V5419-1) Get LNA offset

   Factory Test Mode RF Commands 3.3.1.19 (CL93-V4168-1)  Get LNA offset


      This command calculates and returns the LNA offset value for the input ID parameter. The
      function assumes the user has set up the RX signal into the mobile at an appropriate level. This
      function controls the AGC state for the purpose of calculating the LNA offset of interest. This
      function was designed to be called with the following sequence in mind:

      1. Set the power level into the mobile RX appropriate for id=1.
      2. Call the function Get LNA Offset with id=1.
      3. Set the power level into the mobile RX appropriate for id=2.
      4. Call the function Get LNA Offset with id=2.
      5. Set the power level into the mobile RX appropriate for id=3.
      6. Call the function Get LNA Offset with id=3.

      NOTE Since the DVGA dynamic range is limited, all indexes feed on the previous index being
      calculated (with the exception of id=1). If the above sequence is not followed, the returned
      values may be meaningless.

   This command calculates and returns the LNA offset value for the input ID parameter.

   For all MSM6000/6025/6050 targets, iExpectedRX_AGC is ignored.

   For MSM6100 (DMSS version: ??), iExpectedRX_AGC is ignored.
   For	MSM6300 (DMSS version: ??), iExpectedRX_AGC is ignored.
   For	MSM6500 (DMSS version: ??), iExpectedRX_AGC is ignored.

   For MSM6550 (DMSS version: ??), iExpectedRX_AGC is used.
   For MSM6500 (DMSS version: ??), iExpectedRX_AGC is used.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iLNAindex = LNA offset index, 1-3, zero is the highest gain state, but is not a
                  valid option for this function.
   \param  iExpectedRX_AGC = power level in RX AGC units, at the input to the phone
   \param iLNAoffsetValue = measured value of the LNA offset, in 1/12dB resolution

   \return true if successful, false if fail.

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_LNA_OFFSET
   ( HANDLE hResourceContext, unsigned short iLNAindex , short iExpectedRX_AGC, short* piLNAoffsetValue );

   /******************************************************************************/
   /**
   \code
   Factory Test Mode WCDMA Commands 3.2.2 Get HDET from Tx Sweep Cal
   Factory Test Mode RF Commands 3.3.1.20 (CL93-V4168-1) Get HDET from Tx Sweep Cal
   \endcode

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

   This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iIndex = HDET array index, the 0 index corresponds to the first step
                of the TX Sweep.

   \param  iHDETvalue = value of the HDET at the iIndex position

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_HDET_FROM_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned short iIndex, unsigned short* piHDETvalue );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 4.19 Get HDET from Tx Sweep Cal

   Factory Test Mode 1x RF Commands 3.2.26 (CL93-V5419-1) Get ALL HDET from Tx Sweep Cal

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

      This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  piHDETvalues = Array of 32 (defined by QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL_ARRAY_SIZE) bytes,
                     to store the average HDET for the first 32 steps of the Tx Sweep

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned char * piHDETvalues );
   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 4.19 Get HDET from Tx Sweep Cal

   Factory Test Mode 1x RF Commands 3.2.26 (CL93-V5419-1) Get ALL HDET from Tx Sweep Cal

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

      This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  piHDETvalues = Array of 32 (defined by QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL_ARRAY_SIZE) bytes,
                     to store the average HDET for the first 32 steps of the Tx Sweep

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_LTE_GET_ALL_HDET_FROM_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned short* piHDETvalues );

   /******************************************************************************/
   /**
   \code
   Factory Test Mode WCDMA Commands 3.2.2 Get HDET from Tx Sweep Cal
   Factory Test Mode RF Commands 3.3.1.20 (CL93-V4168-1) Get HDET from Tx Sweep Cal
   \endcode

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

   This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iIndex = HDET array index, the 0 index corresponds to the first step
                of the TX Sweep.

   \param  iHDETvalue = value of the HDET at the iIndex position

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_LPM_HDET_FROM_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned short iIndex, unsigned short* piHDETvalue );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 4.19 Get HDET from Tx Sweep Cal

   Factory Test Mode 1x RF Commands 3.2.26 (CL93-V5419-1) Get ALL HDET from Tx Sweep Cal

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

      This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  piHDETvalues = Array of 32 (defined by QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL_ARRAY_SIZE) bytes,
                     to store the average HDET for the first 32 steps of the Tx Sweep

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_ALL_LPM_HDET_FROM_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned short * piHDETvalues );

   /******************************************************************************/
   /**
   This command enables the LPM HDET.

    \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  ienableHdetLPM = Enable\Disable LPM HDET

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_ENABLE_LPM_HDET( HANDLE hResourceContext, unsigned char ienableHdetLPM  );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 4.19 Get HDET from Tx Sweep Cal (V2)

   Factory Test Mode 1x RF Commands 3.2.26 (CL93-V5419-1) Get ALL HDET from Tx Sweep Cal

      While the TX is ON, data is collected after the complete execution of the TX Sweep CAL subcommand.
      Each entry is bound to a tx_agc_pdm value taken during TX Sweep CAL. If the user wishes to read
      HDET values unrelated to TX Sweep CAL, the FTM_GET_ADC_VAL API should be used.

      This command returns a value from the HDET TX Sweep Cal array.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  piHDETvalues = Array of 32 (defined by QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL_ARRAY_SIZE) word,
                     to store the average HDET for the first 32 steps of the Tx Sweep

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_ALL_HDET_FROM_TX_SWEEP_CAL_V2
   ( HANDLE hResourceContext, unsigned short * piHDETvalues );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands Switch Tx Antenna

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

       \param tx_antenna The Tx Antenna to switch to 0 indicates Primary Tx Antenna and 1 indicates Diversityy Tx Antenna

   \return true if successful, false if fail.

     *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SWITCH_TX_ANT
   (HANDLE hResourceContext, unsigned short tx_antenna);


 
   /******************************************************************************/
   /**
   Factory Test Mode 1x RF Commands 3.2.28 (CL93-V5419-1) Set RF Power Mode

      This command overrides the automatic IntelliCeiver Power mode decision.
      This setting will be lost when changing the channel with the FTM_SET_CHAN API,
      or when the phone is returned to Online mode.

   \param  iMode = Power mode to force the IntelliCeiver to:
               0 = High Power mode
               1 = Mid Power mode
               2 = Low Power mode

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_RF_POWER_MODE( HANDLE hResourceContext, unsigned char iMode );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands 4.20 Set HDET Tracking

      This command enables/disables the HDET tracking. When the HDET tracking is enabled, the
      software will periodically read the HDET value and adjust the uplink power to meet a desired
      level based on a given HDET value.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iOnOff =
      \code
           0 = disable HDET tracking
           1 = enable HDET tracking
      \endcode

   \param  iHDETvalue = desired HDET value for setpoint

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_HDET_TRACKING
   ( HANDLE hResourceContext, unsigned short iOnOff, unsigned short iHDETvalue );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, 4.21 Configure Tx Calibration Sweep

   Factory Test Mode CDMA Commands, 3.4.17  Set Tx Cal Sweep

   This command sets the time interval of each step in the Tx calibration sweep
   and the number of HDET readings averaged per step.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param  iTimeInterval = For CDMA radio, the unit is expressed in terms of ms
                     The value of 20 correponds to 20ms

                     For WCDMA radio, the unit is expressed in terms on 1/32.768 kHz
                     The value of 655 corresponds to 20 ms

                     1/32.768 kHz = 0.0305 ms per count.
                     The minimum value is 66, which is about 2 ms. Any number less
                     than 66 will be forced to 66.

                            The time interval is how long the step is (in time).  We found that a 2ms
                     step time is ideal and anything below that is not repeatable.  We also
                     tested the CMU-200's and the FSP measurement capabilities for 2ms and
                     found them to be acceptable.  For these reasons, we suggest a value of
                     66 (2ms) to be used for the time interval.



   \param  iNumofHDETReadingsPerStep= Number of HDET reads for averaging (from 1 to 255)
                                       During the top 32 power steps, the HDET will be measured.
                              During the measurement of the HDET, each value is averaged
                              with other HDET readings at the same power level.   This
                              setting controls how many HDET averages are taken during
                              each power step.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_CONFIGURE_TX_SWEEP_CAL
   ( HANDLE hResourceContext, unsigned short iTimeInterval, unsigned short iNumofHDETReadingsPerStep);



   /******************************************************************************/
   /**
   Factory Test Mode CDMA Commands, 2.4.32(CL93-V5419-1) FTM SET TX POWER DBM

   Factory Test Mode LTE Commands, 2.1.13(80-VR832-1) FTM SET TX POWER DBM

   Factory Test Mode WCDMA Commands, 2.x.xx (CL93-V5368-1) FTM SET TX POWER DBM

   FTM_SET_TX_POWER_DBM API programs the PA range, LUT Index and Digital Gain to reach the
   programmed Tx Powe level with power limiting enabled.
   The PA range will be selected depending upon the current TXAGC value and by using the PA RISE/FALL
   values in NV. If the requested power is within a PA Rise/Fall hysteresis range then the current
   power level will be used to determine whether the power is rising or falling.

   This API has the following runtime dependencies.
   *	QLIB_FTM_SET_MODE
   *	QLIB_FTM_SET_CHAN
   *	QLIB_FTM_SET_TX_ON

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \param iEnable =
         0 ==> Disable
         1 ==> Enable

   \param iPowerLevel = TXAGC power to be set.
                        Power level in 1/10dBm units, as a signed number

   \param iFTM_Error_Code = FTM Error code returned by FTM command
         *	0 ֠Success
         *	1 ֠Failure occurred

   \return true if successful, false if fail.

   \warning 2012.09: this function is not officially released in AMSS MSM8960 equivalents. 
                     It will only be available in AMSS MSM8974 equivalents according to the 
                     Factory Test Roadmap document 80-NC791-1

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_TX_POWER_DBM( HANDLE hResourceContext, unsigned char iEnable, short iPowerLevel, unsigned short* iFTM_Error_Code);


   /******************************************************************************/
   /**

   Factory Test Mode CDMA Commands, (CL93-V5419-1) FTM SET PA STATE

   Factory Test Mode LTE Commands, (80-VR832-1) FTM SET PA STATE

   Factory Test Mode WCDMA Commands, (CL93-V5368-1) FTM SET PA STATE

   This command programs the PA state. Note that the PA state is the low level hardware control indicator,
   which is different from the PA range software abstraction.

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \param iPA_State = Indicates the PA State to be selected

   \param iFTM_Error_Code = FTM Error code returned by FTM command
         *	0 ֠Success
         *	1 ֠Failure occurred

   \return true if successful, false if fail.

   \warning 2012.09: this function is not officially released in AMSS MSM8960 equivalents. 
                     It will only be available in AMSS MSM8974 equivalents according to the 
                     Factory Test Roadmap document 80-NC791-1

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_PA_STATE( HANDLE hResourceContext, unsigned char iPA_State, unsigned short* iFTM_Error_Code);

   /******************************************************************************/
   /**
   Factory Test Mode RF Commands x.x.x.x (CL93-V4168-1) QLIB_FTM_SET_TX_WAVEFORM

   This command is used to generate an Tx waveform of given type with specified attributes.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iWaveformType = 0 - CDMA_WAVEFORM
                     1 - CW_WAVEFORM

   \param  iWaveformAttrib = 0 - CW_OFFSET_ZER0_KHZ
                       1 - CW_OFFSET_19_2_KHZ

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_TX_WAVEFORM( HANDLE hResourceContext, unsigned char iWaveformType , unsigned char iWaveformAttrib );

   /******************************************************************************/
   /**

   Factory Test Mode CDMA Commands, (CL93-V5419-1) FTM RF GET RX LEVEL DBM

   Factory Test Mode LTE Commands, (80-VR832-1) FTM RF GET RX LEVEL DBM

   Factory Test Mode WCDMA Commands, (CL93-V5368-1) FTM RF GET RX LEVEL DBM

   This command returns the Rx level, for a specified receiver, in both AGC and dBm units.
   This command is only valid when the Rx verification state has been enabled for a specified receiver
   using FTM_RF_SET_RX_VERIFICATION_STATE.
   Accessing different receivers is handled by changes in the RF MODE ID field.

   This API has the following runtime dependencies.

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \param iRX_AGC = RX AGC value (Valid only if  enable_rx_closed_loop is 1.)

   \param iRX_Power_dBm = Rx power in 1/10 dBm units; this is a signed number
                         (Valid only if  enable_rx_closed_loop is 1.)

   \param iFTM_Error_Code = FTM Error code returned by FTM command
         *	0 ֠Success
         *	1 ֠Failure occurred

   \return true if successful, false if fail.

   \warning 2012.09: this function is not officially released in AMSS MSM8960 equivalents. 
                     It will only be available in AMSS MSM8974 equivalents according to the 
                     Factory Test Roadmap document 80-NC791-1

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_RX_LEVEL_DBM( HANDLE hResourceContext ,
         unsigned short* iRX_AGC,
         unsigned short* iRX_Power_dBm,
         unsigned short* iFTM_Error_Code);

 

   /******************************************************************************/
   /**

   Factory Test Mode CDMA Commands, (CL93-V5419-1) FTM SET FREQ ADJUST

   Factory Test Mode LTE Commands, (80-VR832-1) FTM SET FREQ ADJUST

   Factory Test Mode WCDMA Commands, (CL93-V5368-1) FTM SET FREQ ADJUST

   Adjust the frequency of the mobile Tx and Rx (Only Tx for WCDMA). This applies only to 
   XO systems starting with the NikeL modem platform. The default frequency adjustment is zero. 
   Each specified adjustment clears the previous adjustment value before setting the new value. 

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()
   \param User_Freq_Adjust_ppb = User defined frequency adjustment in ppb units

   \param Centre_Freq_Hz = Tx carrier frequency for current cellular band/channel, in Hzҳ units
                           Center frequency is either Rx or Tx, depending on where the reference 
                           measurement was made on

   \param Enable_XO_Freq_Adjust = 
         *	0 - Total Frequency Adjust = User defined frequency adjustment (i.e. user_freq_adjust_ppb)
         *	1 ֠Total Frequency Adjust = User defined frequency adjustment (i.e. user_freq_adjust_ppb) 
                                         + Frequency error reported by XO manager. 
   
   \param Total_Freq_Adjust_Hz = Total Frequency Adjust applied in hz units. This value is only 
                                 used in the response packet. It should be set to 0 in the request 
                                 packet.

   \param iFTM_Error_Code = FTM Error code returned by FTM command
         *	0 ֠Success
         *	1 ֠Failure occurred

   \return true if successful, false if fail.

   \warning 2012.09: this function is not officially released in AMSS MSM8960 equivalents. 
                     It will only be available in AMSS MSM8974 equivalents according to the 
                     Factory Test Roadmap document 80-NC791-1

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_FREQ_ADJUST( HANDLE hResourceContext, 
                                                    long User_Freq_Adjust_ppb,
                                                    long Centre_Freq_Hz,
                                                    char Enable_XO_Freq_Adjust,
                                                    long* Total_Freq_Adjust_Hz,
                                                    unsigned short* iFTM_Error_Code);   
   /******************************************************************************/
   /**

   Factory Test Mode LTE Commands, (80-VR832-1) FTM SET FREQ ADJUST

   This command is specific to LTE TDD mode of operation. It serves:
   1) Sets the TDD Uplink-Downlink configurations as per 3GPP TS3.6.211 Table 4.2-2
   2) Specifies if the UE has to transmit on all subframes within a frame.

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \param Sub_Frame_Assign = Valid subframe assignment
                           - TDD_UL_DL_CFG_0 = 0 
                           - TDD_UL_DL_CFG_1 = 1 
                           - TDD_UL_DL_CFG_2 = 2 
                           - TDD_UL_DL_CFG_3 = 3 
                           - TDD_UL_DL_CFG_4 = 4 
                           - TDD_UL_DL_CFG_5 = 5 
                           - TDD_UL_DL_CFG_6 = 6

   \param Enable_Continuous_Tx = 
                          1 ֠UE will transmit Continuously
                          0 ֠UE will transmit according to SUB_FRAME_ASSIGN

   \param iFTM_Error_Code = FTM Error code returned by FTM command
         *	0x0    ֠Success
         *	0x1    ֠An un-supported value requested in the Sub_Frame_Assign field.
         *  0x2    - An un-supported value requested in the Enable_Continuous_Tx field.
         *  0x3    - An un-supported values requested in the Sub_Frame_Assign and Enable_Continuous_Tx fields.
         *  0xFFFF - Indicates that the LTE TDD band is not set prior to sending this command or UE is in 
                     calibration state.

   \return true if successful, false if fail.

   \warning 2012.09: this function is not officially released in AMSS MSM8960 equivalents. 
                     It will only be available in AMSS MSM8974 equivalents according to the 
                     Factory Test Roadmap document 80-NC791-1

   \note This function can be used in QMSL FTM Sequencer operation

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_LTE_SET_TDD_PARAMS(   HANDLE hResourceContext, 
                                                         char Sub_Frame_Assign,                                                         
                                                         char Enable_Continuous_Tx,
                                                         unsigned short* iFTM_Error_Code);   
   /******************************************************************************/
   /**

   Factory Test Mode WCDMA Commands, 4.24 Set Tx sweep step size

   This command sets the PDM step size used for the Tx calibration sweep. For each step of the
   sweep, the PDM will be reduced by the specified step size. If this command is not used, then the
   step size will default to 10.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iStepSize = PDM step size to use for the Tx calibration sweep

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_TX_SWEEP_STEP_SIZE
   ( HANDLE hResourceContext, unsigned char iStepSize );

   /******************************************************************************/
   /**
   Factory Test Mode 1x RF Commands 3.1.3.14 (CL93-V5864-1)Select receiver antenna source

   This command selects the antenna source for the second receiver. This command is intended for
   radio testing and factory calibration of designs implementing Full-Time SHDR. The
   command allows the second receiver to be switched between the primary antenna and the
   secondary antenna, depending on the input parameter passed to the command. The RF driver
   must have been correctly configured for antenna selection via the RF card file, and the feature
   FEATURE_FULL_TIME_SHDR_3 must be defined.

   The implementation of this command has the side effect of reconnecting Rx0 to the primary
    antenna when the antenna select parameter is 1. This command was designed to support FTS-3
    phone designs, but given the preceeding side effect, it also behaves correctly on FTS-4 designs
    that do not have a true CDMA secondary antenna

   \param  iAntennaSel =   0 ֠Connect Rx1 to primary antenna
                     1 ֠Connect Rx1 to secondary antenna

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SELECT_RX_ANTENNA(HANDLE hResourceContext, unsigned char iAntennaSel);

   /******************************************************************************/
   /**
   Factory Test Mode 1x RF Commands 3.2.23 (CL93-V5419-1)Get CtoN

    This command returns the carrier-to-noise ratio as calculated at the Rx front end of the Rx chain.
    The value returned is 10 * (C/N) dB. This value was chosen to provide the result with one
    decimal place of precision.

    see also: Factory Test Mode Get C-to-N, CL93-V6487-1

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iCtoN = Returns the C/N ratio value with one decimal place = 10 * (C/N) dB

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_CTON( HANDLE hResourceContext, long* iCtoN);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, 4.27 Do DC calibration

   This command performs DC calibration. The Set mode and Set RF Channel commands should
   be called first. The results of the calibration will be written to NV directly.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_DO_DC_CAL(	HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, FTM_DO_IQ_MISMATCH_CAL

   This command performs Rx IQ Mismatch calibration. The Set mode and Set RF Channel commands should
   be called first. The results of the calibration will be written to NV directly by the embedded side.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

   \param ftm_error_code = ftm_error_code returned by embedded side

   \param	iAmpComp = Amplitude mismatch compensation

   \param  iPhaseComp = Phase mismatch compensation

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_DO_IQ_MISMATCH_CAL( HANDLE hResourceContext, unsigned short* iFtm_error_code, unsigned short* iAmpComp, unsigned short* iPhaseComp );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, ICI calibration

   This command performs ICI calibration and collects INO data returned via FTM log.

   QLIB_FTM_SET_MODE, QLIB_FTM_SET_CHAN, QLIB_FTM_SET_SECONDARY_CHAIN and QLIB_FTM_SET_LNA_RANGE should be called first.

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_ICI_IQ_Data, TimeOut) to configure the timeout value for getting IQ data
   from asynchronus FTM log

        \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

        \param multi_carrier = conveys whether multicarrier mode is enabled

        \param carrier_id = the carrier for which IQ data has to be obtained (0 or 1)

        \param ftm_error_code = ftm_error_code returned by embedded side

        \param ino_array = buffer for storing IQ data. Length needs to be 25600 bytes.
                      IQ data is returned as interleaved I and Q bytes ex. I0, Q0, I1, Q1, I2, Q2 ... 12800 pairs of IQ data

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_ICI_CAL_DATA( HANDLE hResourceContext, unsigned char multi_carrier,
         unsigned char carrier_id, unsigned short *iFtm_error_code, char* ino_array );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands

   This command triggers IQ data collection on the phone and returns such data in an array

   QLIB_FTM_SET_MODE, QLIB_FTM_SET_CHAN, QLIB_FTM_SET_SECONDARY_CHAIN should be called first.

   Call QLIB_ConfigureTimeout( hResourceContext, QMSL_Timeout_ICI_IQ_Data, TimeOut) to configure the timeout value for getting IQ data
   from asynchronus FTM log

        \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()

        \param ftm_error_code = ftm_error_code returned by embedded side

        \param ino_array = buffer for storing IQ data. Length needs to be 25600 bytes.
                      IQ data is returned as interleaved I and Q bytes ex. I0, Q0, I1, Q1, I2, Q2 ... 12800 pairs of IQ data

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_DO_WCDMA_IQ_LOGGING( HANDLE hResourceContext, unsigned short carrier_mode,
         unsigned short carrier_id, unsigned short* iFtm_error_code, char* ino_array );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx(CL93-V5368-1) Config EUL Waveform

   FTM_CONFIG_EUL_WAVEFORM API computes and configures all the necessary MDSP
   and hardware registers for EUL waveform generation.  The request packet
   contains all the EUL waveform configuration information.
   It also enables the EUL Waveform
   Users can also use this API to achieve R99 and HSDPA waveforms with the following conditions:
      * For R99 - Set the iDeltaHS, iDeltaEc and iDeltaEd params to -1 (disable) and bHSDPCCHEnable = False
      * For HSDPA - Set the iDeltaEc, iDeltaEd to -1 (disable)

   This API has the following runtime dependencies.
   *	QLIB_FTM_SET_MODE
   *	QLIB_FTM_SET_CHAN
   *	QLIB_FTM_SET_TX_ON
   *	QLIB_FTM_WCDMA_CW_WAVEFORM
   *	QLIB_FTM_SET_PA_RANGE
   *	QLIB_FTM_SET_PDM (optional)

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()
   \param iBetaC = Gain factor for DPCCH.  Range: 0-15
   \param iBetaD = Gain factor for DPDCH.  Range: 0-15
   \param iDeltaHS = Gain factor for HS-DPCCH (ACK/NACK/CQI).  Range -1 & 0-8
   \param iDeltaEc = EUL delta Ec fain factor.  Range -1 & 0-8
   \param iDeltaEd = EUL delta Ed fain factor.  Range -1 & 0-29
   \param iBetaEDSF2val = Beta ED SF2 values depends on Delta ED value selected
   \param iMu = Mu factor.  Range 0,1,2,4,6
   \param bDPDCHEnable = DPDCH enable flag.   Range: 0 or 1.
   \param bHSDPCCHEnable = HS-DPCCH enable flag.   Range: 0 or 1.
   \param iModScheme = Modulation: 0 for QPSK, 1 for 16QAM (if supported)
   \param iMPR = output - MPR backoff in dBx2 ex: '3'=>1.5dB.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_CONFIG_EUL_WAVEFORM( HANDLE hResourceContext,
         unsigned char iBetaC, unsigned char iBetaD, char iDeltaHS,
         char iDeltaEc, char iDeltaEd, char iBetaEDSF2val,
         unsigned char iMu, unsigned char bDPDCHEnable,
         unsigned char bHSDPCCHEnable, unsigned char iModScheme, unsigned char* iMPR );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx(CL93-V5368-1) Disable EUL Waveform

   FTM_DISABLE_EUL_WAVEFORM API reverts back to WCDMA Rҹ9 waveform and
   all the EUL related register settings are erased.

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_DISABLE_EUL_WAVEFORM( HANDLE hResourceContext );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx(CL93-V5368-1) WCDMA Set TxAGC

   FTM_WCDMA_SET_TXAGC API  programs the PA range, RF Gain and Digital Gain to reach the programmed TxAGC.
   User can override the PA range lookup in this API by setting the PA_Override input to TRUE and provide a custom PA range.


   This API has the following runtime dependencies.
   *	QLIB_FTM_SET_MODE
   *	QLIB_FTM_SET_CHAN
   *	QLIB_FTM_SET_TX_ON
   *	QLIB_FTM_WCDMA_CW_WAVEFORM or QLIB_FTM_EUL_CONFIG_WAVEFORM

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \param iTxAGC = TxAGC power to be set.  Range: 0 to 1023
                   AGC resolution is target-dependent
                       TxAGC(dB10) = (TxPower(dBm)+70)/10
                       TxAGC(dB12) = (TxPower(dBm)+57)/12

   \param iPAOverride = TRUE  = Override PA range and use iPARange field always for PA range
                        FALSE = Use iTxAGC and NV PA switchpoints to determine PA range

   \param iPARange = Sets PA range when iPAOverride is TRUE.  PA R0/R1 settings are determined through
                     the NV PA Range Map.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WCDMA_SET_TXAGC( HANDLE hResourceContext,
         short iTxAGC, unsigned char iPAOverride, unsigned char iPARange );

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx(CL93-V5368-1) WCDMA Set TxAGC Sweep

   FTM_WCDMA_TXAGC_SWEEP API performs TxAGC sweep from start to stop TxAGC in step provided.
   User can override the PA range lookup in this API by setting the PA_Override input to TRUE and provide a custom PA range


   This API has the following runtime dependencies.
   *	QLIB_FTM_SET_MODE
   *	QLIB_FTM_SET_CHAN
   *	QLIB_FTM_SET_TX_ON
   *	QLIB_FTM_WCDMA_CW_WAVEFORM or QLIB_FTM_EUL_CONFIG_WAVEFORM

   Resolution of Start/Stop/Step AGC values are target-dependent:
          TxAGC(dB10) = (TxPower(dBm)+70)/10
          TxAGC(dB12) = (TxPower(dBm)+57)/12

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()
   \param iStart = Start TxAGC  Range: 0 to 1023
   \param iStop = Stop TxAGC  Range: 0 to 1023
   \param iStep = Steps in AGC counts (can be positive or negative)
   \param iDelay = Step width in milliseconds (20000 => 20 ms)
   \param iPAOverride = TRUE  = Override PA range and use iPARange field always for PA range
                        FALSE = Use current tx agc value and NV PA switchpoints to determine PA range
   \param iPARange = Sets PA range when iPAOverride is TRUE.  PA R0/R1 settings are determined through
                     the NV PA Range Map.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_WCDMA_TXAGC_SWEEP( HANDLE hResourceContext,
         short iStart, short iStop, short iStep,
         short iDelay, unsigned char iPAOverride, unsigned char iPARange );


   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx FTM IRAT MEAS GTOW TEST WCDMA

   FTM_MEAS_GTOW_TEST_WCDMA API is used as G2W Setup mode where the phone(RTR) is faked to GSM mode (from WCDMA mode) and brought back
   to WCDMA mode so that we can make measurements in WCDMA mode for G TO W InterRAT case going from G to W mode.
   This is done purely in FTM mode where only the RTR settings are changed to provide this scenario.
   This is useful in FTM InterRAT measurements to verify the RF part (not involving upper layers).

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MEAS_GTOW_TEST_WCDMA( HANDLE hResourceContext,
         unsigned short gsmBand, unsigned short gsmChan,
         unsigned short wcdmaBand, unsigned short wcdmaChan);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx FTM IRAT MEAS GTOW TEST GSM

   FTM_MEAS_GTOW_TEST_GSM API is used as G2W Cleanup mode where the phone(RTR) is faked to WCDMA mode (from GSM mode) and brought back
   to GSM mode so that we can make measurements in GSM mode for G TO W InterRAT case going back to G from W mode.
   This is done purely in FTM mode where only the RTR settings are changed to provide this scenario.
   This is useful in FTM InterRAT measurements to verify the RF part (not involving upper layers).

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MEAS_GTOW_TEST_GSM( HANDLE hResourceContext,
         unsigned short gsmBand, unsigned short gsmChan,
         unsigned short wcdmaBand, unsigned short wcdmaChan);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx FTM IRAT MEAS WTOG TEST GSM

   FTM_MEAS_WTOG_TEST_GSM API is used as W2G Setup mode where the phone(RTR) is faked to WCDMA mode (from GSM mode) and brought back
   to GSM mode so that we can make measurements in GSM mode for G TO W InterRAT case going to G from W mode.
   This is done purely in FTM mode where only the RTR settings are changed to provide this scenario.
   This is useful in FTM InterRAT measurements to verify the RF part (not involving upper layers).

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MEAS_WTOG_TEST_GSM( HANDLE hResourceContext,
         unsigned short gsmBand, unsigned short gsmChan,
         unsigned short wcdmaBand, unsigned short wcdmaChan);

   /******************************************************************************/
   /**
   Factory Test Mode WCDMA Commands, x.x.x.xx FTM IRAT MEAS WTOG TEST WCDMA

   FTM_MEAS_WTOG_TEST_WCDMA API is used as W2G Setup mode where the phone(RTR) is faked to GSM mode (from WCDMA mode) and brought back
   to WCDMA mode so that we can make measurements in WCDMA mode for G TO W InterRAT case going back to W from G mode.
   This is done purely in FTM mode where only the RTR settings are changed to provide this scenario.
   This is useful in FTM InterRAT measurements to verify the RF part (not involving upper layers).

   \param hResourceContext = Resource context that was returned from the call
                             to QLIB_ConnectServer()

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_MEAS_WTOG_TEST_WCDMA( HANDLE hResourceContext,
         unsigned short gsmBand, unsigned short gsmChan,
         unsigned short wcdmaBand, unsigned short wcdmaChan);

   /******************************************************************************/
   /**
   Factory Test Mode 1x RF Commands 3.2.25 (CL93-V5419-1)Get CDMA IM2 with suppression

   This command returns the suppression achieved as well as the I, Q, and transconductance (gM)
   optimum IM2 values for the current mode and channel of the mobile. It is intended to augment the original Get CDMA IM2.

   Rx signal inputs are required to be set before issuing this command or results will not be accurate.

   The suppression level is given in units of dB.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iI_Value = I value
   \param	iQ_Value = Q value
   \param	iTransConductance_Value = Transconductance Value
   \param	iSuppression = Suppression

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_CDMA_IM2_WITH_SUPPRESSSION(	HANDLE hResourceContext,
         unsigned char* iI_Value,
         unsigned char* iQ_Value,
         unsigned char* iTransConductance_Value,
         unsigned short* iSuppression);



   /******************************************************************************/
   /**
   Factory Test Mode FM Commands, 3.3.1.16(CL93-V4168-1) Set frequency sense gain

   This command sets the frequency sense gain register to set the DFM modulation index..

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iModulationIndex =
                                 Range of data is from 0 to 63 (6 LSBits of input byte):
                                 0 = lowest
                                 63 = highest.

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_FREQ_SENSE_GAIN(	HANDLE hResourceContext,
         unsigned char iModulationIndex );

   /******************************************************************************/
   /**
   Factory Test Mode FM Commands, 3.3.1.21(CL93-V4168-1) Get FM IQ

   This command returns the A and B compensation values for the FM RF Mode. This command
   assumes that the user is adding a Rx CW on channel center at -75 dBm. This function will lock
   the trk-lo tune of the radio off one channel, calculate the A and B values, retune the radio back to
   the original channel, and return the A and B values as a packed double word.

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param  iFmIQValue =
                                 Most significant 16 bits = B values
                                 Least significant 16 bits = A values.

   \return true if successful, false if fail.

   \warning

   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_GET_FM_IQ(	HANDLE hResourceContext,
         unsigned long* iFmIQValue );



      /******************************************************************************
                     Dual RX Chain FTM RF Commands
   *******************************************************************************/
   /******************************************************************************/
   /**
   Dual Rx Chain FTM Command 3.3.10 (CL93-V5864-1) Set secondary chain

   This command sets the secondary chain in one of three modes, DISABLED, RECEIVE DIVERSITY and OFF FREQUENCY SEARCH

   \param  mode
      \code
           0 = Disabled - puts the secondary chain ZIFRIC and ZIFLNA in disabled mode via SBI
           1 = Receive Diversity - enables the secondary chain ZIFRIC and ZIFLN via SBI and switches the RX_VCO_SEL MUX
                                   to the primary chain PLL; this means that both the primary
                                   and secondary chain are set to the same band class and
                                   channel.

           2 = Off frequency search (OFS) = allows you to set the secondary chain to a band class and channel that is
               independent of the primary chain settings
      \endcode
   \return true if successful, false if fail.

   \note This function can be used in QMSL FTM Sequencer operation

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_SECONDARY_CHAIN( HANDLE hResourceContext, unsigned short iMode );


   /**
   For UMTS/WCDMA:
   Refer to section 2.2.29 (CL93-V5368-1) FTM_SECOND_CHAIN_TEST_CALL

   This command forces the AMSS to use the second chain for all WCDMA Rx processing in FTM mode. This test mode is used for production testing of the second receive chain functionality.
   To use the second chain test call, place the phone in FTM mode. Issue the FTM_SECOND_CHAIN_TEST_CALL command, with the mode parameter set to 1. The
   phone will now only respond to FL signals on the secondary antenna. All Tx functions continue to use the primary antenna port.
   The phone returns to normal operation (primary only) after a reset, or a FTM_SECOND_CHAIN_TEST_CALL command issued with mode parameter 0.
   NOTE:	The mode ID for the secondary chain WCDMA RF commands is 21.

    For cdma2000:
   Dual Rx Chain FTM Command 3.3.12 (CL93-V5864-1) Set second chain test call mode
   See also: CL93-V6408-1, App Note : FTM, Set Up MSM for Call Process on Second Rx Chain

   This function forces the AMSS to use the second chain for all CDMA 1x RX processing in online
   mode. This test mode is intended for production testing of the second receive chain functionality.
   To use the second chain test mode, first place the phone in FTM mode. Then issue the Second
   Chain Test Call Mode command, with the mode parameter set to Enable. Issue a runtime switch
   back to online mode. The phone will now only respond to forward link signals on the second
   antenna. All transmit functions continue to use the primary antenna port. The phone will return to
   normal operation after a reset, or a Second Chain Test Call Mode command with the mode set to
   Disable.


   \param  Byte mode  = enables and disables the second chain test call mode:
   \code
   For cdma2000:

       0 = Disabled
       1 = Enabled

   For WCDMA/UMTS:

       0 = Primary chain only
       1 = Secondary chain only
       2 = Primary and secondary chain

   \endcode

   \return true if successful, false if fail.

   \warning

   \note This function can be used in QMSL FTM Sequencer operation
   *******************************************************************************/
   
   QLIB_API unsigned char QLIB_FTM_SECOND_CHAIN_TEST_CALL( HANDLE hResourceContext, unsigned char iMode );

   /******************************************************************************/
   /**
   [List documentation here when available]

   This command loads the digital pre-distortion (DPD) tables

   \param  [in]       eDevice = RFM Device type for primary Tx path on which the command is triggered (FTM_PriPathDevType_Enum)
   \param  [in]       eXPT_type = Specifies the type of xPT mode for which load DPD/IQ capture must be performed ( 0 = EPT, 1 = ET )
   \param  [in]       eAction_type = Specifies the type of load DPD action which must be performed
   \param  [in]       iNum_AMAM = Specifies the number of elements of piAMAM_array 
   \param  [in]       iNum_AMPM = Specifies the number of elements of piAMPM_array
   \param  [in]       iNum_EPDT = Specifies the number of elements of piEPDT_array
   \param  {in]       piAMAM    = array containing the data for DPD override, must contain the number of elements provided in iNum_amam
   \param  [in]       piAMPM    = array containing the data for DPD override, must contain the number of elements provided in iNum_ampm
   \param  {in]       piEPDT    = array containing the data for DPD override, must contain the number of elements provided in INum_epdt

   \return true if successful, false if fail.

   \note [note whether this function can be used in QMSL FTM Sequencer operation]

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_LOAD_DPD_TBLS( HANDLE hResourceContext, 
                                                  unsigned char eDevice,
                                                  unsigned char eXPT_type,
                                                  unsigned char eAction_type,
                                                  unsigned short iNum_AMAM,
                                                  unsigned short iNum_AMPM,
                                                  unsigned short iNum_EPDT,
                                                  unsigned short* piAMAM,
                                                  unsigned short* piAMPM,
                                                  unsigned short* piEPDT );

   /******************************************************************************/
   /**
   [List documentation here when available]

   This command triggers Enhanced Power Tracking (xPT) IQ Capture.

   \param  [in]     eDevice = RFM Device type for primary Tx path on which the command is triggered (FTM_PriPathDevType_Enum)
   \param  [in]     eDevice_fb = Feedback RFM device type for the feedback path (FTM_FBPathDevType_Enum)
   \param  [in]     eXPT_type = Specifies the type of xPT mode for which load DPD/IQ capture must be performed (FTM_xptType_Enum)
   \param  [in]     iNum_samples = Specifies the number of samples that must be captured for xPT IQ capture
   \param  [in]	  bFirst_trig = Specifies if this is the First IQ capture trigger to FW, allows FW to initialize HW on their side
   \param  [in]	  bLast_trig = Specifies if this is the Last IQ capture trigger to FW, allows FW to de-initialize HW on their side
   \param  [in]	  bDo_processing = Specify to FW whether to process the IQ samples on their side
   \param  [in]	  iNum_captures = Specifies the number of IQ captures that must be triggered internally in SW by this command
   \param  [in]	  iDelay_us = Specifies the delay in microseconds between any set-up for IQ capture and triggering IQ capture
   \param  [in]	  bDo_leakage_cal = Specifies whether to perfom IQ capture for leakage cal or normal xPT IQ capture
   \param  [in]	  iDebug_state = specifies whether files are saved to root EFS. Could also enable any other specialized debug, possibly extra debug messages.
                                   0 ֠debug disabled. No files saved
                                   1 ֠debug enabled. Files saved
   \param  [in]	  iIq_file_name_len = The number of characters including extension for IQ data file name. 
                    Set this param to zero to indicate that an IQ file should not be saved.
   \param  [in]	  iDpd_file_name_len = The number of characters including extension for DPD data file name.
                    Set this param to zero to indicate that an DPD file should not be saved.
   \param  [in]	  pIq_file_name = Optional. specifies the IQ file name to be used when saving IQ capture to EFS
   \param  [in]	  pDPD_file_name = Optional. specifies the DPD file name to be used when saving to EFS
                                

   \return true if successful, false if fail.

   \note [note whether this function can be used in QMSL FTM Sequencer operation]

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_TRIGGER_XPT_IQ_CAP( HANDLE hResourceContext, 
                                                       unsigned char eDevice,
                                                       unsigned char eDevice_fb,
                                                       unsigned char eXPT_type,
                                                       unsigned short iNum_samples,
                                                       unsigned char bFirst_trig,
                                                       unsigned char bLast_trig,
                                                       unsigned char bDo_processing,
                                                       unsigned char bDo_leakage,
                                                       unsigned char iDebug_state,
                                                       unsigned char iIq_file_name_len,
                                                       unsigned char iDpd_file_name_len,
                                                       char* pIq_file_name,
                                                       char* pDpd_file_name );

   /******************************************************************************/
   /**
   [List documentation here when available]

   This command sets Enhanced Power Tracking (EPT) state. EPT is the new name for SAPT.

   \param  [in]      eDevice = RFM Device type for primary Tx path on which the command is triggered (FTM_PriPathDevType_Enum)
   \param  [in]      bSAPT_on = Specifies if SAPT must be turned on or off

   \return true if successful, false if fail.

   \note [note whether this function can be used in QMSL FTM Sequencer operation]

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_SET_SAPT_STATE( HANDLE hResourceContext, 
                                                   unsigned char eDevice,
                                                   unsigned char bSAPT_on );

   /******************************************************************************/
   /**
   [List documentation here when available]

   FTM_XPT_TX_AGC_OVERRIDE - Configures TX AGC Override.


   \param  [in]      eDevice = RFM Device type for primary Tx path on which the command is triggered
   \param  [in]      eXPT_type = XPT Mode type to override
   \param  [in]      eIQ_Override_type = IQ Gain Override Type
   \param  [in]      iIQ_gain = IQ Gain to override
   \param  [in]      eEnvSc_Override_type = Envelope Scale Gain Override Type
   \param  [in]      iEnvScale = Envelope Scale to override
   \param  [in]      iRGI = RGI to override
   \param  [in]      iPA_bias = PA SMPS Bias to override
   \param  [in]      iPA_state = PA State to override
   \param  [in]      iPA_curr = PA Quiescent Current to override
   \param  [in]      eDelay_Override_type = Delay Val Override Type
   \param  [in]      iDelay_us = Delay Value is us to override
   \param  [out]     piStatus = Returned command status

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
   QLIB_API unsigned char QLIB_FTM_XPT_TXAGC_OVERRIDE( HANDLE hResourceContext, 
                                                       unsigned char eDevice, 
                                                       signed char eXPT_type,
                                                       unsigned char eIQ_Override_type,
                                                       long iIQ_gain,
                                                       unsigned char eEnvSc_Override_type,
                                                       long iEnvScale,
                                                       unsigned short iRGI,
                                                       unsigned short iPA_bias,
                                                       unsigned char iPA_state,
                                                       unsigned short iPA_curr,
                                                       unsigned char eDelay_Override_type,
                                                       long  iDelay_us );

   /******************************************************************************/
   /**

   QLIB_FTM_XPT_MLINE_COMPUTE - Computes the M (Magic) Line for XPT


   \param  [in]   iLin_region_pct_x100 = Linear region percentage (x100) used in M Line Calculation
   \param  [in]   iClip_pct_x100 = Clipping Percentage (x100) to be used in M Line Calculation
   \param  [in]   iComp_pct_x10 = Compression Point db10 to be used in M Line Calculation    
   \param  [in]   eCurve_fit_type = M Line Curve Fit Type   
   \param  [in]   iPAR_x100 = Peak to Average Ratio of the Tx waveform
   \param  [in]   iXpt_cal_data_len = length  of xpt_mline_outputs_data array
   \param  [in]   iRgi_delta_data_len = length of peRgi_delta_data array
   \param  [in]   peXpt_cal_data = see enum xpt_cal_data
   \param  [in]   peRgi_delta_data = pointer to the start of array of enum ftm_common_rgi_delta_data
   \param  [in]	iDpd_file_name_len = The number of characters including extension for DPD data file name.
   \param  [in]	pDpd_file_name = Specifies the DPD file name to retrieve from EFS. 
                  iXpt_cal_data_len number of DPD files will be retrieved.
                  '_#' will be appended to the end of the file name to retreive each.
                  ex. if pDpd_file_name is "dpd.bin" then first retrieved file will be "dpd_1.bin and so on.
   \param  [out]  iRsp_len = length of peRsp_payload array
   \param  [out]  peRsp_payload = pointer to start of response array of enum xpt_mline_outputs_data

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/
  QLIB_API unsigned char QLIB_FTM_XPT_MLINE_COMPUTE( HANDLE hResourceContext, 
                                                     unsigned long iLin_region_pct_x100,
                                                     unsigned long iClip_pct_x100,
                                                     unsigned long iComp_pct_x10,
                                                     unsigned char eCurve_fit_type,
                                                     unsigned long iPAR_x100,
                                                     unsigned char iXpt_cal_data_len,
                                                     unsigned char iRgi_delta_data_len,
                                                     void* peXpt_cal_data,
                                                     void* peRgi_delta_data,
                                                     unsigned char iDpd_file_name_len,
                                                     char* pDpd_file_name,
                                                     unsigned short* iRsp_len,
                                                     void* peRsp_payload );

   /******************************************************************************/
   /**

   FTM_SET_TUNER_TUNE_CODE_OVERRIDE

   This command is used to override the complete tune code (a combination of capacitors and switches in the antenna tuner hardware) for the antenna tuner of interest. 

   \param hResourceContext = Resource context that was returned from the call to QLIB_ConnectServer()
   \param iOverride_Flag   Set 1 to override.  Set 0 to disable override
   \param iCommand_Version The pOverrideData and pNV_Data format version.  The current version format is 0.
   \param pOverrideData    Pointer to FTM_Set_Tuner_Override_V0 . See QLib_Defines.h
   \param iResultCode      Result Code
   \param pNV_Data         Pointer to FTM_Set_Tuner_NVData_V0 . The pNV_Data is updated by embedded side with the code word value.  See QLib_Defines.h

   \return true if successful, false if fail.

   \warning
   *******************************************************************************/

   QLIB_API unsigned char QLIB_FTM_SET_TUNER_TUNE_CODE_OVERRIDE(  HANDLE hResourceContext,
                                                                  unsigned char iOverride_Flag,
                                                                  unsigned char iCommand_Version,
                                                                  void* pOverrideData,
                                                                  unsigned long* iResultCode,
                                                                  void* pNV_Data);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif	// defined(_QLIB_FTM_RF_H)
