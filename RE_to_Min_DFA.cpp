#include<iostream>
#include<bits/stdc++.h>
#include<list>
#include<vector>
#include<string>
#include <sstream>
#include<stdio.h>
using namespace std;
#define tr(container, it) \
      for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)

vector< set<int>  > closure;
vector<pair<set<int>,pair<set<int>,set<int> >  > > table;   // dfa table
vector<set<int> > states;  // states in a DFA
set<int > final_states;	//final states in a NFA
set<int> final_states_in_DFA;//final states in a DFA
map<set<int>,int> mapping;

int graph_for_matching[1000][2];

class Graph
{
    int V;   
    list<int> *adj; 
    public:   
    Graph(int V); 
    void addEdge(int v, int w); 
    bool isReachable(int s, int d); 
};

class NFA
{
    public:
    string reg;
    vector<vector<int> > graph;
    int m;
     void create_nfa(string &x)
    {
        reg=x;

        m=reg.length();

        graph.resize(m+1);   // to add all the epislon transitions

        stack <int>  a;


        for(int i=0;i<m;i++)
        {
            int lp=i;
            if(reg[i]=='(' || reg[i]=='|')
            {
                a.push(i);
            }

            else if(reg[i]==')')
            {
                int OR=a.top();
                a.pop();
                if(reg[OR]=='|')
                {
                    lp=a.top();
                    a.pop();

                    graph[OR].push_back(i);
                    graph[lp].push_back(OR+1);
                }
                else
                    lp=OR;
            }

            if(i<m-1 &&reg[i+1]=='*')
            {
                graph[i+1].push_back(lp);
                graph[lp].push_back(i+1);

            }

            if (reg[i] == '(' || reg[i] == '*' || reg[i] == ')')
            {
                graph[i].push_back(i+1);
            }



        }

    }
    
void create_nfa_table()
    {
        cout<<"prnting nfa tabe\n";
        cout<<"states\t"<<"a\t"<<"b\t"<<"epislon\n";
        for(int i=0;i<=m;i++)
        {
            cout<<i<<"\t";
            //checking transitioj of this state on a;
            if(reg[i]=='a')
            {
                cout<<i+1<<"\t";
            }
            else
            {
                cout<<"\t";
            }
            //checking for transition on b;
            if(reg[i]=='b')
            {
                cout<<i+1<<"\t";
            }
            else
            {
                cout<<"\t";
            }

            //checking epislon transition

            tr(graph[i],it)
            {
                cout<<*it<<" ";
            }
                cout<<"\n";

	}

        cout<<"final state  "<<m<<"\n";
    }

};

void find_final_states_in_nfa(string s)
{
    int m =s.length();

    for(int i=0;i<m+1;i++)
    {
       if(closure[i].find(m)!=closure[i].end())
       {
           final_states.insert(i);
       }
    }
}

set<int> find_closure_of_a_set(set<int> s)
{

    set<int > r;
    tr(s,it)
    {
        tr(closure[*it],it1)
        {
            r.insert(*it1);
        }
    }
    return r;
}

void insert_in_table_states(set<int> s)
{
    tr(states,it)
    {
        if(s==*it)
        {
            return;
        }
    }
    states.push_back(s);
}

void convert_to_DFA(string reg)
{

    set<int > s;
    s.insert(0);

    states.push_back(s);

    for(int i=0;i<states.size();i++)
    {
        set<int> A;
        set<int> B;

        set<int> r=find_closure_of_a_set(states[i]);

        tr(r,it1)
        {

            if(reg[*it1]=='a')
            {
                A.insert(*it1+1);
            }
            if(reg[*it1]=='b')
            {
                B.insert(*it1+1);
            }
        }
        
         insert_in_table_states(find_closure_of_a_set(A));
        insert_in_table_states(find_closure_of_a_set(B));

        table.push_back(make_pair(states[i],make_pair(find_closure_of_a_set(A),find_closure_of_a_set(B))));
   }

}

set<int>  bfs(vector<vector<int> > &g,int s)
{
        set<int>  collect;
        vector<int> * visited=new vector<int>(g.size(),0);
        queue<int > q;

        q.push(s);
        visited->at(s)=true;
        collect.insert(s);

        while(!q.empty())
        {
                int t=q.front();
                q.pop();

                tr(g[t],it)
                {
                    if(visited->at(*it)==false)
                    {
                        q.push(*it);
                        collect.insert(*it);
                        visited->at(*it)=true;

                    }
                }
        }
    return collect;
}

