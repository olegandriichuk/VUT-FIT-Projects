/* Header file with all the essential definitions for a given type of MCU */
#include "MK60D10.h"
#include "symbols.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		20

#define BUTTON_RIGHT 0x400     // bit 10
#define BUTTON_DOWN 0x1000    //  bit 12
#define BUTTON_LEFT 0x8000000 //  bit 27
#define BUTTON_UP 0x4000000 //  bit 26


/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );
}


/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}


/* Conversion of requested column number into the 4-to-16 decoder control.  */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i = 0; i < 4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
		    case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}
void row_select(unsigned int row_num)
{
	if(row_num == 0){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(26));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(26));
	}
	if(row_num == 1){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(24));
	}else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(24));
	}
	if(row_num == 2){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(9));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(9));
	}
	if(row_num == 3){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(25));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(25));
	}
	if(row_num == 4){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(28));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(28));
	}
	if(row_num == 5){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(7));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(7));
	}
	if(row_num == 6){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(27));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(27));
	}
	if(row_num == 7){
		PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(29));
	} else{
		PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(29));
	}
}

void dot_coordinate(unsigned int x, unsigned int y)
{
	if (x >= 0 && x <= 15){
		if(y >= 0 && y <= 7) {
		column_select(x);
		row_select(y);
		}
	}

	delay(500, 3);
}


void show_area(char area[RAWS][MAX_LEN_OF_STR], int len, int k)
{
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < RAWS; j++) {
			if (area[j][i] == '1')
				dot_coordinate(k-i, RAWS-1-j);
		}
	}
}


void str_create(char area[RAWS][MAX_LEN_OF_STR], char* str, int str_len){
	int area_len = 0;
	for(int i = 0; i < str_len; ++i){
		check_symbol(str[i], area, area_len);
		area_len = area_len+COLUMNS;
	}
}

void MCUInit(void)
{
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

void print_message(char* message){
	char area[RAWS][MAX_LEN_OF_STR];
	int str_len = strlen(message);
	int message_display_len = str_len*COLUMNS;
	str_create(area, message, str_len);
	for(int i = 0; i < message_display_len*2;i++){
		show_area(area, message_display_len, i);
	}
}


int main(void)
{
	MCUInit();
	SystemConfig();
	PORTE->PCR[10] = PORT_PCR_MUX(0x01); // RIGHT
	PORTE->PCR[12] = PORT_PCR_MUX(0x01); // DOWN
	PORTE->PCR[27] = PORT_PCR_MUX(0x01); // LEFT
	PORTE->PCR[26] = PORT_PCR_MUX(0x01); // UP

	for (;;) {
    	if ((GPIOE_PDIR & BUTTON_RIGHT) == 0) {
    	    char *message = "Christmas is here!";
    	    print_message(message);
    	}else if ((GPIOE_PDIR & BUTTON_UP) == 0) {
    	    char *message = "Holidays.";
    	    print_message(message);

    	}else if ((GPIOE_PDIR & BUTTON_LEFT) == 0) {
    	    char *message = "My log is xandri07";
    	    print_message(message);
    	}else if ((GPIOE_PDIR & BUTTON_DOWN) == 0) {
    	    char *message = "IMP __ 2023";
    	    print_message(message);
    	}

    }
    /* Never leave main */
    return 0;
}
