INC=/lib/modules/$(shell uname -r)/build/arch/x86/include

all: 1_A 1_B 1_D 1_G 1_F

1_A: 1_A.cpp
	g++ -Wall -g3 -I$(INC)/generated/uapi -I$(INC)/uapi 1_A.cpp -o 1_A

1_A: 1_B.cpp
	g++ -Wall -g3 -I$(INC)/generated/uapi -I$(INC)/uapi 1_B.cpp -o 1_B

1_A: 1_D.cpp
	g++ -Wall -g3 -I$(INC)/generated/uapi -I$(INC)/uapi 1_D.cpp -o 1_D

1_A: 1_G.cpp
	g++ -Wall -g3 -I$(INC)/generated/uapi -I$(INC)/uapi 1_G.cpp -o 1_G

1_A: 1_I.cpp
	g++ -Wall -g3 -I$(INC)/generated/uapi -I$(INC)/uapi 1_I.cpp -o 1_I
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f 1_A
	rm -f 1_B
	rm -f 1_D
	rm -f 1_G
	rm -f 1_I
