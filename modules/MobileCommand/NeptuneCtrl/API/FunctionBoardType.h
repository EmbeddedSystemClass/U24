#ifndef _HWTOOL_FUNCTION_BOARD_TYPE_H
#define _HWTOOL_FUNCTION_BOARD_TYPE_H


// 2008-02-18 Mark Hsieh BEGIN
//=================================Function Board Related Constant==================//
#define VOLTAGE_DIVISION_COEFFICIENT		1.975
//=================================Packet Command===================================//
#define CMD_VBAT                0x10                 // No used
#define CMD_VCHG                (CMD_VBAT + 1)       // No used
#define CMD_VRDBB               (CMD_VBAT + 2)
#define CMD_VSENSE1V8           (CMD_VBAT + 3)
#define CMD_VSENSE2V8           (CMD_VBAT + 4)
#define CMD_VRF_ADC             (CMD_VBAT + 5)       // No used
#define CMD_VTEMP               (CMD_VBAT + 6)       // No used
#define CMD_BACKUP_BATTERY      (CMD_VBAT + 7)
#define CMD_VSENSE3             (CMD_VBAT + 8)       // No used
#define CMD_VSENSE4             (CMD_VBAT + 9)
#define CMD_VSENSE5             (CMD_VBAT + 10)      // No used
#define CMD_VSENSE6             (CMD_VBAT + 11)      // No used

#define CMD_VRIO                CMD_VSENSE1V8        // For VRIO 1.8V
#define CMD_BUCK_1P8V           CMD_VSENSE2V8        // For memory power
#define CMD_VRSIM               CMD_VRF_ADC          // For VRSIM 2.85V
#define CMD_VCAP_3P3V           CMD_VTEMP            // For VRSIM 2.85V

#define CMD_BACKLIGHT           0x30
#define CMD_SIDE_KEY_LED        (CMD_BACKLIGHT + 1)
#define CMD_STATUS_LED          (CMD_BACKLIGHT + 2)
#define CMD_VIBRATOR            (CMD_BACKLIGHT + 3)

#define CMD_HANDFREE_R          0x40                 // No used
#define CMD_HANDFREE_L          (CMD_HANDFREE_R + 1) // No used
#define CMD_RECEIVER            (CMD_HANDFREE_R + 2) // No used
#define CMD_LOUD_SPEAKER        (CMD_HANDFREE_R + 3)

#define CMD_32KHz               0x50                   // No used
#define CMD_13MHz               (CMD_32KHz + 1)        // No used
#define CMD_19P2MHz             (CMD_32KHz + 2)        // No used
#define CMD_26MHz               (CMD_32KHz + 3)        // No used

#define CMD_PRESS_HS_POWER_KEY  0x60
#define CMD_ACCIN_INT_ON        (CMD_PRESS_HS_POWER_KEY + 1) // No used
#define CMD_ACCIN_INT_OFF       (CMD_PRESS_HS_POWER_KEY + 2) // No used

#define CMD_CHG_OVP             0x70
#define CMD_PRE_CHG             (CMD_CHG_OVP + 1)
#define CMD_FULL_CHG            (CMD_CHG_OVP + 2)
#define CMD_NORMAL_MODE         (CMD_CHG_OVP + 3)
#define CMD_NO_PWR_To_HS        (CMD_CHG_OVP + 4)
#define CMD_USB_CHG             (CMD_CHG_OVP + 5) // 2009-09-15 Mick: Austin EVT3

#define CMD_RESEND_COMMAND_ACK     0x80
#define CMD_RESEND_COMMAND_RESULT  0x81
#define CMD_SET_ADC12_REF_VOL      0x90 // For ADC12 reference voltage, no used
#define CMD_GET_CODE_VERSION       0xa0

// 2009-09-15 Mick: Austin EVT3, function board new command
#define CMD_ALS_LED_ON          0xd0
#define CMD_ALS_LED_OFF         0xd1
#define CMD_ELECTROMAGNET_ON    0xd2
#define CMD_ELECTROMAGNET_OFF   0xd3
#define CMD_LDO_ON              0xd4
#define CMD_LDO_OFF             0xd5

// 2009-09-15 Mick: Austin EVT3, function board new command
#define CMD_SPK_POS             0x10
#define CMD_SPK_NEG             0x11
#define CMD_REV_POS             0x12
#define CMD_REV_NEG             0x13
#define CMD_ALS_LED1            0x14
#define CMD_ALS_LED2            0x15
#define CMD_ALS_LED3            0x16
#define CMD_HS_POS              0x17
#define CMD_HS_NEG              0x18
#define CMD_LO_POS              0x19
#define CMD_LO_NEG              0x20

//=================================Packet Command===================================//
// 2008-02-18 Mark Hsieh END


#endif // _HWTOOL_FUNCTION_BOARD_TYPE_H
