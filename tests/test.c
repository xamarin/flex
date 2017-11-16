// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See the LICENSE.txt file in the project root
// for the license information.

const char *current_unit = NULL;
#define MAX_FAILURES 100
const char *failures[MAX_FAILURES];
unsigned int failures_n = 0;

int
main(void)
{
    void run_all_units();
    run_all_units();

    if (failures_n > 0) {
        printf("\n");
        for (int i = 0; i < failures_n; i++) {
            printf("%s\n", failures[i]);
        }
        return 1;
    }
    printf(" OK\n");
    return 0;
}
