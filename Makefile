all: st

st: st.c
	gcc st.c -Wall -Wextra -Werror -std=c11 -O0 -o st

clean:
	rm st
