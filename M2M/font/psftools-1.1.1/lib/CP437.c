/* This file was autogenerated from the Unicode mapping 
 * table cp437.uni in the format used by the Linux kbd package */

/*  Unicode table for IBM Codepage 437.  Note that there are many more */
/*  substitutions that could be conceived (for example, thick-line */
/*  graphs probably should be replaced with double-line ones, accented */
/*  Latin characters should replaced with their nonaccented versions, */
/*  and some upper case Greek characters could be replaced by Latin), however, */
/*  I have limited myself to the Unicodes used by the kernel ISO 8859-1, */
/*  DEC VT, and IBM CP 437 tables. */
/*  Modified by John Elliott for psftools, to use weak binding where  */
/*  ASCII letters get mapped to symbols. That way, when converting codepage to */
/*  PSF, the symbols in the codepages that do have them will take priority  */
/*  over these substitutes. */
#include <stdio.h>
#include "psflib.h"

static psf_dword values[] = {
0xffff, 0x0000, 0xffff, 0x263a, 0xffff, 0x263b, 0xffff, 0x2665, 
0xffff, 0x2666, 0x25c6, 0xffff, 0x2663, 0xffff, 0x2660, 0xffff, 
0x2022, 0xffff, 0x25d8, 0xffff, 0x25cb, 0xffff, 0x25d9, 0xffff, 
0x2642, 0xffff, 0x2640, 0xffff, 0x266a, 0xffff, 0x266b, 0xffff, 
0x263c, 0xffff, 0x25b6, 0x25ba, 0xffff, 0x25c0, 0x25c4, 0xffff, 
0x2195, 0xffff, 0x203c, 0xffff, 0x00b6, 0xffff, 0x00a7, 0xffff, 
0x25ac, 0xffff, 0x21a8, 0xffff, 0x2191, 0xffff, 0x2193, 0xffff, 
0x2192, 0xffff, 0x2190, 0xffff, 0x221f, 0xffff, 0x2194, 0xffff, 
0x25b2, 0xffff, 0x25bc, 0xffff, 0x0020, 0xffff, 0x0021, 0xffff, 
0x0022, 0x1ffff, 0x00a8, 0xffff, 0x0023, 0xffff, 0x0024, 0xffff, 
0x0025, 0xffff, 0x0026, 0xffff, 0x0027, 0xffff, 0x0028, 0xffff, 
0x0029, 0xffff, 0x002a, 0xffff, 0x002b, 0xffff, 0x002c, 0x1ffff, 
0x00b8, 0xffff, 0x002d, 0x1ffff, 0x00ad, 0xffff, 0x002e, 0xffff, 
0x002f, 0xffff, 0x0030, 0xffff, 0x0031, 0xffff, 0x0032, 0xffff, 
0x0033, 0xffff, 0x0034, 0xffff, 0x0035, 0xffff, 0x0036, 0xffff, 
0x0037, 0xffff, 0x0038, 0xffff, 0x0039, 0xffff, 0x003a, 0xffff, 
0x003b, 0xffff, 0x003c, 0xffff, 0x003d, 0xffff, 0x003e, 0xffff, 
0x003f, 0xffff, 0x0040, 0xffff, 0x0041, 0x1ffff, 0x00c0, 0x00c1, 
0x00c2, 0x00c3, 0xffff, 0x0042, 0xffff, 0x0043, 0x1ffff, 0x00a9, 
0xffff, 0x0044, 0xffff, 0x0045, 0x1ffff, 0x00c8, 0x00ca, 0x00cb, 
0xffff, 0x0046, 0xffff, 0x0047, 0xffff, 0x0048, 0xffff, 0x0049, 
0x1ffff, 0x00cc, 0x00cd, 0x00ce, 0x00cf, 0xffff, 0x004a, 0xffff, 
0x004b, 0x1ffff, 0x212a, 0xffff, 0x004c, 0xffff, 0x004d, 0xffff, 
0x004e, 0xffff, 0x004f, 0x1ffff, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 
0xffff, 0x0050, 0xffff, 0x0051, 0xffff, 0x0052, 0x1ffff, 0x00ae, 
0xffff, 0x0053, 0xffff, 0x0054, 0xffff, 0x0055, 0x1ffff, 0x00d9, 
0x00da, 0x00db, 0xffff, 0x0056, 0xffff, 0x0057, 0xffff, 0x0058, 
0xffff, 0x0059, 0x1ffff, 0x00dd, 0xffff, 0x005a, 0xffff, 0x005b, 
0xffff, 0x005c, 0xffff, 0x005d, 0xffff, 0x005e, 0xffff, 0x005f, 
0xf804, 0xffff, 0x0060, 0xffff, 0x0061, 0x1ffff, 0x00e3, 0xffff, 
0x0062, 0xffff, 0x0063, 0xffff, 0x0064, 0xffff, 0x0065, 0xffff, 
0x0066, 0xffff, 0x0067, 0xffff, 0x0068, 0xffff, 0x0069, 0xffff, 
0x006a, 0xffff, 0x006b, 0xffff, 0x006c, 0xffff, 0x006d, 0xffff, 
0x006e, 0xffff, 0x006f, 0x1ffff, 0x00f5, 0xffff, 0x0070, 0xffff, 
0x0071, 0xffff, 0x0072, 0xffff, 0x0073, 0xffff, 0x0074, 0xffff, 
0x0075, 0xffff, 0x0076, 0xffff, 0x0077, 0xffff, 0x0078, 0x1ffff, 
0x00d7, 0xffff, 0x0079, 0x1ffff, 0x00fd, 0xffff, 0x007a, 0xffff, 
0x007b, 0xffff, 0x007c, 0x1ffff, 0x00a5, 0xffff, 0x007d, 0xffff, 
0x007e, 0xffff, 0x2302, 0xffff, 0x00c7, 0xffff, 0x00fc, 0xffff, 
0x00e9, 0xffff, 0x00e2, 0xffff, 0x00e4, 0xffff, 0x00e0, 0xffff, 
0x00e5, 0xffff, 0x00e7, 0xffff, 0x00ea, 0xffff, 0x00eb, 0xffff, 
0x00e8, 0xffff, 0x00ef, 0xffff, 0x00ee, 0xffff, 0x00ec, 0xffff, 
0x00c4, 0xffff, 0x00c5, 0x212b, 0xffff, 0x00c9, 0xffff, 0x00e6, 
0xffff, 0x00c6, 0xffff, 0x00f4, 0xffff, 0x00f6, 0xffff, 0x00f2, 
0xffff, 0x00fb, 0xffff, 0x00f9, 0xffff, 0x00ff, 0xffff, 0x00d6, 
0xffff, 0x00dc, 0xffff, 0x00a2, 0xffff, 0x00a3, 0xffff, 0x00a5, 
0xffff, 0x20a7, 0xffff, 0x0192, 0xffff, 0x00e1, 0xffff, 0x00ed, 
0xffff, 0x00f3, 0xffff, 0x00fa, 0xffff, 0x00f1, 0xffff, 0x00d1, 
0xffff, 0x00aa, 0xffff, 0x00ba, 0xffff, 0x00bf, 0xffff, 0x2310, 
0xffff, 0x00ac, 0xffff, 0x00bd, 0xffff, 0x00bc, 0xffff, 0x00a1, 
0xffff, 0x00ab, 0xffff, 0x00bb, 0xffff, 0x2591, 0xffff, 0x2592, 
0xffff, 0x2593, 0xffff, 0x2502, 0xffff, 0x2524, 0xffff, 0x2561, 
0xffff, 0x2562, 0xffff, 0x2556, 0xffff, 0x2555, 0xffff, 0x2563, 
0xffff, 0x2551, 0xffff, 0x2557, 0xffff, 0x255d, 0xffff, 0x255c, 
0xffff, 0x255b, 0xffff, 0x2510, 0xffff, 0x2514, 0xffff, 0x2534, 
0xffff, 0x252c, 0xffff, 0x251c, 0xffff, 0x2500, 0xffff, 0x253c, 
0xffff, 0x255e, 0xffff, 0x255f, 0xffff, 0x255a, 0xffff, 0x2554, 
0xffff, 0x2569, 0xffff, 0x2566, 0xffff, 0x2560, 0xffff, 0x2550, 
0xffff, 0x256c, 0xffff, 0x2567, 0xffff, 0x2568, 0xffff, 0x2564, 
0xffff, 0x2565, 0xffff, 0x2559, 0xffff, 0x2558, 0xffff, 0x2552, 
0xffff, 0x2553, 0xffff, 0x256b, 0xffff, 0x256a, 0xffff, 0x2518, 
0xffff, 0x250c, 0xffff, 0x2588, 0xffff, 0x2584, 0xffff, 0x258c, 
0xffff, 0x2590, 0xffff, 0x2580, 0xffff, 0x03b1, 0xffff, 0x03b2, 
0x00df, 0xffff, 0x0393, 0xffff, 0x03c0, 0xffff, 0x03a3, 0xffff, 
0x03c3, 0xffff, 0x00b5, 0x03bc, 0xffff, 0x03c4, 0xffff, 0x03a6, 
0x00d8, 0xffff, 0x0398, 0xffff, 0x03a9, 0x2126, 0xffff, 0x03b4, 
0xffff, 0x221e, 0xffff, 0x03c6, 0x00f8, 0xffff, 0x03b5, 0xffff, 
0x2229, 0xffff, 0x2261, 0xffff, 0x00b1, 0xffff, 0x2265, 0xffff, 
0x2264, 0xffff, 0x2320, 0xffff, 0x2321, 0xffff, 0x00f7, 0xffff, 
0x2248, 0xffff, 0x00b0, 0xffff, 0x2219, 0xffff, 0x00b7, 0xffff, 
0x221a, 0xffff, 0x207f, 0xffff, 0x00b2, 0xffff, 0x25a0, 0xfffd, 
0xffff, 0x00a0, 0xffff, };

