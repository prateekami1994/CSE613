# comment

clear   #clear console

# checker : Execute several solution and compare output
checker(){

    printf "TestCase : $1 / $2\n"                                   #print test case number


   printf 'Generating general graph...\n'
   g++ -std=c++11 graph_generation.cpp -o graph_generation         #generate general graph
   ./graph_generation


   printf 'Convert to a format which can be given as input...\n'
   g++ -std=c++11 graph_simplify.cpp -o graph_simplify             #convert to simple graph
   ./graph_simplify


   # cat general_graph.txt
   # cat  simple_graph.txt


    printf 'Running Prim’s Algo...\n'
    g++ -std=c++11 prim.cpp -o prim
    ./prim                                                              #execute Prim's algorithm


    printf 'Running Prim’s Algo(minPMA)...\n'
    g++ -std=c++11 prim_minPMA.cpp -o prim_minPMA  -fopenmp
    ./prim_minPMA                                                     #execute Prim's minPMA algorithm


    printf 'Running Prim’s Algo(sortPMA)...\n'
    g++ -std=c++11 prim_sortPMA.cpp -o prim_sortPMA -fopenmp
    ./prim_sortPMA                                                    #execute Prim's sortPMA algorithm

    printf 'Running Prim’s Algo(hybridPMA)...\n'
   g++ -std=c++11 prim_hybridPMA.cpp -o prim_hybridPMA -fopenmp
   ./prim_hybridPMA                                                    #execute Prim's hybridPMA algorithm


    #remove object files
    rm graph_generation                                                 #remove graph_generation executable file
    rm graph_simplify                                                   #remove graph_simplify executable file
    rm prim                                                             #remove prim executable file
    rm prim_minPMA                                                      #remove prim_minPMA executable file
    rm prim_sortPMA                                                     #remove prim_sortPMA executable file
    rm prim_hybridPMA                                                   #remove prim_hybridPMA executable file

    printf 'Getting Status...\n'

    if cmp -s ../output/prim.txt ../output/prim_minPMA.txt &&
        cmp -s ../output/prim.txt ../output/prim_sortPMA.txt &&
         cmp -s ../output/prim.txt ../output/prim_hybridPMA.txt
                                                                        # match output of different algorithm
    then
       printf "All the graphs input and output are different\n"
    else
       printf "Verdict : WA\n"
       exit
    fi

    #sleep 1
}

#sorter : sort comparision data based on number of edges
sorter (){

    printf 'Sorting comparision data...\n'
    g++ -std=c++11 sort_comparision_info.cpp -o sort_comparision_info   #sort comparision_info()
    ./sort_comparision_info

    rm sort_comparision_info                                            #remove sort_comparision_info executable file
}

#drawer : graph ploting
drawer(){

    printf "Drawing a plot to compare run times......Saving it to output/plot.png\n"
    python graph.py
}

#main : driver function
main(){

    rm ../output/time.txt  #remove() previous output

    for((c=1;c<=$1;c++))
    do
       checker $c $1
    done

    #sort comparision info
    sorter

    #draw graph
    drawer
    printf 'Done\n'
}

#driver script
if [ "$#" -ne 1 ]; then
    printf "[Error]Illegal Number of Parameters\n"
    printf "[Sol :]Pass Number of Testcases\n"
    exit
fi
main $1

