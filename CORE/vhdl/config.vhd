-- Dennis' notes
--
-- /root/Xilinx/Vivado/2022.2/bin/vivado
-- ./bit2core mega65r3 CORE-R3.runs/impl_1/CORE_R3.bit MyFirstM2M V1 mfm2m.cor
--
-- OPTM_SIZE   = number of lines in menu, excluding frame
-- OPTM_DX     = number of characters in menu, excluding frame
-- 15 menues selectable
-- Font for frame is Anikki-16x16
-- https://dwarffortresswiki.org/Tileset_repository#Anikki_square_16x16.png
--
-- Changing anything in menus then check:
-- OPTM_SIZE
-- OPTM_ITEMS
-- OPTM_GROUPS
--
-- Keyboard repeat delay
-- M2M$TYPEMATIC_DLY
--
-- Chars from 1-254 (0, 255 and " excluded):
--   "☺☻♥♦♣♠•◘○◙♂♀♪♫☼\n" &
--   "►◄↕‼¶§▬↨↑↓→←∟↔▲▼\n" &
--   " !#$%&'()*+,-./\n" &
--   "0123456789:;<=>?\n" &
--   "@ABCDEFGHIJKLMNO\n" &
--   "PQRSTUVWXYZ[\]^_\n" &
--   "`abcdefghijklmno\n" &
--   "pqrstuvwxyz{|}~⌂\n" &
--   "ÇüéâäàåçêëèïîìÄÅ\n" &
--   "É¢ÆôöòûùÿÖÜ¢£¥₧ƒ\n" &
--   "áíóúñÑªº¿⌐¬½¼¡«»\n" &
--   "░▒▓│┤╡╢╖╕╣║╗╝╜╛╡\n" &
--   "└┴┬├─┼╞╟╚╔╩╦╠═╬╧\n" &
--   "╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀\n" &
--   "αßΓπΣσµτΦΘΩδ∞φε∩\n" &
--   "≡±≥≤⌠⌡÷≈°∙·√ⁿ²≤\n" &
--

----------------------------------------------------------------------------------
-- MiSTer2MEGA65 Framework
--
-- Configuration data for the Shell
--
-- MiSTer2MEGA65 done by sy2002 and MJoergen in 2022 and licensed under GPL v3
----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity config is
port (
   clk_i       : in std_logic;

   -- bits 27 .. 12:    select configuration data block; called "Selector" hereafter
   -- bits 11 downto 0: address the up to 4k the configuration data
   address_i   : in std_logic_vector(27 downto 0);

   -- config data
   data_o      : out std_logic_vector(15 downto 0)
);
end config;

architecture beh of config is

--------------------------------------------------------------------------------------------------------------------
-- String and character constants (specific for the Anikki-16x16 font)
--------------------------------------------------------------------------------------------------------------------

-- !!! DO NOT TOUCH !!!
constant CHR_LINE_1  : character := character'val(196);
constant CHR_LINE_5  : string := CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1;
constant CHR_LINE_10 : string := CHR_LINE_5 & CHR_LINE_5;
constant CHR_LINE_50 : string := CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10;

--------------------------------------------------------------------------------------------------------------------
-- Welcome and Help Screens (Selectors 0x1000 .. 0x1FFF)
--------------------------------------------------------------------------------------------------------------------

-- define the amount of WHS array elements: between 1 and 16
constant WHS_RECORDS   : natural := 2;

-- define the maximum amount of pages per WHS array element: between 1 and 256
-- (this is necessary because Vivado does not support unconstrained arrays in a record)
constant WHS_MAX_PAGES : natural := 3;

 -- !!! DO NOT TOUCH !!!
constant SEL_WHS           : std_logic_vector(15 downto 0) := x"1000";
type WHS_INDEX_TYPE is array (0 to WHS_MAX_PAGES - 1) of natural;
type WHS_RECORD_TYPE is record
   page_count  : natural;
   page_start  : WHS_INDEX_TYPE;
   page_length : WHS_INDEX_TYPE;
end record;
type WHS_RECORD_ARRAY_TYPE is array (0 to WHS_RECORDS - 1) of WHS_RECORD_TYPE;

-- START YOUR CONFIGURATION BELOW THIS LINE

-- Define all your screens as string constants. They will be synthesized as ROMs.
-- You can name these string constants as you want to, as long as you make them part of the WHS array (see below).
--
-- WHS array position 0 is defined as the "Welcome Screen" as controled by WELCOME_ACTIVE and WELCOME_AT_RESET.
-- If you are not using a Welcome Screen but only Help menu items, then you need to leave WHS array pos. 0 empty.
--
-- WHS array position 1 and onwards is for all the Option Menu items tagged as "Help": The first one in the
-- Options menu is WHS array pos. 1, the second one in the menu is WHS array pos. 2 and so on.
--
-- Maximum 16 WHS array positions: The selector's bits 11 downto 8 select the WHS array position; 0=Welcome Screen
-- That means a maximum of 15 menu items in the Options menu can be tagged as "Help"
-- The selector's bits 7 downto 0 are selecting the page within the WHS array, so maximum 256 pages per Welcome Screen or Help menu item
--
-- Within a selector's address range, address 0 is the beginning of the string itself, while address 0xFFF of the 4k
-- window contains the amount of pages, so each zero-terminated string can be up to 4095 bytes = 4094 characters long.

-- Examples on how to build characters and strings
-- constant CHR_LINE_1  : character := character'val(196);
-- constant CHR_LINE_5  : string := CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1 & CHR_LINE_1;
-- constant CHR_LINE_10 : string := CHR_LINE_5 & CHR_LINE_5;
-- constant CHR_LINE_50 : string := CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10 & CHR_LINE_10;

--constant CHR_0          : character := character'val(0); -- cannot be printed, as it will halt more text
constant CHR_1          : character := character'val(1); -- ☺
constant CHR_2          : character := character'val(2); -- ☻
constant CHR_3          : character := character'val(3); -- ♥
constant CHR_4          : character := character'val(4); -- ♦
constant CHR_5          : character := character'val(5); -- ♣
constant CHR_6          : character := character'val(6); -- ♠
constant CHR_7          : character := character'val(7); -- •
constant CHR_8          : character := character'val(8); -- ◘
constant CHR_9          : character := character'val(9); -- ○
constant CHR_10         : character := character'val(10); -- ◙
constant CHR_11         : character := character'val(11); -- ♂
constant CHR_12         : character := character'val(12); -- ♀
constant CHR_13         : character := character'val(13); -- ♪
constant CHR_14         : character := character'val(14); -- ♫
constant CHR_15         : character := character'val(15); -- ☼
constant CHR_16         : character := character'val(16); -- ►
constant CHR_17         : character := character'val(17); -- ◄
constant CHR_18         : character := character'val(18); -- ↕
constant CHR_19         : character := character'val(19); -- ‼
constant CHR_20         : character := character'val(20); -- ¶
constant CHR_21         : character := character'val(21); -- §
constant CHR_22         : character := character'val(22); -- ▬
constant CHR_23         : character := character'val(23); -- ↨
constant CHR_24         : character := character'val(24); -- ↑
constant CHR_25         : character := character'val(25); -- ↓
constant CHR_26         : character := character'val(26); -- →
constant CHR_27         : character := character'val(27); -- ←
constant CHR_28         : character := character'val(28); -- ∟
constant CHR_29         : character := character'val(29); -- ↔
constant CHR_30         : character := character'val(30); -- ▲
constant CHR_31         : character := character'val(31); -- ▼
constant CHR_32         : character := character'val(32); --
constant CHR_33         : character := character'val(33); -- !
constant CHR_34         : character := character'val(34); -- "
constant CHR_35         : character := character'val(35); -- #
constant CHR_36         : character := character'val(36); -- $
constant CHR_37         : character := character'val(37); -- %
constant CHR_38         : character := character'val(38); -- &
constant CHR_39         : character := character'val(39); -- '
constant CHR_40         : character := character'val(40); -- (
constant CHR_41         : character := character'val(41); -- )
constant CHR_42         : character := character'val(42); -- *
constant CHR_43         : character := character'val(43); -- +
constant CHR_44         : character := character'val(44); -- ,
constant CHR_45         : character := character'val(45); -- -
constant CHR_46         : character := character'val(46); -- .
constant CHR_47         : character := character'val(47); -- /
constant CHR_48         : character := character'val(48); -- 0
constant CHR_49         : character := character'val(49); -- 1
constant CHR_50         : character := character'val(50); -- 2
constant CHR_51         : character := character'val(51); -- 3
constant CHR_52         : character := character'val(52); -- 4
constant CHR_53         : character := character'val(53); -- 5
constant CHR_54         : character := character'val(54); -- 6
constant CHR_55         : character := character'val(55); -- 7
constant CHR_56         : character := character'val(56); -- 8
constant CHR_57         : character := character'val(57); -- 9
constant CHR_58         : character := character'val(58); -- :
constant CHR_59         : character := character'val(59); -- ;
constant CHR_60         : character := character'val(60); -- <
constant CHR_61         : character := character'val(61); -- =
constant CHR_62         : character := character'val(62); -- >
constant CHR_63         : character := character'val(63); -- ?
constant CHR_64         : character := character'val(64); -- @
constant CHR_65         : character := character'val(65); -- A
constant CHR_66         : character := character'val(66); -- B
constant CHR_67         : character := character'val(67); -- C
constant CHR_68         : character := character'val(68); -- D
constant CHR_69         : character := character'val(69); -- E
constant CHR_70         : character := character'val(70); -- F
constant CHR_71         : character := character'val(71); -- G
constant CHR_72         : character := character'val(72); -- H
constant CHR_73         : character := character'val(73); -- I
constant CHR_74         : character := character'val(74); -- J
constant CHR_75         : character := character'val(75); -- K
constant CHR_76         : character := character'val(76); -- L
constant CHR_77         : character := character'val(77); -- M
constant CHR_78         : character := character'val(78); -- N
constant CHR_79         : character := character'val(79); -- O
constant CHR_80         : character := character'val(80); -- P
constant CHR_81         : character := character'val(81); -- Q
constant CHR_82         : character := character'val(82); -- R
constant CHR_83         : character := character'val(83); -- S
constant CHR_84         : character := character'val(84); -- T
constant CHR_85         : character := character'val(85); -- U
constant CHR_86         : character := character'val(86); -- V
constant CHR_87         : character := character'val(87); -- W
constant CHR_88         : character := character'val(88); -- X
constant CHR_89         : character := character'val(89); -- Y
constant CHR_90         : character := character'val(90); -- Z
constant CHR_91         : character := character'val(91); -- [
constant CHR_92         : character := character'val(92); -- \
constant CHR_93         : character := character'val(93); -- ]
constant CHR_94         : character := character'val(94); -- ^
constant CHR_95         : character := character'val(95); -- _
constant CHR_96         : character := character'val(96); -- `
constant CHR_97         : character := character'val(97); -- a
constant CHR_98         : character := character'val(98); -- b
constant CHR_99         : character := character'val(99); -- c
constant CHR_100        : character := character'val(100); -- d
constant CHR_101        : character := character'val(101); -- e
constant CHR_102        : character := character'val(102); -- f
constant CHR_103        : character := character'val(103); -- g
constant CHR_104        : character := character'val(104); -- h
constant CHR_105        : character := character'val(105); -- i
constant CHR_106        : character := character'val(106); -- j
constant CHR_107        : character := character'val(107); -- k
constant CHR_108        : character := character'val(108); -- l
constant CHR_109        : character := character'val(109); -- m
constant CHR_110        : character := character'val(110); -- n
constant CHR_111        : character := character'val(111); -- o
constant CHR_112        : character := character'val(112); -- p
constant CHR_113        : character := character'val(113); -- q
constant CHR_114        : character := character'val(114); -- r
constant CHR_115        : character := character'val(115); -- s
constant CHR_116        : character := character'val(116); -- t
constant CHR_117        : character := character'val(117); -- u
constant CHR_118        : character := character'val(118); -- v
constant CHR_119        : character := character'val(119); -- w
constant CHR_120        : character := character'val(120); -- x
constant CHR_121        : character := character'val(121); -- y
constant CHR_122        : character := character'val(122); -- z
constant CHR_123        : character := character'val(123); -- {
constant CHR_124        : character := character'val(124); -- |
constant CHR_125        : character := character'val(125); -- }
constant CHR_126        : character := character'val(126); -- ~
constant CHR_127        : character := character'val(127); -- ⌂
constant CHR_128        : character := character'val(128); -- Ç
constant CHR_129        : character := character'val(129); -- ü
constant CHR_130        : character := character'val(130); -- é
constant CHR_131        : character := character'val(131); -- â
constant CHR_132        : character := character'val(132); -- ä
constant CHR_133        : character := character'val(133); -- à
constant CHR_134        : character := character'val(134); -- å
constant CHR_135        : character := character'val(135); -- ç
constant CHR_136        : character := character'val(136); -- ê
constant CHR_137        : character := character'val(137); -- ë
constant CHR_138        : character := character'val(138); -- è
constant CHR_139        : character := character'val(139); -- ï
constant CHR_140        : character := character'val(140); -- î
constant CHR_141        : character := character'val(141); -- ì
constant CHR_142        : character := character'val(142); -- Ä
constant CHR_143        : character := character'val(143); -- Å
constant CHR_144        : character := character'val(144); -- É
constant CHR_145        : character := character'val(145); -- æ
constant CHR_146        : character := character'val(146); -- Æ
constant CHR_147        : character := character'val(147); -- ô
constant CHR_148        : character := character'val(148); -- ö
constant CHR_149        : character := character'val(149); -- ò
constant CHR_150        : character := character'val(150); -- û
constant CHR_151        : character := character'val(151); -- ù
constant CHR_152        : character := character'val(152); -- ÿ
constant CHR_153        : character := character'val(153); -- Ö
constant CHR_154        : character := character'val(154); -- Ü
constant CHR_155        : character := character'val(155); -- ¢
constant CHR_156        : character := character'val(156); -- £
constant CHR_157        : character := character'val(157); -- ¥
constant CHR_158        : character := character'val(158); -- ₧
constant CHR_159        : character := character'val(159); -- ƒ
constant CHR_160        : character := character'val(160); -- á
constant CHR_161        : character := character'val(161); -- í
constant CHR_162        : character := character'val(162); -- ó
constant CHR_163        : character := character'val(163); -- ú
constant CHR_164        : character := character'val(164); -- ñ
constant CHR_165        : character := character'val(165); -- Ñ
constant CHR_166        : character := character'val(166); -- ª
constant CHR_167        : character := character'val(167); -- º
constant CHR_168        : character := character'val(168); -- ¿
constant CHR_169        : character := character'val(169); -- ⌐
constant CHR_170        : character := character'val(170); -- ¬
constant CHR_171        : character := character'val(171); -- ½
constant CHR_172        : character := character'val(172); -- ¼
constant CHR_173        : character := character'val(173); -- ¡
constant CHR_174        : character := character'val(174); -- «
constant CHR_175        : character := character'val(175); -- »
constant CHR_176        : character := character'val(176); -- ░
constant CHR_177        : character := character'val(177); -- ▒
constant CHR_178        : character := character'val(178); -- ▓
constant CHR_179        : character := character'val(179); -- │
constant CHR_180        : character := character'val(180); -- ┤
constant CHR_181        : character := character'val(181); -- ╡
constant CHR_182        : character := character'val(182); -- ╢
constant CHR_183        : character := character'val(183); -- ╖
constant CHR_184        : character := character'val(184); -- ╕
constant CHR_185        : character := character'val(185); -- ╣
constant CHR_186        : character := character'val(186); -- ║
constant CHR_187        : character := character'val(187); -- ╗
constant CHR_188        : character := character'val(188); -- ╝
constant CHR_189        : character := character'val(189); -- ╜
constant CHR_190        : character := character'val(190); -- ╛
constant CHR_191        : character := character'val(191); -- ┐
constant CHR_192        : character := character'val(192); -- └
constant CHR_193        : character := character'val(193); -- ┴
constant CHR_194        : character := character'val(194); -- ┬
constant CHR_195        : character := character'val(195); -- ├
constant CHR_196        : character := character'val(196); -- ─
constant CHR_197        : character := character'val(197); -- ┼
constant CHR_198        : character := character'val(198); -- ╞
constant CHR_199        : character := character'val(199); -- ╟
constant CHR_200        : character := character'val(200); -- ╚
constant CHR_201        : character := character'val(201); -- ╔
constant CHR_202        : character := character'val(202); -- ╩
constant CHR_203        : character := character'val(203); -- ╦
constant CHR_204        : character := character'val(204); -- ╠
constant CHR_205        : character := character'val(205); -- ═
constant CHR_206        : character := character'val(206); -- ╬
constant CHR_207        : character := character'val(207); -- ╧
constant CHR_208        : character := character'val(208); -- ╨
constant CHR_209        : character := character'val(209); -- ╤
constant CHR_210        : character := character'val(210); -- ╥
constant CHR_211        : character := character'val(211); -- ╙
constant CHR_212        : character := character'val(212); -- ╘
constant CHR_213        : character := character'val(213); -- ╒
constant CHR_214        : character := character'val(214); -- ╓
constant CHR_215        : character := character'val(215); -- ╫
constant CHR_216        : character := character'val(216); -- ╪
constant CHR_217        : character := character'val(217); -- ┘
constant CHR_218        : character := character'val(218); -- ┌
constant CHR_219        : character := character'val(219); -- █
constant CHR_220        : character := character'val(220); -- ▄
constant CHR_221        : character := character'val(221); -- ▌
constant CHR_222        : character := character'val(222); -- ▐
constant CHR_223        : character := character'val(223); -- ▀
constant CHR_224        : character := character'val(224); -- α
constant CHR_225        : character := character'val(225); -- ß
constant CHR_226        : character := character'val(226); -- Γ
constant CHR_227        : character := character'val(227); -- π
constant CHR_228        : character := character'val(228); -- Σ
constant CHR_229        : character := character'val(229); -- σ
constant CHR_230        : character := character'val(230); -- µ
constant CHR_231        : character := character'val(231); -- τ
constant CHR_232        : character := character'val(232); -- Φ
constant CHR_233        : character := character'val(233); -- Θ
constant CHR_234        : character := character'val(234); -- Ω
constant CHR_235        : character := character'val(235); -- δ
constant CHR_236        : character := character'val(236); -- ∞
constant CHR_237        : character := character'val(237); -- φ
constant CHR_238        : character := character'val(238); -- ε
constant CHR_239        : character := character'val(239); -- ∩
constant CHR_240        : character := character'val(240); -- ≡
constant CHR_241        : character := character'val(241); -- ±
constant CHR_242        : character := character'val(242); -- ≥
constant CHR_243        : character := character'val(243); -- ≤
constant CHR_244        : character := character'val(244); -- ⌠
constant CHR_245        : character := character'val(245); -- ⌡
constant CHR_246        : character := character'val(246); -- ÷
constant CHR_247        : character := character'val(247); -- ≈
constant CHR_248        : character := character'val(248); -- °
constant CHR_249        : character := character'val(249); -- ∙
constant CHR_250        : character := character'val(250); -- ·
constant CHR_251        : character := character'val(251); -- √
constant CHR_252        : character := character'val(252); -- ⁿ
constant CHR_253        : character := character'val(253); -- ²
constant CHR_254        : character := character'val(254); -- ■
constant CHR_255        : character := character'val(255); --

