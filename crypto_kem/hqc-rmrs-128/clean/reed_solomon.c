#include "fft.h"
#include "gf.h"
#include "parameters.h"
#include "reed_solomon.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/**
 * @file reed_solomon.c
 * Constant time implementation of Reed-Solomon codes
 */


static void compute_syndromes(uint16_t *syndromes, uint8_t *cdw);
static size_t compute_elp(uint16_t *sigma, const uint16_t *syndromes);
static void compute_roots(uint8_t *error, uint16_t *sigma);
static void compute_z_poly(uint16_t *z, const uint16_t *sigma, uint8_t degree, const uint16_t *syndromes);
static void compute_error_values(uint16_t *error_values, const uint16_t *z, const uint8_t *error);
static void correct_errors(uint8_t *cdw, const uint16_t *error_values);

static const uint16_t alpha_ij_pow [48][79] = {{2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240}, {4, 16, 64, 29, 116, 205, 19, 76, 45, 180, 234, 143, 6, 24, 96, 157, 78, 37, 148, 106, 181, 238, 159, 70, 5, 20, 80, 93, 105, 185, 222, 95, 97, 153, 94, 101, 137, 30, 120, 253, 211, 107, 177, 254, 223, 91, 113, 217, 67, 17, 68, 13, 52, 208, 103, 129, 62, 248, 199, 59, 236, 151, 102, 133, 46, 184, 218, 79, 33, 132, 42, 168, 154, 82, 85, 73, 57, 228, 183}, {8, 64, 58, 205, 38, 45, 117, 143, 12, 96, 39, 37, 53, 181, 193, 70, 10, 80, 186, 185, 161, 97, 47, 101, 15, 120, 231, 107, 127, 223, 182, 217, 134, 68, 26, 208, 206, 62, 237, 59, 197, 102, 23, 184, 169, 33, 21, 168, 41, 85, 146, 228, 115, 191, 145, 252, 179, 241, 219, 150, 196, 110, 87, 130, 100, 7, 56, 221, 166, 89, 242, 195, 86, 138, 36, 61, 245, 251, 139}, {16, 29, 205, 76, 180, 143, 24, 157, 37, 106, 238, 70, 20, 93, 185, 95, 153, 101, 30, 253, 107, 254, 91, 217, 17, 13, 208, 129, 248, 59, 151, 133, 184, 79, 132, 168, 82, 73, 228, 230, 198, 252, 123, 227, 150, 149, 165, 130, 200, 28, 221, 81, 121, 195, 172, 18, 61, 247, 203, 44, 250, 27, 173, 2, 32, 58, 135, 152, 117, 3, 48, 39, 74, 212, 193, 140, 40, 186, 111}, {32, 116, 38, 180, 3, 96, 156, 106, 193, 5, 160, 185, 190, 94, 15, 253, 214, 223, 226, 17, 26, 103, 124, 59, 51, 46, 169, 132, 77, 85, 114, 230, 145, 215, 255, 150, 55, 174, 100, 28, 167, 89, 239, 172, 36, 244, 235, 44, 233, 108, 1, 32, 116, 38, 180, 3, 96, 156, 106, 193, 5, 160, 185, 190, 94, 15, 253, 214, 223, 226, 17, 26, 103, 124, 59, 51, 46, 169, 132}, {64, 205, 45, 143, 96, 37, 181, 70, 80, 185, 97, 101, 120, 107, 223, 217, 68, 208, 62, 59, 102, 184, 33, 168, 85, 228, 191, 252, 241, 150, 110, 130, 7, 221, 89, 195, 138, 61, 251, 44, 207, 173, 8, 58, 38, 117, 12, 39, 53, 193, 10, 186, 161, 47, 15, 231, 127, 182, 134, 26, 206, 237, 197, 23, 169, 21, 41, 146, 115, 145, 179, 219, 196, 87, 100, 56, 166, 242, 86}, {128, 19, 117, 24, 156, 181, 140, 93, 161, 94, 60, 107, 163, 67, 26, 129, 147, 102, 109, 132, 41, 57, 209, 252, 255, 98, 87, 200, 224, 89, 155, 18, 245, 11, 233, 173, 16, 232, 45, 3, 157, 53, 159, 40, 185, 194, 137, 231, 254, 226, 68, 189, 248, 197, 46, 158, 168, 170, 183, 145, 123, 75, 110, 25, 28, 166, 249, 69, 61, 235, 176, 54, 2, 29, 38, 234, 48, 37, 119}, {29, 76, 143, 157, 106, 70, 93, 95, 101, 253, 254, 217, 13, 129, 59, 133, 79, 168, 73, 230, 252, 227, 149, 130, 28, 81, 195, 18, 247, 44, 27, 2, 58, 152, 3, 39, 212, 140, 186, 190, 202, 231, 225, 175, 26, 31, 118, 23, 158, 77, 146, 209, 229, 219, 55, 25, 56, 162, 155, 36, 243, 88, 54, 4, 116, 45, 6, 78, 181, 5, 105, 97, 137, 211, 223, 67, 52, 62, 236}, {58, 45, 12, 37, 193, 80, 161, 101, 231, 223, 134, 208, 237, 102, 169, 168, 146, 191, 179, 150, 87, 7, 166, 195, 36, 251, 125, 173, 64, 38, 143, 39, 181, 10, 185, 47, 120, 127, 217, 26, 62, 197, 184, 21, 85, 115, 252, 219, 110, 100, 221, 242, 138, 245, 44, 54, 8, 205, 117, 96, 53, 70, 186, 97, 15, 107, 182, 68, 206, 59, 23, 33, 41, 228, 145, 241, 196, 130, 56}, {116, 180, 96, 106, 5, 185, 94, 253, 223, 17, 103, 59, 46, 132, 85, 230, 215, 150, 174, 28, 89, 172, 244, 44, 108, 32, 38, 3, 156, 193, 160, 190, 15, 214, 226, 26, 124, 51, 169, 77, 114, 145, 255, 55, 100, 167, 239, 36, 235, 233, 1, 116, 180, 96, 106, 5, 185, 94, 253, 223, 17, 103, 59, 46, 132, 85, 230, 215, 150, 174, 28, 89, 172, 244, 44, 108, 32, 38, 3}, {232, 234, 39, 238, 160, 97, 60, 254, 134, 103, 118, 184, 84, 57, 145, 227, 220, 7, 162, 172, 245, 176, 71, 58, 180, 192, 181, 40, 95, 15, 177, 175, 208, 147, 46, 21, 73, 99, 241, 55, 200, 166, 43, 122, 44, 216, 128, 45, 48, 106, 10, 222, 202, 107, 226, 52, 237, 133, 66, 85, 209, 123, 196, 50, 167, 195, 144, 11, 54, 32, 76, 12, 148, 140, 185, 188, 211, 182, 13}, {205, 143, 37, 70, 185, 101, 107, 217, 208, 59, 184, 168, 228, 252, 150, 130, 221, 195, 61, 44, 173, 58, 117, 39, 193, 186, 47, 231, 182, 26, 237, 23, 21, 146, 145, 219, 87, 56, 242, 36, 139, 54, 64, 45, 96, 181, 80, 97, 120, 223, 68, 62, 102, 33, 85, 191, 241, 110, 7, 89, 138, 251, 207, 8, 38, 12, 53, 10, 161, 15, 127, 134, 206, 197, 169, 41, 115, 179, 196}, {135, 6, 53, 20, 190, 120, 163, 13, 237, 46, 84, 228, 229, 98, 100, 81, 69, 251, 131, 32, 45, 192, 238, 186, 94, 187, 217, 189, 236, 169, 82, 209, 241, 220, 28, 242, 72, 22, 173, 116, 201, 37, 140, 222, 15, 254, 34, 62, 204, 132, 146, 63, 75, 130, 167, 43, 245, 250, 4, 38, 24, 212, 80, 194, 253, 182, 52, 147, 184, 77, 183, 179, 149, 141, 89, 9, 203, 54, 128}, {19, 24, 181, 93, 94, 107, 67, 129, 102, 132, 57, 252, 98, 200, 89, 18, 11, 173, 232, 3, 53, 40, 194, 231, 226, 189, 197, 158, 170, 145, 75, 25, 166, 69, 235, 54, 29, 234, 37, 5, 95, 120, 91, 52, 59, 218, 82, 191, 227, 174, 221, 43, 247, 207, 32, 90, 39, 35, 111, 15, 225, 136, 237, 92, 77, 115, 246, 220, 56, 239, 122, 125, 4, 76, 96, 238, 105, 101, 177}, {38, 96, 193, 185, 15, 223, 26, 59, 169, 85, 145, 150, 100, 89, 36, 44, 1, 38, 96, 193, 185, 15, 223, 26, 59, 169, 85, 145, 150, 100, 89, 36, 44, 1, 38, 96, 193, 185, 15, 223, 26, 59, 169, 85, 145, 150, 100, 89, 36, 44, 1, 38, 96, 193, 185, 15, 223, 26, 59, 169, 85, 145, 150, 100, 89, 36, 44, 1, 38, 96, 193, 185, 15, 223, 26, 59, 169, 85, 145}, {76, 157, 70, 95, 253, 217, 129, 133, 168, 230, 227, 130, 81, 18, 44, 2, 152, 39, 140, 190, 231, 175, 31, 23, 77, 209, 219, 25, 162, 36, 88, 4, 45, 78, 5, 97, 211, 67, 62, 46, 154, 191, 171, 50, 89, 72, 176, 8, 90, 156, 10, 194, 187, 134, 124, 92, 41, 99, 75, 100, 178, 144, 125, 16, 180, 37, 20, 153, 107, 17, 248, 184, 82, 198, 150, 200, 121, 61, 250}, {152, 78, 10, 153, 214, 68, 147, 79, 146, 215, 220, 221, 69, 11, 1, 152, 78, 10, 153, 214, 68, 147, 79, 146, 215, 220, 221, 69, 11, 1, 152, 78, 10, 153, 214, 68, 147, 79, 146, 215, 220, 221, 69, 11, 1, 152, 78, 10, 153, 214, 68, 147, 79, 146, 215, 220, 221, 69, 11, 1, 152, 78, 10, 153, 214, 68, 147, 79, 146, 215, 220, 221, 69, 11, 1, 152, 78, 10, 153}, {45, 37, 80, 101, 223, 208, 102, 168, 191, 150, 7, 195, 251, 173, 38, 39, 10, 47, 127, 26, 197, 21, 115, 219, 100, 242, 245, 54, 205, 96, 70, 97, 107, 68, 59, 33, 228, 241, 130, 89, 61, 207, 58, 12, 193, 161, 231, 134, 237, 169, 146, 179, 87, 166, 36, 125, 64, 143, 181, 185, 120, 217, 62, 184, 85, 252, 110, 221, 138, 44, 8, 117, 53, 186, 15, 182, 206, 23, 41}, {90, 148, 186, 30, 226, 62, 109, 73, 179, 174, 162, 61, 131, 232, 96, 140, 153, 127, 52, 51, 168, 99, 98, 56, 172, 22, 8, 234, 212, 185, 240, 67, 237, 79, 114, 241, 25, 121, 245, 108, 19, 39, 20, 188, 223, 189, 133, 41, 63, 55, 221, 9, 176, 64, 3, 238, 161, 211, 34, 59, 66, 183, 219, 200, 239, 251, 71, 152, 37, 160, 137, 182, 129, 92, 85, 229, 165, 166, 72}, {180, 106, 185, 253, 17, 59, 132, 230, 150, 28, 172, 44, 32, 3, 193, 190, 214, 26, 51, 77, 145, 55, 167, 36, 233, 116, 96, 5, 94, 223, 103, 46, 85, 215, 174, 89, 244, 108, 38, 156, 160, 15, 226, 124, 169, 114, 255, 100, 239, 235, 1, 180, 106, 185, 253, 17, 59, 132, 230, 150, 28, 172, 44, 32, 3, 193, 190, 214, 26, 51, 77, 145, 55, 167, 36, 233, 116, 96, 5}, {117, 181, 161, 107, 26, 102, 41, 252, 87, 89, 245, 173, 45, 53, 185, 231, 68, 197, 168, 145, 110, 166, 61, 54, 38, 37, 186, 120, 134, 59, 21, 191, 196, 221, 36, 207, 205, 39, 80, 15, 217, 237, 33, 115, 150, 56, 138, 125, 58, 96, 10, 101, 182, 62, 169, 228, 219, 7, 86, 44, 64, 12, 70, 47, 223, 206, 184, 146, 241, 100, 195, 139, 8, 143, 193, 97, 127, 208, 23}, {234, 238, 97, 254, 103, 184, 57, 227, 7, 172, 176, 58, 192, 40, 15, 175, 147, 21, 99, 55, 166, 122, 216, 45, 106, 222, 107, 52, 133, 85, 123, 50, 195, 11, 32, 12, 140, 188, 182, 124, 158, 115, 49, 224, 36, 131, 19, 37, 105, 253, 68, 151, 154, 252, 174, 121, 251, 2, 201, 193, 194, 225, 206, 109, 114, 219, 14, 69, 125, 116, 157, 80, 30, 67, 59, 42, 198, 110, 81}, {201, 159, 47, 91, 124, 33, 209, 149, 166, 244, 71, 117, 238, 194, 223, 31, 79, 115, 98, 167, 61, 216, 90, 181, 190, 254, 206, 218, 213, 150, 224, 72, 54, 152, 106, 161, 177, 189, 184, 114, 171, 56, 18, 131, 38, 148, 111, 107, 104, 46, 146, 227, 14, 138, 233, 135, 37, 210, 211, 26, 133, 170, 241, 141, 172, 125, 232, 78, 186, 253, 136, 102, 164, 123, 100, 43, 88, 58, 157}, {143, 70, 101, 217, 59, 168, 252, 130, 195, 44, 58, 39, 186, 231, 26, 23, 146, 219, 56, 36, 54, 45, 181, 97, 223, 62, 33, 191, 110, 89, 251, 8, 12, 10, 15, 134, 197, 41, 179, 100, 86, 125, 205, 37, 185, 107, 208, 184, 228, 150, 221, 61, 173, 117, 193, 47, 182, 237, 21, 145, 87, 242, 139, 64, 96, 80, 120, 68, 102, 85, 241, 7, 138, 207, 38, 53, 161, 127, 206}, {3, 5, 15, 17, 51, 85, 255, 28, 36, 108, 180, 193, 94, 226, 59, 77, 215, 100, 172, 233, 38, 106, 190, 223, 124, 132, 145, 174, 239, 44, 116, 156, 185, 214, 103, 169, 230, 55, 89, 235, 32, 96, 160, 253, 26, 46, 114, 150, 167, 244, 1, 3, 5, 15, 17, 51, 85, 255, 28, 36, 108, 180, 193, 94, 226, 59, 77, 215, 100, 172, 233, 38, 106, 190, 223, 124, 132, 145, 174}, {6, 20, 120, 13, 46, 228, 98, 81, 251, 32, 192, 186, 187, 189, 169, 209, 220, 242, 22, 116, 37, 222, 254, 62, 132, 63, 130, 43, 250, 38, 212, 194, 182, 147, 77, 179, 141, 9, 54, 180, 159, 101, 67, 151, 85, 227, 112, 61, 142, 3, 10, 60, 136, 23, 114, 49, 166, 243, 16, 96, 93, 211, 208, 218, 230, 110, 121, 11, 58, 156, 111, 127, 31, 66, 145, 65, 155, 125, 19}, {12, 80, 231, 208, 169, 191, 87, 195, 125, 38, 181, 47, 217, 197, 85, 219, 221, 245, 8, 96, 186, 107, 206, 33, 145, 130, 86, 207, 45, 193, 101, 134, 102, 146, 150, 166, 251, 64, 39, 185, 127, 62, 21, 252, 100, 138, 54, 117, 70, 15, 68, 23, 228, 196, 89, 139, 58, 37, 161, 223, 237, 168, 179, 7, 36, 173, 143, 10, 120, 26, 184, 115, 110, 242, 44, 205, 53, 97, 182}, {24, 93, 107, 129, 132, 252, 200, 18, 173, 3, 40, 231, 189, 158, 145, 25, 69, 54, 234, 5, 120, 52, 218, 191, 174, 43, 207, 90, 35, 15, 136, 92, 115, 220, 239, 125, 76, 238, 101, 17, 133, 228, 149, 121, 44, 135, 212, 47, 175, 51, 146, 49, 162, 139, 116, 148, 97, 113, 236, 85, 171, 83, 251, 128, 156, 161, 163, 147, 41, 255, 224, 245, 16, 157, 185, 254, 248, 168, 123}, {48, 105, 127, 248, 77, 241, 224, 247, 64, 156, 95, 182, 236, 170, 150, 162, 11, 205, 212, 94, 134, 133, 213, 110, 239, 250, 45, 35, 30, 26, 218, 99, 130, 69, 108, 143, 40, 211, 206, 132, 229, 7, 144, 2, 96, 210, 254, 237, 154, 255, 221, 243, 128, 37, 190, 113, 197, 73, 49, 89, 22, 135, 181, 188, 17, 23, 183, 220, 195, 233, 90, 70, 60, 52, 169, 198, 25, 138, 216}, {96, 185, 223, 59, 85, 150, 89, 44, 38, 193, 15, 26, 169, 145, 100, 36, 1, 96, 185, 223, 59, 85, 150, 89, 44, 38, 193, 15, 26, 169, 145, 100, 36, 1, 96, 185, 223, 59, 85, 150, 89, 44, 38, 193, 15, 26, 169, 145, 100, 36, 1, 96, 185, 223, 59, 85, 150, 89, 44, 38, 193, 15, 26, 169, 145, 100, 36, 1, 96, 185, 223, 59, 85, 150, 89, 44, 38, 193, 15}, {192, 222, 182, 151, 114, 110, 155, 27, 143, 160, 177, 237, 82, 75, 89, 88, 152, 70, 240, 103, 21, 123, 224, 251, 116, 212, 101, 136, 218, 145, 200, 144, 8, 78, 190, 217, 204, 183, 87, 172, 216, 12, 105, 225, 59, 170, 98, 242, 250, 180, 10, 211, 31, 168, 255, 83, 139, 135, 238, 15, 52, 158, 252, 14, 244, 64, 74, 153, 134, 46, 209, 130, 9, 142, 96, 111, 91, 197, 57}, {157, 95, 217, 133, 230, 130, 18, 2, 39, 190, 175, 23, 209, 25, 36, 4, 78, 97, 67, 46, 191, 50, 72, 8, 156, 194, 134, 92, 99, 100, 144, 16, 37, 153, 17, 184, 198, 200, 61, 32, 74, 47, 34, 109, 145, 141, 122, 64, 148, 94, 68, 218, 63, 7, 244, 128, 53, 188, 136, 169, 126, 14, 245, 29, 106, 101, 13, 79, 252, 28, 247, 58, 212, 202, 26, 158, 229, 56, 243}, {39, 97, 134, 184, 145, 7, 245, 58, 181, 15, 208, 21, 241, 166, 44, 45, 10, 107, 237, 85, 196, 195, 54, 12, 185, 182, 102, 115, 130, 36, 8, 37, 47, 68, 169, 252, 56, 251, 205, 193, 120, 206, 168, 219, 89, 125, 117, 80, 127, 59, 146, 110, 86, 173, 96, 161, 217, 23, 191, 100, 61, 64, 53, 101, 26, 33, 179, 221, 139, 38, 70, 231, 62, 41, 150, 242, 207, 143, 186}, {78, 153, 68, 79, 215, 221, 11, 152, 10, 214, 147, 146, 220, 69, 1, 78, 153, 68, 79, 215, 221, 11, 152, 10, 214, 147, 146, 220, 69, 1, 78, 153, 68, 79, 215, 221, 11, 152, 10, 214, 147, 146, 220, 69, 1, 78, 153, 68, 79, 215, 221, 11, 152, 10, 214, 147, 146, 220, 69, 1, 78, 153, 68, 79, 215, 221, 11, 152, 10, 214, 147, 146, 220, 69, 1, 78, 153, 68, 79}, {156, 94, 26, 132, 255, 89, 233, 3, 185, 226, 46, 145, 28, 235, 38, 5, 214, 59, 114, 174, 36, 32, 106, 15, 103, 77, 150, 239, 108, 96, 190, 17, 169, 215, 167, 44, 180, 160, 223, 51, 230, 100, 244, 116, 193, 253, 124, 85, 55, 172, 1, 156, 94, 26, 132, 255, 89, 233, 3, 185, 226, 46, 145, 28, 235, 38, 5, 214, 59, 114, 174, 36, 32, 106, 15, 103, 77, 150, 239}, {37, 101, 208, 168, 150, 195, 173, 39, 47, 26, 21, 219, 242, 54, 96, 97, 68, 33, 241, 89, 207, 12, 161, 134, 169, 179, 166, 125, 143, 185, 217, 184, 252, 221, 44, 117, 186, 182, 23, 145, 56, 139, 45, 80, 223, 102, 191, 7, 251, 38, 10, 127, 197, 115, 100, 245, 205, 70, 107, 59, 228, 130, 61, 58, 193, 231, 237, 146, 87, 36, 64, 181, 120, 62, 85, 110, 138, 8, 53}, {74, 137, 206, 82, 55, 138, 16, 212, 120, 124, 73, 87, 72, 29, 193, 211, 147, 228, 25, 244, 205, 140, 177, 197, 230, 141, 251, 76, 40, 223, 204, 198, 56, 11, 180, 186, 113, 92, 252, 167, 176, 143, 111, 67, 169, 123, 162, 207, 24, 190, 68, 66, 227, 242, 108, 157, 47, 52, 84, 150, 155, 142, 37, 202, 103, 41, 149, 69, 8, 106, 60, 62, 170, 165, 36, 128, 238, 231, 199}, {148, 30, 62, 73, 174, 61, 232, 140, 127, 51, 99, 56, 22, 234, 185, 67, 79, 241, 121, 108, 39, 188, 189, 41, 55, 9, 64, 238, 211, 59, 183, 200, 251, 152, 160, 182, 92, 229, 166, 233, 24, 97, 13, 42, 150, 43, 2, 53, 60, 124, 146, 65, 122, 205, 5, 254, 102, 198, 112, 44, 201, 111, 134, 158, 255, 242, 216, 78, 101, 103, 82, 110, 18, 128, 193, 187, 118, 115, 141}, {53, 120, 237, 228, 100, 251, 45, 186, 217, 169, 241, 242, 173, 37, 15, 62, 146, 130, 245, 38, 80, 182, 184, 179, 89, 54, 39, 101, 206, 85, 87, 61, 205, 10, 223, 23, 252, 166, 207, 96, 47, 208, 41, 110, 36, 58, 70, 127, 102, 145, 221, 125, 12, 97, 26, 168, 196, 138, 64, 193, 107, 197, 191, 56, 44, 143, 161, 68, 21, 150, 86, 8, 181, 231, 59, 115, 7, 139, 117}, {106, 253, 59, 230, 28, 44, 3, 190, 26, 77, 55, 36, 116, 5, 223, 46, 215, 89, 108, 156, 15, 124, 114, 100, 235, 180, 185, 17, 132, 150, 172, 32, 193, 214, 51, 145, 167, 233, 96, 94, 103, 85, 174, 244, 38, 160, 226, 169, 255, 239, 1, 106, 253, 59, 230, 28, 44, 3, 190, 26, 77, 55, 36, 116, 5, 223, 46, 215, 89, 108, 156, 15, 124, 114, 100, 235, 180, 185, 17}, {212, 211, 197, 198, 167, 207, 157, 202, 62, 114, 200, 139, 201, 95, 26, 154, 220, 61, 19, 160, 217, 158, 171, 86, 32, 159, 127, 133, 229, 89, 216, 74, 120, 147, 230, 56, 176, 24, 47, 103, 170, 130, 243, 90, 185, 34, 42, 196, 18, 116, 10, 91, 109, 241, 239, 2, 181, 187, 151, 145, 83, 131, 39, 137, 124, 228, 141, 11, 143, 190, 52, 41, 165, 122, 38, 93, 175, 33, 75}, {181, 107, 102, 252, 89, 173, 53, 231, 197, 145, 166, 54, 37, 120, 59, 191, 221, 207, 39, 15, 237, 115, 56, 125, 96, 101, 62, 228, 7, 44, 12, 47, 206, 146, 100, 139, 143, 97, 208, 85, 130, 251, 117, 161, 26, 41, 87, 245, 45, 185, 68, 168, 110, 61, 38, 186, 134, 21, 196, 36, 205, 80, 217, 33, 150, 138, 58, 10, 182, 169, 219, 86, 64, 70, 223, 184, 241, 195, 8}, {119, 177, 23, 123, 239, 8, 159, 225, 184, 255, 43, 64, 140, 91, 169, 171, 69, 58, 20, 226, 33, 49, 18, 205, 160, 67, 21, 149, 144, 38, 105, 34, 168, 220, 244, 45, 111, 13, 41, 174, 243, 117, 95, 104, 85, 25, 203, 143, 194, 103, 146, 200, 22, 12, 94, 31, 228, 14, 176, 96, 202, 248, 115, 112, 233, 39, 30, 147, 191, 167, 27, 37, 240, 236, 145, 81, 216, 53, 211}, {238, 254, 184, 227, 172, 58, 40, 175, 21, 55, 122, 45, 222, 52, 85, 50, 11, 12, 188, 124, 115, 224, 131, 37, 253, 151, 252, 121, 2, 193, 225, 109, 219, 69, 116, 80, 67, 42, 110, 244, 90, 161, 104, 170, 100, 22, 24, 101, 248, 230, 221, 27, 74, 231, 51, 229, 242, 4, 159, 223, 218, 171, 138, 232, 160, 134, 84, 220, 245, 180, 95, 208, 73, 200, 44, 48, 202, 237, 209}, {193, 223, 169, 150, 36, 38, 185, 26, 85, 100, 44, 96, 15, 59, 145, 89, 1, 193, 223, 169, 150, 36, 38, 185, 26, 85, 100, 44, 96, 15, 59, 145, 89, 1, 193, 223, 169, 150, 36, 38, 185, 26, 85, 100, 44, 96, 15, 59, 145, 89, 1, 193, 223, 169, 150, 36, 38, 185, 26, 85, 100, 44, 96, 15, 59, 145, 89, 1, 193, 223, 169, 150, 36, 38, 185, 26, 85, 100, 44}, {159, 91, 33, 149, 244, 117, 194, 31, 115, 167, 216, 181, 254, 218, 150, 72, 152, 161, 189, 114, 56, 131, 148, 107, 46, 227, 138, 135, 210, 26, 170, 141, 125, 78, 253, 102, 123, 43, 58, 160, 34, 41, 25, 22, 96, 30, 236, 252, 249, 32, 10, 175, 84, 87, 235, 6, 101, 199, 198, 89, 2, 35, 182, 66, 55, 245, 234, 153, 62, 230, 83, 173, 119, 225, 169, 49, 144, 45, 95}, {35, 113, 21, 165, 235, 12, 137, 118, 252, 239, 128, 80, 34, 82, 100, 176, 78, 231, 133, 255, 138, 19, 111, 208, 114, 112, 54, 212, 254, 169, 98, 122, 117, 153, 124, 191, 162, 2, 70, 226, 42, 87, 203, 24, 15, 236, 229, 195, 29, 160, 68, 164, 200, 125, 156, 211, 23, 227, 9, 38, 222, 189, 228, 224, 108, 181, 225, 79, 196, 244, 234, 47, 248, 99, 89, 4, 140, 217, 84}, {70, 217, 168, 130, 44, 39, 231, 23, 219, 36, 45, 97, 62, 191, 89, 8, 10, 134, 41, 100, 125, 37, 107, 184, 150, 61, 117, 47, 237, 145, 242, 64, 80, 68, 85, 7, 207, 53, 127, 169, 196, 245, 143, 101, 59, 252, 195, 58, 186, 26, 146, 56, 54, 181, 223, 33, 110, 251, 12, 15, 197, 179, 86, 205, 185, 208, 228, 221, 173, 193, 182, 21, 87, 139, 96, 120, 102, 241, 138}};



