#include "stdafx.h"
#include "CMDModuleExport.h"
#include "stdlib.h"
#include <map>
using namespace std;  

namespace Berlin
{
	namespace Band
	{
		typedef __int64  cm_band_pref_e_type;
		#define SYS_BM_64BIT(val) ((__int64)1<<(__int64)(val))

		#define NV_BAND_PREF_I			441
		#define NV_BAND_PREF_16_31_I	946
		#define NV_BAND_PREF_32_63_I	2954




		typedef enum 
		{
		  SYS_SBAND_BC0_A = 0,
			/**< Band Class 0, A-System. */
		  SYS_SBAND_BC0_B = 1,
			/**< Band Class 0, B-System. */
			/* Band Class 0 AB , GSM 850 Band? */
		  SYS_SBAND_BC1 = 2,
			/**< Band Class 1, all blocks. */
		  SYS_SBAND_BC2 = 3,
			/**< Band Class 2 place holder. */
		  SYS_SBAND_BC3 = 4,
			/**< Band Class 3, A-System. */
		  SYS_SBAND_BC4 = 5,
			/**< Band Class 4, all blocks. */
		  SYS_SBAND_BC5 = 6,
			/**< Band Class 5, all blocks. */
		  SYS_SBAND_GSM_DCS_1800 = 7,
			/**< GSM Digital Cellular Standard (DCS) band. */
		  SYS_SBAND_GSM_EGSM_900 = 8,
			/**< GSM Extended GSM (E-GSM) band. */
		  SYS_SBAND_GSM_PGSM_900 = 9,
			/**< GSM Primary GSM (P-GSM) band. */
		  SYS_SBAND_BC6 = 10,
			/**< Band Class 6. */
		  SYS_SBAND_BC7 = 11,
			/**< Band Class 7. */
		  SYS_SBAND_BC8 = 12,
			/**< Band Class 8. */
		  SYS_SBAND_BC9 = 13,
			/**< Band Class 9. */
		  SYS_SBAND_BC10 = 14,
			/**< Band Class 10. */
		  SYS_SBAND_BC11 = 15,
			/**< Band Class 11. */
		  SYS_SBAND_GSM_450 = 16,
			/**< GSM 450 band. */
		  SYS_SBAND_GSM_480 = 17,
			/**< GSM 480 band. */
		  SYS_SBAND_GSM_750 = 18,
			/**< GSM 750 band. */
		  SYS_SBAND_GSM_850 = 19,
			/**< GSM 850 band. */
		  SYS_SBAND_GSM_RGSM_900 = 20,
			/**< GSM Railways GSM Band. */
		  SYS_SBAND_GSM_PCS_1900 = 21,
			/**< GSM PCS band. */
		  SYS_SBAND_WCDMA_I_IMT_2000 = 22,
			/**< WCDMA Europe, Japan, and China IMT 2100 band. */
		  SYS_SBAND_WCDMA_II_PCS_1900 = 23,
			/**< WCDMA US PCS 1900 band. */
		  SYS_SBAND_WCDMA_III_1700 = 24,
			/**< WCDMA Europe and China DCS 1800 band. */
		  SYS_SBAND_WCDMA_IV_1700 = 25,
			/**< WCDMA US 1700 band. */
		  SYS_SBAND_WCDMA_V_850 = 26,
			/**< WCDMA US850 band. */
		  SYS_SBAND_WCDMA_VI_800 = 27,
			/**< WCDMA Japan 800 band. */
		  SYS_SBAND_BC12 = 28,
			/**< Band Class 12. */
		  SYS_SBAND_BC14 = 29,
		   /**< Band Class 14. */
		  SYS_SBAND_RESERVED_2 = 30,
		   /**< Reserved 2. */
		  SYS_SBAND_BC15 = 31,
		   /**< Band Class 15. */
		  SYS_SBAND_WLAN_US_2400 = 32,
			/**< WLAN US 2400 band. */
		  SYS_SBAND_WLAN_EUROPE_2400 = 33,
			/**< WLAN ETSI 2400 band. */
		  SYS_SBAND_WLAN_FRANCE_2400 = 34,
			/**< WLAN France 2400 band. */
		  SYS_SBAND_WLAN_SPAIN_2400 = 35,
			/**< WLAN Spain 2400 band. */
		  SYS_SBAND_WLAN_JAPAN_2400 = 36,
			/**< WLAN Japan 2400 band. */
		  SYS_SBAND_WLAN_US_5000 = 37,
			/**< WLAN US 2400 band. */
		  SYS_SBAND_WLAN_EUROPE_5000 = 38,
			/**< WLAN Europe 5000 band. */
		  SYS_SBAND_WLAN_FRANCE_5000 = 39,
			/**< WLAN France 5000 band. */
		  SYS_SBAND_WLAN_SPAIN_5000 = 40,
			/**< WLAN Spain 5000 band. */
		  SYS_SBAND_WLAN_JAPAN_5000 = 41,
			/**< WLAN Japan 5000 band. */
		  SYS_SBAND_WCDMA_VII_2600 = 48,
			/**< WCDMA Europe 2600 band. */
		  SYS_SBAND_WCDMA_VIII_900 = 49,
			/**< WCDMA Europe and Japan 900 band. */
		  SYS_SBAND_WCDMA_IX_1700 = 50,
			/**< WCDMA Japan 1700 band. */
		  /* 51-55 reserved for WLAN */
		  SYS_SBAND_BC16 = 56,
		   /**< Band Class 16. */
		  SYS_SBAND_BC17 = 57,
		   /**< Band Class 17. */
		  SYS_SBAND_BC18 = 58,
		   /**< Band Class 18. */
		  SYS_SBAND_BC19 = 59,
		   /**< Band Class 19. */
		  SYS_SBAND_WCDMA_XIX_850 = 60,
			/**< WCDMA Japan 850 band. */
		  SYS_SBAND_WCDMA_XI_1500 = 61,
			/**< WCDMA 1500 band.  */
		  SYS_SBAND_PERSISTENT = 62,
			/**< Persistent value from NV. */
		  SYS_SBAND_MAX = 63  /* FOR INTERNAL USE OF CM ONLY! */
		} sys_sband_e_type;
		/*~ SENTINEL sys_sband_e_type.SYS_SBAND_MAX */




