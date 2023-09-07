#include <avr/pgmspace.h>
#include <stdint.h>

const int8_t WT_SINE[256] PROGMEM = {
  0.0, // 0
  3.0, // 1
  6.0, // 2
  9.0, // 3
  12.0, // 4
  16.0, // 5
  19.0, // 6
  22.0, // 7
  25.0, // 8
  28.0, // 9
  31.0, // 10
  34.0, // 11
  37.0, // 12
  40.0, // 13
  43.0, // 14
  46.0, // 15
  49.0, // 16
  51.0, // 17
  54.0, // 18
  57.0, // 19
  60.0, // 20
  63.0, // 21
  65.0, // 22
  68.0, // 23
  71.0, // 24
  73.0, // 25
  76.0, // 26
  78.0, // 27
  81.0, // 28
  83.0, // 29
  85.0, // 30
  88.0, // 31
  90.0, // 32
  92.0, // 33
  94.0, // 34
  96.0, // 35
  98.0, // 36
  100.0, // 37
  102.0, // 38
  104.0, // 39
  106.0, // 40
  107.0, // 41
  109.0, // 42
  111.0, // 43
  112.0, // 44
  113.0, // 45
  115.0, // 46
  116.0, // 47
  117.0, // 48
  118.0, // 49
  120.0, // 50
  121.0, // 51
  122.0, // 52
  122.0, // 53
  123.0, // 54
  124.0, // 55
  125.0, // 56
  125.0, // 57
  126.0, // 58
  126.0, // 59
  126.0, // 60
  127.0, // 61
  127.0, // 62
  127.0, // 63
  127.0, // 64
  127.0, // 65
  127.0, // 66
  127.0, // 67
  126.0, // 68
  126.0, // 69
  126.0, // 70
  125.0, // 71
  125.0, // 72
  124.0, // 73
  123.0, // 74
  122.0, // 75
  122.0, // 76
  121.0, // 77
  120.0, // 78
  118.0, // 79
  117.0, // 80
  116.0, // 81
  115.0, // 82
  113.0, // 83
  112.0, // 84
  111.0, // 85
  109.0, // 86
  107.0, // 87
  106.0, // 88
  104.0, // 89
  102.0, // 90
  100.0, // 91
  98.0, // 92
  96.0, // 93
  94.0, // 94
  92.0, // 95
  90.0, // 96
  88.0, // 97
  85.0, // 98
  83.0, // 99
  81.0, // 100
  78.0, // 101
  76.0, // 102
  73.0, // 103
  71.0, // 104
  68.0, // 105
  65.0, // 106
  63.0, // 107
  60.0, // 108
  57.0, // 109
  54.0, // 110
  51.0, // 111
  49.0, // 112
  46.0, // 113
  43.0, // 114
  40.0, // 115
  37.0, // 116
  34.0, // 117
  31.0, // 118
  28.0, // 119
  25.0, // 120
  22.0, // 121
  19.0, // 122
  16.0, // 123
  12.0, // 124
  9.0, // 125
  6.0, // 126
  3.0, // 127
  0.0, // 128
  -3.0, // 129
  -6.0, // 130
  -9.0, // 131
  -12.0, // 132
  -16.0, // 133
  -19.0, // 134
  -22.0, // 135
  -25.0, // 136
  -28.0, // 137
  -31.0, // 138
  -34.0, // 139
  -37.0, // 140
  -40.0, // 141
  -43.0, // 142
  -46.0, // 143
  -49.0, // 144
  -51.0, // 145
  -54.0, // 146
  -57.0, // 147
  -60.0, // 148
  -63.0, // 149
  -65.0, // 150
  -68.0, // 151
  -71.0, // 152
  -73.0, // 153
  -76.0, // 154
  -78.0, // 155
  -81.0, // 156
  -83.0, // 157
  -85.0, // 158
  -88.0, // 159
  -90.0, // 160
  -92.0, // 161
  -94.0, // 162
  -96.0, // 163
  -98.0, // 164
  -100.0, // 165
  -102.0, // 166
  -104.0, // 167
  -106.0, // 168
  -107.0, // 169
  -109.0, // 170
  -111.0, // 171
  -112.0, // 172
  -113.0, // 173
  -115.0, // 174
  -116.0, // 175
  -117.0, // 176
  -118.0, // 177
  -120.0, // 178
  -121.0, // 179
  -122.0, // 180
  -122.0, // 181
  -123.0, // 182
  -124.0, // 183
  -125.0, // 184
  -125.0, // 185
  -126.0, // 186
  -126.0, // 187
  -126.0, // 188
  -127.0, // 189
  -127.0, // 190
  -127.0, // 191
  -127.0, // 192
  -127.0, // 193
  -127.0, // 194
  -127.0, // 195
  -126.0, // 196
  -126.0, // 197
  -126.0, // 198
  -125.0, // 199
  -125.0, // 200
  -124.0, // 201
  -123.0, // 202
  -122.0, // 203
  -122.0, // 204
  -121.0, // 205
  -120.0, // 206
  -118.0, // 207
  -117.0, // 208
  -116.0, // 209
  -115.0, // 210
  -113.0, // 211
  -112.0, // 212
  -111.0, // 213
  -109.0, // 214
  -107.0, // 215
  -106.0, // 216
  -104.0, // 217
  -102.0, // 218
  -100.0, // 219
  -98.0, // 220
  -96.0, // 221
  -94.0, // 222
  -92.0, // 223
  -90.0, // 224
  -88.0, // 225
  -85.0, // 226
  -83.0, // 227
  -81.0, // 228
  -78.0, // 229
  -76.0, // 230
  -73.0, // 231
  -71.0, // 232
  -68.0, // 233
  -65.0, // 234
  -63.0, // 235
  -60.0, // 236
  -57.0, // 237
  -54.0, // 238
  -51.0, // 239
  -49.0, // 240
  -46.0, // 241
  -43.0, // 242
  -40.0, // 243
  -37.0, // 244
  -34.0, // 245
  -31.0, // 246
  -28.0, // 247
  -25.0, // 248
  -22.0, // 249
  -19.0, // 250
  -16.0, // 251
  -12.0, // 252
  -9.0, // 253
  -6.0, // 254
  -3.0, // 255
};