constant CHRS_1_15      : string := CHR_1 & CHR_2 & CHR_3 & CHR_4 & CHR_5 & CHR_6 & CHR_7 & CHR_8 & CHR_9 & CHR_10 & CHR_11 & CHR_12 & CHR_13 & CHR_14 & CHR_15;
constant CHRS_16_31     : string := CHR_16 & CHR_17 & CHR_18 & CHR_19 & CHR_20 & CHR_21 & CHR_22 & CHR_23 & CHR_24 & CHR_25 & CHR_26 & CHR_27 & CHR_28 & CHR_29 & CHR_30 & CHR_31;
constant CHRS_32_47     : string := CHR_32 & CHR_33 & CHR_34 & CHR_35 & CHR_36 & CHR_37 & CHR_38 & CHR_39 & CHR_40 & CHR_41 & CHR_42 & CHR_43 & CHR_44 & CHR_45 & CHR_46 & CHR_47;
constant CHRS_48_63     : string := CHR_48 & CHR_49 & CHR_50 & CHR_51 & CHR_52 & CHR_53 & CHR_54 & CHR_55 & CHR_56 & CHR_57 & CHR_58 & CHR_59 & CHR_60 & CHR_61 & CHR_62 & CHR_63;
constant CHRS_64_79     : string := CHR_64 & CHR_65 & CHR_66 & CHR_67 & CHR_68 & CHR_69 & CHR_70 & CHR_71 & CHR_72 & CHR_73 & CHR_74 & CHR_75 & CHR_76 & CHR_77 & CHR_78 & CHR_79;
constant CHRS_80_95     : string := CHR_80 & CHR_81 & CHR_82 & CHR_83 & CHR_84 & CHR_85 & CHR_86 & CHR_87 & CHR_88 & CHR_89 & CHR_90 & CHR_91 & CHR_92 & CHR_93 & CHR_94 & CHR_95;
constant CHRS_96_111    : string := CHR_96 & CHR_97 & CHR_98 & CHR_99 & CHR_100 & CHR_101 & CHR_102 & CHR_103 & CHR_104 & CHR_105 & CHR_106 & CHR_107 & CHR_108 & CHR_109 & CHR_110 & CHR_111;
constant CHRS_112_127   : string := CHR_112 & CHR_113 & CHR_114 & CHR_115 & CHR_116 & CHR_117 & CHR_118 & CHR_119 & CHR_120 & CHR_121 & CHR_122 & CHR_123 & CHR_124 & CHR_125 & CHR_126 & CHR_127;
constant CHRS_128_143   : string := CHR_128 & CHR_129 & CHR_130 & CHR_131 & CHR_132 & CHR_133 & CHR_134 & CHR_135 & CHR_136 & CHR_137 & CHR_138 & CHR_139 & CHR_140 & CHR_141 & CHR_142 & CHR_143;
constant CHRS_144_159   : string := CHR_144 & CHR_145 & CHR_146 & CHR_147 & CHR_148 & CHR_149 & CHR_150 & CHR_151 & CHR_152 & CHR_153 & CHR_154 & CHR_155 & CHR_156 & CHR_157 & CHR_158 & CHR_159;
constant CHRS_160_175   : string := CHR_160 & CHR_161 & CHR_162 & CHR_163 & CHR_164 & CHR_165 & CHR_166 & CHR_167 & CHR_168 & CHR_169 & CHR_170 & CHR_171 & CHR_172 & CHR_173 & CHR_174 & CHR_175;
constant CHRS_176_191   : string := CHR_176 & CHR_177 & CHR_178 & CHR_179 & CHR_180 & CHR_181 & CHR_182 & CHR_183 & CHR_184 & CHR_185 & CHR_186 & CHR_187 & CHR_188 & CHR_189 & CHR_190 & CHR_191;
constant CHRS_192_207   : string := CHR_192 & CHR_193 & CHR_194 & CHR_195 & CHR_196 & CHR_197 & CHR_198 & CHR_199 & CHR_200 & CHR_201 & CHR_202 & CHR_203 & CHR_204 & CHR_205 & CHR_206 & CHR_207;
constant CHRS_208_223   : string := CHR_208 & CHR_209 & CHR_210 & CHR_211 & CHR_212 & CHR_213 & CHR_214 & CHR_215 & CHR_216 & CHR_217 & CHR_218 & CHR_219 & CHR_220 & CHR_221 & CHR_222 & CHR_223;
constant CHRS_224_239   : string := CHR_224 & CHR_225 & CHR_226 & CHR_227 & CHR_228 & CHR_229 & CHR_230 & CHR_231 & CHR_232 & CHR_233 & CHR_234 & CHR_235 & CHR_236 & CHR_237 & CHR_238 & CHR_239;
constant CHRS_240_255   : string := CHR_240 & CHR_241 & CHR_242 & CHR_243 & CHR_244 & CHR_245 & CHR_246 & CHR_247 & CHR_248 & CHR_249 & CHR_250 & CHR_251 & CHR_252 & CHR_253 & CHR_254 & CHR_255;

