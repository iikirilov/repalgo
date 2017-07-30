/*
 * I verify that I am the sole author of this code, 
 * except where explicitly stated to the contrary.
 * This code is based on the pseudocode in the paper, 
 * An optimal algorithm for computing the repetitions in a word - M. Crochemore
 * /

/* 
 * File:   main.cpp
 * Author: ivaylo
 *
 * Created on 15 March 2017, 23:39
 */

#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <list>
#include <stack>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <ostream>
#include <omp.h>
#include <chrono>
#include <iostream>
#include <omp.h>
#include <assert.h>

using namespace std;

//print contents of reps
void printDV(deque< shared_ptr< vector< int > > > o){
    for(int i = 0; i < o.size(); ++ i){
        if(!o[i]->empty()){
            vector<int>::iterator vit = o[i]->begin();
            printf("(");
            while(vit != o[i]->end()){
                printf("%d",*vit);
                vit++;
                if(vit != o[i]->end()){
                    printf(",");
                }
            }
            printf(")\n");
        }
    }
}

/* input: string of character
 * An optimal algorithm for computing the repetitions in a word
 * as proposed by Max. Crochemore
 * O(n log n) time
 * output: repetitions printed to std out
 */
void ikrep(char const * input){
    
    // 1    
    static int n,twon,p; // length of input, twice length, recursion level
    n = strlen(input);
    twon = n*2;
    // for each position gieves index of e-class, for each position gives 
    //index of d-class 
    int E[n], D[n]; 
    // for e-class index the positions in that e-class, for d-class index the 
    // positions in that d-class
    vector< shared_ptr< list< int > > > eClass, dClass; 
    list<int>::iterator lit;
    
    stack<int> nI; // contains available indicies of e-classes    
    // init eClass and nI
    eClass.reserve(twon);
    for(int k = twon; k != 0; --k){
        nI.push(k);   
        shared_ptr< list<int> > nl(new list<int>); 
        eClass.push_back(nl);
    }    
    E[0] = 0;
    eClass[0]->push_back(0);
    
    // calculate E, update eClass, init dClass 
    dClass.reserve(n);    
    for(int i = 1; i < n; ++i){
        int j,k;
        j = 0;
        bool placed = false;
        while(!placed){
            if(eClass[j]->size() > 0 && input[i] == input[eClass[j]->front()]){
                k = j;
                placed = true;
            } else if(eClass[j + 1]->size() > 0){
                j++;
            } else{
                break;
            } 
        }
        if(!placed){
            k = nI.top();
            nI.pop();
        }
        E[i] = k;
        eClass[k]->push_back(i);        
        shared_ptr< list<int> > nl(new list<int>);
        dClass.push_back(nl);
    }
    
    
    // calculate D, update dDclass
    for(int k = 0; k < twon; ++k){
        if(eClass[k]->size() > 0){
            for(int i = 0; i < eClass[k]->size(); ++i){
                lit = eClass[k]->begin();
                advance(lit,i);
                int y = *lit;    
                lit++;
                if((i + 1) < eClass[k]->size()){
                    int diff = *lit - y;
                    D[y] = diff;
                    dClass[diff]->push_back(y); 
                } else {
                    D[y] = 0;
                    dClass[0]->push_back(y);
                }
                
            }

        }
        else{
            break;
        }
    }
    
    //init p
    p = 1;
    
    //queue of small classes
    deque<int> smcl;  
    deque<int>::iterator dit;
    //list of repetitions
    deque< shared_ptr< vector< int > > > reps; 
    
    //init small
    for(int i = 0; i < n; ++i){
        if(eClass[i]->size() > 0){
            smcl.push_back(i);
        } else {
            break;
        }
    }
    
    //2     
    while(!smcl.empty()){        
        //3 finding repetitions of length p
        while(!dClass[p]->empty()){
            int i = dClass[p]->front();
            dClass[p]->pop_front();
            do{
                i = i + p;
            } while(D[i] == p);
            int e = 1;
            do{
                i = i - p;
                e++;
                shared_ptr< vector<int> > nv(new vector<int>);
                nv->push_back(i);
                nv->push_back(p);
                nv->push_back(e);
                reps.push_back(nv);
                dClass[p]->remove(i);
            }while(i - p > 0 && D[i - p] == p);
        }
        //4 increment p
        p = p + 1;
        if(p > n/2){
            printf("Number of Repetitions: %li \n",reps.size());
            printDV(reps);            
            printf("MISSION COMPLETE\n");
            break;
        }
        //5.1 copy small classes to queue        
        //set of eclasses, used in 6.
        set<int> split;
        set<int>::iterator sit; 
        //for each eclass in split a list of subclass indicies
        unordered_map<int,list<int> > subCs;
        //for each eclass k in split the last eclass used to split k 
        unordered_map<int,int> lS;
        omp_set_num_threads(smcl.size());
        #pragma omp parallel
        {
            int ID = omp_get_thread_num(); 
            //a list of subclass indicies
            list<int> subC;
            list<int>::iterator subCit;
            //queue of indicies of eclasses refined by this thread in 
            //preserved increasing order
            deque< shared_ptr< vector< int > > > tque;
            int s;
            #pragma omp critical
            {
                s = smcl.front();
                smcl.pop_front(); 
            }
            list<int>::iterator lit = eClass[s]->begin();
            while(lit != eClass[s]->end()){
                int j = *lit;
                if(j != 0){
                    shared_ptr< vector<int> > nv(new vector<int>);
                    nv->push_back(j);
                    nv->push_back(s);
                    tque.push_back(nv);
                    int k = E[j - 1];
                    bool contains = false;
                    subCit = subC.begin();                    
                    while(subCit != subC.end()){
                        if(k == *subCit){
                            contains = true;
                            break;
                        }
                        ++subCit;
                    }
                    if(!contains){
                        subC.push_back(k);                        
                    }      
                    #pragma omp critical
                    {
                        if(split.find(k) == split.end()){
                            split.insert(k);                            
                            subCs.insert({k,subC});   
                            lS.insert({k,0});
                        }
                    }
                }
                lit++;
            }  
            #pragma omp barrier
            #pragma omp critical
            {
                //5.2 one position at a time is transfered to a new class    
                while(!tque.empty()){
                    vector<int> curr = *(tque.front());
                    tque.pop_front();
                    int j = curr[0];
                    int s = curr[1];
                    int i = j - 1;
                    int k = E[i];

                        if(lS.at(k) != s){
                            lS[k] = s;                
                            subCs[k].push_back(nI.top());
                            subC.push_back(nI.top());
                            nI.pop();
                        }

                    int kbar = subC.back();
                    lit = eClass[k]->begin();
                    while(*lit != i){
                        lit++;
                    }
                    if(lit != eClass[k]->begin()){
                        lit--;
                        int iprime = *lit;
                        dClass[D[iprime]]->remove(iprime);
                        D[iprime] = D[iprime] + D[i];                 
                        dClass[D[iprime]]->push_back(iprime);

                    }
                    eClass[k]->remove(i);
                    eClass[kbar]->push_back(i);
                    E[i] = kbar;
                    dClass[D[i]]->remove(i);
                    dClass[0]->push_back(i);
                    D[i] = 0;
                    lit = eClass[kbar]->begin();
                    while(*lit != i){
                        lit++;
                    }
                    if(lit != eClass[kbar]->begin()){
                        lit--;
                        int iprime = *lit;
                        dClass[D[iprime]]->remove(iprime);
                        int t = i - iprime;
                        if(t < 0){
                            D[iprime] = 0; 
                        } else {
                            D[iprime] = t;
                        }              
                        dClass[D[iprime]]->push_back(iprime);
                    }

                } 
            }         
        }
        
        
        //6 calculate new value of small
        while(!split.empty()){  
            sit = split.end();
            sit--;
            int k = *sit;   
            split.erase(sit);
            if(eClass[k]->empty()){
                nI.push(k);
                subCs[k].remove(k);                                    
            }            
            lit = subCs[k].begin();               
            while(lit != subCs[k].end()){    
                smcl.push_front(*lit);
                ++lit;                
            }            
        } 
    }
}

int main(int argc, char **argv) {
    //check if input is nulll
    if(argv[1] == NULL){
        fprintf(stderr,"Error at input: Invalid input length\n");
        exit(EXIT_FAILURE);
    }  
    string input = argv[1];
    input = input + '$';
    printf("%s\n",input.c_str());
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for(int i = 0; i < 1; ++i){
        ikrep(input.c_str());
    }    
    std::chrono::steady_clock::time_point endPar= std::chrono::steady_clock::now();
    std::cout << "Approach took: " << std::chrono::duration_cast<std::chrono::nanoseconds> (endPar - begin).count() <<std::endl;
    return 0;
}