		typedef enum {
		  SYS_SBAND_LTE_EUTRAN_BAND1 = 0,
		  /**< UL:1920-1980; DL:2110-2170. */
		  SYS_SBAND_LTE_EUTRAN_BAND2 = 1,
		  /**< UL:1850-1910; DL:1930-1990. */
		  SYS_SBAND_LTE_EUTRAN_BAND3 = 2,
		  /**< UL:1710-1785; DL:1805-1880. */
		  SYS_SBAND_LTE_EUTRAN_BAND4 = 3,
		  /**< UL:1710-1755; DL:2110-2155. */
		  SYS_SBAND_LTE_EUTRAN_BAND5 = 4,
		  /**< UL: 824-849; DL: 869-894. */
		  SYS_SBAND_LTE_EUTRAN_BAND6 = 5,
		  /**< UL: 830-840; DL: 875-885. */
		  SYS_SBAND_LTE_EUTRAN_BAND7 = 6,
		  /**< UL:2500-2570; DL:2620-2690. */
		  SYS_SBAND_LTE_EUTRAN_BAND8 = 7,
		  /**< UL: 880-915; DL: 925-960. */
		  SYS_SBAND_LTE_EUTRAN_BAND9 = 8,
		  /**< UL:1749.9-1784.9; DL:1844.9-1879.9. */
		  SYS_SBAND_LTE_EUTRAN_BAND10 = 9,
		  /**< UL:1710-1770; DL:2110-2170. */
		  SYS_SBAND_LTE_EUTRAN_BAND11 = 10,
		  /**< UL:1427.9-1452.9; DL:1475.9-1500.9. */
		  SYS_SBAND_LTE_EUTRAN_BAND12 = 11,
		  /**< UL:698-716; DL:728-746. */
		  SYS_SBAND_LTE_EUTRAN_BAND13 = 12,
		  /**< UL: 777-787; DL: 746-756. */
		  SYS_SBAND_LTE_EUTRAN_BAND14 = 13,
		  /**< UL: 788-798; DL: 758-768. */
		  SYS_SBAND_LTE_EUTRAN_BAND17 = 16,
		  /**< UL: 704-716; DL: 734-746. */
		  SYS_SBAND_LTE_EUTRAN_BAND18 = 17,
		  /**< UL: 815-830; DL: 860-875. */
		  SYS_SBAND_LTE_EUTRAN_BAND19 = 18,
		  /**< UL: 830-845; DL: 875-890. */
		  SYS_SBAND_LTE_EUTRAN_BAND20 = 19,
		  /**< UL: 832-862; DL: 791-821. */
		  SYS_SBAND_LTE_EUTRAN_BAND21 = 20,
		  /**< UL: 1447.9-1462.9; DL: 1495.9-1510.9. */
		  SYS_SBAND_LTE_EUTRAN_BAND24 = 23,
		  /**< UL: 1626.5-1660.5; DL: 1525 -1559. */
		  SYS_SBAND_LTE_EUTRAN_BAND25 = 24,
		  /**< UL: 1850-1915; DL: 1930 -1995 . */
		  SYS_SBAND_LTE_EUTRAN_BAND33 = 32,
		  /**< UL: 1900-1920; DL: 1900-1920. */
		  SYS_SBAND_LTE_EUTRAN_BAND34 = 33,
		  /**< UL: 2010-2025; DL: 2010-2025. */
		  SYS_SBAND_LTE_EUTRAN_BAND35 = 34,
		  /**< UL: 1850-1910; DL: 1850-1910. */
		  SYS_SBAND_LTE_EUTRAN_BAND36 = 35,
		  /**< UL: 1930-1990; DL: 1930-1990. */
		  SYS_SBAND_LTE_EUTRAN_BAND37 = 36,
		  /**< UL: 1910-1930; DL: 1910-1930. */
		  SYS_SBAND_LTE_EUTRAN_BAND38 = 37,
		  /**< UL: 2570-2620; DL: 2570-2620. */
		  SYS_SBAND_LTE_EUTRAN_BAND39 = 38,
		  /**< UL: 1880-1920; DL: 1880-1920. */
		  SYS_SBAND_LTE_EUTRAN_BAND40 = 39,
		  /**< UL: 2300-2400; DL: 2300-2400. */
		  SYS_SBAND_LTE_EUTRAN_BAND41 = 40,
		  /**< UL: 2496-2690; DL: 2496-2690 */
		  SYS_SBAND_LTE_EUTRAN_BAND42 = 41,
		  /**< UL: 3400-3600; DL: 3400-3600 */
		  SYS_SBAND_LTE_EUTRAN_BAND43 = 42,
		  /**< UL: 3600-3800; DL: 3600-3800 */
		  SYS_SBAND_LTE_EUTRAN_BAND_MAX
		  /**< Maximum number of EUTRAN bands. */
		} sys_sband_lte_e_type;
		 

