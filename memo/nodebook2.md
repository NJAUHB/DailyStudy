在十六进制中，**2位（两个十六进制字符）代表 1 个字节（byte）**。

---

### 🔢 原因解释：

* **1 个十六进制位**（如 `A` 或 `F`）表示 4 个二进制位（bits），也叫 **4 位二进制数**。
* **1 个字节 = 8 位二进制数（8 bits）**
* 所以：
  **8 bits ÷ 4 bits/hex digit = 2 hex digits**

---

### ✅ 示例：

| 十六进制   | 二进制                 | 字节数  |
| ------ | ------------------- | ---- |
| `0F`   | `00001111`          | 1 字节 |
| `FF`   | `11111111`          | 1 字节 |
| `0F1A` | `00001111 00011010` | 2 字节 |

---

### 总结：

> **2 个十六进制字符 = 1 字节**（8 位） ✅