/**
 * @brief Encodes a message message of PARAM_K bits to a Reed-Solomon codeword codeword of PARAM_N1 bytes
 *
 * Following @cite lin1983error (Chapter 4 - Cyclic Codes),
 * We perform a systematic encoding using a linear (PARAM_N1 - PARAM_K)-stage shift register
 * with feedback connections based on the generator polynomial PARAM_RS_POLY of the Reed-Solomon code.
 *
 * @param[out] cdw Array of size VEC_N1_SIZE_64 receiving the encoded message
 * @param[in] msg Array of size VEC_K_SIZE_64 storing the message
 */
void PQCLEAN_HQCRMRS128_CLEAN_reed_solomon_encode(uint64_t *cdw, const uint64_t *msg) {
    uint8_t gate_value = 0;

    uint16_t tmp[PARAM_G] = {0};
    uint16_t PARAM_RS_POLY [] = {RS_POLY_COEFS};

    uint8_t msg_bytes[PARAM_K] = {0};
    uint8_t cdw_bytes[PARAM_N1] = {0};

    for (size_t i = 0; i < VEC_K_SIZE_64; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            msg_bytes[i * 8 + j] = (uint8_t) (msg[i] >> (j * 8));
        }
    }

    for (int i = PARAM_K - 1; i >= 0; --i) {
        gate_value = msg_bytes[i] ^ cdw_bytes[PARAM_N1 - PARAM_K - 1];

        for (size_t j = 0; j < PARAM_G; ++j) {
            tmp[j] = PQCLEAN_HQCRMRS128_CLEAN_gf_mul(gate_value, PARAM_RS_POLY[j]);
        }

        for (size_t k = PARAM_N1 - PARAM_K - 1; k; --k) {
            cdw_bytes[k] = cdw_bytes[k - 1] ^ tmp[k];
        }

        cdw_bytes[0] = tmp[0];
    }

    memcpy(cdw_bytes + PARAM_N1 - PARAM_K, msg_bytes, PARAM_K);
    memcpy(cdw, cdw_bytes, PARAM_N1);
}



