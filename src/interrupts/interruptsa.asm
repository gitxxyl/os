%include "include/pusha64.inc"

%macro ISR_ERROR 1
    global isr%1
    isr%1: 
        cli 
        push %1
        jmp isr_common_stub
%endmacro

%macro ISR_NOERROR 1
    global isr%1
    isr%1:
        cli 
        push 0
        push %1
        jmp isr_common_stub

%endmacro 

extern isr_handler
isr_common_stub:
    pusha64
    mov rdi, rsp 
    call isr_handler 
    mov rsp, rax
    popa64
    add rsp, 16
    iretq

ISR_NOERROR 0
ISR_NOERROR 1
ISR_NOERROR 2
ISR_NOERROR 3
ISR_NOERROR 4
ISR_NOERROR 5
ISR_NOERROR 6
ISR_NOERROR 7
ISR_ERROR 8
ISR_NOERROR 9
ISR_ERROR 10
ISR_ERROR 11
ISR_ERROR 12
ISR_ERROR 13
ISR_ERROR 14
ISR_NOERROR 15
ISR_NOERROR 16
ISR_NOERROR 17
ISR_NOERROR 18
ISR_NOERROR 19
ISR_NOERROR 20
ISR_NOERROR 21
ISR_NOERROR 22
ISR_NOERROR 23
ISR_NOERROR 24
ISR_NOERROR 25
ISR_NOERROR 26
ISR_NOERROR 27
ISR_NOERROR 28
ISR_NOERROR 29
ISR_NOERROR 30
ISR_NOERROR 31
ISR_NOERROR 32
ISR_NOERROR 33
ISR_NOERROR 34
ISR_NOERROR 35
ISR_NOERROR 36
ISR_NOERROR 37
ISR_NOERROR 38
ISR_NOERROR 39
ISR_NOERROR 40
ISR_NOERROR 41
ISR_NOERROR 42
ISR_NOERROR 43
ISR_NOERROR 44
ISR_NOERROR 45
ISR_NOERROR 46
ISR_NOERROR 47
; ISR_NOERROR 48
; ISR_NOERROR 49
; ISR_NOERROR 50
; ISR_NOERROR 51
; ISR_NOERROR 52
; ISR_NOERROR 53
; ISR_NOERROR 54
; ISR_NOERROR 55
; ISR_NOERROR 56
; ISR_NOERROR 57
; ISR_NOERROR 58
; ISR_NOERROR 59
; ISR_NOERROR 60
; ISR_NOERROR 61
; ISR_NOERROR 62
; ISR_NOERROR 63
; ISR_NOERROR 64
; ISR_NOERROR 65
; ISR_NOERROR 66
; ISR_NOERROR 67
; ISR_NOERROR 68
; ISR_NOERROR 69
; ISR_NOERROR 70
; ISR_NOERROR 71
; ISR_NOERROR 72
; ISR_NOERROR 73
; ISR_NOERROR 74
; ISR_NOERROR 75
; ISR_NOERROR 76
; ISR_NOERROR 77
; ISR_NOERROR 78
; ISR_NOERROR 79
; ISR_NOERROR 80
; ISR_NOERROR 81
; ISR_NOERROR 82
; ISR_NOERROR 83
; ISR_NOERROR 84
; ISR_NOERROR 85
; ISR_NOERROR 86
; ISR_NOERROR 87
; ISR_NOERROR 88
; ISR_NOERROR 89
; ISR_NOERROR 90
; ISR_NOERROR 91
; ISR_NOERROR 92
; ISR_NOERROR 93
; ISR_NOERROR 94
; ISR_NOERROR 95
; ISR_NOERROR 96
; ISR_NOERROR 97
; ISR_NOERROR 98
; ISR_NOERROR 99
; ISR_NOERROR 100
; ISR_NOERROR 101
; ISR_NOERROR 102
; ISR_NOERROR 103
; ISR_NOERROR 104
; ISR_NOERROR 105
; ISR_NOERROR 106
; ISR_NOERROR 107
; ISR_NOERROR 108
; ISR_NOERROR 109
; ISR_NOERROR 110
; ISR_NOERROR 111
; ISR_NOERROR 112
; ISR_NOERROR 113
; ISR_NOERROR 114
; ISR_NOERROR 115
; ISR_NOERROR 116
; ISR_NOERROR 117
; ISR_NOERROR 118
; ISR_NOERROR 119
; ISR_NOERROR 120
; ISR_NOERROR 121
; ISR_NOERROR 122
; ISR_NOERROR 123
; ISR_NOERROR 124
; ISR_NOERROR 125
; ISR_NOERROR 126
; ISR_NOERROR 127
; ISR_NOERROR 128
; ISR_NOERROR 129
; ISR_NOERROR 130
; ISR_NOERROR 131
; ISR_NOERROR 132
; ISR_NOERROR 133
; ISR_NOERROR 134
; ISR_NOERROR 135
; ISR_NOERROR 136
; ISR_NOERROR 137
; ISR_NOERROR 138
; ISR_NOERROR 139
; ISR_NOERROR 140
; ISR_NOERROR 141
; ISR_NOERROR 142
; ISR_NOERROR 143
; ISR_NOERROR 144
; ISR_NOERROR 145
; ISR_NOERROR 146
; ISR_NOERROR 147
; ISR_NOERROR 148
; ISR_NOERROR 149
; ISR_NOERROR 150
; ISR_NOERROR 151
; ISR_NOERROR 152
; ISR_NOERROR 153
; ISR_NOERROR 154
; ISR_NOERROR 155
; ISR_NOERROR 156
; ISR_NOERROR 157
; ISR_NOERROR 158
; ISR_NOERROR 159
; ISR_NOERROR 160
; ISR_NOERROR 161
; ISR_NOERROR 162
; ISR_NOERROR 163
; ISR_NOERROR 164
; ISR_NOERROR 165
; ISR_NOERROR 166
; ISR_NOERROR 167
; ISR_NOERROR 168
; ISR_NOERROR 169
; ISR_NOERROR 170
; ISR_NOERROR 171
; ISR_NOERROR 172
; ISR_NOERROR 173
; ISR_NOERROR 174
; ISR_NOERROR 175
; ISR_NOERROR 176
; ISR_NOERROR 177
; ISR_NOERROR 178
; ISR_NOERROR 179
; ISR_NOERROR 180
; ISR_NOERROR 181
; ISR_NOERROR 182
; ISR_NOERROR 183
; ISR_NOERROR 184
; ISR_NOERROR 185
; ISR_NOERROR 186
; ISR_NOERROR 187
; ISR_NOERROR 188
; ISR_NOERROR 189
; ISR_NOERROR 190
; ISR_NOERROR 191
; ISR_NOERROR 192
; ISR_NOERROR 193
; ISR_NOERROR 194
; ISR_NOERROR 195
; ISR_NOERROR 196
; ISR_NOERROR 197
; ISR_NOERROR 198
; ISR_NOERROR 199
; ISR_NOERROR 200
; ISR_NOERROR 201
; ISR_NOERROR 202
; ISR_NOERROR 203
; ISR_NOERROR 204
; ISR_NOERROR 205
; ISR_NOERROR 206
; ISR_NOERROR 207
; ISR_NOERROR 208
; ISR_NOERROR 209
; ISR_NOERROR 210
; ISR_NOERROR 211
; ISR_NOERROR 212
; ISR_NOERROR 213
; ISR_NOERROR 214
; ISR_NOERROR 215
; ISR_NOERROR 216
; ISR_NOERROR 217
; ISR_NOERROR 218
; ISR_NOERROR 219
; ISR_NOERROR 220
; ISR_NOERROR 221
; ISR_NOERROR 222
; ISR_NOERROR 223
; ISR_NOERROR 224
; ISR_NOERROR 225
; ISR_NOERROR 226
; ISR_NOERROR 227
; ISR_NOERROR 228
; ISR_NOERROR 229
; ISR_NOERROR 230
; ISR_NOERROR 231
; ISR_NOERROR 232
; ISR_NOERROR 233
; ISR_NOERROR 234
; ISR_NOERROR 235
; ISR_NOERROR 236
; ISR_NOERROR 237
; ISR_NOERROR 238
; ISR_NOERROR 239
; ISR_NOERROR 240
; ISR_NOERROR 241
; ISR_NOERROR 242
; ISR_NOERROR 243
; ISR_NOERROR 244
; ISR_NOERROR 245
; ISR_NOERROR 246
; ISR_NOERROR 247
; ISR_NOERROR 248
; ISR_NOERROR 249
; ISR_NOERROR 250
; ISR_NOERROR 251
; ISR_NOERROR 252
; ISR_NOERROR 253
; ISR_NOERROR 254
; ISR_NOERROR 255