void find_closure(vector<vector<int > > &g,string s)
{

    for(int i=0;i<=s.length();i++)
    {
        closure.push_back(bfs(g,i));
    }
}

void map_states_to_integer()
{
    int coun=1;
    tr(states,it)
    {
        mapping[*it]=coun;
        coun++;
    }
}

void print_final_DFA_table()
{
    cout<<"printing dfa table\n";
    cout<<"states\ta\tb\n";
    for(int i=0;i<table.size();i++)
    {
       cout<< mapping[table[i].first]-1<<"\t"<<mapping[table[i].second.first]-1<<"\t"<<mapping[table[i].second.second]-1;
        cout<<"\n";
    }

}

void find_final_states_in_dfa()
{
    tr(mapping,it)
    {
        set<int> r=it->first;
        set<int> intersect;
       set_intersection(r.begin(),r.end(),final_states.begin(),final_states.end(),inserter(intersect,intersect.begin()));
        if(intersect.size()!=0)
        {
            final_states_in_DFA.insert(it->second);
        }
    }
}

bool Graph::isReachable(int s, int d)
{
    if (s == d)
      return true;
 
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
 
    list<int> queue;

    visited[s] = true;
    queue.push_back(s);
 
    list<int>::iterator i;
 
    while (!queue.empty())
    {
        s = queue.front();
        queue.pop_front();
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (*i == d)
                return true;

            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
 
    return false;
}

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}
 
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); 
}

void build_graph()
{	
	
	 for(int i=0;i<table.size();i++)
    {
      graph_for_matching[mapping[table[i].first]-1][0]=mapping[table[i].second.first]-1;
    	graph_for_matching[mapping[table[i].first]-1][1]=mapping[table[i].second.second]-1;
    }
}

void match_string_finally(string s)
{
		int fla_g=0;
		if(s.length()==1 && s[0]=='E')
		{
			tr(final_states_in_DFA,it)
    			{

       				 if(0==*it-1)
       				 {
       				 	cout<<"string _match\n";
       				 	fla_g=1;
       				 }
   			 }
		}
		
		int state=0;
		
		for(int i=0;i<s.length();i++)
		{
			if(s[i]=='a')
			{
			state=graph_for_matching[state][0];
			}
			else if(s[i]=='b')
			{
			state=graph_for_matching[state][1];
			}
			else
			if(fla_g==0)
			{
				cout<<"invalid_string\n";
				return;
			}
			
		}
		
		
		tr(final_states_in_DFA,it)
    		{
			if(state==*it-1)
		       	{
		       		cout<<"finally state is "<<state+1<<"\n";
		       		cout<<"string is matched\n";
		       		return;
		       	}
      		}
      			
		cout<<"finally state is "<<state+1<<"\n";
		cout<<"not matched";
		return ;
      
}
		