/**
 * @brief Computes 2 * PARAM_DELTA syndromes
 *
 * @param[out] syndromes Array of size 2 * PARAM_DELTA receiving the computed syndromes
 * @param[in] cdw Array of size PARAM_N1 storing the received vector
 */
void compute_syndromes(uint16_t *syndromes, uint8_t *cdw) {
    for (size_t i = 0; i < 2 * PARAM_DELTA; ++i) {
        for (size_t j = 1; j < PARAM_N1; ++j) {
            syndromes[i] ^= PQCLEAN_HQCRMRS128_CLEAN_gf_mul(cdw[j], alpha_ij_pow[i][j - 1]);
        }
        syndromes[i] ^= cdw[0];
    }
}



/**
 * @brief Computes the error locator polynomial (ELP) sigma
 *
 * This is a constant time implementation of Berlekamp's simplified algorithm (see @cite lin1983error (Chapter 6 - BCH Codes). <br>
 * We use the letter p for rho which is initialized at -1. <br>
 * The array X_sigma_p represents the polynomial X^(mu-rho)*sigma_p(X). <br>
 * Instead of maintaining a list of sigmas, we update in place both sigma and X_sigma_p. <br>
 * sigma_copy serves as a temporary save of sigma in case X_sigma_p needs to be updated. <br>
 * We can properly correct only if the degree of sigma does not exceed PARAM_DELTA.
 * This means only the first PARAM_DELTA + 1 coefficients of sigma are of value
 * and we only need to save its first PARAM_DELTA - 1 coefficients.
 *
 * @returns the degree of the ELP sigma
 * @param[out] sigma Array of size (at least) PARAM_DELTA receiving the ELP
 * @param[in] syndromes Array of size (at least) 2*PARAM_DELTA storing the syndromes
 */
