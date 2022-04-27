#include "coder.c"
#include <inttypes.h>
#include <stdlib.h>

int encode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* in = fopen(in_file_name, "r"); //Ð¾Ñ‚ÐºÑ€Ð¾ÐµÐ¼ Ñ„Ð°Ð¹Ð» Ð´Ð»Ñ Ð² Ñ€ÐµÐ¶Ð¸Ð¼Ðµ Ñ‡Ñ‚ÐµÐ½Ð¸Ñ
    if (in == NULL) {
        return -1;
    }
    FILE* out = fopen(out_file_name, "wb"); //Ð¾Ñ‚ÐºÑ€Ð¾ÐµÐ¼ Ð±Ð¸Ð½Ð°Ñ€Ð½Ñ‹Ð¹ Ñ„Ð°Ð¹Ð» Ð² Ñ€ÐµÐ¶Ð¸Ð¼Ðµ Ð·Ð°Ð¿Ð¸ÑÐ¸

    if (out == NULL) { // Ð•ÑÐ»Ð¸ Ñ„Ð°Ð¹Ð» Ð½Ðµ Ð¾Ñ‚ÐºÑ€Ñ‹Ð»ÑÑ Ð²Ð¾Ð·Ð²Ñ€Ð°Ñ‰Ð°ÐµÐ¼ -1
        fclose(in);
        return -1;
    }
    CodeUnits* c = malloc(sizeof(CodeUnits)); //Ð¡Ð¾Ð·Ð´Ð°ÐµÐ¼ ÑÑ‚Ñ€ÑƒÐºÑ‚ÑƒÑ€Ñƒ
    uint32_t n;
    while (feof(in) == 0) {
        size_t k = fscanf(in, "%" SCNx32, &n); //ÑÑ‡Ð¸Ñ‚Ñ‹Ð²Ð°ÐµÐ¼ Ñ‡Ð¸ÑÐ»Ð¾ Ð¸Ð· Ñ„Ð°Ð¹Ð»Ð° Ð¸ ÐºÐ¾Ð´Ð¸Ñ€ÑƒÐµÐ¼
        int t = encode(n, c);
        int y = write_code_unit(out, c); //Ð·Ð°Ð¿Ð¸ÑÑ‹Ð²Ð°ÐµÐ¼ Ð² Ð±Ð¸Ð½Ð°Ñ€Ð½Ñ‹Ð¹ Ñ„Ð°Ð¹Ð» Ð·Ð°ÐºÐ¾Ð´Ð¸Ñ€Ð¾Ð²Ð°Ð½Ð¾Ðµ Ñ‡Ð¸ÑÐ»Ð¾
        if (t != 0 || y != 0 || k != 1) // ÐµÑÐ»Ð¸ Ð¿Ñ€Ð¾Ð¸Ð·Ð¾ÑˆÐ»Ð° Ð¾ÑˆÐ¸Ð±ÐºÐ°,Ñ‚Ð¾ Ð·Ð°ÐºÑ€Ñ‹Ð²Ð°ÐµÐ¼ Ð²ÑÐµ Ñ„Ð°Ð¹Ð»Ñ‹,Ð¾ÑÐ²Ð¾Ð±Ð¾Ð¶Ð´Ð°ÐµÐ¼ Ð¿Ð°Ð¼ÑÑ‚ÑŒ Ð¸ Ð²Ð¾Ð·Ñ€Ð°Ñ‰Ð°ÐµÐ¼ -1
        {
            free(c);
            fclose(in);
            fclose(out);
            return -1;
        }
    }

    fclose(in);
    fclose(out);
    free(c);
    return 0;
}

int decode_file(const char* in_file_name, const char* out_file_name)
{
    FILE* in = fopen(in_file_name, "rb");
    if (in == NULL) {
        return -1;
    }
    FILE* out = fopen(out_file_name, "w");
    if (out == NULL) {
        fclose(in);
        return -1;
    }
    CodeUnits* c = malloc(sizeof(CodeUnits));
    while (read_next_code_unit(in, c) != -1) //Ð¿Ð¾ÐºÐ° Ð½Ðµ Ð½Ð°Ð¹Ð´ÐµÐ½Ð° Ð¾ÑˆÐ¸Ð±ÐºÐ° Ð² Ñ„ÑƒÐ½ÐºÑ†Ð¸Ð¸ Ð¼Ñ‹ Ð´ÐµÐºÐ¾Ð´Ð¸Ñ€ÑƒÐµÐ¼ Ð¸ Ð¿Ð¸ÑˆÐµÐ¼ Ð² Ñ„Ð°Ð¹Ð»
    {
        uint32_t dec = decode(c);
        size_t t = fprintf(out, "\n%" SCNx32, dec);
        if (t == 0) // Ð•ÑÐ»Ð¸ Ð½Ðµ Ð·Ð°Ð¿Ð¸ÑÐ°Ð»Ð¾ Ñ‡Ð¸ÑÐ»Ð¾
        {
            free(c);
            return -1;
        }
    }
    free(c);
    return 0;
}

