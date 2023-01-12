ifdef BREAKDOWN
BREAK = -DBREAKDOWN
endif

ifdef MEMORY
MEMO = -DMEM
endif

ifdef EVALUE
EVALUATE = -DEVAL
endif

ifdef SCORE
SCORES = -DSCORE
endif

ifdef SPARSE_BREAK
SP_BREAK = -DSPARSE_BREAK
endif

ifdef NLONG
INTN = -DNLONG
endif

ifdef ELONG
INTE = -DELONG
endif

ifdef CLANG
#CC = clang++
CC = /tmp/llvm/bin/clang++
else
CC = g++
endif

# -Wall

CPPFLAGS = -std=c++17  -Wextra -Werror \
$(INTN) $(INTE) $(BREAK) $(SP_BREAK) $(MEMO) $(EVALUATE) $(SCORES) \
-I/home/csgrads/lwang323/include/ -I/home/csgrads/lwang323/PAM/include/

ifdef CILKPLUS
CPPFLAGS += -DPARLAY_CILKPLUS -DCILK -fcilkplus
else ifdef OPENCILK
CPPFLAGS += -DPARLAY_OPENCILK -DCILK -fopencilk
else ifdef SERIAL
CPPFLAGS += -DPARLAY_SEQUENTIAL
else
CPPFLAGS += -pthread
endif

ifdef DEBUG
CPPFLAGS += -DDEBUG -O0 -g
else
CPPFLAGS += -O3 -mcx16 -march=native
endif

ifdef STDALLOC
CPPFLAGS += -DPARLAY_USE_STD_ALLOC
endif

COMMON = graph.hpp parseCommandLine.hpp utilities.h get_time.hpp

all: graph IM general_cascade test_PAM

	
graph: graph.cpp $(COMMON)
	$(CC) $(CPPFLAGS) graph.cpp -o graph

IM: IM.cpp IM.hpp union_find.hpp IM_compact.hpp PMC.hpp PrunedEstimater.hpp $(COMMON)
	$(CC) $(CPPFLAGS) IM.cpp -o IM
test_PAM: test_PAM.cpp
	$(CC) $(CPPFLAGS) test_PAM.cpp -o test_PAM

# connect: connect.h connect.cpp ldd.hpp $(COMMON)
# 	$(CC) $(CPPFLAGS) connect.cpp -o connect

# bfs: bfs.cpp bfs.hpp $(COMMON)
# 	$(CC) $(CPPFLAGS) bfs.cpp -o bfs

general_cascade: general_cascade.cpp BFS_cascade.hpp bfs.hpp $(COMMON)
	$(CC) $(CPPFLAGS) general_cascade.cpp -o general_cascade

# scc: scc.hpp scc.cpp resizable_table.h hash_bag.h $(COMMON) reach.hpp 
# 	$(CC) $(CPPFLAGS) scc.cpp -o scc
# tarjan_scc: tarjan_scc.hpp tarjan_scc.cpp $(COMMON)
# 	$(CC) $(CPPFLAGS) tarjan_scc.cpp -o tarjan_scc
# Tarjan_scc: Tarjan_scc.hpp Tarjan_scc.cpp $(COMMON)
# 	$(CC) $(CPPFLAGS) Tarjan_scc.cpp -o Tarjan_scc
# reach: reach.cpp reach.hpp hash_bag.h $(COMMON)
# 	$(CC) $(CPPFLAGS) reach.cpp -o reach

# union_find: union_find.cpp union_find.hpp $(COMMON)
# 	$(CC) $(CPPFLAGS) union_find.cpp -o union_find

clean:
	rm -f graph IM general_cascade