# linux-kernel
for raspi, linux, kernel, device driver, porting study example

- [해당 페이지 내용 참조](https://www.joinc.co.kr/w/Site/Embedded/Documents/LinuxKernelModuleProg)

#### 1. 커널 모듈이란?

> 커널 모듈이란 필요에 따라 커널에 로드하거나 언로드 할 수 있는 특정한 기능을 수행하는 코드(프로그램)이다.
>
> 해당 모듈을 통해 쉽게 커널의 기능을 확장할 수 있을 뿐만 아니라 운영체제를 리부팅 하지 않고도 원하는 기능을 수행할 수 있도록 만들 수 있다.
>
> 예를 들면, 어떤 하드웨어를 제어하기 위한 문자 장치(device drive)를 작성해야 한다고 생각해보자. 만약 모듈기능을 제공하지 않는 커널이라면 커널을 직접 수정하는 방식을 동원해서 커널에 필요한 기능을 추가시켜야 할 것이다. 프로그램 자체가 어려워지는 것은 물론이고 기능을 테스트 하기 위해서는 계속적인 리부팅 작업이 필요하게 되므로 개발기간 역시 극적으로 늘어날 수 밖에 없을 것이다. 또한 커널에 필요한 기능이 추가될 때마다 커널에 계속해서 코드가 추가 됨으로 커널의 크기도 매우 커지게 될것이다. 사운드카드를 위한 기능을 추가했는데 해당 사운드카드를 가지지 않는 유저도 있을 것이다. 이럴 경우는 그야말로 쓸데 없는 자원낭비가 되는 셈이다. ` 커널 모듈로써 작동하도록 만들었다면 쓸데없는 기능을 하는 모듈은 언로드 시키면 그만이다.

#### 2. 커널 모듈 프로그래밍의 기본

- 현재 커널에서 작동중인 모듈의 목록은 lsmod 명령을 통해서 확인할 수 있다.

> lsmod는 /proc/modules 파일의 내용을 그대로 출력한다.
>
> 
>
> 그럼 커널은 이러한 모듈을 어떻게 찾아서 적재시키는 걸까. 커널이 어떤 모듈을 포함하고자 할때 해당 모듈이 아직 커널에 적재되어 있지 않다면 모듈 데몬(daemon)인 **kmod**가 **modprobe**를 실행시켜서 모듈을 읽어들이게 된다. 이때 modprobe는 다음중 하나의 방법을 이용해서 읽어들여야할 모듈을 찾게 된다.
>
> 
>
> - softdog, ppp와 같은 모듈이름을 직접 찾는다.
> - char-major-10-30 과 같은 일반적인 식별자(generic identifier)를 이용한다.
>
> 
>
> 만약 modprobe가 식별자를 이용할 경우, 해당 식별자에 대한 진짜 모듈이름을 알아와야 할것이다. 이에 대한 정보는 /etc/modules.conf에 저장되어 있다.



#### 3. 간단한 모듈 제작 

```c

#include <linux/module.h>
#include <linux/kernel.h>

//커널에 적제될 때 사용되는 함수
int init_module(void)
{
    printk("<1>Hello World 1.\n");
    return 0;
}

//커널에 삭제될 때 사용되는 함수
void cleanup_module(void)
{
    printk(KERN_ALERT "Goodbye world 1.\n");
}
```

- 커널모듈은 최소한 2개의 함수를 가지고 있어야만 한다. 하나는 init_module()라는 이름의 시작(초기화)함수로써 insmod에 의해서 커널로 적재될때 호출된다. 다른 하나는 cleanup_module()라는 이름의 종료함수로써 rmmod를 호출해서 모듈을 삭제할때 호출된다. 

- printk는 다음과 같은 개념을 가진다.

  > 일반적으로 printk를 이용하면 (함수이름의 어감 때문에) 특정한 메시지를 표준출력할 것으로 생각하는 경우가 많은데 printk는 유저를 위한 어떤 출력도 하지 않는다. 이름과는 달리 로그나 경고 메시지를 남기기 위한 커널로깅 목적으로 사용된다.
  >
  > 일반적인 로그관련 라이브러리나 함수들이 그렇듯이 printk도 우선순위(priority)를 가진다. 모두 8단계의 우선순위를 가지며 <1> KERN_ALERT 와 같은 방식으로 결정할 수 있다. 이들 우선순위에 대한 선언정보는 linux/kernel.h에서 확인할 수 있다. 만약 우선순위를 정하기 귀찮거나 정할 수 없다면 기본 우선순위 DEFAULT_MESSAGE_LOGLEVEL을 사용하면 된다.
  >
  > 만약 **syslogd**와 **klogd**가 실행중이라면 메시지는 /var/log/messages에 추가 된다. 다음은 실제 저장된 로그들이다.
  >
  > ```
  > Oct  6 01:15:39 localhost kernel: Hello World 1. Oct  6 01:16:11 localhost kernel: Goodbye world 1. 			
  > ```

  

#### 4. 커널 모듈 컴파일

- 커널 모듈을 컴파일 하기 위해서는 특별한 gcc 옵션과 더불어 몇가지 값들의 정의가 필요하다. 커널 모듈 컴파일 시 사용되는 커널 헤더들이 커널버젼에 매우 의존적일 수 있기 때문이다.
- gcc의 -D 옵션을 이용하거나 혹은 #define 선행처리자를 이용하면 된다. 

> - `-c `: 커널 모듈은 독립적으로 실행되지 않으며 (main 함수 자체를 포함하고 있지 않다) object파일 형태로 커널에 링크되어서 실행된다. 결과적으로 -c 옵션을 이용해서 오브젝트 형태로 만들어 주어야 한다.
> - `-O2:` 커널은 inline 함수를 매우 많이 사용하며, 그런 이유로 모듈은 반드시 최적화(optimization)옵션을 사용해서 컴파일 되어야 한다. 최적화 옵션을 사용하지 않을 경우 어셈블러 매크로 등을 사용하는데 있어서 문제가 생길 수 있다. 이럴경우 모듈의 적제가 실패하게 될 것이다.
> - `-D__KERNEL__ `: 이 코드가 유저 프로세스가 아닌 커널모드에서 작동할 것이라는걸 커널헤더에 알려준다.
> - -`W -Wall`: 모듈 프로그램은 커널에 매우 민감한 영향을 끼칠 수 있으며 커널을 다운 시킬 수도 있다. 그러므로 가능한한 모든 종류의 경고메시지를 검사해야할 필요가 있다. 이 옵션을 사용하면 컴파일러가 발생시킬 수 있는 모든 경고 메시지를 출력한다.
> - `-DMODULE` : 커널모듈로 작성되는 코드라는걸 알려주기 위해서 사용한다.

- Makefile에 대한 예시는 다음과 같다.

```c
TARGET  := hello
WARN    := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
INCLUDE := -isystem /lib/modules/`uname -r`/build/include
CFLAGS  := -O2 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE}
CC      := gcc
	
${TARGET}.o: ${TARGET}.c

.PHONY: clean

clean:
    rm -rf ${TARGET}.o
```

> make를 실행하면 hello.c를 컴파일하고 그 결과 커널 모듈(오브젝트) 파일인 hello.o를 생성해낸다. 생성된 커널 모듈은 insmod ./hello.o를 통해서 적재 할 수 있다. 이걸로 최초의 커널 모듈 작성에 성공했다. 적재된 커널 모듈은 rmmod hello로 제거할 수 있다. printk()출력은 /var/log/message에 쌓일 것이다.