int main()
{
	
	
	
	NFA b;
    	string s;
    	cin>>s;
    	s="("+s+")";
    
	b.create_nfa(s);
	b.create_nfa_table();
	
	find_closure(b.graph,b.reg);
  	convert_to_DFA(b.reg);

    	map_states_to_integer();
	print_final_DFA_table();
	find_final_states_in_nfa(b.reg);
	find_final_states_in_dfa();

    	cout<<"final states in the dfa are : \n";
    	tr(final_states_in_DFA,it)
    	{

        	cout<<*it-1<<"\n";
    	}
	
	int n;
	n=table.size();
	int len=0;
	int arr[n][2];
	Graph g(n);
	int flag=0;
	bool reachable[n];
	for(int i=0;i<n;i++)
	{
		reachable[i]=true;
	}
	for(int i=0;i<n;i++)
	{
		arr[i][0]=mapping[table[i].second.first]-1;
		arr[i][1]=mapping[table[i].second.second]-1;
		int x=arr[i][0];
		int y=arr[i][1];
		g.addEdge(i,x);
		g.addEdge(i,y);
	}
	
	
	int final_states[n];
	int state;
	
	tr(final_states_in_DFA,it)
	{
		
		final_states[len++]=*it-1;
	
	}
	
	for(int i=1;i<n;i++)
	{
		if(!g.isReachable(0,i))
		{
			reachable[i]=false;
		}
	}
	
	int min_table[n][n];
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			min_table[i][j]=0;
	
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			int flag1=0;
			int flag2=0;
			if(i==j || reachable[i]==false || reachable[j]==false )
			{
				min_table[i][j]=-2;
			}
			else 
			{
				for(int k=0;k<len;k++)
				{
					if(i==final_states[k])
					{
						flag1=1;
						break;
					}
				}
				for(int k=0;k<len;k++)
				{
					if(j==final_states[k])
					{
						flag2=1;
						break;
					}
				}
				if(flag1!=flag2 )
				{
					min_table[i][j]=1;
				}
			}
	
		}
	}
	/*	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<min_table[i][j]<<" ";
		}
		cout<<endl;
	}
	*/

	
	int pcount=0;
	int ncount=0;
	
	do
	{
		pcount=ncount;
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
			
				int p1=arr[i][0];
				int p2=arr[j][0];
				int q1=arr[i][1];
				int q2=arr[j][1];
				//cout<<"p1"<<" "<<p1<<"p2"<<" "<<p2<<"  "<<"q1"<<" "<<q1<<"q2"<<" "<<q2<<endl;
			
				if(min_table[i][j]==-2)
				{
					continue;
				}
				else
				if(min_table[i][j]!=1 && (min_table[p1][p2]==1 ||min_table[q1][q2]==1))
				{
					min_table[i][j]=1;	
					min_table[j][i]=1;
					ncount++;
				}
			
			}
			//cout<<"*******"<<endl;
		}
		
	}while(pcount!=ncount);
	/*for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			cout<<min_table[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	*/
	Graph g2(n);
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<i;j++)
		{
			if(min_table[i][j]==0)
			{
				g2.addEdge(i,j);
				g2.addEdge(j,i);
			}
		}
	}
	bool indicator[n];
	int final_matrix[n][n];
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			final_matrix[i][j]=-1;
		}
	}
	for(int i=0;i<n;i++)
	{
		indicator[i]=false;
	}
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			if(g2.isReachable(i,j) && indicator[j]==false)
			{
				//cout<<"hi"<<endl;
				final_matrix[i][j]=j;
				indicator[j]=true;				
			}
		}
	}
	int k=0;
	/*for(int i=0;i<n;i++)
	{
		sort(final_matrix[i],final_matrix[i]+n);
	}*/
	/*for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			cout<<final_matrix[i][k]<<" ";
		}
		cout<<endl;
		
	}
	*/
	bool check[n];
	for(int i=0;i<n;i++)
	{
		check[i]=false;
	}
	
	string str;
	
	vector<string> mystring;
	for(int i=0;i<n;i++)
	{
		str = "";
		if(check[i]==false)
		{
			int a = i;
			stringstream ss;
			ss << a;
			str = str+ss.str();
			check[i]=true;
		}
		
		for(int j=0;j<n;j++)
		{
			if(check[j]==false && final_matrix[i][j]!=-1)
			{
				int a = j;
				stringstream ss;
				ss << a;
				str = str+ss.str();
				check[j]=true;
			}
		
		}
		if(str!="")
		{
			mystring.push_back(str);
			
		}
	
	}
	int flag5=0;
	//cout<<mystring.size()<<endl;
	int bla=mystring.size();
	cout<<"States"<<"\t"<<"a"<<"\t"<<"b"<<endl;
	tr(mystring,it)
	{
		
			string p=*it;
			char c=p[0];
			int tr=(int)c-48;
			//cout<<tr<<endl;
			
			int pos_a=arr[tr][0];
			int pos_b=arr[tr][1];
			char ch_a = (char)(((int)'0')+pos_a);
			char ch_b = (char)(((int)'0')+pos_b);
			
			//cout<<ch_a<<" "<<ch_b<<endl;
			for(int x=0;x<bla;x++)
			{
				
				if(find(mystring[x].begin(),mystring[x].end(),ch_a)!=mystring[x].end())
				{
					cout<<p<<"\t"<<mystring[x]<<"\t";
				}
				
			}
			for(int x=0;x<bla;x++)
			{
				if(find(mystring[x].begin(),mystring[x].end(),ch_b)!=mystring[x].end())
				{
					cout<<mystring[x]<<" ";
				}
				
			}
			cout<<endl;
			
		
	}
	
	build_graph();
    
        while(1)
    	{
   		 cout<<"enter the string";
    		string entry;
    		cin>>entry;
    		match_string_finally(entry);
    		cout<<"press q to exit or any other key to continue\n";
    		char c;
    		cin>>c;
   		 if(c=='q')
   		 {
    			break;
    		 }
    	}
	
	return 0;
}