static size_t compute_elp(uint16_t *sigma, const uint16_t *syndromes) {
    sigma[0] = 1;
    size_t deg_sigma = 0;
    size_t deg_sigma_p = 0;
    uint16_t sigma_copy[PARAM_DELTA + 1] = {0};
    size_t deg_sigma_copy = 0;
    uint16_t X_sigma_p[PARAM_DELTA + 1] = {0, 1};
    int32_t pp = -1; // 2*rho
    uint16_t d_p = 1;
    uint16_t d = syndromes[0];

    for (size_t mu = 0; (mu < (2 * PARAM_DELTA)); ++mu) {
        // Save sigma in case we need it to update X_sigma_p
        memcpy(sigma_copy, sigma, 2 * (PARAM_DELTA));
        deg_sigma_copy = deg_sigma;

        uint16_t dd = PQCLEAN_HQCRMRS128_CLEAN_gf_mul(d, PQCLEAN_HQCRMRS128_CLEAN_gf_inverse(d_p));

        for (size_t i = 1; (i <= mu + 1) && (i <= PARAM_DELTA); ++i) {
            sigma[i] ^= PQCLEAN_HQCRMRS128_CLEAN_gf_mul(dd, X_sigma_p[i]);
        }

        size_t deg_X = mu - pp;
        size_t deg_X_sigma_p = deg_X + deg_sigma_p;

        // mask1 = 0xffff if(d != 0) and 0 otherwise
        int16_t mask1 = -((uint16_t) - d >> 15);

        // mask2 = 0xffff if(deg_X_sigma_p > deg_sigma) and 0 otherwise
        int16_t mask2 = -((uint16_t) (deg_sigma - deg_X_sigma_p) >> 15);

        // mask12 = 0xffff if the deg_sigma increased and 0 otherwise
        int16_t mask12 = mask1 & mask2;
        deg_sigma = (mask12 & deg_X_sigma_p) ^ (~mask12 & deg_sigma);

        if (mu == (2 * PARAM_DELTA - 1)) {
            break;
        }

        pp = (mask12 & mu) ^ (~mask12 & pp);
        d_p = (mask12 & d) ^ (~mask12 & d_p);
        for (size_t i = PARAM_DELTA; i; --i) {
            X_sigma_p[i] = (mask12 & sigma_copy[i - 1]) ^ (~mask12 & X_sigma_p[i - 1]);
        }

        deg_sigma_p = (mask12 & deg_sigma_copy) ^ (~mask12 & deg_sigma_p);
        d = syndromes[mu + 1];

        for (size_t i = 1; (i <= mu + 1) && (i <= PARAM_DELTA); ++i) {
            d ^= PQCLEAN_HQCRMRS128_CLEAN_gf_mul(sigma[i], syndromes[mu + 1 - i]);
        }
    }

    return deg_sigma;
}