constant CURVED_PAIR    : string := CHR_232 & CHR_233;
constant CURVED_LINE    : string := CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR & CURVED_PAIR;

constant SCR_WELCOME : string :=

     "\n" &
     "          MiSTer2MEGA65 Demo Core\n" &
     "\n" &
     " Powered by MiSTer2MEGA65 Version [0.9.1]\n" &
     "    Done by sy2002 and MJoergen in 2022\n" &
     "\n" &
     CURVED_LINE & "\n" &
     "\n" &
     "CHARACTER TEST PATTERN\n" &
     " - showing all 255 characters\n" &
     " - charset used: modified " & CHR_34 & "Anikki-16x16" & CHR_34 & "\n" &
     "\n" &
     "    1-15 : " & CHRS_1_15 & " (0x00 excluded)\n" &
     "   16-31 : " & CHRS_16_31 & "\n" &
     "   32-47 : " & CHRS_32_47 & "\n" &
     "   48-63 : " & CHRS_48_63 & "\n" &
     "   64-79 : " & CHRS_64_79 & "\n" &
     "   80-95 : " & CHRS_80_95 & "\n" &
     "  96-111 : " & CHRS_96_111 & "\n" &
     " 112-127 : " & CHRS_112_127 & "\n" &
     " 128-143 : " & CHRS_128_143 & "\n" &
     " 144-159 : " & CHRS_144_159 & "\n" &
     " 160-175 : " & CHRS_160_175 & "\n" &
     " 176-191 : " & CHRS_176_191 & "\n" &
     " 192-207 : " & CHRS_192_207 & "\n" &
     " 208-223 : " & CHRS_208_223 & "\n" &
     " 224-239 : " & CHRS_224_239 & "\n" &
     " 240-255 : " & CHRS_240_255 & "\n" &
     "\n" &
     CURVED_LINE & "\n" &
     "\n" &
     "       Tweaks by MegaBeauvais, 2023\n";

