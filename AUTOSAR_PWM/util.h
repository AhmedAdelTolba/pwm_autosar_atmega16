/*
 * util.h
 *
 *  Created on: Apr 14, 2016
 *      Author: Ahmed
 */

#ifndef SHARED_LIBRARIES_UTIL_H_
#define SHARED_LIBRARIES_UTIL_H_

#define CONC(x0,x1,x2,x3,x4,x5,x6,x7)         CONC_HELPER(x0,x1,x2,x3,x4,x5,x6,x7)
#define CONC_HELPER(y0,y1,y2,y3,y4,y5,y6,y7)  0b##y0##y1##y2##y3##y4##y5##y6##y7
#define GET_BIT(Reg,Bit_no)                   (((Reg)>>(Bit_no))&1)
#define CLR_BIT(Reg,Bit_no)                   ((Reg)&=(~(1<<(Bit_no))))
#define SET_BIT(Reg,Bit_no)                   ((Reg)|=(1<<(Bit_no)))
#define WRITE_BIT(Reg,Bit_no,Bit_val)        do{ switch (Bit_val) {\
		case 1:\
			SET_BIT(Reg, Bit_no);\
			break;\
		case 0:\
			CLR_BIT(Reg, Bit_no);\
			break;\
		default:\
			break;\
		}}while(0)

#endif /* SHARED_LIBRARIES_UTIL_H_ */
