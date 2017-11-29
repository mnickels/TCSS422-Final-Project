/*
init: create initial processes, create 2 io threads and timer thread
quantum = (20*priority)+50
globals = readyqueue, currentProcess, S= 50,000
processes = 50 IO, 25 compute intensive, 10 pc pairs, 10 mutual resource
for(;;)
    run process() / pc++
    check io arrays()
    checktermination()
    generateProcesses()
    if cpu->pc%S == 0
        resetQueue()

scheduler()
dispatcher() starts timer
timer()        

/*