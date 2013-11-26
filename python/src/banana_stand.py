import sys, time
from serial import Serial
from jenkinsapi.jenkins import Jenkins

RED = 'FF0000'
GREEN = 'FFFF00'
YELLOW = '00FF00'
BLACK = '000000'
GRAY = '535353'

STATUS_COLORS = {
    'FAILURE': RED,
    'UNSTABLE': YELLOW,
    'SUCCESS': GREEN,
    'ABORTED': GRAY
}

def monitorForever(url, jobName):
    print 'watching {0} on {1}'.format(jobName, url)
    serial = Serial('/dev/tty.usbserial-A6008lTP', 9600)
    time.sleep(5) # time for arduino to reset
    jenkins = Jenkins(url)
    job = jenkins[jobName]
    while True:
        status = job.get_last_completed_build().get_status()
        print jobName, status
        color = STATUS_COLORS[status]
        isRunning = job.is_running();
        serial.write('#{0}\n'.format(color))
        if isRunning:
            print '\tRUNNING'
            serial.write('b{0}\n'.format(BLACK))
        else:
            serial.write('b{0}\n'.format(color))
        time.sleep(30)
        
usage = "python banana_stand.py <url> <Jenkins Job Name>"
if __name__=='__main__':
    if len(sys.argv) == 1:
        print usage
    else:
        url = sys.argv[1]
        jobName = sys.argv[2]
        monitorForever(url, jobName)


