#include "types.h"
#include "stat.h"
#include "user.h"

#define MAX_FILE_SIZE 8192 // 최대 파일 크기를 넉넉하게 설정
#define BUF_SIZE 1024
char buf[MAX_FILE_SIZE];
char **arr;

void htac(int fd, int minLine)
{
    // n은 읽은 바이트 수
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n < 0)
    {
        printf(1, "Error reading file\n");
        exit();
    }

    buf[n] = '\0';

    int endIdx = 0;
    int idx = 0;
    int lineIdx = 0;
    char stringbuf[BUF_SIZE];
    memset(stringbuf, 0, BUF_SIZE);

    // 웬만하면 동적할당으로!
    arr = malloc(sizeof(char *) * MAX_FILE_SIZE);
    for (int i = 0; i < MAX_FILE_SIZE; i++)
    {
        arr[i] = malloc(BUF_SIZE * sizeof(char));
        memset(arr[i], 0, BUF_SIZE); // 메모리 할당 후 초기화
    }

    while (endIdx < n)
    {
        if (buf[endIdx] == '\n' || buf[endIdx] == '\0')
        {
            stringbuf[idx] = '\0';
            strcpy(arr[lineIdx], stringbuf);
            memset(stringbuf, 0, BUF_SIZE);
            idx = 0;
            lineIdx++;
        }
        else
        {
            stringbuf[idx++] = buf[endIdx]; // 다음 문자 추가
        }
        endIdx++;
    }

    int cnt = 0;
    int writeIdx = lineIdx;
    while (writeIdx >= 0 && cnt < minLine) // 출력할 줄 수가 충분하지 않을 때까지 반복
    {
        int len = strlen(arr[writeIdx]);
        if (len > 0)
        {
            write(1, arr[writeIdx], len);
            write(1, "\n", 1); // 줄 바꿈 추가
            cnt++;
        }
        writeIdx--;
    }

    for (int i = 0; i < MAX_FILE_SIZE; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        exit();
    }
    int line = atoi(argv[1]);

    for (int i = 2; i < argc; i++)
    {
        int fd = open(argv[i], 0);
        if (fd < 0)
        {
            printf(1, "Cannot open %s\n", argv[i]);
            exit();
        }
        htac(fd, line);
        close(fd);
    }
    exit();
}
