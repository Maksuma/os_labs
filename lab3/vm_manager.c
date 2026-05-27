#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE       256
#define FRAME_SIZE      256
#define NUM_PAGES       256
#define NUM_FRAMES      256
#define TLB_SIZE        16
#define PHYS_MEM_SIZE   (NUM_FRAMES * FRAME_SIZE)  /* 65536 */

typedef struct {
    int page;
    int frame;
} TLBEntry;

static TLBEntry tlb[TLB_SIZE];
static int page_table[NUM_PAGES];
static signed char phys_mem[PHYS_MEM_SIZE];

static int next_free_frame = 0;
static int tlb_fifo = 0;


/* Поиск номера страницы в TLB; возвращает номер фрейма или -1 при промахе */
static int tlb_lookup(int page)
{
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page == page)
            return tlb[i].frame;
    }
    return -1;
}

/* Вставка/обновление записи TLB */
static void tlb_insert(int page, int frame)
{
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].page == page) {
            tlb[i].frame = frame;
            return;
        }
    }
    tlb[tlb_fifo].page  = page;
    tlb[tlb_fifo].frame = frame;
    tlb_fifo = (tlb_fifo + 1) % TLB_SIZE;
}

/* Загрузить страницу из backing store в физическую память */
static int load_page(int page_num, FILE *bs)
{
    int frame = next_free_frame++;

    /* Читаем PAGE_SIZE байт со смещения page_num * PAGE_SIZE */
    fseek(bs, (long)page_num * PAGE_SIZE, SEEK_SET);
    if (fread(&phys_mem[frame * FRAME_SIZE], sizeof(signed char), PAGE_SIZE, bs) != PAGE_SIZE) {
        fprintf(stderr, "Error: failed to read page %d from BACKING_STORE\n", page_num);
        exit(1);
    }

    page_table[page_num] = frame;
    return frame;
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s addresses.txt\n", argv[0]);
        return 1;
    }

    FILE *addr_file = fopen(argv[1], "r");
    if (!addr_file) {
        perror("Cannot open addresses file");
        return 1;
    }

    FILE *backing = fopen("BACKING_STORE.bin", "rb");
    if (!backing) {
        perror("Cannot open BACKING_STORE.bin");
        fclose(addr_file);
        return 1;
    }

    /* Инициализация */
    memset(page_table, -1, sizeof(page_table));
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].page = -1;

    int logical_addr;
    while (fscanf(addr_file, "%d", &logical_addr) == 1) {

        /* Маскируем до 16 бит */
        int addr16 = logical_addr & 0xFFFF;
        int page_num = (addr16 >> 8) & 0xFF;
        int offset = addr16 & 0xFF;

        /* 1. Проверяем TLB */
        int frame = tlb_lookup(page_num);
        if (frame == -1) {
            /* 2. Обращаемся к таблице страниц */
            frame = page_table[page_num];
            if (frame == -1) {
                /* 3. Ошибка страницы — загружаем из backing store */
                frame = load_page(page_num, backing);
            }
            /* Обновляем TLB */
            tlb_insert(page_num, frame);
        }

        int phys_addr = frame * FRAME_SIZE + offset;
        signed char val = phys_mem[phys_addr];

        printf("Virtual address: %d Physical address: %d Value: %d\n",
               logical_addr, phys_addr, (int)val);
    }

    fclose(addr_file);
    fclose(backing);
    return 0;
}
