#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COIN_KINDS   5U
#define TOY_COUNT    4U
#define NAME_LEN    16U

static const uint16_t COIN_VALUE[COIN_KINDS] = { 25U, 50U, 100U, 200U, 500U };

static uint16_t coinCount[COIN_KINDS];

typedef struct {
    char     name[NAME_LEN];
    uint32_t price;        
} Toy_t;

static Toy_t shop[TOY_COUNT];

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* consume extra input */
    }
}

static void seedBank(void) {
    uint8_t i;
    for (i = 0U; i < COIN_KINDS; i++) {
        coinCount[i] = 4U;
    }

    strncpy(shop[0].name, "Whistle", NAME_LEN - 1U);
    shop[0].name[NAME_LEN - 1U] = '\0';
    shop[0].price = 150U;

    strncpy(shop[1].name, "Yo-Yo", NAME_LEN - 1U);
    shop[1].name[NAME_LEN - 1U] = '\0';
    shop[1].price = 350U;

    strncpy(shop[2].name, "Spin Top", NAME_LEN - 1U);
    shop[2].name[NAME_LEN - 1U] = '\0';
    shop[2].price = 750U;

    strncpy(shop[3].name, "Robot Car", NAME_LEN - 1U);
    shop[3].name[NAME_LEN - 1U] = '\0';
    shop[3].price = 1500U;

    printf("\n[Bank seeded with default coins and shop items!]\n");
}

static uint32_t bankTotal(void) {
    uint32_t total = 0U;
    uint8_t i;
    for (i = 0U; i < COIN_KINDS; i++) {
        total += (uint32_t)coinCount[i] * COIN_VALUE[i];
    }
    return total;
}

static uint32_t sumCoins(const uint16_t *counts, uint8_t n) {
    if (n == 0U) {
        return 0U;
    }
    return (uint32_t)counts[n - 1U] + sumCoins(counts, n - 1U);
}

static uint8_t biggestPile(void) {
    uint8_t maxIdx = 0U;
    uint8_t i;
    for (i = 1U; i < COIN_KINDS; i++) {
        if (coinCount[i] > coinCount[maxIdx]) {
            maxIdx = i;
        }
    }
    return maxIdx;
}

static void drawBar(uint16_t value, uint16_t full, uint8_t width) {
    uint8_t filled;
    uint8_t i;
    if (value > full) {
        value = full;
    }
    if (full == 0U) {
        filled = 0U;
    } else {
        filled = (uint8_t)(((uint32_t)value * width) / full);
    }
    for (i = 0U; i < width; i++) {
        if (i < filled) {
            putchar('#');
        } else {
            putchar('-');
        }
    }
}

static void showBank(void) {
    uint8_t i;
    uint16_t maxCount = coinCount[biggestPile()];
    if (maxCount == 0U) {
        maxCount = 1U;
    }
    printf("\n--- PIGGY BANK CONTENTS ---\n");
    for (i = 0U; i < COIN_KINDS; i++) {
        printf("Kind %u [%3u Pt]: %3u coins | [", i, COIN_VALUE[i], coinCount[i]);
        drawBar(coinCount[i], maxCount, 15U);
        printf("]\n");
    }
}

static void addCoins(void) {
    int idx;
    int qty;

    printf("Enter coin kind index (0-4): ");
    if (scanf("%d", &idx) != 1 || idx < 0 || idx >= (int)COIN_KINDS) {
        printf("That coin kind does not exist. Nothing changed.\n");
        clearInputBuffer();
        return;
    }

    printf("Enter quantity to add: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity. Nothing changed.\n");
        clearInputBuffer();
        return;
    }

    coinCount[idx] += (uint16_t)qty;
    printf("Successfully added %d x %u Pt coin(s).\n", qty, COIN_VALUE[idx]);
}

static void takeCoins(void) {
    int idx;
    int qty;

    printf("Enter coin kind index (0-4): ");
    if (scanf("%d", &idx) != 1 || idx < 0 || idx >= (int)COIN_KINDS) {
        printf("Invalid coin kind. Refused and took nothing.\n");
        clearInputBuffer();
        return;
    }

    printf("Enter quantity to take out: ");
    if (scanf("%d", &qty) != 1 || qty <= 0) {
        printf("Invalid quantity. Refused and took nothing.\n");
        clearInputBuffer();
        return;
    }

    if ((uint16_t)qty > coinCount[idx]) {
        printf("Refused! You only have %u of that coin. Took nothing.\n", coinCount[idx]);
        return;
    }

    coinCount[idx] -= (uint16_t)qty;
    printf("Successfully removed %d x %u Pt coin(s).\n", qty, COIN_VALUE[idx]);
}

static void buyToy(void) {
    uint8_t i;
    int choice;
    uint32_t total = bankTotal();

    printf("\n--- TOY SHOP ---\n");
    for (i = 0U; i < TOY_COUNT; i++) {
        printf("  [%u] %-12s - %u Pt\n", i, shop[i].name, shop[i].price);
    }
    printf("Which toy would you like to check (0-3)? ");
    if (scanf("%d", &choice) != 1 || choice < 0 || choice >= (int)TOY_COUNT) {
        printf("Invalid toy selection.\n");
        clearInputBuffer();
        return;
    }

    if (total >= shop[choice].price) {
        printf("You can buy %s! You will have %u Pt left over.\n",
               shop[choice].name, total - shop[choice].price);
    } else {
        printf("You cannot afford %s. You need %u more Pt.\n",
               shop[choice].name, shop[choice].price - total);
    }
}

static void bankReport(void) {
    uint32_t totalMoney = bankTotal();
    uint32_t totalCoinCount = sumCoins(coinCount, COIN_KINDS);
    uint8_t tallestIdx = biggestPile();
    uint8_t affordableToys = 0U;
    uint8_t i;

    for (i = 0U; i < TOY_COUNT; i++) {
        if (totalMoney >= shop[i].price) {
            affordableToys++;
        }
    }

    printf("\n================ BANK REPORT ================\n");
    printf(" Total Money Inside : %u Pt\n", totalMoney);
    printf(" Total Coin Count   : %u coin(s)\n", totalCoinCount);
    printf(" Tallest Coin Pile  : %u Pt coin (%u coins)\n",
           COIN_VALUE[tallestIdx], coinCount[tallestIdx]);
    printf(" Toys You Can Afford: %u of %u\n", affordableToys, TOY_COUNT);
    printf("=============================================\n");
}

int main(void) {
    int choice = 0;
    seedBank();

    do {
        printf("\n=== PIGGY BANK MENU ===\n");
        printf("1. Show Piggy Bank Visuals\n");
        printf("2. Add Coins\n");
        printf("3. Take Coins\n");
        printf("4. Check Toy Shop\n");
        printf("5. Bank Summary Report\n");
        printf("6. Reset / Seed Bank\n");
        printf("7. Exit\n");
        printf("Choose an option (1-7): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 7.\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: showBank(); break;
            case 2: addCoins(); break;
            case 3: takeCoins(); break;
            case 4: buyToy(); break;
            case 5: bankReport(); break;
            case 6: seedBank(); break;
            case 7: printf("Goodbye!\n"); break;
            default: printf("Please pick a valid option (1-7).\n"); break;
        }
    } while (choice != 7);

    return 0;
}