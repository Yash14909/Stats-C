#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 100

typedef struct {
    double* values;
    size_t size;
} Dataset;

Dataset load_data(const char* filename);
void free_dataset(Dataset* ds);
double compute_mean(const Dataset* ds);
double compute_median(Dataset ds);
double compute_variance(const Dataset* ds, double mean);
void sort_dataset(Dataset* ds);

int main() {
    const char* filename = "data.csv";
    Dataset ds = load_data(filename);
    if (ds.size == 0) {
        printf("No data loaded.\n");
        return 1;
    }

    sort_dataset(&ds);
    double mean = compute_mean(&ds);
    double median = compute_median(ds);
    double variance = compute_variance(&ds, mean);
    double stddev = sqrt(variance);

    printf("Total entries: %zu\n", ds.size);
    printf("Mean: %.2f\n", mean);
    printf("Median: %.2f\n", median);
    printf("Variance: %.2f\n", variance);
    printf("Standard Deviation: %.2f\n", stddev);

    free_dataset(&ds);
    return 0;
}

Dataset load_data(const char* filename) {
    FILE* file = fopen(filename, "r");
    Dataset ds = {NULL, 0};
    if (!file) {
        perror("File opening failed");
        return ds;
    }

    double* values = NULL;
    size_t count = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {
        double value;
        if (sscanf(line, "%lf", &value) == 1) {
            values = realloc(values, (count + 1) * sizeof(double));
            if (!values) {
                perror("Memory allocation failed");
                fclose(file);
                ds.values = NULL;
                return ds;
            }
            values[count++] = value;
        }
    }

    fclose(file);
    ds.values = values;
    ds.size = count;
    return ds;
}

void free_dataset(Dataset* ds) {
    free(ds->values);
    ds->values = NULL;
    ds->size = 0;
}

double compute_mean(const Dataset* ds) {
    double sum = 0.0;
    for (size_t i = 0; i < ds->size; i++) {
        sum += ds->values[i];
    }
    return sum / ds->size;
}

double compute_median(Dataset ds) {
    if (ds.size % 2 == 0) {
        return (ds.values[ds.size / 2 - 1] + ds.values[ds.size / 2]) / 2.0;
    } else {
        return ds.values[ds.size / 2];
    }
}

double compute_variance(const Dataset* ds, double mean) {
    double sum_sq = 0.0;
    for (size_t i = 0; i < ds->size; i++) {
        double diff = ds->values[i] - mean;
        sum_sq += diff * diff;
    }
    return sum_sq / ds->size;
}

void sort_dataset(Dataset* ds) {
    for (size_t i = 0; i < ds->size - 1; i++) {
        for (size_t j = 0; j < ds->size - i - 1; j++) {
            if (ds->values[j] > ds->values[j + 1]) {
                double tmp = ds->values[j];
                ds->values[j] = ds->values[j + 1];
                ds->values[j + 1] = tmp;
            }
        }
    }
}
