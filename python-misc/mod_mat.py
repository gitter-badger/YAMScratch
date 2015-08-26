with open("prev_K.txt","r") as in_f:
    with open("mod_K.txt", "w") as out_f:
        for line in in_f:
            ar = line.split()
            if len(ar) == 3:
                ar[0] = str(int(ar[0])+1)
                ar[1] = str(int(ar[1])+1)
                out_f.write("\t".join(ar)+'\n')
