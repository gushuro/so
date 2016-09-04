export LC_ALL=C
CXXFLAGS= -Wall -pedantic -std=c++11 -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_sjf.o sched_rsjf.o sched_mfq.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
	sched_mfq.h sched_rr.h sched_rsjf.h sched_sjf.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all

ejercicio1:
	make && ./simusched lotes/ej1.tsk 1 1 1 SchedFCFS | python graphsched.py > diagramas/ej1.png

ejercicio2:
	make && ./simusched lotes/ej2.tsk 1 2 1 SchedFCFS | python graphsched.py > ej2-1core.png && \
	./simusched lotes/ej2.tsk 2 2 1 SchedFCFS | python graphsched.py > diagramas/ej2-2cores.png && \
	./simusched lotes/ej2.tsk 4 2 1 SchedFCFS | python graphsched.py > diagramas/ej2-4cores.png

ejercicio3:
	make && ./simusched lotes/ej3.tsk 1 2 1 SchedFCFS | python graphsched.py > diagramas/ej3-1core.png && \
	./simusched lotes/ej3.tsk 2 2 1 SchedFCFS | python graphsched.py > diagramas/ej3-2cores.png

ejercicio4:

ejercicio5:
	make && ./simusched lotes/ej2.tsk 1 1 1 SchedRR 2 | python graphsched.py > diagramas/ej5-quantum2.png && \
	./simusched lotes/ej2.tsk 1 1 1 SchedRR 5 | python graphsched.py > diagramas/ej5-quantum5.png && \
	./simusched lotes/ej2.tsk 1 1 1 SchedRR 10 | python graphsched.py > diagramas/ej5-quantum10.png

ejercicio7:
	make && ./simusched lotes/ej7sjf.tsk 1 1 1 SchedSJF 1 7 5 3 9 10 2 | python graphsched.py > diagramas/ej7sjf.png \
	./simusched lotes/ejrsjf.tsk 1 1 1 SchedRSJF 3 1 7 5 6 9 2 | python graphsched.py > img.png

