#include <stdint.h>
#include <stdio.h>

enum {MaxCodeLength = 4};

typedef struct {
    uint8_t code[MaxCodeLength];
    size_t length;
} CodeUnits;

int encode (uint32_t code_point, CodeUnits* code_units) {
    uint32_t r = code_point, count = 0; // Ð¡Ñ‡Ð¸Ñ‚Ð°ÐµÑ‚ ÐºÐ¾Ð»-Ð²Ð¾ Ð±Ð¸Ñ‚ Ð² Ñ‡Ð¸ÑÐ»Ðµ
    while (r > 0) {
        r = r >> 1;
        count++;
    }
    if (count < 8) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð¼ÐµÐ½ÑŒÑˆÐµ 8 Ð±Ð¸Ñ‚,Ñ‚Ð¾ Ð¾Ð½Ð° Ð¿Ð¾Ð¿Ð°Ð´Ð°ÐµÑ‚ Ð² 1 Ð¾ÐºÑ‚ÐµÑ‚
        code_units->length = 1;
        code_units->code[0] = code_point;
    }
    if (count > 8 && count <= 11) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð±Ð¾Ð»ÑŒÑˆÐµ 8,Ð½Ð¾ Ð¼ÐµÐ½ÑŒÑˆÐµ Ð¸Ð»Ð¸ Ñ€Ð°Ð²Ð½Ð° 11 Ð±Ð¸Ñ‚Ð°Ð¼,Ñ‚Ð¾ Ð¾Ð½Ð° Ð¿Ð¾Ð¿Ð°Ð´Ð°ÐµÑ‚ Ð² 2Ð¾ÐºÑ‚ÐµÑ‚Ð°
        code_units->length = 2;
        code_units->code[0] = code_units->code[0] | 192; // 11000000
        code_units->code[1] = code_point & 63;           // 00xxxxxx
        code_units->code[1] = code_units->code[1] | 128; // 10xxxxxx
        code_point = code_point >> 6;
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 11 && count <= 16) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð±Ð¾Ð»ÑŒÑˆÐµ 11,Ð½Ð¾ Ð¼ÐµÐ½ÑŒÑˆÐµ Ð¸Ð»Ð¸ Ñ€Ð°Ð²Ð½Ð° 16 Ð±Ð¸Ñ‚Ð°Ð¼,Ñ‚Ð¾ Ð¾Ð½Ð° Ð¿Ð¾Ð¿Ð°Ð´Ð°ÐµÑ‚ Ð² 3 Ð¾ÐºÑ‚ÐµÑ‚Ð°
        code_units->length = 3;
        code_units->code[0] = code_units->code[0] | 224; // 11100000
        for (int i = code_units->length - 1; i >= 1; i--) {
            code_units->code[i] = code_point & 63;           // 00xxxxxx
            code_units->code[i] = code_units->code[i] | 128; // 10xxxxxx
            code_point = code_point >> 6;
        }
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 16 && count <= 21) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð±Ð¾Ð»ÑŒÑˆÐµ 16,Ð½Ð¾ Ð¼ÐµÐ½ÑŒÑˆÐµ Ð¸Ð»Ð¸ Ñ€Ð°Ð²Ð½Ð° 21 Ð±Ð¸Ñ‚Ñƒ,Ñ‚Ð¾ Ð¾Ð½Ð° Ð¿Ð¾Ð¿Ð°Ð´Ð°ÐµÑ‚ Ð² 4 Ð¾ÐºÑ‚ÐµÑ‚Ð°
        code_units->length = 4;
        code_units->code[0] = code_units->code[0] | 240; // 11110000
        for (int i = code_units->length - 1; i >= 1; i--) {
            code_units->code[i] = code_point & 63;           // 00xxxxxx
            code_units->code[i] = code_units->code[i] | 128; // 10xxxxxx
            code_point = code_point >> 6;
        }
        code_units->code[0] = code_units->code[0] | code_point;
    }
    if (count > 21) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð¿Ñ€ÐµÐ²Ñ‹ÑˆÐ°ÐµÑ‚ Ð±Ð¾Ð»ÑŒÑˆÐµ 21 Ð±Ð¸Ñ‚Ð°,Ñ‚Ð¾ Ð¾Ð½Ð° Ð¿Ð¾Ð»Ð½Ð¾ÑÑ‚ÑŒÑŽ Ð½Ðµ Ð·Ð°ÐºÐ¾Ð´Ð¸Ñ€ÑƒÐµÑ‚ÑÑ Ð¸ Ð½ÑƒÐ¶Ð½Ð¾ Ð²ÐµÑ€Ð½ÑƒÑ‚ÑŒ -1
        return -1;
    }
    return 0;
}

