import subprocess
import sys

null = open('/dev/null', 'w');

if not len(sys.argv) == 4:
	print 'Usage: python diffdominion.py <dominion directory 1> <dominion directory 2> <random seed>'
	exit()

print ''
print ' -- diffdominion.py -- '
print ''

print 'Building testdominion in both directories...'
print '$ make clean testdominion'
subprocess.call('make clean testdominion', shell=True, stdout=null, cwd=sys.argv[1]);
subprocess.call('make clean testdominion', shell=True, stdout=null, cwd=sys.argv[2]);

print ''

tdout1 = open(sys.argv[1] + '/testdominion.out', 'w+');
tdout2 = open(sys.argv[2] + '/testdominion.out', 'w+');
print 'Running testdominion in both directories', sys.argv[3]
print '$ ./testdominion',sys.argv[3]
subprocess.call('./testdominion ' + sys.argv[3], shell=True, stdout=tdout1, stderr=tdout1, cwd=sys.argv[1]);
subprocess.call('./testdominion ' + sys.argv[3], shell=True, stdout=tdout2, stderr=tdout2, cwd=sys.argv[2]);

print ''

filepath = sys.argv[1] + '/diffdominion.out'
diffdominion = open(filepath, 'w');
print 'Creating diff file @', filepath
subprocess.call('diff ' + sys.argv[1] + '/testdominion.out ' + sys.argv[2] + '/testdominion.out', stdout=diffdominion, stderr=diffdominion, shell=True);

print ''

print 'TEST PASEED' if diffdominion.tell() == 0 else 'TEST FAILED'