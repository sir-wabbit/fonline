/* .BMP header, including the palette */

unsigned char bmpheader[] = {
	0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x04, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x63, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFD, 0xFD, 0xFD, 0x00, 0xF0, 0xF0,
	0xF0, 0x00, 0xE1, 0xE1, 0xE1, 0x00, 0xD2, 0xD2, 0xD2, 0x00, 0xC5, 0xC5, 0xC5, 0x00, 0xB3, 0xB3,
	0xB3, 0x00, 0x9E, 0x9E, 0x9E, 0x00, 0x8D, 0x8D, 0x8D, 0x00, 0x72, 0x72, 0x72, 0x00, 0x50, 0x50,
	0x50, 0x00, 0x3B, 0x3B, 0x3B, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFD, 0xFD, 0xFF, 0x00, 0xF0, 0xF0, 0xFF, 0x00, 0xE1, 0xE1, 0xFD, 0x00, 0xD2, 0xD2,
	0xF3, 0x00, 0xC1, 0xC1, 0xE5, 0x00, 0xAE, 0xAE, 0xD6, 0x00, 0x9E, 0x9E, 0xCA, 0x00, 0x87, 0x87,
	0xB8, 0x00, 0x72, 0x72, 0xA4, 0x00, 0x59, 0x59, 0x93, 0x00, 0x46, 0x46, 0x79, 0x00, 0x22, 0x22,
	0x59, 0x00, 0x03, 0x03, 0x3B, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFD, 0xFD, 0x00, 0xFF, 0xF0, 0xF0, 0x00, 0xFD, 0xE1, 0xE1, 0x00, 0xF3, 0xD2,
	0xD2, 0x00, 0xE5, 0xC1, 0xC1, 0x00, 0xD6, 0xAE, 0xAE, 0x00, 0xCA, 0x9E, 0x9E, 0x00, 0xB8, 0x87,
	0x87, 0x00, 0xA4, 0x72, 0x72, 0x00, 0x93, 0x59, 0x59, 0x00, 0x79, 0x46, 0x46, 0x00, 0x59, 0x22,
	0x22, 0x00, 0x3B, 0x03, 0x03, 0x00, 0xFF, 0xFD, 0xFF, 0x00, 0xF6, 0xAE, 0xFF, 0x00, 0xAE, 0x46,
	0xB8, 0x00, 0x8D, 0x12, 0x93, 0x00, 0x6A, 0x00, 0x72, 0x00, 0x46, 0x03, 0x50, 0x00, 0x46, 0x00,
	0x46, 0x00, 0x22, 0x00, 0x2F, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xCE, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0x2F, 0xFF, 0xFF, 0x00, 0x50, 0xEC, 0xFF, 0x00, 0x62, 0xDA, 0xF3, 0x00, 0x46, 0xCA,
	0xE1, 0x00, 0x22, 0xB8, 0xCE, 0x00, 0x03, 0xA4, 0xBD, 0x00, 0x00, 0x8D, 0xA4, 0x00, 0x00, 0x72,
	0x8D, 0x00, 0x00, 0x50, 0x6A, 0x00, 0x00, 0x22, 0x3B, 0x00, 0xEC, 0xFF, 0xFF, 0x00, 0xD6, 0xFF,
	0xFF, 0x00, 0xC1, 0xFF, 0xE9, 0x00, 0xA9, 0xE9, 0xCA, 0x00, 0x8D, 0xCA, 0xA9, 0x00, 0x6A, 0xA4,
	0x80, 0x00, 0x3B, 0x72, 0x50, 0x00, 0x99, 0xAE, 0xC1, 0x00, 0x6A, 0x8D, 0x9E, 0x00, 0x3B, 0x62,
	0x72, 0x00, 0x99, 0xCA, 0xB8, 0x00, 0x3B, 0xCA, 0xC1, 0x00, 0x50, 0xB8, 0xC1, 0x00, 0x46, 0xAE,
	0xAE, 0x00, 0x46, 0x87, 0x93, 0x00, 0x3B, 0x62, 0x72, 0x00, 0xEC, 0xFA, 0xEC, 0x00, 0xCA, 0xE5,
	0xCA, 0x00, 0xA4, 0xCE, 0xA4, 0x00, 0x80, 0xB8, 0x80, 0x00, 0xA4, 0xA4, 0x72, 0x00, 0x8D, 0x93,
	0x62, 0x00, 0x79, 0x87, 0x50, 0x00, 0x59, 0x79, 0x3B, 0x00, 0x46, 0x62, 0x2F, 0x00, 0x22, 0x50,
	0x12, 0x00, 0x03, 0x3B, 0x03, 0x00, 0x22, 0x62, 0x22, 0x00, 0x00, 0x46, 0x03, 0x00, 0x00, 0x2F,
	0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0xEC, 0xEC, 0xDE, 0x00, 0xE9, 0xE5,
	0xCA, 0x00, 0xE5, 0xDA, 0xB3, 0x00, 0xE1, 0xCE, 0x99, 0x00, 0xDE, 0xBD, 0x80, 0x00, 0xDE, 0xA4,
	0x62, 0x00, 0xCE, 0x93, 0x59, 0x00, 0xBD, 0x87, 0x50, 0x00, 0xA9, 0x72, 0x3B, 0x00, 0x93, 0x62,
	0x2F, 0x00, 0x80, 0x50, 0x22, 0x00, 0xF3, 0xF3, 0xEC, 0x00, 0xB8, 0x8D, 0x72, 0x00, 0xA4, 0xA4,
	0x99, 0x00, 0xD6, 0xB8, 0xA4, 0x00, 0x99, 0x80, 0x72, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xE9, 0xF3,
	0xFA, 0x00, 0xCE, 0xE1, 0xF0, 0x00, 0xAE, 0xCE, 0xE5, 0x00, 0x93, 0xB8, 0xDA, 0x00, 0x6A, 0xA4,
	0xCE, 0x00, 0x46, 0x8D, 0xC1, 0x00, 0x12, 0x79, 0xB3, 0x00, 0x00, 0x62, 0xA4, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFD, 0xFD, 0xFF, 0x00, 0xE9, 0xE9, 0xFF, 0x00, 0xCE, 0xCE, 0xFF, 0x00, 0xB3, 0xB3,
	0xFF, 0x00, 0x8D, 0x8D, 0xFF, 0x00, 0x62, 0x62, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xF6, 0x00, 0x00, 0x00, 0xE1, 0x00, 0x00, 0x00,
	0xC5, 0x00, 0x00, 0x00, 0xA4, 0x00, 0x00, 0x00, 0x80, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xE5, 0xFF,
	0xFF, 0x00, 0xCA, 0xFF, 0xFF, 0x00, 0xAE, 0xFA, 0xFF, 0x00, 0x8D, 0xEC, 0xFF, 0x00, 0x59, 0xE5,
	0xFF, 0x00, 0x12, 0xDA, 0xFF, 0x00, 0x00, 0xCE, 0xFF, 0x00, 0x00, 0xBD, 0xFF, 0x00, 0x00, 0xAE,
	0xFF, 0x00, 0x00, 0x99, 0xF3, 0x00, 0x00, 0x87, 0xD6, 0x00, 0x00, 0x6A, 0xB8, 0x00, 0x00, 0x46,
	0x93, 0x00, 0x00, 0x22, 0x62, 0x00, 0xF3, 0xFF, 0xFF, 0x00, 0xCA, 0xFD, 0xFF, 0x00, 0xB3, 0xF0,
	0xFF, 0x00, 0x9E, 0xE1, 0xFF, 0x00, 0x87, 0xD2, 0xFA, 0x00, 0x6A, 0xC5, 0xEC, 0x00, 0x50, 0xB3,
	0xDE, 0x00, 0x2F, 0xA4, 0xCE, 0x00, 0x12, 0x93, 0xC1, 0x00, 0x00, 0x80, 0xAE, 0x00, 0x00, 0x6A,
	0x99, 0x00, 0x00, 0x50, 0x80, 0x00, 0x00, 0x3B, 0x6A, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xE9, 0xFF,
	0xFF, 0x00, 0xCE, 0xFA, 0xFF, 0x00, 0xB3, 0xE1, 0xFF, 0x00, 0x93, 0xC5, 0xFD, 0x00, 0x72, 0xA9,
	0xF0, 0x00, 0x59, 0x93, 0xE1, 0x00, 0x3B, 0x79, 0xD6, 0x00, 0x22, 0x59, 0xCA, 0x00, 0x03, 0x3B,
	0xBD, 0x00, 0x00, 0x12, 0xA9, 0x00, 0x00, 0x00, 0x8D, 0x00, 0x00, 0x00, 0x79, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xF0, 0xFF, 0xFF, 0x00, 0xD6, 0xFD, 0xFF, 0x00, 0xBD, 0xF0,
	0xFF, 0x00, 0xA9, 0xE5, 0xFF, 0x00, 0x9E, 0xD2, 0xFF, 0x00, 0x8D, 0xC1, 0xFF, 0x00, 0x80, 0xAE,
	0xF6, 0x00, 0x72, 0x99, 0xE1, 0x00, 0x62, 0x80, 0xCA, 0x00, 0x46, 0x62, 0xAE, 0x00, 0x2F, 0x46,
	0x8D, 0x00, 0x12, 0x22, 0x72, 0x00, 0xB3, 0xFF, 0xB3, 0x00, 0x12, 0xE9, 0x12, 0x00, 0x00, 0xF3,
	0x00, 0x00, 0x8D, 0x99, 0x99, 0x00, 0x00, 0xBD, 0x00, 0x00, 0xD6, 0xDE, 0xDE, 0x00, 0x2F, 0x2F,
	0x2F, 0x00, 0x72, 0x99, 0xB8, 0x00, 0x3B, 0x50, 0x62, 0x00, 0xAE, 0xC1, 0xDE, 0x00, 0x50, 0x72,
	0x8D, 0x00, 0x03, 0x03, 0x03, 0x00, 0x79, 0x79, 0x79, 0x00, 0xBD, 0xC5, 0xBD, 0x00, 0xCA, 0xD6,
	0xCA, 0x00, 0xDA, 0xE5, 0xDA, 0x00, 0xE5, 0xF3, 0xE5, 0x00, 0xAE, 0xB8, 0xA4, 0x00, 0xB8, 0xC1,
	0xAE, 0x00, 0x00, 0xFF, 0x79, 0x00, 0x00, 0xFF, 0x72, 0x00, 0x03, 0xFF, 0x6A, 0x00, 0x12, 0xE5,
	0x62, 0x00, 0x22, 0xC5, 0x50, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xDA, 0xFF,
	0xFF, 0x00, 0x99, 0xCE, 0xE9, 0x00, 0x79, 0xA4, 0xB8, 0x00, 0x46, 0x80, 0x99, 0x00, 0x2F, 0x50,
	0x6A, 0x00, 0x00, 0x03, 0x22, 0x00, 0x03, 0x03, 0x03, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00
};