		#define CM_BAND_PREF_BC0_A  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC0_A))
		#define CM_BAND_PREF_BC0_B  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC0_B))
		#define CM_BAND_PREF_BC0    ((cm_band_pref_e_type)(SYS_BM_64BIT(SYS_SBAND_BC0_A)|SYS_BM_64BIT(SYS_SBAND_BC0_B)))
		#define CM_BAND_PREF_BC1    ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC1))
		#define CM_BAND_PREF_BC3    ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC3))
		#define CM_BAND_PREF_BC4    ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC4))
		#define CM_BAND_PREF_BC5    ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC5))
		#define CM_BAND_PREF_GSM_DCS_1800  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_GSM_DCS_1800))
		#define CM_BAND_PREF_GSM_EGSM_900  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_GSM_EGSM_900))
		#define CM_BAND_PREF_GSM_PGSM_900  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_GSM_PGSM_900))
		#define CM_BAND_PREF_BC6  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC6))
		#define CM_BAND_PREF_BC7  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC7))
		#define CM_BAND_PREF_BC8 ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC8))
		#define CM_BAND_PREF_BC9  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC9))
		#define CM_BAND_PREF_BC10  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC10))
		#define CM_BAND_PREF_BC11  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC11))
		#define CM_BAND_PREF_BC12  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC12))
		#define CM_BAND_PREF_BC14  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC14))
		#define CM_BAND_PREF_BC15  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC15))
		#define CM_BAND_PREF_BC16  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_BC16))
		#define CM_BAND_PREF_GSM_450  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_GSM_450 ))
		#define CM_BAND_PREF_GSM_480  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_GSM_480 ))
		#define CM_BAND_PREF_GSM_750  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_GSM_750 ))
		#define CM_BAND_PREF_GSM_850  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_GSM_850 ))
		#define CM_BAND_PREF_GSM_RGSM_900  ((cm_band_pref_e_type)SYS_BM_64BIT(SYS_SBAND_GSM_RGSM_900))
		#define CM_BAND_PREF_GSM_PCS_1900  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_GSM_PCS_1900 ))
		#define CM_BAND_PREF_WCDMA_I_IMT_2000  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_I_IMT_2000 ))
		#define CM_BAND_PREF_WCDMA_II_PCS_1900  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_II_PCS_1900 ))
		#define CM_BAND_PREF_WCDMA_III_1700  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_III_1700 ))
		#define CM_BAND_PREF_WCDMA_IV_1700  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_IV_1700 ))
		#define CM_BAND_PREF_WCDMA_V_850  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_V_850 ))
		#define CM_BAND_PREF_WCDMA_VI_800  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_VI_800 ))
		#define CM_BAND_PREF_WCDMA_VII_2600      ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_VII_2600 ))
		#define CM_BAND_PREF_WCDMA_VIII_900      ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_VIII_900 ))
		#define CM_BAND_PREF_WCDMA_IX_1700      ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_IX_1700 ))
		#define CM_BAND_PREF_WCDMA_XI_1500      ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_XI_1500 ))
		#define CM_BAND_PREF_WCDMA_XIX_850      ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WCDMA_XIX_850 ))
		#define CM_BAND_PREF_WLAN_2400_US  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_US_2400))
		#define CM_BAND_PREF_WLAN_2400_JP  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_JAPAN_2400))
		#define CM_BAND_PREF_WLAN_2400_ETSI ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_EUROPE_2400))
		#define CM_BAND_PREF_WLAN_2400_SP    ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_SPAIN_2400))
		#define CM_BAND_PREF_WLAN_2400_FR    ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_FRANCE_2400))
		#define CM_BAND_PREF_WLAN_5000_US  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_US_5000))
		#define CM_BAND_PREF_WLAN_5000_JP  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_JAPAN_5000))
		#define CM_BAND_PREF_WLAN_5000_ETSI ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_EUROPE_5000))
		#define CM_BAND_PREF_WLAN_5000_SP    ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_SPAIN_5000))
		#define CM_BAND_PREF_WLAN_5000_FR    ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_WLAN_FRANCE_5000))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND1   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND1))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND2   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND2))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND3   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND3))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND4   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND4))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND5   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND5))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND6   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND6))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND7   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND7))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND8   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND8))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND9   ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND9))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND10  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND10))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND11  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND11))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND12  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND12))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND13  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND13))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND14  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND14))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND17  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND17))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND18  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND18))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND19  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND19))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND20  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND20))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND21  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND21))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND24  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND24))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND25  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND25))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND33  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND33))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND34  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND34))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND35  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND35))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND36  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND36))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND37  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND37))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND38  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND38))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND39  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND39))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND40  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND40))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND41  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND41))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND42  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND42))
		#define CM_BAND_PREF_LTE_EUTRAN_BAND43  ((cm_band_pref_e_type)SYS_BM_64BIT( SYS_SBAND_LTE_EUTRAN_BAND43))


	
		
