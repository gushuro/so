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
	make && ./simusched lotes/ej2.tsk 1 2 1 SchedFCFS | python graphsched.py > diagramas/ej2-1core.png && \
	./simusched lotes/ej2.tsk 2 2 1 SchedFCFS | python graphsched.py > diagramas/ej2-2cores.png && \
	./simusched lotes/ej2.tsk 4 2 1 SchedFCFS | python graphsched.py > diagramas/ej2-4cores.png

ejercicio3:
	make && ./simusched lotes/ej3.tsk 1 2 1 SchedFCFS | python graphsched.py > diagramas/ej3-1core.png && \
	./simusched lotes/ej3.tsk 2 2 1 SchedFCFS | python graphsched.py > diagramas/ej3-2cores.png

ejercicio4:
	make && ./simusched lotes/ej4.tsk 1 2 1 SchedRR 4 | python graphsched.py > diagramas/ej4.png

ejercicio5:
	make && ./simusched lotes/ej2.tsk 1 2 1 SchedRR 2 | python graphsched.py > diagramas/ej5-quantum2.png && \
	./simusched lotes/ej2.tsk 1 2 1 SchedRR 5 | python graphsched.py > diagramas/ej5-quantum5.png && \
	./simusched lotes/ej2.tsk 1 2 1 SchedRR 10 | python graphsched.py > diagramas/ej5-quantum10.png

ejercicio6:
	echo "\n"

ejercicio7:
	make && ./simusched lotes/ej7sjf.tsk 1 1 1 SchedSJF 10 9 1 7 5 3 2 | python graphsched.py > diagramas/ej7-sjf.png && \
	./simusched lotes/ej7rsjf.tsk 1 1 1 SchedRSJF 3 1 7 5 6 2 2 | python graphsched.py > diagramas/ej7-rsjf.png

ejercicio8:
	make && ./simusched lotes/ej8-cpulargomuchoschicos.tsk 1 1 1 SchedRR 3 | python graphsched.py > diagramas/ej8-cpulargomuchoschicosRR.png && \
	./simusched lotes/ej8-cpulargomuchoschicos.tsk 1 1 1 SchedSJF 15 2 2 3 4 2 | python graphsched.py > diagramas/ej8-cpulargomuchoschicosSJF.png && \
	./simusched lotes/ej8-cpulargomuchoschicos.tsk 1 1 1 SchedRSJF 3 15 2 2 3 4 2 | python graphsched.py > diagramas/ej8-cpulargomuchoschicosRSJF.png && \
	./simusched lotes/ej8-cpulargomuchoschicos.tsk 1 1 1 SchedFCFS | python graphsched.py > diagramas/ej8-cpulargomuchoschicosFCFS.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 1 1 1 SchedRR 3 | python graphsched.py > diagramas/ej8-10tareasduran5RR.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 1 1 1 SchedSJF 15 2 2 3 4 2 | python graphsched.py > diagramas/ej8-10tareasduran5SJF.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 1 1 1 SchedRSJF 3 15 2 2 3 4 2 | python graphsched.py > diagramas/ej8-10tareasduran5RSJF.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 1 1 1 SchedFCFS | python graphsched.py > diagramas/ej8-10tareasduran5FCFS.png
	./simusched lotes/ej8-contextocaro.tsk 1 5 1 SchedFCFS | python graphsched.py > diagramas/contextocaroFCFS.png && \
	./simusched lotes/ej8-contextocaro.tsk 1 5 1 SchedSJF 8 8 8 | python graphsched.py > diagramas/contextocaroSJF.png && \
	./simusched lotes/ej8-contextocaro.tsk 1 5 1 SchedRR 3 | python graphsched.py > diagramas/contextocaroRR.png && \
	./simusched lotes/ej8-contextocaro.tsk 1 5 1 SchedRSJF 3 8 8 8 | python graphsched.py > diagramas/contextocaroRSJF.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 2 1 5 SchedRR 3 3 3 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroRR.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 2 1 5 SchedSJF 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroSJF.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 2 1 5 SchedSJF 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroSJF.png && \
	./simusched lotes/ej8-10tareasduran5.tsk 2 1 5 SchedRSJF 3 3 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroRSJF.png && \
	./simusched lotes/ej8-bloqueantes.tsk 2 1 5 SchedRR 3 3 3 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroRR.png && \
	./simusched lotes/ej8-bloqueantes.tsk 2 1 5 SchedSJF 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroSJF.png && \
	./simusched lotes/ej8-bloqueantes.tsk 2 1 5 SchedSJF 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroSJF.png && \
	./simusched lotes/ej8-bloqueantes.tsk 2 1 5 SchedRSJF 3 3 5 5 5 5 5 | python graphsched.py > diagramas/ej8-2cores-tareasimpares-cambiocaroRSJF.png



ejercicio9:
	make && ./simusched lotes/ej9-exploit.tsk 1 1 1 SchedMFQ 3 3 | python graphsched.py > diagramas/ej9-exploit.png && \
	./simusched lotes/ej9-2.tsk 1 1 1 SchedMFQ 4 | python graphsched.py > diagramas/ej9-2a.png && \
	./simusched lotes/ej9-2.tsk 1 1 1 SchedMFQ 4 3 2 1 | python graphsched.py > diagramas/ej9-2b.png && \
	echo "hola"