/**
 * @brief Computes the error polynomial error from the error locator polynomial sigma
 *
 * See function PQCLEAN_HQCRMRS128_CLEAN_fft for more details.
 *
 * @param[out] error Array of 2^PARAM_M elements receiving the error polynomial
 * @param[out] error_compact Array of PARAM_DELTA + PARAM_N1 elements receiving a compact representation of the vector error
 * @param[in] sigma Array of 2^PARAM_FFT elements storing the error locator polynomial
 */
static void compute_roots(uint8_t *error, uint16_t *sigma) {
    uint16_t w[1 << PARAM_M] = {0};

    PQCLEAN_HQCRMRS128_CLEAN_fft(w, sigma, PARAM_DELTA + 1);
    PQCLEAN_HQCRMRS128_CLEAN_fft_retrieve_error_poly(error, w);
}



/**
 * @brief Computes the polynomial z(x)
 *
 * See @cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * @param[out] z Array of PARAM_DELTA + 1 elements receiving the polynomial z(x)
 * @param[in] sigma Array of 2^PARAM_FFT elements storing the error locator polynomial
 * @param[in] degree Integer that is the degree of polynomial sigma
 * @param[in] syndromes Array of 2 * PARAM_DELTA storing the syndromes
 */
static void compute_z_poly(uint16_t *z, const uint16_t *sigma, uint8_t degree, const uint16_t *syndromes) {
    z[0] = 1;

    for (size_t i = 1; i < PARAM_DELTA + 1; ++i) {
        int16_t mask2 = -((uint16_t) (i - degree - 1) >> 15);
        z[i] = ((uint16_t)mask2) & sigma[i];
    }

    z[1] ^= syndromes[0];

    for (size_t i = 2; i <= PARAM_DELTA; ++i) {
        int16_t mask2 = -((uint16_t) (i - degree - 1) >> 15);
        z[i] ^= ((uint16_t)mask2 & syndromes[i - 1]);

        for (size_t j = 1; j < i; ++j) {
            z[i] ^= ((uint16_t)mask2) & PQCLEAN_HQCRMRS128_CLEAN_gf_mul(sigma[j], syndromes[i - j - 1]);
        }
    }
}



