#TouchOut读取
*主机发送到H2E
| Offset    | Size          | Discription  |
| --------- |:-------------:| -----:|
| 0         | BYTE | 31h |
| 1         | WORD|   0001h |
| 3         | BYTE  |    00h |

*H2E回复
| Offset    | Size          | Discription  |
| --------- |:-------------:| -----:|
| 0         | BYTE | 00h |
| 1         | WORD|   0001h |
| 3         | BYTE  |    00h：None |
|          |   |    01h：TouchOut1 |
|          |  |    10h：TouchOut2 |
|          |  |    11h：Both |