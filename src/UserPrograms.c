#include "UserPrograms.h"

/*
 * HelloWorld()
 * Simple program to illustrate output
 */
void HelloWorld() {
	putsln("\nHello World!");
}

/*
 * Scroller(char* msg)
 * Execute Scroller and set Malta contents to msg
 * @param char* msg - New malta text
 */
void Scroller(char* msg) {	
	int i = 0, count = 0, strEnd = -1;
	
	for (i = 0; i <= 7; i++) {
		if (msg[i] == '\0') {
			strEnd = i;
			break;
		} else
			maltaText[i] = msg[i];
	}
	
	if (strEnd == -1)
		strEnd = 8;
		
	for (i = strEnd; i <= 7; i++)
		maltaText[i] = ' ';
	
	while (1) {
		displayS((uint32_t)maltaText, (uint8_t)(count%8) );
		sleep(2, SCROLLWAIT);
		count = (count - 1) % 8;
	}
}

/*
 * Increment (int a)
 * Increment all numbers up to a
 * @param int a - Upper limit
 */
void Increment(int a) {
	char buf[10];
	puts("\nIncrement to ");
	putsln(itoa(a, buf, 10));
	_increment(a);
	
	putsln("Increment done!");
}

/*
 * Fibonacci (int a)
 * Sum Fibonacci numbers up to a
 * @param int a - Upper limit
 */
void Fibonacci(int a) {
	char buf[10];
	puts("\nFibonacci to ");
	putsln(itoa(a, buf, 10));
	_fibonacci(1, 1, a);
}

/*
 * _fibonacci
 * Help function for Fibonacci
 * "Fibonacci" up to i
 * @param int n_1 - Previous sum
 * @param int n_2 - New sum
 * @param int i - Limiter
 * @return New sum
 */
int _fibonacci(int n_1, int n_2, int i) {
	char buf[10];
	
	puts("Fib: ");
	putsln(itoa(n_1+n_2, buf, 10));
	
	if (i <= 0)
		return 0;
	
	i--;
	_fibonacci(n_2, n_1+n_2, i);
	
	return 0;
}

/*
 * _increment(int n)
 * Help function for Increment, iterate up to n
 * @param int n - Int to iterate up to
 */
void _increment(int n) {
	int i;
	char buf[10];

	for (i = 0; i <= n; i++) {
		puts("Inc: ");
		putsln(itoa(i, buf, 10));
	}
}

/*
 * initUserPrograms()
 * Initialize our main "user programs" that the user can execute
 * Setup all programs addresses and titles
 */
void initUserPrograms() {
	userProgramsAddresses[0] = (int)&HelloWorld;
	userProgramsAddresses[1] = (int)&Scroller;
	userProgramsAddresses[2] = (int)&Increment;
	userProgramsAddresses[3] = (int)&Fibonacci;
	userProgramsAddresses[4] = (int)&Shell;
	userProgramsAddresses[5] = (int)&ASCII;
	
	userProgramsNames[0] = "HelloWorld";
	userProgramsNames[1] = "Scroller";
	userProgramsNames[2] = "Increment";
	userProgramsNames[3] = "Fibonacci";
	userProgramsNames[4] = "Shell";
	userProgramsNames[5] = "ASCII";
}

/*
 * ASCII()
 * Easter egg
 */