		CMDMODULEEXPORT_API bool Get_UMTS_PrefBand_SPU ( const char* szPort, const char *szUseQPST, const char* szRetryTime, const char* szRetryPeriod , char *szPrefBand)
		{
			CQualcommCmdModule CmdModule( atoi(szPort), 1 == atoi(szUseQPST) );

			bool ret;
			__int64 tmp=0;
			__int64 BAND_PREF_I=0;
			__int64 BAND_PREF_16_31_I=0;
			__int64 BAND_PREF_32_63_I=0;
			__int64 bAllBand=0;
		
			map<int, CString> BandDesc;
			BandDesc[0]="BC0_A";

			for(int i = 0;i < atoi(szRetryTime);i++)
			{
				ret = CmdModule.ReadNVItem(NV_BAND_PREF_I, NV_BAND_PREF_I, (unsigned char *)&BAND_PREF_I) &&
					  CmdModule.ReadNVItem(NV_BAND_PREF_16_31_I, NV_BAND_PREF_16_31_I, (unsigned char *)&BAND_PREF_16_31_I) &&
					  CmdModule.ReadNVItem(NV_BAND_PREF_32_63_I, NV_BAND_PREF_32_63_I, (unsigned char *)&BAND_PREF_32_63_I);

				//NV 441 , first byte is NAM, band pref is 16 bit
				BAND_PREF_I = (BAND_PREF_I>>8) & 0X000000000000FFFF;
				//NV 946 , first byte is NAM, band pref is 16 bit
				BAND_PREF_16_31_I = (BAND_PREF_16_31_I>>8) & 0X000000000000FFFF;
				//NV 2954 , first byte is NAM, band pref is 32 bit
				BAND_PREF_32_63_I = (BAND_PREF_32_63_I>>8) & 0X00000000FFFFFFFF;
				//combine all 
				bAllBand = (BAND_PREF_32_63_I<<32) | (BAND_PREF_16_31_I<<16) | BAND_PREF_I;
				
				if( ret )
					break;

				::Sleep(atoi(szRetryPeriod));
			}

			return ret;
		}

	}
}