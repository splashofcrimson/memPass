import csv 
import subprocess
import glob,os
flag = True
    

def generate_report(data):
    ''' Takes in data and outputs a report'''
    print("generating report....")
    errors = []
    alloc_hash = [] # [ loc1, loc2 ,... ]
    alloc_dict = {} # { loc1:size1  ...  }
    cases = ['m', 'aa', 'f', 'q']
    for d in data:
        
        if len(d) <= 1:
            continue
        type_instr = d[0]
        type_instr = type_instr[:-1]
        
        
        
        if type_instr == 'm':
            alloc_hash.append(d[1])
            alloc_dict[d[1]] = d[2]
        if type_instr == 'aa':
            
            alloc_hash.append(d[1])
            alloc_dict[ d[1]  ] = 0
        if type_instr == 'f':
            
            if d[1] in alloc_hash:
                del alloc_dict[d[1]]
            else:
                errors.append( 'Error: Double free error')
    return(set(errors))
            
        


def run_tests(root, file):
    
    data = []
    path_to_file = file + "/" + root
    outfile = file + "/" + root.split('.')[0] + '.csv'
    print(outfile)
    try:
        with open(outfile, newline='') as csvfile:
            spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
            for row in spamreader:
                data.append(row)
        report = generate_report(data)
        print(outfile, ':   ' , report, '\n')
    except:
        print('you done goofed')

    


if __name__ == "__main__":
    print('GENERATING BENCHMARK TEST REPORT: ')
    print('-------------------------')
    flag = True
    for root, dirs, files in os.walk("tests/benchmarktests/toyota/double-free/"):
        for file in files:
            if file.endswith(".csv"):
                
                if (flag):
                    result = run_tests(file, root)
                flag = True
                    