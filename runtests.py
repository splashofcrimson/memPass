import subprocess
import glob,os

def run_tests(root, file):
    FNULL = open(os.devnull, 'w')
    full_path = file + '/' + root
    ll = root.split('.')[0] + '.ll'
    out_file = file + '/' + root.split('.')[0] 
    # --------------
    bashCommand = ['bash', 'eval_test.sh', full_path, ll, out_file]
    process = subprocess.Popen(bashCommand, stdout=FNULL)
    output, error = process.communicate()    
    print("Printing {} to {}".format(full_path, out_file))
if __name__ == "__main__":
    print('GENERATING BENCHMARK TEST REPORT: ')
    print('-------------------------')
    for root, dirs, files in os.walk("tests/benchmarktests/"):
        for file in files:
            if file.endswith(".c"):
                # if root == "tests/benchmarktests/toyota/memory-leak":
                result = run_tests(file, root)
                print(os.path.join(root, file))