uint32_t decode (const CodeUnits* code_unit) {
    uint32_t dec = 0; // Ð”ÐµÐºÐ¾Ð´Ð¸Ñ€ÑƒÐµÐ¼Ð¾Ðµ Ñ‡Ð¸ÑÐ»Ð¾

    if (code_unit->length  == 1) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð¸Ð¼ÐµÐµÑ‚ Ð²ÑÐµÐ³Ð¾ 1 Ð¾ÐºÑ‚ÐµÑ‚,Ñ‚Ð¾ Ð´ÐµÐºÐ¾Ð´Ð¸Ñ€ÑƒÐµÐ¼Ð¾Ðµ Ñ‡Ð¸ÑÐ»Ð¾ ÑÑ‚Ð°Ð½Ð¾Ð²Ð¸Ñ‚ÑÑ Ñ€Ð°Ð²Ð½Ñ‹Ð¼ ÑÑ‚Ð¾Ð¼Ñƒ Ñ‡Ð¸ÑÐ»Ñƒ
        dec = code_unit->code[0];
    }
    if (code_unit->length == 2) { // Ð•ÑÐ»Ð¸ Ñ‡Ð¸ÑÐ»Ð¾ Ð¸Ð¼ÐµÐµÑ‚ 2 Ð¾ÐºÑ‚ÐµÑ‚Ð°
        dec = code_unit->code[0] & 31; // 00011111 // Ð¾Ð±Ð½ÑƒÐ»ÑÐµÐ¼ Ð·Ð½Ð°Ñ‡Ð°Ñ‰Ð¸Ðµ ÐµÐ´Ð¸Ð½Ð¸Ñ†Ñ‹ Ð¸ Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ 1 Ð¾ÐºÑ‚ÐµÑ‚
        dec = dec << 6; // ÑÐ´Ð²Ð¸Ð³Ð°ÐµÐ¼ Ð½Ð° 6 Ð±Ð¸Ñ‚ Ð²Ð¿Ñ€Ð°Ð²Ð¾,Ñ‡Ñ‚Ð¾Ð±Ñ‹ Ð·Ð°Ð¿Ð¸ÑÐ°Ñ‚ÑŒ 2 Ð¾ÐºÑ‚ÐµÑ‚
        dec = dec | (63 & code_unit->code[1]); // Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ 2 Ð¾ÐºÑ‚ÐµÑ‚
    }
    if (code_unit->length == 3) {
        dec = code_unit->code[0] & 15; // 000011111
        for (size_t i = 1; i < code_unit->length;  i++) { // Ð¦Ð¸ÐºÐ»Ð¸Ñ‡Ð½Ð¾ Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ Ð¾ÐºÑ‚ÐµÑ‚Ñ‹
            dec = dec << 6;
            dec = dec | (63 & code_unit->code[i]);
        }
    }
    if (code_unit->length == 4) {
        dec = code_unit->code[0] & 7; // 00000111
        for (size_t i = 1; i < code_unit->length; i++) {
            dec = dec << 6;
            dec = dec | (63 & code_unit->code[i]);
        }
    }
    return dec;
}

int write_code_unit (FILE* out, const CodeUnits* code_unit) {
    for (size_t i = code_unit->length; i > 0; i--) { // Ð¦Ð¸ÐºÐ»Ð¸Ñ‡Ð½Ð¾ Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ Ð±Ð°Ð¹Ñ‚Ñ‹ Ð² Ñ„Ð°Ð¹Ð»
        size_t count = fwrite(
                &code_unit->code[code_unit->length - i], 1, sizeof(uint8_t), out);
        if (count != sizeof(uint8_t)) {
            return -1;
        }
    }
    return 0;
}

int read_next_code_unit (FILE* in, CodeUnits* code_units) {
    code_units->length = 0;

    size_t t = fread(&code_units->code[0],sizeof(uint8_t),1,  in); //ÑÑ‡Ð¸Ñ‚Ñ‹Ð²Ð°ÐµÐ¼ Ð¿ÐµÑ€Ð²Ñ‹Ð¹ Ð±Ð°Ð¹Ñ‚
    if (t != 1) {
        return -1;
    }
    if (code_units->code[0] < 128) //ÐµÑÐ»Ð¸ Ð¿ÐµÑ€Ð²Ñ‹Ð¹ Ð±Ð¸Ñ‚ 0
    {
        code_units->length = 1;
    }

        if (code_units->code[0] < 192) { // ÐµÑÐ»Ð¸ 10,Ñ‡Ñ‚Ð¾ Ð½Ðµ ÑÐ¾Ð¾Ñ‚Ð²ÐµÑ‚ÑÑ‚Ð²ÑƒÐµÑ‚ ÑˆÐ°Ð±Ð»Ð¾Ð½Ñƒ
            return read_next_code_unit(in, code_units); // 10xxxxxx
        }
        // ÐŸÑ€Ð¸ÑÐ²Ð¾Ð¸Ð¼ ÐºÐ¾Ð»-Ð²Ð¾ Ð¾ÐºÑ‚ÐµÑ‚Ð¾Ð² Ð² Ð·Ð°Ð²Ð¸ÑÐ¸Ð¼Ð¾ÑÑ‚Ð¸ Ð¾Ñ‚ ÑˆÐ°Ð±Ð»Ð¾Ð½Ð°
        if (code_units->code[0] < 248) { // 11110xxx
            code_units->length = 4;
        }
        if (code_units->code[0] < 240) { // 1110xxxx
            code_units->length = 3;
        }
        if (code_units->code[0] < 224) { // 110xxxxx
            code_units->length = 2;
        }
        for (size_t i = 1; i < code_units->length; i++) { // Ð¦Ð¸ÐºÐ»Ð¸Ñ‡Ð½Ð¾ ÑÑ‡Ð¸Ñ‚Ñ‹Ð²Ð°ÐµÐ¼ Ð±Ð°Ð¹Ñ‚Ñ‹ Ð² Ð·Ð°Ð²Ð¸ÑÐ¸Ð¼Ð¾ÑÑ‚Ð¸ Ð¾Ñ‚ Ð¾ÐºÑ‚ÐµÑ‚Ð¾Ð²
            t = fread(&code_units->code[i], sizeof(uint8_t), 1, in);
            if (t != 1) {
                return -1;
            }
            if (code_units->code[i] >> 6 != 2) {
                return read_next_code_unit(in, code_units);
            }
        }
    if (code_units->length == 0) //ÐµÑÐ»Ð¸ Ð½Ðµ Ð·Ð°ÑˆÑ‘Ð» Ð² Ñ†Ð¸ÐºÐ» - Ð±Ð¸Ñ‚Ñ‹Ð¹ Ð±Ð°Ð¹Ñ‚ (10xxxxxx)
    {
        return read_next_code_unit(in, code_units);
    }
    return 0;
}