constant HELP_1 : string :=

   "\n Demo Core for MEGA65 Version 1\n\n" &

   " MiSTer port 2022 by YOU\n" &
   " Powered by MiSTer2MEGA65\n\n\n" &

   " Lorem ipsum dolor sit amet, consetetur\n" &
   " sadipscing elitr, sed diam nonumy eirmod\n" &
   " Mpor invidunt ut labore et dolore magna\n" &
   " aliquyam erat, sed diam voluptua. At vero\n" &
   " eos et accusam et justo duo.\n\n" &

   " Dolores et ea rebum. Stet clita kasd gube\n" &
   " gren, no sea takimata sanctus est Lorem ip\n" &
   " Sed diam nonumy eirmod tempor invidunt ut\n" &
   " labore et dolore magna aliquyam era\n\n" &

   " Cursor right to learn more.       (1 of 3)\n" &
   " Press Space to close the help screen.";

constant HELP_2 : string :=

   "\n Demo Core for MEGA65 Version 1\n\n" &

   " XYZ ABCDEFGH:\n\n" &

   " 1. ABCD EFGH\n" &
   " 2. IJK LM NOPQ RSTUVWXYZ\n" &
   " 3. 10 20 30 40 50\n\n" &

   " a) Dolores et ea rebum\n" &
   " b) Takimata sanctus est\n" &
   " c) Tempor Invidunt ut\n" &
   " d) Sed Diam Nonumy eirmod te\n" &
   " e) Awesome\n\n" &

   " Ut wisi enim ad minim veniam, quis nostru\n" &
   " exerci tation ullamcorper suscipit lobor\n" &
   " tis nisl ut aliquip ex ea commodo.\n\n" &

   " Crsr left: Prev  Crsr right: Next (2 of 3)\n" &
   " Press Space to close the help screen.";

