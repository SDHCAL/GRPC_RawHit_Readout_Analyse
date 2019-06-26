
# returns [ [list of input files name], output file base name ]
def generateFileList():
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-r", "--runs",help="If set the list is a list of run instead of a list of files",action="store_true")
    parser.add_argument("-d","--directory",type=str,help="Specify the directory where the files are",default="")
    parser.add_argument("frList",nargs='*',help="List of input files or runs to process")
    args = parser.parse_args()
    directory=args.directory
    if len(directory) != 0 and directory[-1] != '/':
        directory=directory+"/"
    inputFiles=[]
    outputFileBase=''
    if args.runs:
        #run list
        import glob
        outputFileBase="run_"
        for run in args.frList:
            inputFiles.extend( glob.glob(directory+'/*_'+run+'_*.slcio') )
            outputFileBase=outputFileBase+run+"_"
    else:
       #file List
        import os
        for f in args.frList:
            inputFiles.append(directory+f)
            outputFileBase=outputFileBase+os.path.splitext(os.path.basename(f))[0]+'_'

    outputFileBase=outputFileBase+"TriventSplit"    
    return [args.runs,directory,args.frList,inputFiles,outputFileBase]

    
if __name__ == '__main__':
    a=generateFileList()
    print a
    