/**
 * @brief Computes the error values
 *
 * See @cite lin1983error (Chapter 6 - BCH Codes) for more details.
 *
 * @param[out] error_values Array of PARAM_DELTA elements receiving the error values
 * @param[in] z Array of PARAM_DELTA + 1 elements storing the polynomial z(x)
 * @param[in] z_degree Integer that is the degree of polynomial z(x)
 * @param[in] error_compact Array of PARAM_DELTA + PARAM_N1 storing compact representation of the error
 */
static void compute_error_values(uint16_t *error_values, const uint16_t *z, const uint8_t *error) {
    uint16_t beta_j[PARAM_DELTA] = {0};
    uint16_t e_j[PARAM_DELTA] = {0};

    uint16_t delta_counter = 0;
    uint16_t delta_real_value;

    // Compute the beta_{j_i} page 31 of the documentation
    for (size_t i = 0; i < PARAM_N1; i++) {
        uint16_t found = 0;
        uint16_t valuemask = (uint16_t) (-((int32_t)error[i]) >> 31); // error[i] != 0
        for (uint16_t j = 0; j < PARAM_DELTA; j++) {
            uint16_t indexmask = ~((uint16_t) (-((int32_t) j ^ delta_counter) >> 31)); // j == delta_counter
            beta_j[j] += indexmask & valuemask & gf_exp[i];
            found += indexmask & valuemask & 1;
        }
        delta_counter += found;
    }
    delta_real_value = delta_counter;

    // Compute the e_{j_i} page 31 of the documentation
    for (size_t i = 0; i < PARAM_DELTA; ++i) {
        uint16_t tmp1 = 1;
        uint16_t tmp2 = 1;
        uint16_t inverse = PQCLEAN_HQCRMRS128_CLEAN_gf_inverse(beta_j[i]);
        uint16_t inverse_power_j = 1;

        for (size_t j = 1; j <= PARAM_DELTA; ++j) {
            inverse_power_j = PQCLEAN_HQCRMRS128_CLEAN_gf_mul(inverse_power_j, inverse);
            tmp1 ^= PQCLEAN_HQCRMRS128_CLEAN_gf_mul(inverse_power_j, z[j]);
        }
        for (size_t k = 1; k < PARAM_DELTA; ++k) {
            tmp2 = PQCLEAN_HQCRMRS128_CLEAN_gf_mul(tmp2, (1 ^ PQCLEAN_HQCRMRS128_CLEAN_gf_mul(inverse, beta_j[(i + k) % PARAM_DELTA])));
        }
        uint16_t mask = (uint16_t) (((int16_t) i - delta_real_value) >> 15); // i < delta_real_value
        e_j[i] = mask & PQCLEAN_HQCRMRS128_CLEAN_gf_mul(tmp1, PQCLEAN_HQCRMRS128_CLEAN_gf_inverse(tmp2));
    }

    // Place the delta e_{j_i} values at the right coordinates of the output vector
    delta_counter = 0;
    for (size_t i = 0; i < PARAM_N1; ++i) {
        uint16_t found = 0;
        uint16_t valuemask = (uint16_t) (-((int32_t)error[i]) >> 31); // error[i] != 0
        for (size_t j = 0; j < PARAM_DELTA; j++) {
            uint16_t indexmask = ~((uint16_t) (-((int32_t) j ^ delta_counter) >> 31)); // j == delta_counter
            error_values[i] += indexmask & valuemask & e_j[j];
            found += indexmask & valuemask & 1;
        }
        delta_counter += found;
    }
}