const int8_t WT_SAW[256] PROGMEM = {
  -128, // 0
  -127, // 1
  -126, // 2
  -125, // 3
  -124, // 4
  -123, // 5
  -122, // 6
  -121, // 7
  -120, // 8
  -119, // 9
  -118, // 10
  -117, // 11
  -116, // 12
  -115, // 13
  -114, // 14
  -113, // 15
  -112, // 16
  -111, // 17
  -110, // 18
  -109, // 19
  -108, // 20
  -107, // 21
  -106, // 22
  -105, // 23
  -104, // 24
  -103, // 25
  -102, // 26
  -101, // 27
  -100, // 28
  -99, // 29
  -98, // 30
  -97, // 31
  -96, // 32
  -95, // 33
  -94, // 34
  -93, // 35
  -92, // 36
  -91, // 37
  -90, // 38
  -89, // 39
  -88, // 40
  -87, // 41
  -86, // 42
  -85, // 43
  -84, // 44
  -83, // 45
  -82, // 46
  -81, // 47
  -80, // 48
  -79, // 49
  -78, // 50
  -77, // 51
  -76, // 52
  -75, // 53
  -74, // 54
  -73, // 55
  -72, // 56
  -71, // 57
  -70, // 58
  -69, // 59
  -68, // 60
  -67, // 61
  -66, // 62
  -65, // 63
  -64, // 64
  -63, // 65
  -62, // 66
  -61, // 67
  -60, // 68
  -59, // 69
  -58, // 70
  -57, // 71
  -56, // 72
  -55, // 73
  -54, // 74
  -53, // 75
  -52, // 76
  -51, // 77
  -50, // 78
  -49, // 79
  -48, // 80
  -47, // 81
  -46, // 82
  -45, // 83
  -44, // 84
  -43, // 85
  -42, // 86
  -41, // 87
  -40, // 88
  -39, // 89
  -38, // 90
  -37, // 91
  -36, // 92
  -35, // 93
  -34, // 94
  -33, // 95
  -32, // 96
  -31, // 97
  -30, // 98
  -29, // 99
  -28, // 100
  -27, // 101
  -26, // 102
  -25, // 103
  -24, // 104
  -23, // 105
  -22, // 106
  -21, // 107
  -20, // 108
  -19, // 109
  -18, // 110
  -17, // 111
  -16, // 112
  -15, // 113
  -14, // 114
  -13, // 115
  -12, // 116
  -11, // 117
  -10, // 118
  -9, // 119
  -8, // 120
  -7, // 121
  -6, // 122
  -5, // 123
  -4, // 124
  -3, // 125
  -2, // 126
  -1, // 127
  0, // 128
  1, // 129
  2, // 130
  3, // 131
  4, // 132
  5, // 133
  6, // 134
  7, // 135
  8, // 136
  9, // 137
  10, // 138
  11, // 139
  12, // 140
  13, // 141
  14, // 142
  15, // 143
  16, // 144
  17, // 145
  18, // 146
  19, // 147
  20, // 148
  21, // 149
  22, // 150
  23, // 151
  24, // 152
  25, // 153
  26, // 154
  27, // 155
  28, // 156
  29, // 157
  30, // 158
  31, // 159
  32, // 160
  33, // 161
  34, // 162
  35, // 163
  36, // 164
  37, // 165
  38, // 166
  39, // 167
  40, // 168
  41, // 169
  42, // 170
  43, // 171
  44, // 172
  45, // 173
  46, // 174
  47, // 175
  48, // 176
  49, // 177
  50, // 178
  51, // 179
  52, // 180
  53, // 181
  54, // 182
  55, // 183
  56, // 184
  57, // 185
  58, // 186
  59, // 187
  60, // 188
  61, // 189
  62, // 190
  63, // 191
  64, // 192
  65, // 193
  66, // 194
  67, // 195
  68, // 196
  69, // 197
  70, // 198
  71, // 199
  72, // 200
  73, // 201
  74, // 202
  75, // 203
  76, // 204
  77, // 205
  78, // 206
  79, // 207
  80, // 208
  81, // 209
  82, // 210
  83, // 211
  84, // 212
  85, // 213
  86, // 214
  87, // 215
  88, // 216
  89, // 217
  90, // 218
  91, // 219
  92, // 220
  93, // 221
  94, // 222
  95, // 223
  96, // 224
  97, // 225
  98, // 226
  99, // 227
  100, // 228
  101, // 229
  102, // 230
  103, // 231
  104, // 232
  105, // 233
  106, // 234
  107, // 235
  108, // 236
  109, // 237
  110, // 238
  111, // 239
  112, // 240
  113, // 241
  114, // 242
  115, // 243
  116, // 244
  117, // 245
  118, // 246
  119, // 247
  120, // 248
  121, // 249
  122, // 250
  123, // 251
  124, // 252
  125, // 253
  126, // 254
  127, // 255
};

