import os
import subprocess
import sys
import random
import re

null = open('/dev/null', 'w');

def main():
	random.seed(int(sys.argv[2]) if len(sys.argv) > 2 else 1);

	build_test_dominion()
	make_dirs()
	run_tests(int(sys.argv[1]) if len(sys.argv) > 1 else 10)
	create_report()

def build_test_dominion():
	print('Building testdominion')
	if not subprocess.call('make clean testdominion', shell=True, stdout=null, stderr=null) == 0:
		print 'Error building testdominion!'
		return
	print(' >> Done.')

def make_dirs():
	for dir in ['tests-failed', 'tests-passed']:
		# create if necessary
		if not os.path.exists(dir):
			os.makedirs(dir)
		# clear it out
		for file in os.listdir(dir):
			path = os.path.join(dir, file)
			if os.path.isfile(path):
				os.unlink(path)

def run_tests(count):
	for i in range(count):
		seed = random.randint(0, 100000)
		ret_val = run_test_dominion(seed)
		gen_gcov_file()
		if ret_val == 0:
			print ' >> Result: pass.'
			move_gcov_file('tests-passed', seed, i)
		else:
			print ' >> Result: fail!'
			move_gcov_file('tests-failed', seed, i)

def run_test_dominion(seed):
	print('Running testdominion with seed ' + str(seed))
	i = subprocess.call('ulimit -t 3; ./testdominion ' + str(seed), shell=True, stdout=null, stderr=null)
	print(' >> Done.')
	return i;

def gen_gcov_file():
	subprocess.call('gcov dominion.c', shell=True, stdout=null, stderr=null)

def move_gcov_file(dir, seed, index):
	os.rename('dominion.c.gcov', './' + dir + '/' + str(index) + '-' + str(seed) + 'dominion.c.gcov')
	if os.path.isfile('dominion.gcda'):
		os.remove('dominion.gcda')

def create_report():
	print 'Creating report (tarantula-report.txt)'

	file = open('tarantula-report.txt', 'w+')
	file.write('====================================================================\n')
	file.write('========================= TARANTULA REPORT =========================\n')
	file.write('====================================================================\n')
	file.write('============First column represents a graph of suspicion============\n')
	file.write('=======(0-10 # for percent suspicion, dashes for not executed)======\n')
	file.write('====================================================================\n')

	fail_count = len(os.listdir('tests-failed'))
	pass_count = len(os.listdir('tests-passed'))

	print ' >> Total failed:', fail_count, '/ Total passed:', pass_count

	if fail_count == 0 or pass_count == 0:
		print '\nFATAL ERROR! Cannot continue because either all tests passed or failed.'
		print 'In either case, Tarantula cannot be applied.\n'
		return

	print ' >> Parsing failed test results'
	fail_dict = build_gcov_dict('tests-failed')

	print ' >> Parsing passed test results'
	pass_dict = build_gcov_dict('tests-passed')

	for line in sorted(pass_dict.keys()):
		exec_count = fail_dict[line] + pass_dict[line]

		if exec_count == 0:
			file.write(('%-10s ln ' + str(line) + ' never executed\n') % ('- - - - - '))
		else:
			fail_percent = float(fail_dict[line]) / float(fail_count)
			pass_percent = float(pass_dict[line]) / float(pass_count)
			suspicion = fail_percent / (pass_percent + fail_percent)
			suspicion_string = ''
			for i in range(0, int(suspicion * 100), 10):
				suspicion_string += '#'
			file.write(('%-10s ln ' + str(line) + ' executed when test failed ' + str(fail_dict[line]) + '/' + str(exec_count) + '. suspicion ' + '%4.4f.\n') % (suspicion_string, suspicion))

def build_gcov_dict(dir):
	total_dict = {}
	for gcov_file in os.listdir(dir):
		if 'gcov' not in gcov_file:
			continue
		dict = find_executions(os.path.join(dir, gcov_file))
		for key in dict.keys():
			existing = total_dict.get(key)
			if not existing == None:
				total_dict[key] = existing + dict[key]
			else:
				total_dict[key] = dict[key]

	return total_dict

def find_executions(file):
	regex = re.compile('\s*(.*?):\s*(.*?):\s*(.*?)\s*')

	executions_dict = {};
	with open(file) as f:
		lines = f.readlines()
		for line in lines:
			matcher = regex.match(line)
			if matcher == None:
				print 'l ', line
			executions = matcher.group(1)
			line_num = int(matcher.group(2))

			if executions == '-':
				continue
			elif executions == '#####':
				executions_dict[line_num] = 0
			else:
				executions_dict[line_num] = 1

	return executions_dict

if __name__ == '__main__':
	main()