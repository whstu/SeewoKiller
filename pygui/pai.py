def pi_digits():
    """生成π的十进制数字的生成器"""
    q, r, t, k, n, l = 1, 0, 1, 1, 3, 3
    while True:
        if 4 * q + r - t < n * t:
            yield n
            nr = 10 * (r - n * t)
            n = ((10 * (3 * q + r)) // t) - 10 * n
            q *= 10
            r = nr
        else:
            nr = (2 * q + r) * l
            nn = (q * (7 * k + 2) + r * l) // (t * l)
            q *= k
            t *= l
            l += 2
            k += 1
            n = nn
            r = nr

# 实时输出π的每一位
import sys
num=input("请输入小数点后的位数：")
digits = pi_digits()

# 输出整数部分和小数点
sys.stdout.write(f"{next(digits)}.")
sys.stdout.flush()

# 输出小数点后 x 位
for _ in range(int(num)):
    sys.stdout.write(str(next(digits)))
    sys.stdout.flush()
from os import system
print("\n")
system("pause")