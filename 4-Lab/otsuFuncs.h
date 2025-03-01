﻿#pragma once
#include <omp.h>
#include <fstream>
#include <cstring>
#include <vector>
#include "PnmImage.h"

double* calculateProbabilities(const PnmImage& image, bool ompEnabled = true);

double* calculatePrefOmegas(const double* probability);

double* calculatePrefMus(const double* probability);

double getPrefOmegaRange(const double* omega, int left, int right);

double getPrefMuRange(const double* mu, int left, int right);

std::vector<int> calculateOtsuThresholds(const PnmImage& image, bool ompEnabled = true);
