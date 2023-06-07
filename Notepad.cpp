
#include <iostream>
#include <string>
using namespace std;


//defining the linked list structure
struct Node{
    string line;
    Node* next;
};


class Notepad
{
    private: 
    Node* head; //head of the text editor
    int numLines=0;
    public:
    
    //constructor
    Notepad(){
        head = NULL;
    }
    
    void Insert(int position, string text){
        Node* node = new Node;
        node->line = text;
        
        if(position ==1){
            node->next = head;
            head = node;
        }
        else{
            Node* temp = head;
            for(int i=1;i<position-1;i++){
                if(temp){
                    temp= temp->next;
                }
                else{
                    cout<<"Can't perform this!\n";
                    return;
                }
            }
            if(temp){
                 node->next = temp->next;
                 temp->next = node;
            }
            else{
                cout<<"Can't perform this!\n";
                return;
            }
           
        }
        numLines++;
        cout<<"Line Insertion is complete\n";
        
    }
    
    
    void Delete(int N){
        if(!head){
            cout<<"No lines to Delete\n";
            return;
        }
        else{
            if(N==1){
                Node* temp=head->next;
                if(temp){
                  head=temp;
                }
                else head = NULL;
            }
            else{
                Node* temp= head;Node* prev=NULL;
                for(int i=1;i<=N-1;i++){
                    if(temp){
                        prev=temp;
                        temp=temp->next;
                    }
                    else{
                        cout<<"Wrong choice\n";
                        return;
                    }
                }
                
                if(temp){
                    prev->next=temp->next;
                }
                else{
                    cout<<"Wrong choice\n";
                    return;
                }
                
                numLines--;
                cout<<"Deletion of line "<<N<<" is complete\n";
            }
        
        }
    }
    
    void moveLine(int n, int m){
        if(head == NULL){
            cout<<"No lines to move\n";
            
        }
        else if(n==m){
            cout<<"from and to position are same\n";
            return;
        }
        else{
        Node* node_n=head;
        Node* node_m=head;
        Node* node_m_prev=NULL;
        Node* node_n_prev=NULL;
        for(int i=1;i<=n-1;i++){
            if(node_n){
                node_n_prev=node_n;
                node_n=node_n->next;
            }
        }
        
        for(int i=1;i<=m-1;i++){
            if(node_m){
                node_m_prev=node_m;
                node_m=node_m->next;
            }
        }
        
        if(node_m==NULL || node_n==NULL){
            cout<<"wrong choice\n";
            return;
        }
        
        if(node_n_prev){
            node_n_prev->next = node_n->next;
        }
        else{
            head->next= node_n->next;
        }
        
        if(node_m_prev){
            node_m_prev->next = node_n;
        }
        else{
            head = node_n;
        }
        
        node_n->next=node_m;
        
        cout<<"Moved Successfully\n";
        }
        
    }
    
    void replace(int line, string text){
        if(head==NULL){
            cout<<"No lines to replace\n";
        }
        Node* node=head;
        for(int i=1;i<=line-1;i++){
            if(node){
                node=node->next;
            }
            else{
                cout<<"wrong choice\n"<<line<<endl;
                return;
            }
        }
        if(node){
            node->line = text;
            cout<<"Replace Operation complete.\n";
        }
        else{
            cout<<"wrong choice\n";
            return;
        }
    }
    
    void printAll(){
        if(head == NULL){
            cout<<"No lines to print\n";
            return;
        }
        Node* temp = head;
        
        while(temp){
            cout<<temp->line<<endl;
            temp=temp->next;
        }
        
    }
    
    void save(){
        
    }
    
    void undo(){
        
    }
    
    void open(string nameOfFile){
        
    }
    
    void printNextpage(){
        
    }
    
    void printPreviouspage(){
        
    }
    
    
    
};


int main()
{
    Notepad editor;
    string text;
    int choice,position, fromPos, toPos;
    do{
        cout<<"========TEXT EDITOR========\n"<<endl;
        cout<<"Please choose what you want to do\n";
        cout<<"1.  Insert text into line N\n";
        cout<<"2.  Delete line N\n";
        cout<<"3.  Move line N into line M\n";
        cout<<"4.  Replace text in line N\n";
        cout<<"5.  Print all\n";
        cout<<"6.  Save into a .txt file\n";
        cout<<"7.  Undo\n";
        cout<<"8.  Open a .txt file\n";
        cout<<"9.  Print the next page\n";
        cout<<"10. Print the previous page\n";
        cout<<"11. Exit\n";
        cin>>choice;
        
        switch(choice){
            case 1:
            cout<<"Enter the text: ";
            cin.ignore();
            getline(cin,text);
            cout<<"enter line you want the text to be placed into : ";
            cin>>position;
            editor.Insert(position,text);
            break;
            
            case 2:
            cout<<"Enter the line to be deleted: ";
            cin>>position;
            editor.Delete(position);
            break;
            
            case 3:
            cout<<"Enter the from position of the line: ";
            cin>>fromPos;
            cout<<"Enter the to position of the line: ";
            cin>>toPos;
            editor.moveLine(fromPos,toPos);
            break;
            
            case 4:
            cout<<"Enter the position to replace: ";
            cin>>position;
            cout<<"Enter the text: ";
            cin.ignore();
            getline(cin,text);
            editor.replace(position,text);
            break;
            
            case 5:
            editor.printAll();
            break;
            
            
            case 6:
            
            
            
            break;
            
            
            
            case 7:
            
            
            break;
            
            case 8:
            break;
            
            
            case 9:
            break;
            
            case 10: 
            break;
            
            case 11:
            cout<<"Exiting the program...\n";
            break;
            
            default:
            cout<<"Invalid choice"<<endl;
            break;
            
        
        }
    }while(choice!=11);

    return 0;
}
