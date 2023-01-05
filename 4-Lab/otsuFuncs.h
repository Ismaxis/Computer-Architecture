#pragma once
#include <omp.h>
#include <iostream>

#include "PnmImage.h"

constexpr int INTENSITY_LAYER_COUNT = 256;

double* calculateProbabilities(const PnmImage& image);

double* calculatePrefOmegas(const double* probability);

int calculateMuTotal(const PnmImage& image);

double* calculatePrefMus(const double* probabilities);

void incDigit(std::vector<int>& thresholds, int digitIndexToInc);

bool isFinished(const std::vector<int>& thresholds);

void updateToNextThresholdSignature(std::vector<int>& thresholds);

double getOmegaRange(const double* omega, const std::vector<int>& thresholds, int i);

double getMuRange(const double* mu, const std::vector<int>& thresholds, int i);

std::vector<int> otsuThreshold(const PnmImage& image, int thresholdsCount);