const int8_t WT_SQUARE[256] PROGMEM = {
  -128, // 0
  -128, // 1
  -128, // 2
  -128, // 3
  -128, // 4
  -128, // 5
  -128, // 6
  -128, // 7
  -128, // 8
  -128, // 9
  -128, // 10
  -128, // 11
  -128, // 12
  -128, // 13
  -128, // 14
  -128, // 15
  -128, // 16
  -128, // 17
  -128, // 18
  -128, // 19
  -128, // 20
  -128, // 21
  -128, // 22
  -128, // 23
  -128, // 24
  -128, // 25
  -128, // 26
  -128, // 27
  -128, // 28
  -128, // 29
  -128, // 30
  -128, // 31
  -128, // 32
  -128, // 33
  -128, // 34
  -128, // 35
  -128, // 36
  -128, // 37
  -128, // 38
  -128, // 39
  -128, // 40
  -128, // 41
  -128, // 42
  -128, // 43
  -128, // 44
  -128, // 45
  -128, // 46
  -128, // 47
  -128, // 48
  -128, // 49
  -128, // 50
  -128, // 51
  -128, // 52
  -128, // 53
  -128, // 54
  -128, // 55
  -128, // 56
  -128, // 57
  -128, // 58
  -128, // 59
  -128, // 60
  -128, // 61
  -128, // 62
  -128, // 63
  -128, // 64
  -128, // 65
  -128, // 66
  -128, // 67
  -128, // 68
  -128, // 69
  -128, // 70
  -128, // 71
  -128, // 72
  -128, // 73
  -128, // 74
  -128, // 75
  -128, // 76
  -128, // 77
  -128, // 78
  -128, // 79
  -128, // 80
  -128, // 81
  -128, // 82
  -128, // 83
  -128, // 84
  -128, // 85
  -128, // 86
  -128, // 87
  -128, // 88
  -128, // 89
  -128, // 90
  -128, // 91
  -128, // 92
  -128, // 93
  -128, // 94
  -128, // 95
  -128, // 96
  -128, // 97
  -128, // 98
  -128, // 99
  -128, // 100
  -128, // 101
  -128, // 102
  -128, // 103
  -128, // 104
  -128, // 105
  -128, // 106
  -128, // 107
  -128, // 108
  -128, // 109
  -128, // 110
  -128, // 111
  -128, // 112
  -128, // 113
  -128, // 114
  -128, // 115
  -128, // 116
  -128, // 117
  -128, // 118
  -128, // 119
  -128, // 120
  -128, // 121
  -128, // 122
  -128, // 123
  -128, // 124
  -128, // 125
  -128, // 126
  -128, // 127
  127, // 128
  127, // 129
  127, // 130
  127, // 131
  127, // 132
  127, // 133
  127, // 134
  127, // 135
  127, // 136
  127, // 137
  127, // 138
  127, // 139
  127, // 140
  127, // 141
  127, // 142
  127, // 143
  127, // 144
  127, // 145
  127, // 146
  127, // 147
  127, // 148
  127, // 149
  127, // 150
  127, // 151
  127, // 152
  127, // 153
  127, // 154
  127, // 155
  127, // 156
  127, // 157
  127, // 158
  127, // 159
  127, // 160
  127, // 161
  127, // 162
  127, // 163
  127, // 164
  127, // 165
  127, // 166
  127, // 167
  127, // 168
  127, // 169
  127, // 170
  127, // 171
  127, // 172
  127, // 173
  127, // 174
  127, // 175
  127, // 176
  127, // 177
  127, // 178
  127, // 179
  127, // 180
  127, // 181
  127, // 182
  127, // 183
  127, // 184
  127, // 185
  127, // 186
  127, // 187
  127, // 188
  127, // 189
  127, // 190
  127, // 191
  127, // 192
  127, // 193
  127, // 194
  127, // 195
  127, // 196
  127, // 197
  127, // 198
  127, // 199
  127, // 200
  127, // 201
  127, // 202
  127, // 203
  127, // 204
  127, // 205
  127, // 206
  127, // 207
  127, // 208
  127, // 209
  127, // 210
  127, // 211
  127, // 212
  127, // 213
  127, // 214
  127, // 215
  127, // 216
  127, // 217
  127, // 218
  127, // 219
  127, // 220
  127, // 221
  127, // 222
  127, // 223
  127, // 224
  127, // 225
  127, // 226
  127, // 227
  127, // 228
  127, // 229
  127, // 230
  127, // 231
  127, // 232
  127, // 233
  127, // 234
  127, // 235
  127, // 236
  127, // 237
  127, // 238
  127, // 239
  127, // 240
  127, // 241
  127, // 242
  127, // 243
  127, // 244
  127, // 245
  127, // 246
  127, // 247
  127, // 248
  127, // 249
  127, // 250
  127, // 251
  127, // 252
  127, // 253
  127, // 254
  127, // 255
};