constant HELP_3 : string :=

   "\n Help Screens\n\n" &

   " You can have 255 screens per help topic.\n\n" &

   " 15 topics overall.\n" &
   " 1 menu item per topic.\n\n\n\n" &

   " Cursor left to go back.           (3 of 3)\n" &
   " Press Space to close the help screen.";

-- Concatenate all your Welcome and Help screens into one large string, so that during synthesis one large string ROM can be build.
constant WHS_DATA : string := SCR_WELCOME & HELP_1 & HELP_2 & HELP_3;

-- The WHS array needs the start address of each page. As a best practice: Just define some constants, that you can name for example
-- just like you named the string constants and then add _START. Use the 'length attribute of VHDL to add up all previous strings
-- so that the Synthesis tool can calculate the start addresses: Your first string starts at zero, your next one at the address which
-- is equal to the length of the first one, your next one at the address which is equal to the sum of the previous ones, and so on.
constant SCR_WELCOME_START : natural := 0;
constant HELP_1_START      : natural := SCR_WELCOME'length;
constant HELP_2_START      : natural := HELP_1_START + HELP_1'length;
constant HELP_3_START      : natural := HELP_2_START + HELP_2'length;

-- Fill the WHS array with page start addresses and the length of each page.
-- Make sure that array element 0 is always your Welcome page. If you don't use a welcome page, fill everything with zeros.
constant WHS : WHS_RECORD_ARRAY_TYPE := (
   --- Welcome Screen
   (page_count    => 1,
    page_start    => (SCR_WELCOME_START,  0, 0),
    page_length   => (SCR_WELCOME'length, 0, 0)),

   --- Help pages
   (page_count    => 3,
    page_start    => (HELP_1_START,  HELP_2_START,  HELP_3_START),
    page_length   => (HELP_1'length, HELP_2'length, HELP_3'length))
);

--------------------------------------------------------------------------------------------------------------------
-- Set start folder for file browser and specify config file for menu persistence (Selectors 0x0100 and 0x0101)
--------------------------------------------------------------------------------------------------------------------

-- !!! DO NOT TOUCH !!!
constant SEL_DIR_START     : std_logic_vector(15 downto 0) := x"0100";
constant SEL_CFG_FILE      : std_logic_vector(15 downto 0) := x"0101";

-- START YOUR CONFIGURATION BELOW THIS LINE

constant DIR_START         : string := "/m2m";
constant CFG_FILE          : string := "/m2m/m2mcfg";

--------------------------------------------------------------------------------------------------------------------
-- General configuration settings: Reset, Pause, OSD behavior, Ascal, etc. (Selector 0x0110)
--------------------------------------------------------------------------------------------------------------------

constant SEL_GENERAL       : std_logic_vector(15 downto 0) := x"0110";  -- !!! DO NOT TOUCH !!!

-- START YOUR CONFIGURATION BELOW THIS LINE

-- keep the core in RESET state after the hardware starts up and after pressing the MEGA65's reset button
constant RESET_KEEP        : boolean := false;

-- alternative to RESET_KEEP: keep the reset line active for this amount of "QNICE loops" (see shell.asm)
-- "0" means: deactivate this feature
constant RESET_COUNTER     : natural := 100;

-- put the core in PAUSE state if any OSD opens
constant OPTM_PAUSE        : boolean := false;

-- show the welcome screen in general
constant WELCOME_ACTIVE    : boolean := true;

-- shall the welcome screen also be shown after the core is reset?
-- (only relevant if WELCOME_ACTIVE is true)
constant WELCOME_AT_RESET  : boolean := true;

-- keyboard and joystick connection during reset and OSD
constant KEYBOARD_AT_RESET : boolean := false;
constant JOY_1_AT_RESET    : boolean := false;
constant JOY_2_AT_RESET    : boolean := false;

constant KEYBOARD_AT_OSD   : boolean := false;
constant JOY_1_AT_OSD      : boolean := false;
constant JOY_2_AT_OSD      : boolean := false;

-- Avalon Scaler settings (see ascal.vhd, used for HDMI output only)
-- 0=set ascal mode (via QNICE's ascal_mode_o) to the value of the config.vhd constant ASCAL_MODE
-- 1=do nothing, leave ascal mode alone, custom QNICE assembly code can still change it via M2M$ASCAL_MODE
--               and QNICE's CSR will be set to not automatically sync ascal_mode_i
-- 2=keep ascal mode in sync with the QNICE input register ascal_mode_i:
--   use this if you want to control the ascal mode for example via the Options menu
--   where you would wire the output of certain options menu bits with ascal_mode_i
constant ASCAL_USAGE       : natural := 2;
constant ASCAL_MODE        : natural := 0;   -- see ascal.vhd for the meaning of this value

-- Save on-screen-display settings if the file specified by CFG_FILE exists and if it has
-- the length of OPTM_SIZE bytes. If the first byte of the file has the value 0xFF then it
-- is considered as "default", i.e. the menu items specified by OPTM_G_STDSEL are selected.
-- If the file does not exists, then settings are not saved and OPTM_G_STDSEL always denotes the standard settings.
constant SAVE_SETTINGS     : boolean := true;

-- Delay in ms between the last write request to a virtual drive from the core and the start of the
-- cache flushing (i.e. writing to the SD card). Since every new write from the core invalidates the cache,
-- and therefore leads to a completely new writing of the cache (flushing), this constant prevents thrashing.
-- The default is 2 seconds (2000 ms). Should be reasonable for many systems, but if you have a very fast
-- or very slow system, you might need to change this constant.
--
-- Constraint (@TODO): Currently we have only one constant for all virtual drives, i.e. the delay is
-- the same for all virtual drives. This might be absolutely OK; future will tell. If we need to have
-- more flexibility: vdrives.vhd already supports one delay per virtual drive. All what would need
-- to be done in such a case is: Enhance config.vhd to have more constants plus enhance the initialization
-- routine VD_INIT in vdrives.asm (tagged by @TODO) to store different values in the appropriate registers.
constant VD_ANTI_THRASHING_DELAY : natural := 2000;

-- Amount of bytes saved in one iteration of the background saving (buffer flushing) process
-- Constraint (@TODO): Similar constraint as in VD_ANTI_THRASHING_DELAY: Only one value for all drives.
-- shell.asm and shell_vars.asm already supports distinct values per drive; config.vhd and VD_INIT would
-- needs to be updated in case we would need this feature in future
constant VD_ITERATION_SIZE       : natural := 100;

--------------------------------------------------------------------------------------------------------------------
-- Load one or more mandatory or optional BIOS/ROMs  (Selectors 0x0200 .. 0x02FF)
--------------------------------------------------------------------------------------------------------------------

-- !!! CAUTION: CURRENTLY NOT YET SUPPORTED BY THE FIRMWARE !!!

--------------------------------------------------------------------------------------------------------------------
-- "Help" menu / Options menu  (Selectors 0x0300 .. 0x0307)
--------------------------------------------------------------------------------------------------------------------

-- !!! DO NOT TOUCH !!! Selectors for accessing the menu configuration data
constant SEL_OPTM_ITEMS       : std_logic_vector(15 downto 0) := x"0300";
constant SEL_OPTM_GROUPS      : std_logic_vector(15 downto 0) := x"0301";
constant SEL_OPTM_STDSEL      : std_logic_vector(15 downto 0) := x"0302";
constant SEL_OPTM_LINES       : std_logic_vector(15 downto 0) := x"0303";
constant SEL_OPTM_START       : std_logic_vector(15 downto 0) := x"0304";
constant SEL_OPTM_ICOUNT      : std_logic_vector(15 downto 0) := x"0305";
constant SEL_OPTM_MOUNT_DRV   : std_logic_vector(15 downto 0) := x"0306";
constant SEL_OPTM_SINGLESEL   : std_logic_vector(15 downto 0) := x"0307";
constant SEL_OPTM_MOUNT_STR   : std_logic_vector(15 downto 0) := x"0308";
constant SEL_OPTM_DIMENSIONS  : std_logic_vector(15 downto 0) := x"0309";
constant SEL_OPTM_SAVING_STR  : std_logic_vector(15 downto 0) := x"030A";
constant SEL_OPTM_HELP        : std_logic_vector(15 downto 0) := x"0310";

-- !!! DO NOT TOUCH !!! Configuration constants for OPTM_GROUPS (shell.asm and menu.asm expect them to be like this)
constant OPTM_G_TEXT       : integer := 0;                -- text that cannot be selected
constant OPTM_G_CLOSE      : integer := 16#00FF#;         -- menu items that closes menu
constant OPTM_G_STDSEL     : integer := 16#0100#;         -- item within a group that is selected by default
constant OPTM_G_LINE       : integer := 16#0200#;         -- draw a line at this position
constant OPTM_G_START      : integer := 16#0400#;         -- selector / cursor position after startup (only use once!)
constant OPTM_G_HEADLINE   : integer := 16#1000#;         -- like OPTM_G_TEXT but will be shown in a brigher color
constant OPTM_G_MOUNT_DRV  : integer := 16#8800#;         -- line item means: mount drive; first occurance = drive 0, second = drive 1, ...
constant OPTM_G_HELP       : integer := 16#A000#;         -- line item means: help screen; first occurance = WHS(1), second = WHS(2), ...
constant OPTM_G_SINGLESEL  : integer := 16#8000#;         -- single select item
-- @TODO/REMINDER: As soon as we extend the OSM system so that we support loading ROMs and other things that need to be ignored
-- when saving settings: Make sure to extend _ROSMS_4A and _ROSMC_NEXTBIT in options.asm accordingly

-- START YOUR CONFIGURATION BELOW THIS LINE:

-- Strings with which %s will be replaced in case the menu item is of type OPTM_G_MOUNT_DRV
constant OPTM_S_MOUNT      : string := "<Mount>";        -- no disk image mounted, yet
constant OPTM_S_SAVING     : string := "<Saving>";       -- the internal write cache is dirty and not yet written back to the SD card

-- Size of menu and menu items
-- CAUTION: 1. End each line (also the last one) with a \n and make sure empty lines / separator lines are only consisting of a "\n"
--             Do use a lower case \n. If you forget one of them or if you use upper case, you will run into undefined behavior.
--          2. Start each line that contains an actual menu item (multi- or single-select) with a Space character,
--             otherwise you will experience visual glitches.
--constant OPTM_SIZE         : natural := 27;  -- amount of items including empty lines:
constant OPTM_SIZE         : natural := 21;  -- amount of items including empty lines:
                                             -- needs to be equal to the number of lines in OPTM_ITEMS and amount of items in OPTM_GROUPS
                                             -- IMPORTANT: If SAVE_SETTINGS is true and OPTM_SIZE changes: Make sure to re-generate and
                                             -- and re-distribute the config file. You can make a new one using M2M/tools/make_config.sh

-- Net size of the Options menu on the screen in characters (excluding the frame, which is hardcoded to two characters)
-- We advise to use OPTM_SIZE as height, but there might be reasons for you to change it.
constant OPTM_DX           : natural := 30;
constant OPTM_DY           : natural := OPTM_SIZE;

constant OPTM_ITEMS        : string :=

   " Single Selector\n"     &
   " Item B.X\n"            &
   " Item A.2\n"            &
   " Item A.3\n"            &
   " Item A.4\n"            &
   "\n"                     &
   " HDMI Mode\n"           &
   " 720p 50Hz 16:9\n"      &
   " 720p 60Hz 16:9\n"      &
   " 576p 50Hz 4:3\n"       &
   " 576p 50Hz 5:4\n"       &
   "\n"                     &
   " Drives\n"              &
   " Drive X:%s\n"          &
   " Drive Y:%s\n"          &
   " Drive Z:%s\n"          &
   "\n"                     &
   " Display & Audio\n"    &
   " HDMI: CRT Emulation\n" &
   " HDMI: Zoom-In\n"       &
   " Audio Improvements\n";

-- define your own constants here and choose meaningful names
-- make sure that your first group uses the value 1 (0 means "no menu item", such as text and line),
-- and be aware that you can only have a maximum of 254 groups (255 means "Close Menu");
-- also make sure that your group numbers are monotonic increasing (e.g. 1, 2, 3, 4, ...)
-- single-select items and therefore also drive mount items need to have unique identifiers
constant OPTM_G_Demo_A     : integer := 1;
constant OPTM_G_HDMI       : integer := 2;
constant OPTM_G_Drive_X    : integer := 3;
constant OPTM_G_Drive_Y    : integer := 4;
constant OPTM_G_Drive_Z    : integer := 5;
constant OPTM_G_CRT        : integer := 6;
constant OPTM_G_Zoom       : integer := 7;
constant OPTM_G_Audio      : integer := 8;

-- define your menu groups: which menu items are belonging together to form a group?
-- where are separator lines? which items should be selected by default?
-- make sure that you have exactly the same amount of entries here than in OPTM_ITEMS and defined by OPTM_SIZE
type OPTM_GTYPE is array (0 to OPTM_SIZE - 1) of integer range 0 to 65535;
constant OPTM_GROUPS       : OPTM_GTYPE := ( OPTM_G_TEXT + OPTM_G_HEADLINE,            -- Headline "Demo Headline"
                                             OPTM_G_Demo_A + OPTM_G_START,             -- Item A.1, cursor start position
                                             OPTM_G_Demo_A + OPTM_G_STDSEL,            -- Item A.2, selected by default
                                             OPTM_G_Demo_A,                            -- Item A.3
                                             OPTM_G_Demo_A,                            -- Item A.4
                                             OPTM_G_LINE,                              -- Line
                                             OPTM_G_TEXT + OPTM_G_HEADLINE,            -- Headline "HDMI Mode"
                                             OPTM_G_HDMI + OPTM_G_STDSEL,              -- 720p 50 Hz 16:9, selected by default
                                             OPTM_G_HDMI,                              -- 720p 60 Hz 16:9
                                             OPTM_G_HDMI,                              -- 576p 50 Hz 4:3
                                             OPTM_G_HDMI,                              -- 576p 50 Hz 5:4
                                             OPTM_G_LINE,                              -- Line
                                             OPTM_G_TEXT + OPTM_G_HEADLINE,            -- Headline "Drives"
                                             OPTM_G_Drive_X + OPTM_G_MOUNT_DRV,        -- Drive X
                                             OPTM_G_Drive_Y + OPTM_G_MOUNT_DRV,        -- Drive Y
                                             OPTM_G_Drive_Z + OPTM_G_MOUNT_DRV,        -- Drive Z
                                             OPTM_G_LINE,                              -- Line
                                             OPTM_G_TEXT + OPTM_G_HEADLINE,            -- Headline "Another Headline"
                                             OPTM_G_CRT     + OPTM_G_SINGLESEL,        -- On/Off toggle ("Single Select")
                                             OPTM_G_Zoom    + OPTM_G_SINGLESEL,        -- On/Off toggle ("Single Select")
                                             OPTM_G_Audio   + OPTM_G_SINGLESEL         -- On/Off toggle ("Single Select")
                                           );

--------------------------------------------------------------------------------------------------------------------
-- !!! CAUTION: M2M FRAMEWORK CODE !!! DO NOT TOUCH ANYTHING BELOW THIS LINE !!!
--------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------------------------
-- Address Decoding
--------------------------------------------------------------------------------------------------------------------

begin

addr_decode : process(clk_i, address_i)
   variable index : integer;
   variable whs_array_index : integer;
   variable whs_page_index : integer;

   -- return ASCII value of given string at the position defined by address_i(11 downto 0)
   function str2data(str : string) return std_logic_vector is
   variable strpos : integer;
   begin
      strpos := to_integer(unsigned(address_i(11 downto 0))) + 1;
      if strpos <= str'length then
         return std_logic_vector(to_unsigned(character'pos(str(strpos)), 16));
      else
         return (others => '0'); -- zero terminated strings
      end if;
   end;

   -- return the dimensions of the Options menu
   function getDXDY(dx, dy, index: natural) return std_logic_vector is
   begin
      case index is
         when 0 => return std_logic_vector(to_unsigned(dx + 2, 16));
         when 1 => return std_logic_vector(to_unsigned(dy + 2, 16));
         when others => return (others => '0');
      end case;
   end;

   -- convert bool to std_logic_vector
   function bool2slv(b: boolean) return std_logic_vector is
   begin
      if b then
         return x"0001";
      else
         return x"0000";
      end if;
   end;

   -- return the General Configuration settings
   function getGenConf(index: natural) return std_logic_vector is
   begin
      case index is
         when 0      => return bool2slv(RESET_KEEP);
         when 1      => return std_logic_vector(to_unsigned(RESET_COUNTER, 16));
         when 2      => return bool2slv(OPTM_PAUSE);
         when 3      => return bool2slv(WELCOME_ACTIVE);
         when 4      => return bool2slv(WELCOME_AT_RESET);
         when 5      => return bool2slv(KEYBOARD_AT_RESET);
         when 6      => return bool2slv(JOY_1_AT_RESET);
         when 7      => return bool2slv(JOY_2_AT_RESET);
         when 8      => return bool2slv(KEYBOARD_AT_OSD);
         when 9      => return bool2slv(JOY_1_AT_OSD);
         when 10     => return bool2slv(JOY_2_AT_OSD);
         when 11     => return std_logic_vector(to_unsigned(ASCAL_USAGE, 16));
         when 12     => return std_logic_vector(to_unsigned(ASCAL_MODE, 16));
         when 13     => return std_logic_vector(to_unsigned(VD_ANTI_THRASHING_DELAY, 16));
         when 14     => return std_logic_vector(to_unsigned(VD_ITERATION_SIZE, 16));
         when 15     => return bool2slv(SAVE_SETTINGS);
         when others => return x"0000";
      end case;
   end;

begin
   index := to_integer(unsigned(address_i(11 downto 0)));
   whs_array_index := to_integer(unsigned(address_i(23 downto 20)));
   whs_page_index  := to_integer(unsigned(address_i(19 downto 12)));

   if falling_edge(clk_i) then
      data_o <= x"EEEE";

      -----------------------------------------------------------------------------------
      -- Welcome & Help System: upper 4 bits of address equal SEL_WHS' upper 4 bits
      -----------------------------------------------------------------------------------

      if address_i(27 downto 24) = SEL_WHS(15 downto 12) then

         if  whs_array_index < WHS_RECORDS then
            if index = 4095 then
               data_o <= std_logic_vector(to_unsigned(WHS(whs_array_index).page_count, 16));
            else
               if index < WHS(whs_array_index).page_length(whs_page_index) then
                  data_o <= std_logic_vector(to_unsigned(character'pos(
                                             WHS_DATA(WHS(whs_array_index).page_start(whs_page_index) + index + 1)
                                            ), 16));
               else
                  data_o <= (others => '0'); -- zero-terminated strings
               end if;
            end if;
         end if;

      -----------------------------------------------------------------------------------
      -- All other selectors, which are 16-bit values
      -----------------------------------------------------------------------------------

      else

         case address_i(27 downto 12) is
            when SEL_GENERAL           => data_o <= getGenConf(index);
            when SEL_DIR_START         => data_o <= str2data(DIR_START);
            when SEL_CFG_FILE          => data_o <= str2data(CFG_FILE);
            when SEL_OPTM_ITEMS        => data_o <= str2data(OPTM_ITEMS);
            when SEL_OPTM_MOUNT_STR    => data_o <= str2data(OPTM_S_MOUNT);
            when SEL_OPTM_SAVING_STR   => data_o <= str2data(OPTM_S_SAVING);
            when SEL_OPTM_GROUPS       => data_o <= std_logic(to_unsigned(OPTM_GROUPS(index), 16)(15)) & "00" &
                                                    std_logic(to_unsigned(OPTM_GROUPS(index), 16)(12)) & "0000" &
                                                    std_logic_vector(to_unsigned(OPTM_GROUPS(index), 16)(7 downto 0));
            when SEL_OPTM_STDSEL       => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(8));
            when SEL_OPTM_LINES        => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(9));
            when SEL_OPTM_START        => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(10));
            when SEL_OPTM_MOUNT_DRV    => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(11));
            when SEL_OPTM_HELP         => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(13));
            when SEL_OPTM_SINGLESEL    => data_o <= x"000" & "000" & std_logic(to_unsigned(OPTM_GROUPS(index), 16)(15));
            when SEL_OPTM_ICOUNT       => data_o <= x"00" & std_logic_vector(to_unsigned(OPTM_SIZE, 8));
            when SEL_OPTM_DIMENSIONS   => data_o <= getDXDY(OPTM_DX, OPTM_DY, index);

            when others                => null;
         end case;
      end if;
   end if;
end process;

end beh;