void ASCII() {
	putsln("                 ____________");
	putsln("            ,ad8888888888888888ba,");
	putsln("          ad88888888888888888888888a,");
	putsln("        a88888\\\"8888888888888888888888,");
	putsln("      ,8888\"\\ \\ \\\"P88888888888888888888b,");
	putsln("      d88 \\ \\       `\"\"P88888888888888888,");
	putsln("     ,8888b               \"\"88888888888888,");
	putsln("     d8P'''  ,aa,              \"\"888888888b");
	putsln("     888bbdd888888ba,  ,I         \"88888888,");
	putsln("     8888888888888888ba8\"         ,88888888b");
	putsln("    ,888888888888888888b,        ,8888888888");
	putsln("    (88888888888888888888,      ,88888888888,");
	putsln("    d888888888888888888888,    ,8   \"8888888b");
	putsln("    88888888888888888888888  .;8'\"\"\"  (888888");
	putsln("    8888888888888I\"8888888P ,8\" ,aaa,  888888");
	putsln("    888888888888I:8888888\" ,8\"  `b8d'  (88888");
	putsln("    Y8888888888I'888888P' ,8) \\         88888");
	putsln("    `88888888I\"  8888P'  ,8\")  \\        88888");
	putsln("     8888888I'   888\"   ,8\" (._.)       88888");
	putsln("     Y8888I\"     \"88,  ,8\"             ,8888P");
	putsln("     `888I'       \"P8 ,8\"   ____      ,88888)");
	putsln("     (88I'          \",8\"  M\"\"\"\"\"\"M   ,888888'");
	putsln("    ,8I\"            ,8(    \"aaaa\"   ,8888888");
	putsln("   ,8I'            ,888a           ,8888888)");
	putsln("  ,8I'            ,888888,       ,888888888");
	putsln(" ,8I'            ,8888888'`-===-'888888888'");
	putsln(",8I'            ,8888888888,     888888888P");
	putsln("8I'            ,8\"\"888888888a    \"88888888'");
	putsln("8I            ,8'  88888888888a,  `P88888'");
	putsln("8I           ,8I   `8888888888888a  \"P88'");
	putsln("(8,         ,8P'    `88888888888888a   \"\"I8bma,.");
	putsln(" 8I        ,8P'       \"88888888888888a       \"\"P8ma,");
	putsln(" (8,      ,8d\"          `\"8888888888888a,        `\"8a");
	putsln("  8I     ,8dP             `88888) \"888888b,   .    :8b");
	putsln("  (8    ,8dP'  ,I         ,88888    \"888888,  8,    8)");
	putsln("   8,   8dP'  ,I          88888'  8,  \"88888  \"8,  ,8");
	putsln("   8I   8P'  ,I           \"888'   `8, ,8888'    \"8,I8");
	putsln("   I8,  \"   ,I             \"88     `8,8888\"       \"8I,");
	putsln("   `8I     ,I'               \"      `8\"88'          \"8,");
	putsln("    8I     (8          ,aaa,         8 88,            \"8,");
	putsln("   ,8I     8I          88W88         8 \"88             \"8,");
	putsln("   d8I,    I8          88M88         8  \"8              ,8,");
	putsln("   888I    \"8,          \"\"\"          8                 (88W");
	putsln("   8888,    I8,                     ,8I                (88M");
	putsln("  ,8888I     Pb,                   ,8'8,                `8\"");
	putsln("  d88888,     Pb,                 ,8' `8,              ,8\"");
	putsln("  888888I      \"Pa              ,aP'   `8,            ,8\"");
	putsln("  8888888,      `Iba.         ,aP\"      `8a          a8\"");
	putsln("  8888888I,       \"I8bmmmmmmdP\"           \"8a      ,8\"");
	putsln("  (8888888I,         \"\"\"\"\"\"       .         \"8mmmmP\"");
	putsln("   88888888I,                   ,8I   8,       I8\"");
	putsln("    \"\"\"88888I,                ,8I'    \"I8,    ;8\"");
	putsln("           `8I,             ,8I'       `I8,   8)");
	putsln("            `8I,           ,8I'          I8  :8'");
	putsln("             `8I,         ,8I'           I8  :8");
	putsln("              `8I       ,8I'             `8  (8");
	putsln("               8I     ,8I'                8  (8;");
	putsln("               8I    ,8\"                  I   88,");
	putsln("              .8I   ,8'                       8\"8,");
	putsln("              (PI   '8                        8,`8,");
	putsln("             .8I'            ,@@              `8,`8,");
	putsln("             (PI             @@@               `8,`8,");
	putsln("            (RI'  .          @@'        .       \"b `8,");
	putsln("           .BI'  ,I                     8I,      \"a `8,");
	putsln("          .8I'  ,8'                     ,8I       9, `8,");
	putsln("         .8I'  ,8'                     ,8I'       `8  `8,");
	putsln("        .8I'  ,8'                     ,8I'        ,8   `8,");
	putsln("        d8'   8'                    ,dI\"          8  ,  `8,");
	putsln("       (8I    8                   ,dI\"            8  II  `8");
	putsln("       8I                        ,8I'            (8  8)   8;");
	putsln("      (8'                       .8\"              (8  8)   8I");
	putsln("      8I      :                 :                 8, (8   :8");
	putsln("     (8'       V,              :                  `8, 8    8");
	putsln("     8I        `V,           ,8                    `8,8   ;8");
	putsln("     8'          Va,       ,a8'                     `8I  ,8'");
	putsln("     8            V8baaaaad88'                       8' ,8'");
	putsln("     8             V88888888'                        8 ,8'");
	putsln("     8              V888888\"                        d' 8'");
	putsln("     8               V88888                         8 8'");
	putsln("     8                \"V88'                         \"8\"");
	putsln("     8,                 V8                           8");
	putsln("     8I                ,8'                          d)");
	putsln("     `8,               d8                          ,8");
	putsln("      (b               8'                         ,8'");
	putsln("       8,             dP                         ,8'");
	putsln("       (b             8'                        ,8'");
	putsln("        8,           d8                        ,8'");
	putsln("        (b           8'                       ,8'");
	putsln("         8,         a8                       ,8'");
	putsln("         (b         8'                      ,8'");
	putsln("          8,       ,8                      ,8'");
	putsln("          (b       8'                     ,8'");
	putsln("           8,     ,8                     ,8'");
	putsln("           (b     8'                    ,8'");
	putsln("            8,   d8                    ,8'");
	putsln("            (b  ,8'                   ,8'");
	putsln("             8,,I8                   ,8'");
	putsln("             I8I8'                  ,8'");
	putsln("             `I8I                  ,8'");
	putsln("              I8'                 ,8'");
	putsln("              \"8                 ,8'");
	putsln("              (8                ,8'");
	putsln("              8I               ,8'");
	putsln("              (b,   8,        ,8)");
	putsln("              `8I   \"88      ,8i8,");
	putsln("               (b,          ,8\"8\")");
	putsln("               `8I  ,8      8) 8 8");
	putsln("                8I  8I      \"  8 8");
	putsln("                (b  8I         8 8");
	putsln("                `8  (8,        b 8,");
	putsln("                 8   8)        \"b\"8,");
	putsln("                 8   8(         \"b\"8");
	putsln("                 8   \"I          \"b8,");
	putsln("                 8                `8)");
	putsln("                 8                 I8");
	putsln("                 8                 (8");
	putsln("                 8,                 8,");
	putsln("                 Ib                 8)");
	putsln("                 (8                 I8");
	putsln("                  8                 I8");
	putsln("                  8                 I8");
	putsln("                  8,                I8");
	putsln("                  Ib                8I");
	putsln("                  (8               (8'");
	putsln("                   8               I8");
	putsln("                   8,              8I");
	putsln("                   Ib             (8'");
	putsln("                   (8             I8");
	putsln("                   `8             8I");
	putsln("                    8            (8'");
	putsln("                    8,           I8");
	putsln("                    Ib           8I");
	putsln("                    (8           8'");
	putsln("                     8,         (8");
	putsln("                     Ib         I8");
	putsln("                     (8         8I");
	putsln("                      8,        8'");
	putsln("                      (b       (8");
	putsln("                       8,      I8");
	putsln("                       I8      I8");
	putsln("                       (8      I8");
	putsln("                        8      I8,");
	putsln("                        8      8 8,");
	putsln("                        8,     8 8'");
	putsln("                       ,I8     \"8\"");
	putsln("                      ,8\"8,     8,");
	putsln("                     ,8' `8     `b");
	putsln("                    ,8'   8      8,");
	putsln("                   ,8'    (a     `b");
	putsln("                  ,8'     `8      8,");
	putsln("                  I8/      8      `b,");
	putsln("                  I8-/     8       `8,");
	putsln("                  (8/-/    8        `8,  ");
	putsln("                   8I/-/  ,8         `8  ");
	putsln("                   `8I/--,I8        \\-8) ");
	putsln("                    `8I,,d8I       \\-\\8) ");
	putsln("                      \"bdI\"8,     \\-\\I8  ");
	putsln("                           `8,   \\-\\I8'  ");
	putsln("                            `8,,--\\I8'   ");
	putsln("                             `Ib,,I8'     ");
	putsln("                              `I8I'");
}
