# TouchOut读取
* 主机发送到H2E

| Offset    | Size          | Discription  |
| --------- |:-------------:| -----:|
| 0         | BYTE | 31h |
| 1         | WORD|   0001h |
| 3         | BYTE  |    00h |

* H2E回复

| Offset    | Size          | Discription  |
| --------- |:-------------:| -----:|
| 0         | BYTE | 00h |
| 1         | WORD|   0001h |
| 3         | BYTE  |    00h：None<br>01h：TouchOut1<br>10h：TouchOut2<br>11h：Both |