/**
 * @brief Correct the errors
 *
 * @param[out] cdw Array of PARAM_N1 elements receiving the corrected vector
 * @param[in] error Array of the error vector
 * @param[in] error_values Array of PARAM_DELTA elements storing the error values
 */
static void correct_errors(uint8_t *cdw, const uint16_t *error_values) {
    for (size_t i = 0; i < PARAM_N1; ++i) {
        cdw[i] ^= error_values[i];
    }
}



/**
 * @brief Decodes the received word
 *
 * This function relies on six steps:
 *    <ol>
 *    <li> The first step, is the computation of the 2*PARAM_DELTA syndromes.
 *    <li> The second step is the computation of the error-locator polynomial sigma.
 *    <li> The third step, done by additive FFT, is finding the error-locator numbers by calculating the roots of the polynomial sigma and takings their inverses.
 *    <li> The fourth step, is the polynomial z(x).
 *    <li> The fifth step, is the computation of the error values.
 *    <li> The sixth step is the correction of the errors in the received polynomial.
 *    </ol>
 * For a more complete picture on Reed-Solomon decoding, see Shu. Lin and Daniel J. Costello in Error Control Coding: Fundamentals and Applications @cite lin1983error
 *
 * @param[out] msg Array of size VEC_K_SIZE_64 receiving the decoded message
 * @param[in] cdw Array of size VEC_N1_SIZE_64 storing the received word
 */
