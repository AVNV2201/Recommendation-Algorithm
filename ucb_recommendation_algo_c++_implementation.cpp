#include  <bits/stdc++.h>
using namespace std;

#define NSIOR 50    // number of selections required to be predicted ( maximum limit )
#define NOC 6       // number of categories for which the selections if needed to be done

// function to print the elements of array of size n
void print( double arr[] , int n ){
    for( int i = 0; i < n; i++ )
        cout << arr[i] << " ";
    cout << endl;
}

// function to return the category wise selections
vector<int> get_selections( int no_of_rewards[],        // array containing the no of times each category was clicked 
                          int no_of_selections[],       // array containing the no of times each category was selected to be displated
                          int no_of_round,              // total no of selections done begore the function is called
                          int &total_count){            // a referece variable which will store the actual no of selections done in this function call
    
    // vector to store the category wise selections 
    vector<int> result(NOC,0);
    
    // for the first time selection of categories, we will simply give equal selections to all 
    if( no_of_round == 1 ){ 
        int tmp = NSIOR / NOC ;
        total_count = tmp * NOC;
        for( int i = 0; i < NOC; i++ )
            result[i] = tmp;
    }
    // selection on the basis upper confidence bound algorithm
    else{
        double avg,             // to store the average clicks per selection for each category
                delta,          // to store a quantity for the algorithm which species the deviation of confidence bounds, but here we will consider only the upper confidence 
                sum_ucb = 0;    // store the sum of upper bound confidences of all categories
        double arr[NOC];        // store the ubc of each category
        
        // loop for calculation of ucb of each category
        for( int i = 0; i < NOC; i++ ){
            
            // calculate the average clicks per selection 
            avg = (double)no_of_rewards[i] / no_of_selections[i];
            
            // calculation of deviation of confidence bound from average
            delta = sqrt( ((double)3/2) *
                         log((double)no_of_round)/no_of_selections[i] );
                         
            // calculation of upper confidence bound
            arr[i] = avg + delta;
            
            // calculation to filter the ucb obtained ( in this case, it will magnify the result and we will have extreme and quick changes in the selections  )
            arr[i] *= arr[i] * arr[i];
            
            // updating the sum_ucb variable
            sum_ucb += arr[i];
        }
        
        print(arr,NOC);
        
        // applying unitary method to obtain the no of selections in accordance with the ucb obtained
        double ratio = NSIOR / sum_ucb;
        for( int i = 0; i < NOC; i++ ){
            result[i] = arr[i] * ratio;
            total_count += result[i];
        }
    }
    
    // return the selection list
    return result;
}

// function to print the elements of a vetor
void print_v( vector<int> v ){
    for( int i: v )
        cout << i << " ";
    cout << endl;
}

// driver function
int main(){
    
    declaration of the states which will store the state of user response after each iteration
    int no_of_round = 1;
    int no_of_rewards[NOC] = {0};
    int no_of_selections[NOC] = {0};
    
    // infinite loop for user interation until the user stops the program
    while( true ){
        
        // store the actual no of selections done
        int total_count = 0;
        
        // function call to run the reinforcement learning ( upper confidence bound ) algorithm
        vector<int> selections = get_selections(no_of_rewards,
                                               no_of_selections,
                                               no_of_round,
                                               total_count);
        
        
        // store the category no at each index 
        vector<int> list;
        
        // binary array to store the state of each selection whether it is clicked or not
        vector<bool> clicked(total_count, false );
        
        // initialize the category-index mapping in accordance with the selections done by the algorithm
        for( int i = 0; i < NOC; i++ ){
            for( int j = 0; j < selections[i]; j++ ){
                list.push_back( i );
            }
        }
        
        // check the results and states
        print_v(selections);
        print_v(list);
        
        // store the user input
        int input;
        
        // record the user inputs ( clicks in real case  )
        while( true ){
            cin >> input;
            if( input < 0 or input >= total_count )
                break;
            clicked[input] = true;      update the state of clicked array
        }
        
        // update the user response in the basic array for the next iteration
        for( int i = 0; i < total_count; i++ ){
            if( clicked[i] ){
                no_of_rewards[list[i]]++ ;
            }
        }
        
        no_of_round += total_count;
        for( int i = 0; i < NOC; i++ ){
            no_of_selections[i] += selections[i];
        }
    }
    
    return 0;
}

