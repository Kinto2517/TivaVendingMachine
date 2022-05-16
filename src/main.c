#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "inc/tm4c123gh6pm.h"

#define PORTD (GPIO_PORTD_DATA_R)

#define BIT_SET(VAR, BIT_NO) do {VAR |= 1<<BIT_NO;} while (0)
#define BIT_RESET(VAR, BIT_NO) do {VAR &= ~(1<<BIT_NO);} while (0)

#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#define BAUDRATE 600

//#define LCD GPIOB
#define RS 1 // PB0
#define RW 2 // PB1
#define EN 4 // PB2
void delay_3000(int n);
void delay_10(int n);
volatile unsigned long delay;

void LCD_yaz(unsigned char data, unsigned char control);
void LCD_Cmp(unsigned char command);
void LCD_K_Yaz(unsigned char data);
void LCD_Init(void);

void init_port_E(void);
void init_port_F(void);
void init_port_D(void);


void satir2Sil(void);
void satir1Sil(void);

void satirYazdir(char* s);
void satirDegis(char x, char y);

void paraYazdir(int makinedekiPara);
void stringIntYazdir(int makinedekiPara);

int main() {

	int kasa[5];
	int stok[5];
	int para[5];
	char tur[10][10];

	char txt[25][25] = { "20", "20", "10", "30", "5", "1", "kopukleme", "30",
			"15 TL", "2", "yikama", "50", "10 TL", "3", "kurulama", "100",
			"5 TL", "4", "cilalama", "20", "50 TL" };

	kasa[0] = atoi(txt[0]);
	kasa[1] = atoi(txt[1]);
	kasa[2] = atoi(txt[2]);
	kasa[3] = atoi(txt[3]);
	kasa[4] = atoi(txt[4]);


	const char *ayrac = ",";
	int b = 0, p = 0, i;

	for (i = 6; txt[i] != '\0'; i++) {

		if (strcmp(txt[i], "kopukleme") == 0) {
			int x = atoi(txt[i + 1]);
			stok[0] = x;
			char *kelime = strtok(txt[i + 2], ayrac);

			while (kelime != '\0') {

				if (p == 0) {
					float y = atoi(kelime);
					para[0] = y;
					p = 1;
				} else {
					strcpy(tur[0], kelime);
				}

				kelime = strtok('\0', ayrac);
			}

		}

		if (strcmp(txt[i], "yikama") == 0) {
			int x = atoi(txt[i + 1]);
			stok[1] = x;
			char *kelime = strtok(txt[i + 2], ayrac);

			while (kelime != '\0') {

				if (p == 0) {
					float y = atoi(kelime);
					para[1] = y;
					p = 1;
				} else {
					strcpy(tur[1], kelime);
				}

				kelime = strtok('\0', ayrac);
			}

		}

		if (strcmp(txt[i], "kurulama") == 0) {
			int x = atoi(txt[i + 1]);
			stok[2] = x;
			char *kelime = strtok(txt[i + 2], ayrac);

			while (kelime != '\0') {

				if (p == 0) {
					float y = atoi(kelime);
					para[2] = y;
					p = 1;
				} else {
					strcpy(tur[2], kelime);
				}

				kelime = strtok('\0', ayrac);
			}

		}

		if (strcmp(txt[i], "cilalama") == 0) {
			int x = atoi(txt[i + 1]);
			stok[3] = x;
			char *kelime = strtok(txt[i + 2], ayrac);

			while (kelime != '\0') {

				if (p == 0) {
					float y = atoi(kelime);
					para[3] = y;
					p = 1;
				} else {
					strcpy(tur[3], kelime);
				}

				kelime = strtok('\0', ayrac);
			}

			break;
		}
		p = 0;
	}

	int kalanHizmetYedegi[4], kasaYedegi[5];
	for (i = 0; i < 4; i++) {
		kalanHizmetYedegi[i] = stok[i];
	}
	for (i = 0; i < 5; i++) {
		kasaYedegi[i] = kasa[i];
	}

	int randSayi;

	init_port_E();
	init_port_F();
	init_port_D();
	LCD_Init();
	LCD_Cmp(1);
	LCD_Cmp(0x80);

	delay_3000(500);
	Bas: delay_3000(1);

	satirDegis(1, 1);
	satirYazdir("Lutfen Para");
	satirDegis(2, 1);
	satirYazdir("Yatirin");
	delay_3000(800);

	satir2Sil();
	int makinedekiPara = 0, makineParaYedegi = 0;

	int buton1, buton2, buton3, buton4, buton5, buton6, buton7;

	while (1) {

		buton1 = GPIO_PORTE_DATA_R & 0b00001;
		buton2 = GPIO_PORTE_DATA_R & 0b00010;
		buton3 = GPIO_PORTE_DATA_R & 0b00100;
		buton4 = GPIO_PORTE_DATA_R & 0b01000;
		buton5 = GPIO_PORTE_DATA_R & 0b10000;
		buton6 = GPIO_PORTE_DATA_R & 0b100000;
		buton7 = GPIO_PORTE_DATA_R & 0b010000;

		if (buton1 != 0) {
			makinedekiPara = makinedekiPara + 5;
			paraYazdir(makinedekiPara);
			kasa[0]++;
		}

		else if (buton2 != 0) {
			makinedekiPara = makinedekiPara + 10;
			paraYazdir(makinedekiPara);
			kasa[1]++;
		}

		else if (buton3 != 0) {
			makinedekiPara = makinedekiPara + 20;
			paraYazdir(makinedekiPara);
			kasa[2]++;
		} else if (buton4 != 0) {
			makinedekiPara = makinedekiPara + 50;
			paraYazdir(makinedekiPara);
			kasa[3]++;
		} else if (buton5 != 0) {
			makinedekiPara = makinedekiPara + 100;
			paraYazdir(makinedekiPara);
			kasa[4]++;
		} else if (buton6 != 0) {
			break;
		}

		else if (buton7 != 0) {
			goto Bas;

		}

	}

	satirDegis(2, 0);
	satirYazdir("Hangi Urunler:");
	delay_3000(1000);
	satir2Sil();

	makineParaYedegi = makinedekiPara;

	while (1) {

		buton1 = GPIO_PORTE_DATA_R & 0b00001;
		buton2 = GPIO_PORTE_DATA_R & 0b00010;
		buton3 = GPIO_PORTE_DATA_R & 0b00100;
		buton4 = GPIO_PORTE_DATA_R & 0b01000;
		buton5 = GPIO_PORTE_DATA_R & 0b10000;
		buton6 = GPIO_PORTE_DATA_R & 0b100000;

		if (buton1 != 0) {
			if (makinedekiPara >= 15) {
				if (stok[0] != 0) {
					makinedekiPara -= 15;
					paraYazdir(makinedekiPara);
					stok[0] = stok[0] - 1;
				} else {
					satirDegis(2, 0);
					satirYazdir("Hizmet Eksik");
					delay_3000(1000);
					satir2Sil();
				}
			} else {
				satirDegis(2, 0);
				satirYazdir("Paraniz Yetersiz");
				delay_3000(1000);
				satir2Sil();
			}

		}

		else if (buton2 != 0) {
			if (makinedekiPara >= 10) {
				if (stok[1] != 0) {
					makinedekiPara -= 10;
					paraYazdir(makinedekiPara);
					stok[1] = stok[1] - 1;
				} else {
					satirDegis(2, 0);
					satirYazdir("Hizmet Eksik");
					delay_3000(1000);
					satir2Sil();
				}
			} else {
				satirDegis(2, 0);
				satirYazdir("Paraniz Yetersiz");
				delay_3000(1000);
				satir2Sil();
			}

		} else if (buton3 != 0) {
			if (makinedekiPara >= 5) {
				if (stok[2] != 0) {
					makinedekiPara -= 5;
					paraYazdir(makinedekiPara);
					stok[2] = stok[2] - 1;
				} else {
					satirDegis(2, 0);
					satirYazdir("Hizmet Eksik");
					delay_3000(1000);
					satir2Sil();
				}
			} else {
				satirDegis(2, 0);
				satirYazdir("Paraniz Yetersiz");
				delay_3000(1000);
				satir2Sil();
			}

		} else if (buton4 != 0) {
			if (makinedekiPara >= 50) {
				if (stok[3] != 0) {
					makinedekiPara -= 50;
					paraYazdir(makinedekiPara);
					stok[3] = stok[3] - 1;
				} else {
					satirDegis(2, 0);
					satirYazdir("Hizmet Eksik");
					delay_3000(1000);
					satir2Sil();
				}
			} else {
				satirDegis(2, 0);
				satirYazdir("Paraniz Yetersiz");
				delay_3000(1000);
				satir2Sil();
			}
		}

		else if (buton5 != 0)
			break;

		else if (buton6 != 0) {
			makinedekiPara = makineParaYedegi;
			paraYazdir(makinedekiPara);
			stok[0] = kalanHizmetYedegi[0];
			stok[1] = kalanHizmetYedegi[1];
			stok[2] = kalanHizmetYedegi[2];
			stok[3] = kalanHizmetYedegi[3];
		}
	}

	satir2Sil();


	srand(500);
	randSayi = rand() % 4 + 1;

	stringIntYazdir(randSayi);
	satirDegis(2, 4);

	satirYazdir(" Random sayi");

	delay_3000(1000);
	satir2Sil();


	if (randSayi == 2) {

		BIT_SET(PORTD, 3);
		BIT_RESET(PORTD, 2);
		delay_3000(1000);
		satirDegis(2, 0);


		satirYazdir("Para Sikisti");
		satir2Sil();
		delay_3000(1000);

		int tl5 = kasa[0] - kasaYedegi[0];
		int tl10 = kasa[1] - kasaYedegi[1];
		int tl20 = kasa[2] - kasaYedegi[2];
		int tl50 = kasa[3] - kasaYedegi[3];
		int tl100 = kasa[4] - kasaYedegi[4];

		kasa[0] = kasaYedegi[0];
		kasa[1] = kasaYedegi[1];
		kasa[2] = kasaYedegi[2];
		kasa[3] = kasaYedegi[3];
		kasa[4] = kasaYedegi[4];

		stok[0] = kalanHizmetYedegi[0];
		stok[1] = kalanHizmetYedegi[1];
		stok[2] = kalanHizmetYedegi[2];
		stok[3] = kalanHizmetYedegi[3];

		satirDegis(2, 0);
		satirYazdir("Para Iade");
		delay_3000(1000);
		satir2Sil();

		stringIntYazdir(tl100);
		satirDegis(2, 5);
		satirYazdir("tane 100 TL");
		delay_3000(1000);
		satir2Sil();

		stringIntYazdir(tl50);
		satirDegis(2, 3);
		satirYazdir("tane 50 TL");
		delay_3000(1000);
		satir2Sil();

		stringIntYazdir(tl20);
		satirDegis(2, 3);
		satirYazdir("tane 20 TL");
		delay_3000(1000);
		satir2Sil();

		stringIntYazdir(tl10);
		satirDegis(2, 3);
		satirYazdir("tane 10 TL");
		delay_3000(1000);
		satir2Sil();

		stringIntYazdir(tl5);
		satirDegis(2, 3);
		satirYazdir("tane 5 TL");
		delay_3000(1000);
		satir2Sil();

	} else {

		BIT_SET(PORTD, 2);
		BIT_RESET(PORTD, 3);
		satirDegis(2, 0);
		satirYazdir("Basari ile gerceklesti");
		delay_3000(1000);
		satir2Sil();

		if (kasa[0] * 5 + kasa[1] * 10 + kasa[2] * 20 + kasa[3] * 50
				+ kasa[4] * 100 < makinedekiPara) {
			satirDegis(2, 1);
			satirYazdir("Para Ustu Yetersiz");
			delay_3000(1000);
		}

		else {

			if (kasa[4] > 0) {

				int tl100Sayisi = makinedekiPara / 100;
				if (tl100Sayisi != 0) {

					int i, n = tl100Sayisi;
					if (n > kasa[4]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(kasa[4]);
						satirDegis(2, 4);

						satirYazdir("tane 100 TL");
						delay_3000(1000);
						satir2Sil();

						makinedekiPara -= 100 * kasa[4];
						kasa[4] -= kasa[4];
					}

					if (n <= kasa[4]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(n);
						satirDegis(2, 4);
						satirYazdir("tane 100 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara = makinedekiPara % 100;
						kasa[4] -= n;
					}

				}
			}

			if (kasa[3] > 0) {

				int tl50Sayisi = makinedekiPara / 50;
				if (tl50Sayisi != 0) {

					int i, n = tl50Sayisi;
					if (n > kasa[3]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(kasa[3]);
						satirDegis(2, 4);

						satirYazdir("tane 50 TL");
						delay_3000(1000);
						satir2Sil();

						makinedekiPara -= 50 * kasa[3];
						kasa[3] -= kasa[3];
					}

					if (n <= kasa[3]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(n);
						satirDegis(2, 4);
						satirYazdir("tane 50 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara = makinedekiPara % 50;
						kasa[3] -= n;
					}

				}
			}

			if (kasa[2] > 0) {
				int tl25Sayisi = makinedekiPara / 20;
				if (tl25Sayisi != 0) {
					int i, n = tl25Sayisi;
					if (n > kasa[2]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(kasa[2]);
						satirDegis(2, 4);
						satirYazdir("tane 20 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara -= 20 * kasa[2];
						kasa[2] -= kasa[2];
					}
					if (n <= kasa[2]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(n);
						satirDegis(2, 4);
						satirYazdir("tane 20 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara = makinedekiPara % 20;
						kasa[2] -= n;
					}
				}
			}

			if (kasa[1] > 0) {
				int tl10Sayisi = makinedekiPara / 10;
				if (tl10Sayisi != 0) {
					int i, n = tl10Sayisi;
					if (n > kasa[1]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(kasa[1]);
						satirDegis(2, 4);
						satirYazdir("tane 10 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara -= 10 * kasa[1];
						kasa[1] -= kasa[1];
					}
					if (n <= kasa[1]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(n);
						satirDegis(2, 4);
						satirYazdir("tane 10 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara = makinedekiPara % 10;
						kasa[1] -= n;
					}
				}
			}

			if (kasa[0] > 0) {
				int tl5Sayisi = makinedekiPara / 5;
				if (tl5Sayisi != 0) {

					int i, n = tl5Sayisi;
					if (n > kasa[0]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(kasa[0]);
						satirDegis(2, 4);
						satirYazdir("tane 5 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara -= 5 * kasa[0];
						kasa[0] -= kasa[0];
					}
					if (n <= kasa[0]) {
						satirDegis(2, 0);
						satirYazdir("Para Ustu");
						delay_3000(1000);
						satir2Sil();
						stringIntYazdir(n);
						satirDegis(2, 4);
						satirYazdir("tane 5 TL");
						delay_3000(1000);
						satir2Sil();
						makinedekiPara = makinedekiPara % 5;
						kasa[0] -= n;
					}
				}
			}

		}

	}

	BIT_RESET(PORTD, 2);
	BIT_RESET(PORTD, 3);

	satirDegis(2, 0);
	satirYazdir("Kasa");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(kasa[0]);
	satirDegis(2, 4);
	satirYazdir("tane 5 TL");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Kasa");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(kasa[1]);
	satirDegis(2, 4);
	satirYazdir("tane 10 TL");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Kasa");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(kasa[2]);
	satirDegis(2, 4);
	satirYazdir("tane 20 TL");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Kasa");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(kasa[3]);
	satirDegis(2, 4);
	satirYazdir("tane 50 TL");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Kasa");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(kasa[4]);
	satirDegis(2, 4);
	satirYazdir("tane 100 TL");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Stok Durum");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(stok[0]);
	satirDegis(2, 4);
	satirYazdir("tane kopukleme");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Stok Durum");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(stok[1]);
	satirDegis(2, 4);
	satirYazdir("tane yikama");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Stok Durum");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(stok[2]);
	satirDegis(2, 4);
	satirYazdir("tane kurulama");
	delay_3000(1000);
	satir2Sil();

	satirDegis(2, 0);
	satirYazdir("Stok Durum");
	delay_3000(1000);
	satir2Sil();
	stringIntYazdir(stok[3]);
	satirDegis(2, 4);
	satirYazdir("tane cilalama");
	delay_3000(1000);
	satir2Sil();


}

void init_port_E() {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;		// E portunu aktive et
	delay = SYSCTL_RCGC2_R;			// E portunun aktive edilmesini 1 tick bekle
	GPIO_PORTE_DIR_R |= 0x00;			// E portunun giriş çıkışlarını belirle
	GPIO_PORTE_DEN_R |= 0b00111111;			// E portunun pinlerini aktifleştir
}

void init_port_F() {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_DIR_R |= 0x00;
	GPIO_PORTF_DEN_R |= 0xFF;
}

void init_port_D() {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; //port D yi aktiflestir
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTD_DIR_R |= 0x0F; //Port D 3,2,1,0 pinlerini cikis yap
	//GPIO_PORTD_AFSEL_R &= ~0x0F; //Port D 3,2,1,0 pinlerini alternatif fonksiyonu 0 yap
	GPIO_PORTD_DEN_R |= 0x0F; //Port D 3,2,1,0 pinlerini aktiflestir
}

void satir2Sil() {
	satirDegis(2, 1);
	satirYazdir("                   ");
}
void satir1Sil() {
	satirDegis(1, 1);
	satirYazdir("                   ");
}

void satirYazdir(char* s) {

	while (*s)
		LCD_K_Yaz(*s++);
}

void satirDegis(char x, char y) {

	if (x == 1)
		LCD_Cmp(0x80 + ((y - 1) % 16));
	else
		LCD_Cmp(0xC0 + ((y - 1) % 16));
}

void LCD_Init(void) {
	SYSCTL_RCGC2_R |= (1 << 1);
	GPIO_PORTB_DIR_R |= 0xFF;
	GPIO_PORTB_DEN_R |= 0XFF;
	LCD_Cmp(0x28);
	LCD_Cmp(0x06);
	LCD_Cmp(0x01);
	LCD_Cmp(0x0F);

}

void LCD_yaz(unsigned char data, unsigned char control) {
	data &= 0xF0; // son dort bit data
	control &= 0x0F; // ilk dort bit control
	GPIO_PORTB_DATA_R = data | control;
	GPIO_PORTB_DATA_R = data | control | EN;
	delay_10(0);
	GPIO_PORTB_DATA_R = data;

}

void LCD_Cmp(unsigned char command) {
	LCD_yaz(command & 0xF0, 0);
	LCD_yaz(command << 4, 0);

	if (command < 4) {
		delay_3000(2);
	} else
		delay_10(40);
}

void LCD_K_Yaz(unsigned char data) {
	LCD_yaz(data & 0xF0, RS);
	LCD_yaz(data << 4, RS);
	delay_10(40);
}

void delay_3000(int n) {
	int x, y;
	for (x = 0; x < n; x++) {
		for (y = 0; y < 3000; y++) {

		}
	}

}

void delay_10(int n) {
	int x, y;
	for (x = 0; x < n; x++) {
		for (y = 0; y < 10; y++) {

		}
	}
}

void paraYazdir(int makinedekiPara) {

	int toplamTL;
	toplamTL = makinedekiPara / 100;
	char parayiStringYaz[10];

	for (int i = 0; i < 10; i++) {
		parayiStringYaz[i] = '\0';
	}

	if (makinedekiPara < 10) {
		parayiStringYaz[0] = makinedekiPara + '0';
		parayiStringYaz[1] = ' ';
		parayiStringYaz[2] = 'T';
		parayiStringYaz[3] = 'L';

	} else if (makinedekiPara < 100 && makinedekiPara >= 10) {
		parayiStringYaz[0] = (makinedekiPara / 10) + '0';
		parayiStringYaz[1] = (makinedekiPara % 10) + '0';
		parayiStringYaz[2] = ' ';
		parayiStringYaz[3] = 'T';
		parayiStringYaz[4] = 'L';
	}

	else {
		parayiStringYaz[0] = makinedekiPara / 100 + '0';
		makinedekiPara = makinedekiPara % 100;
		parayiStringYaz[1] = makinedekiPara / 10 + '0';
		makinedekiPara = makinedekiPara % 10;
		parayiStringYaz[2] = makinedekiPara + '0';
        parayiStringYaz[3] = ' ';

		satirDegis(2, 0);
		satirYazdir(parayiStringYaz);
	}

	satirDegis(1, 1);
	satirYazdir("Toplam Tutar");
	satirDegis(2, 1);
	satirYazdir(parayiStringYaz);
	delay_3000(500);
	satir1Sil();

}

void stringIntYazdir(int makinedekiPara) {
	char parayiStringYaz[10];

	for (int i = 0; i < 10; i++) {
		parayiStringYaz[i] = '\0';
	}

	int n = makinedekiPara / 10;
	int m = makinedekiPara % 10;

	if (n == 0) {
		parayiStringYaz[0] = m + '0';
		satirDegis(2, 1);
		satirYazdir(parayiStringYaz);
	}

	else if (n != 0 && n < 10) {
		parayiStringYaz[0] = n + '0';
		parayiStringYaz[1] = m + '0';
		satirDegis(2, 0);
		satirYazdir(parayiStringYaz);
	} else {
		parayiStringYaz[0] = makinedekiPara / 100 + '0';
		makinedekiPara = makinedekiPara % 100;
		parayiStringYaz[1] = makinedekiPara / 10 + '0';
		makinedekiPara = makinedekiPara % 10;
		parayiStringYaz[2] = makinedekiPara + '0';
		satirDegis(2, 0);
		satirYazdir(parayiStringYaz);
	}

}