const int8_t WT_RANDOM[256] PROGMEM = {
  -87.0, // 0
  9.0, // 1
  -73.0, // 2
  -126.0, // 3
  16.0, // 4
  13.0, // 5
  -63.0, // 6
  30.0, // 7
  -95.0, // 8
  23.0, // 9
  -52.0, // 10
  16.0, // 11
  -105.0, // 12
  74.0, // 13
  82.0, // 14
  73.0, // 15
  -46.0, // 16
  -5.0, // 17
  78.0, // 18
  -46.0, // 19
  -127.0, // 20
  123.0, // 21
  -12.0, // 22
  103.0, // 23
  -96.0, // 24
  -127.0, // 25
  -126.0, // 26
  127.0, // 27
  -42.0, // 28
  26.0, // 29
  63.0, // 30
  44.0, // 31
  99.0, // 32
  94.0, // 33
  -1.0, // 34
  91.0, // 35
  -109.0, // 36
  -90.0, // 37
  75.0, // 38
  111.0, // 39
  -116.0, // 40
  -27.0, // 41
  -24.0, // 42
  -27.0, // 43
  105.0, // 44
  57.0, // 45
  -64.0, // 46
  -83.0, // 47
  -10.0, // 48
  -37.0, // 49
  -38.0, // 50
  33.0, // 51
  -123.0, // 52
  -105.0, // 53
  3.0, // 54
  79.0, // 55
  -111.0, // 56
  -38.0, // 57
  -54.0, // 58
  -5.0, // 59
  81.0, // 60
  10.0, // 61
  111.0, // 62
  -116.0, // 63
  109.0, // 64
  114.0, // 65
  1.0, // 66
  -27.0, // 67
  78.0, // 68
  -110.0, // 69
  72.0, // 70
  68.0, // 71
  14.0, // 72
  -41.0, // 73
  -98.0, // 74
  11.0, // 75
  -106.0, // 76
  -39.0, // 77
  96.0, // 78
  71.0, // 79
  58.0, // 80
  53.0, // 81
  -53.0, // 82
  18.0, // 83
  59.0, // 84
  -68.0, // 85
  21.0, // 86
  -83.0, // 87
  38.0, // 88
  -101.0, // 89
  -98.0, // 90
  69.0, // 91
  83.0, // 92
  26.0, // 93
  -32.0, // 94
  -103.0, // 95
  2.0, // 96
  -99.0, // 97
  63.0, // 98
  79.0, // 99
  93.0, // 100
  103.0, // 101
  -85.0, // 102
  -72.0, // 103
  -91.0, // 104
  79.0, // 105
  -124.0, // 106
  7.0, // 107
  77.0, // 108
  -1.0, // 109
  73.0, // 110
  103.0, // 111
  -44.0, // 112
  20.0, // 113
  -122.0, // 114
  -22.0, // 115
  92.0, // 116
  -116.0, // 117
  16.0, // 118
  -59.0, // 119
  -102.0, // 120
  -114.0, // 121
  -126.0, // 122
  -34.0, // 123
  -28.0, // 124
  121.0, // 125
  -77.0, // 126
  106.0, // 127
  89.0, // 128
  65.0, // 129
  -105.0, // 130
  -80.0, // 131
  -26.0, // 132
  -114.0, // 133
  -56.0, // 134
  -99.0, // 135
  -83.0, // 136
  125.0, // 137
  -96.0, // 138
  117.0, // 139
  -98.0, // 140
  75.0, // 141
  -52.0, // 142
  -31.0, // 143
  26.0, // 144
  -111.0, // 145
  84.0, // 146
  -91.0, // 147
  -126.0, // 148
  -25.0, // 149
  114.0, // 150
  -3.0, // 151
  -41.0, // 152
  69.0, // 153
  -127.0, // 154
  -69.0, // 155
  83.0, // 156
  14.0, // 157
  -102.0, // 158
  63.0, // 159
  50.0, // 160
  -106.0, // 161
  4.0, // 162
  120.0, // 163
  55.0, // 164
  -29.0, // 165
  33.0, // 166
  -73.0, // 167
  111.0, // 168
  -81.0, // 169
  103.0, // 170
  -60.0, // 171
  96.0, // 172
  -5.0, // 173
  -96.0, // 174
  113.0, // 175
  86.0, // 176
  127.0, // 177
  -50.0, // 178
  -29.0, // 179
  81.0, // 180
  -110.0, // 181
  -3.0, // 182
  106.0, // 183
  -35.0, // 184
  -81.0, // 185
  -92.0, // 186
  75.0, // 187
  56.0, // 188
  12.0, // 189
  97.0, // 190
  -69.0, // 191
  -100.0, // 192
  74.0, // 193
  60.0, // 194
  106.0, // 195
  69.0, // 196
  19.0, // 197
  101.0, // 198
  -86.0, // 199
  -23.0, // 200
  59.0, // 201
  41.0, // 202
  -12.0, // 203
  40.0, // 204
  -6.0, // 205
  -109.0, // 206
  65.0, // 207
  -15.0, // 208
  -9.0, // 209
  53.0, // 210
  74.0, // 211
  9.0, // 212
  66.0, // 213
  11.0, // 214
  -72.0, // 215
  86.0, // 216
  37.0, // 217
  -64.0, // 218
  12.0, // 219
  8.0, // 220
  -124.0, // 221
  28.0, // 222
  -57.0, // 223
  19.0, // 224
  20.0, // 225
  54.0, // 226
  81.0, // 227
  -89.0, // 228
  6.0, // 229
  44.0, // 230
  -101.0, // 231
  -26.0, // 232
  -103.0, // 233
  48.0, // 234
  2.0, // 235
  40.0, // 236
  34.0, // 237
  -99.0, // 238
  -45.0, // 239
  35.0, // 240
  -58.0, // 241
  28.0, // 242
  126.0, // 243
  43.0, // 244
  32.0, // 245
  -5.0, // 246
  26.0, // 247
  -22.0, // 248
  -92.0, // 249
  116.0, // 250
  100.0, // 251
  86.0, // 252
  -28.0, // 253
  -113.0, // 254
  -90.0, // 255
};