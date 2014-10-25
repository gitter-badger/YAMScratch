import sys

if len(sys.argv) == 2:
    mystring = sys.argv[1]
else: print " too many arguments"

rev_string = "".join(mystring[::-1])

max_len = 0
max_start_index = -1
max_end_index = -1


for i in range(0,len(mystring)):
    temp_count = 0
    temp_start_index = 0
    temp_end_index = 0
    print "#"*25
    print mystring,"\n"," "*i+rev_string
    k = 0
    for j in range(i,len(mystring)):
        #print mystring[j], rev_string[k]
        if mystring[j] == rev_string[k]:
            
            temp_count +=1
            if -1 == temp_start_index: # if there is nothing in start then add the current index
                temp_start_index = j
            else:
                temp_end_index = j # otherwise advance the index pointer
                if temp_count > max_len:
                    max_len = temp_count
                    max_start_index = temp_start_index
                    max_end_index = temp_end_index
            print temp_count,"char matches {} = {}".format(mystring[j], rev_string[k])
        else:
            temp_count = -1 #reset the pointers
            temp_end_index = -1
            temp_start_index = -1






        k+= 1 # advance to the next char
print max_len
print max_start_index
print max_end_index

print mystring[max_start_index:max_end_index+1]