void PQCLEAN_HQCRMRS128_CLEAN_reed_solomon_decode(uint64_t *msg, uint64_t *cdw) {
    uint8_t cdw_bytes[PARAM_N1] = {0};
    uint16_t syndromes[2 * PARAM_DELTA] = {0};
    uint16_t sigma[1 << PARAM_FFT] = {0};
    uint8_t error[1 << PARAM_M] = {0};
    uint16_t z[PARAM_N1] = {0};
    uint16_t error_values[PARAM_N1] = {0};

    // Copy the vector in an array of bytes
    memcpy(cdw_bytes, cdw, PARAM_N1);

    // Calculate the 2*PARAM_DELTA syndromes
    compute_syndromes(syndromes, cdw_bytes);

    // Compute the error locator polynomial sigma
    // Sigma's degree is at most PARAM_DELTA but the FFT requires the extra room
    size_t deg = compute_elp(sigma, syndromes);

    // Compute the error polynomial error
    compute_roots(error, sigma);

    // Compute the polynomial z(x)
    compute_z_poly(z, sigma, deg, syndromes);

    // Compute the error values
    compute_error_values(error_values, z, error);

    // Correct the errors
    correct_errors(cdw_bytes, error_values);

    // Retrieve the message from the decoded codeword
    memcpy(msg, cdw_bytes + (PARAM_G - 1), PARAM_K);

}
