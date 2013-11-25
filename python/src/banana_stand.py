import sys, time
from serial import Serial
from jenkinsapi.jenkins import Jenkins

RED = 'FF0000'
GREEN = '00FF00'
YELLOW = 'FFFF00'
BLACK = '000000'

def monitor(jobName, url = "http://jenkins.dev.bb.schrodinger.com"):
    print 'watching', jobName, 'on', url
    while True:
        jenkins = Jenkins(url)
        job = jenkins[jobName]
        status = job.get_last_completed_build().get_status()
        if status=='FAILURE':
            color = RED
        elif status=='UNSTABLE':
            color = YELLOW
        elif status=='SUCCESS':
            color = GREEN
        
        isRunning = job.is_running();
    
        #ser = Serial('/dev/tty.usbserial-A6008lTP', 9600)
        #ser.write('#' + color)
        print '#', color
        if isRunning:
            #ser.write('b' + BLACK)
            print 'b', BLACK
        else:
            # no pulsing
            #ser.write('b' + color)
            print 'b', color
        time.sleep(5)

if __name__=='__main__':
    jobName = sys.argv[1]
    monitor(jobName)