struct psf_mapping256 m_CP437 = { "CP437",
256, {
&values[1], /* 0  */
&values[3], /* 1  */
&values[5], /* 2  */
&values[7], /* 3  */
&values[9], /* 4  */
&values[12], /* 5  */
&values[14], /* 6  */
&values[16], /* 7  */
&values[18], /* 8  */
&values[20], /* 9  */
&values[22], /* 10  */
&values[24], /* 11  */
&values[26], /* 12  */
&values[28], /* 13  */
&values[30], /* 14  */
&values[32], /* 15  */
&values[34], /* 16  */
&values[37], /* 17  */
&values[40], /* 18  */
&values[42], /* 19  */
&values[44], /* 20  */
&values[46], /* 21  */
&values[48], /* 22  */
&values[50], /* 23  */
&values[52], /* 24  */
&values[54], /* 25  */
&values[56], /* 26  */
&values[58], /* 27  */
&values[60], /* 28  */
&values[62], /* 29  */
&values[64], /* 30  */
&values[66], /* 31  */
&values[68], /* 32  */
&values[70], /* 33  */
&values[72], /* 34  */
&values[76], /* 35  */
&values[78], /* 36  */
&values[80], /* 37  */
&values[82], /* 38  */
&values[84], /* 39  */
&values[86], /* 40  */
&values[88], /* 41  */
&values[90], /* 42  */
&values[92], /* 43  */
&values[94], /* 44  */
&values[98], /* 45  */
&values[102], /* 46  */
&values[104], /* 47  */
&values[106], /* 48  */
&values[108], /* 49  */
&values[110], /* 50  */
&values[112], /* 51  */
&values[114], /* 52  */
&values[116], /* 53  */
&values[118], /* 54  */
&values[120], /* 55  */
&values[122], /* 56  */
&values[124], /* 57  */
&values[126], /* 58  */
&values[128], /* 59  */
&values[130], /* 60  */
&values[132], /* 61  */
&values[134], /* 62  */
&values[136], /* 63  */
&values[138], /* 64  */
&values[140], /* 65  */
&values[147], /* 66  */
&values[149], /* 67  */
&values[153], /* 68  */
&values[155], /* 69  */
&values[161], /* 70  */
&values[163], /* 71  */
&values[165], /* 72  */
&values[167], /* 73  */
&values[174], /* 74  */
&values[176], /* 75  */
&values[180], /* 76  */
&values[182], /* 77  */
&values[184], /* 78  */
&values[186], /* 79  */
&values[193], /* 80  */
&values[195], /* 81  */
&values[197], /* 82  */
&values[201], /* 83  */
&values[203], /* 84  */
&values[205], /* 85  */
&values[211], /* 86  */
&values[213], /* 87  */
&values[215], /* 88  */
&values[217], /* 89  */
&values[221], /* 90  */
&values[223], /* 91  */
&values[225], /* 92  */
&values[227], /* 93  */
&values[229], /* 94  */
&values[231], /* 95  */
&values[234], /* 96  */
&values[236], /* 97  */
&values[240], /* 98  */
&values[242], /* 99  */
&values[244], /* 100  */
&values[246], /* 101  */
&values[248], /* 102  */
&values[250], /* 103  */
&values[252], /* 104  */
&values[254], /* 105  */
&values[256], /* 106  */
&values[258], /* 107  */
&values[260], /* 108  */
&values[262], /* 109  */
&values[264], /* 110  */
&values[266], /* 111  */
&values[270], /* 112  */
&values[272], /* 113  */
&values[274], /* 114  */
&values[276], /* 115  */
&values[278], /* 116  */
&values[280], /* 117  */
&values[282], /* 118  */
&values[284], /* 119  */
&values[286], /* 120  */
&values[290], /* 121  */
&values[294], /* 122  */
&values[296], /* 123  */
&values[298], /* 124  */
&values[302], /* 125  */
&values[304], /* 126  */
&values[306], /* 127  */
&values[308], /* 128  */
&values[310], /* 129  */
&values[312], /* 130  */
&values[314], /* 131  */
&values[316], /* 132  */
&values[318], /* 133  */
&values[320], /* 134  */
&values[322], /* 135  */
&values[324], /* 136  */
&values[326], /* 137  */
&values[328], /* 138  */
&values[330], /* 139  */
&values[332], /* 140  */
&values[334], /* 141  */
&values[336], /* 142  */
&values[338], /* 143  */
&values[341], /* 144  */
&values[343], /* 145  */
&values[345], /* 146  */
&values[347], /* 147  */
&values[349], /* 148  */
&values[351], /* 149  */
&values[353], /* 150  */
&values[355], /* 151  */
&values[357], /* 152  */
&values[359], /* 153  */
&values[361], /* 154  */
&values[363], /* 155  */
&values[365], /* 156  */
&values[367], /* 157  */
&values[369], /* 158  */
&values[371], /* 159  */
&values[373], /* 160  */
&values[375], /* 161  */
&values[377], /* 162  */
&values[379], /* 163  */
&values[381], /* 164  */
&values[383], /* 165  */
&values[385], /* 166  */
&values[387], /* 167  */
&values[389], /* 168  */
&values[391], /* 169  */
&values[393], /* 170  */
&values[395], /* 171  */
&values[397], /* 172  */
&values[399], /* 173  */
&values[401], /* 174  */
&values[403], /* 175  */
&values[405], /* 176  */
&values[407], /* 177  */
&values[409], /* 178  */
&values[411], /* 179  */
&values[413], /* 180  */
&values[415], /* 181  */
&values[417], /* 182  */
&values[419], /* 183  */
&values[421], /* 184  */
&values[423], /* 185  */
&values[425], /* 186  */
&values[427], /* 187  */
&values[429], /* 188  */
&values[431], /* 189  */
&values[433], /* 190  */
&values[435], /* 191  */
&values[437], /* 192  */
&values[439], /* 193  */
&values[441], /* 194  */
&values[443], /* 195  */
&values[445], /* 196  */
&values[447], /* 197  */
&values[449], /* 198  */
&values[451], /* 199  */
&values[453], /* 200  */
&values[455], /* 201  */
&values[457], /* 202  */
&values[459], /* 203  */
&values[461], /* 204  */
&values[463], /* 205  */
&values[465], /* 206  */
&values[467], /* 207  */
&values[469], /* 208  */
&values[471], /* 209  */
&values[473], /* 210  */
&values[475], /* 211  */
&values[477], /* 212  */
&values[479], /* 213  */
&values[481], /* 214  */
&values[483], /* 215  */
&values[485], /* 216  */
&values[487], /* 217  */
&values[489], /* 218  */
&values[491], /* 219  */
&values[493], /* 220  */
&values[495], /* 221  */
&values[497], /* 222  */
&values[499], /* 223  */
&values[501], /* 224  */
&values[503], /* 225  */
&values[506], /* 226  */
&values[508], /* 227  */
&values[510], /* 228  */
&values[512], /* 229  */
&values[514], /* 230  */
&values[517], /* 231  */
&values[519], /* 232  */
&values[522], /* 233  */
&values[524], /* 234  */
&values[527], /* 235  */
&values[529], /* 236  */
&values[531], /* 237  */
&values[534], /* 238  */
&values[536], /* 239  */
&values[538], /* 240  */
&values[540], /* 241  */
&values[542], /* 242  */
&values[544], /* 243  */
&values[546], /* 244  */
&values[548], /* 245  */
&values[550], /* 246  */
&values[552], /* 247  */
&values[554], /* 248  */
&values[556], /* 249  */
&values[558], /* 250  */
&values[560], /* 251  */
&values[562], /* 252  */
&values[564], /* 253  */
&values[566], /* 254  */
&values[569], /* 255  */
}};
