import subprocess
import sys

null = open('/dev/null', 'w');

if not len(sys.argv) == 4:
	print 'Usage: python diffdominion.py <dominion directory 1> <dominion directory 2> <random seed>'
	exit()

print ''
print ' -- diffdominion.py -- '
print ''

print 'Copying testdominion.c from directory 1 to directory 2...'
print '$ mv dir2/testdominion.c dir2/testdominion.c.orig'
print '$ cp dir1/testdominion.c dir2/testdominion.c'
subprocess.call('mv testdominion.c testdominion.c.orig', shell=True, stdout=null, cwd=sys.argv[2]);
subprocess.call('cp ' + sys.argv[1] + '/testdominion.c ' + sys.argv[2] + '/testdominion.c', shell=True, stdout=null);

print ''

print 'Building testdominion in both directories...'
print '$ make clean testdominion'
subprocess.call('make clean testdominion', shell=True, stdout=null, stderr=null, cwd=sys.argv[1]);
subprocess.call('make clean testdominion', shell=True, stdout=null, stderr=null, cwd=sys.argv[2]);

print ''

tdout1 = open(sys.argv[1] + '/testdominion.out', 'w+');
tdout2 = open(sys.argv[2] + '/testdominion.out', 'w+');
print 'Running testdominion in both directories...', sys.argv[3]
print '$ ./testdominion',sys.argv[3]
subprocess.call('./testdominion ' + sys.argv[3] + ' | grep "test:"', shell=True, stdout=tdout1, stderr=tdout1, cwd=sys.argv[1]);
subprocess.call('./testdominion ' + sys.argv[3] + ' | grep "test:"', shell=True, stdout=tdout2, stderr=tdout2, cwd=sys.argv[2]);

print ''

print 'Moving back original testdominion.c in directory 2...'
print '$ rm -f dir2/testdominion.c'
print '$ mv dir2/testdominion.c.orig dir2/testdominion.c'
subprocess.call('rm -f testdominion.c', shell=True, stdout=null, cwd=sys.argv[2]);
subprocess.call('mv testdominion.c.orig testdominion.c', shell=True, stdout=null, cwd=sys.argv[2]);

print ''

filepath = sys.argv[1] + '/diffdominion.out'
diffdominion = open(filepath, 'w');
print 'Creating diff file @', filepath
subprocess.call('diff ' + sys.argv[1] + '/testdominion.out ' + sys.argv[2] + '/testdominion.out', stdout=diffdominion, stderr=diffdominion, shell=True);

print ''

print 'TEST PASEED' if diffdominion.tell() == 0 else 'TEST FAILED'