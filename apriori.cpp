#include<bits/stdc++.h>
using namespace std;


void record_data(vector< vector<string> > &db , map<string,vector<int>> &record)
{
    for(int i=0;i<db.size();i++)
    {
        for(int j=1;j<db[i].size();j++)
        {
            if(record.find(db[i][j])==record.end())
            {
                vector<int>tmp;
                for(int k=0;k<db.size();k++)
                {
                    tmp.push_back(0);
                }
                record.insert({db[i][j],tmp});
                record[db[i][j]][i]=1;
            }
            else
            {
                record[db[i][j]][i]=1;
            }
        }
    }
}

void combinations(vector<string>input , string output,int k,vector<string>  &new_can)
{
    if(input.size()==0)
    {
        if(output.length()==k)
        new_can.push_back(output);
        return;
    }
    string first=input[0];
    input.erase(input.begin());
    combinations(input,output+first,k,new_can);
    combinations(input,output,k,new_can);
}

vector<int> vector_and(vector<int> &a, vector<int> &b)
{
    vector<int>result;
    for(int i=0;i<a.size();i++)
    result.push_back(a[i] & b[i]);
    return result;
}

int count_1(vector<int> &a)
{
    int count=0;
    for(int i=0;i<a.size();i++)
    if(a[i]==1) count++;
    return count;
}

void count_support(map<string,vector<int>> &record, map<string,int> &new_freq , vector<string> &new_can, int n)
{
    for(int i=0;i<new_can.size();i++)
    {
        //cout<<new_can[i]<<"\n";
        vector<int>tmp;
        for(int x=0;x<n;x++)
        tmp.push_back(1);

        for(int j=0;j<new_can[i].length();j++)
        {
            string x(1,new_can[i][j]);
            tmp=vector_and(tmp,record[x]);
        }
        new_freq[new_can[i]]=count_1(tmp);
    }
}

void extract_frequent(map<string,int> &new_freq , int sup_min)
{
    map<string,int> tmp;
    for(auto i=new_freq.begin();i!=new_freq.end();i++)
    {
        if(i->second >= sup_min)
        {
            tmp.insert({i->first,i->second});
        }
    }
    new_freq.clear();
    new_freq=tmp;
}

void rem_candidates(map<string,int> &new_freq,vector<string> &candidates)
{
    int found=0;
    vector<string>tmp;
    for(int i=0;i<candidates.size();i++)
    {
            found=0;
            for(auto k=new_freq.begin();k!=new_freq.end();k++)
            {
                string s=k->first;
                for(int l=0;l<s.length();l++)
                {
                    if(s[l]==candidates[i][0])
                    {
                        found=1;
                        break;
                    }
                }
                if(found==1)
                break;
            }
            if(found==1)
            tmp.push_back(candidates[i]);
    }
    candidates=tmp;
}
int main()
{
	vector< vector<string> > db{ {"10","A","B"},
							   {"20","C","E"},
							   {"30","A","C","D"},
							   {"40","D","B","E"},
							   {"50","B","C","E"},
							   {"60","A","B","C","E"}
							   };
	
	int sup_min=2;
	
    cout<<"\n\nT.id     Items\n";
    cout<<"_____________________\n";
    for(int i=0;i<db.size();i++)
    {
        cout<<db[i][0]<<"   |   ";
        for(int j=1;j<db[i].size();j++)
        {
            cout<<db[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\nsup_min= "<<sup_min<<"\n";
    cout<<"_______________________________________________\n\n";

	map<string,int>frequent;
	vector<string> candidates;
    map<string,vector<int>>record;
    record_data(db,record); //create bit vectors
    

    //__________________________________________________________________1st SCAN______________________________________________________
	for(int i=0;i<db.size();i++)
	{
	    for(int j=1;j<db[i].size();j++)
	    {
	        if(frequent.find(db[i][j])!=frequent.end())
	        frequent[db[i][j]]++;
	        else
	        {
	         candidates.push_back(db[i][j]);
	         frequent[db[i][j]]=1;
	        }
	        
	    }
	}
    


	int k=1;
	cout<<"K= 1\n\n";
    cout<<"Itemset     sup_count\n";
    cout<<"_____________________\n";
	for (auto itr = frequent.begin(); itr != frequent.end(); ++itr) 
	{
        cout << itr->first<< "       |       " << itr->second << "\n";
    }
    
    std::vector<string>::iterator it;
    for (auto itr = frequent.begin(); itr != frequent.end(); ++itr) 
    {
        if(itr->second < sup_min)
        {
            frequent.erase(itr->first);
            it = find (candidates.begin(), candidates.end(), itr->first);
            candidates.erase(it);
        }
            

    }
    cout<<"\nAfter extracting frequent itemsets with sup_count >= sup_min\n\n";
    cout<<"Itemset   sup_count\n";
    cout<<"_____________________\n";
    for (auto itr = frequent.begin(); itr != frequent.end(); ++itr) 
	{
        cout << itr->first<< "       |       " << itr->second << "\n";
    }
    cout<<"_______________________________________________\n\n";
    //______________________________________________________REPEAT TILL FREQUENT ITEMSETS ARE FOUND____________________________________________


    vector<string>new_can;
    map<string,int>new_freq;

    while(frequent.size()>0)
    {
        k++;
        int n=candidates.size();
        if(k>n) break;

        //generate new combinations for current iteration i.e. itemsets
        combinations(candidates,"",k,new_can);

        //count support for itemsets
        count_support(record, new_freq , new_can,db.size());
        
        cout<<"\nK= "<<k<<"\n\n";
        cout<<"Itemset     sup_count\n";
        cout<<"_____________________\n";
        for (auto itr = new_freq.begin(); itr != new_freq.end(); ++itr) 
	        {
                cout << itr->first<< "       |       " << itr->second << "\n";
            }


        //extract frequent itemsets
        extract_frequent(new_freq , sup_min);
        cout<<"\nAfter extracting frequent itemsets with sup_count >= sup_min\n\n";
        cout<<"Itemset     sup_count\n";
        cout<<"_____________________\n";
        for (auto itr = new_freq.begin(); itr != new_freq.end(); ++itr) 
	        {
                cout << itr->first<< "       |       " << itr->second << "\n";
            }
        cout<<"_______________________________________________\n";

        //removing items which are no longer required
        rem_candidates(new_freq,candidates);
        
        //updating frequent itemsets list
        frequent=new_freq;

        //resetting temporary variables before next iteration
        new_can.clear();
        new_freq.clear();
        
    } 
}