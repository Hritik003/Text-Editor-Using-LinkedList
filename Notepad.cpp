#include<bits/stdc++.h>
using namespace std;

class LineNode{
    public:
    LineNode(int lineNo){
        lineNumber=lineNo;
        nextLine=NULL;
    }
    private:
    string text;
    int lineNumber;
    LineNode* nextLine;
    friend class TextEditor;
};

class UndoState{
    public:
    UndoState(){
        type=0;
        l1=l2=p1=p2=0;
        text1_behind=text2_behind="";
    }
    private:
    int type;
    int l1,l2,p1,p2;
    string text1_behind,text2_behind;
    friend class TextEditor;

};

class PageNode{
    public:
    PageNode(int pageNo){
        pageNumber = pageNo;
        nextPage = NULL;
        nextPLine = NULL;
    }
    private:
    int pageNumber;
    PageNode* nextPage;
    LineNode* nextPLine;
    friend class TextEditor;
};

class TextEditor{
    public:
    TextEditor();
    void insertLine(int pageNo, int lineNo, string userText);
    PageNode* searchPage(int pageNo);
    LineNode* searchLine(PageNode* page,int lineNo);
    void addToLine1(int page1, int line1, int page2, int line2);
    void printAll();
    void saveFile();
    void printPage(int pageNo);
    void deletePage(int pageNo);
    void deleteLine(PageNode* p, int lineNo);
    void undo();
    private:
    vector<UndoState*> undoVector;
    PageNode* head;
};

TextEditor::TextEditor(){
    head = NULL;
}

PageNode* TextEditor::searchPage(int pageNo){
    PageNode* temp = head;
    while(temp){
        if(temp->pageNumber == pageNo){
            return temp;
        }
        temp = temp->nextPage;
    }
    return temp;
}

LineNode* TextEditor::searchLine(PageNode* page,int lineNo){
    LineNode* temp = page->nextPLine;
    while(temp){
        if(temp->lineNumber == lineNo){
            return temp;
        }
        temp = temp->nextLine;
    }
    return temp;
}

void TextEditor::insertLine(int pageNo, int lineNo, string userText){
    UndoState* undo = new UndoState();
    undo->type = 1;
    undo->p1 = pageNo;
    undo->l1 = lineNo;
    PageNode* pageSearch = searchPage(pageNo);
    if(pageSearch && !pageSearch->nextPLine){
        pageSearch->nextPLine = new LineNode(lineNo);
        pageSearch->nextPLine->text=userText;
    }
    else if(pageSearch){
        LineNode* lineSearch =  searchLine(pageSearch, lineNo);
        if(lineSearch){
            cout<<"You are replacing the text, as the page number and line number given exists"<<endl;
            undo->type = 2;
            undo->text1_behind = lineSearch->text;
            lineSearch->text = userText;
        }
        else{
            LineNode* tempLine = pageSearch->nextPLine;
            if(tempLine->lineNumber > lineNo){
                LineNode* firstNode = new LineNode(lineNo);
                firstNode->text=userText;
                firstNode->nextLine = pageSearch->nextPLine;
                pageSearch->nextPLine=firstNode;
            }
            else{
                while(tempLine){
                    LineNode* nextTempLine = tempLine->nextLine;
                    if(!nextTempLine || nextTempLine->lineNumber > lineNo){
                        LineNode* NewLineNode = new LineNode(lineNo);
                        NewLineNode->text = userText;
                        tempLine->nextLine = NewLineNode;
                        NewLineNode->nextLine = nextTempLine;
                        break;
                    }
                    tempLine = tempLine->nextLine;
                }
            }
        }
    }
    else{
        PageNode* newPageNode = new PageNode(pageNo);
        newPageNode->nextPLine = new LineNode(lineNo);
        newPageNode->nextPLine->text = userText;
        PageNode* tempPageNode = head;
        if(!tempPageNode || tempPageNode->pageNumber > pageNo){
            newPageNode->nextPage = head;
            head = newPageNode;
        }
        else{
            while(tempPageNode){
                PageNode* nextTempPage = tempPageNode->nextPage;
                if(!nextTempPage || nextTempPage->pageNumber > pageNo){
                    tempPageNode->nextPage = newPageNode;
                    newPageNode->nextPage=nextTempPage;
                    break;
                }
                tempPageNode=tempPageNode->nextPage;
            }
        }
    }
    undoVector.push_back(undo);
}

void TextEditor::deleteLine(PageNode* p, int lineNo){
    UndoState* undo = new UndoState();
    undo->type = 2;
    undo->l1 = lineNo;
    undo->p1 = p->pageNumber;
   
    if(p->nextPLine->lineNumber == lineNo){
        LineNode* temp = p->nextPLine;
        undo->text1_behind = temp->text;
        p->nextPLine = p->nextPLine->nextLine;
        delete temp;
        return;
    }
    else{
        LineNode* tempLine = p->nextPLine;
        LineNode* prevLine;
        while(tempLine){
            if(tempLine->lineNumber == lineNo){
                prevLine->nextLine = tempLine->nextLine;
                undo->text1_behind = tempLine->text;
                delete tempLine;
                return;
            }
            prevLine = tempLine;
            tempLine = tempLine->nextLine;
        }
    }
    undoVector.push_back(undo);
}

void TextEditor::deletePage(int pageNo){
    if(head->pageNumber == pageNo){
        PageNode* temp = head;
        head = head->nextPage;
        delete temp;
        return;
    }
    else{
        PageNode* tempPage = head;
        PageNode* prevPage;
        while(tempPage){
            if(tempPage->pageNumber == pageNo){
                prevPage->nextPage = tempPage->nextPage;
                delete tempPage;
                return;
            }
            prevPage = tempPage;
            tempPage = tempPage->nextPage;
        }
    }
}

void TextEditor::addToLine1(int page1, int line1, int page2, int line2){
    UndoState* undo = new UndoState();
    undo->l1 = line1;
    undo->l2 = line2;
    undo->p1 = page1;
    undo->p2 = page2;
    PageNode* p1 = searchPage(page1);
    PageNode* p2 = searchPage(page2);
    if(p1 && p2){
        LineNode* lineNode1 = searchLine(p1,line1);
        LineNode* lineNode2 = searchLine(p2,line2);
        if(undo->l1 && undo->l2){
            undo->text1_behind = lineNode1->text;
            undo->text2_behind = lineNode2->text;
            lineNode1->text += lineNode2->text;
            deleteLine(p2,line2);
        }
        else{
            cout<<"line not found"<<endl;
        }
    }
    else{
        cout<<"page and line not found"<<endl;
    }
    undoVector.push_back(undo);
}

void TextEditor::undo(){
    UndoState* undo = undoVector.back();
    undoVector.pop_back();
    PageNode* temp;
    switch (undo->type)
    {
    case 0:
        break;
    case 1:
    //undoing the line in the same page
        temp = searchPage(undo->p1);
        deleteLine(temp,undo->l1);
        undo->type=0;
        break;
    case 2:
    // undoing the insertion of some line
        insertLine(undo->p1,undo->l1,undo->text1_behind);
        undo->type=0;
        break;
    case 3:
    // undoing the replacement of some line of page that happpened
        insertLine(undo->p2,undo->l2,undo->text2_behind);
        insertLine(undo->p1,undo->l1,undo->text1_behind);
        undo->type = 0;
        break;
    default:
        undo->type =0;
        cout<<"Undo Failed"<<endl;
    }
}

void TextEditor::printPage(int pageNo){
    PageNode* tempPage = head;
    while(tempPage){
        if(tempPage->pageNumber == pageNo){
            LineNode* tempLine = tempPage->nextPLine;
            while(tempLine){
                cout<<tempLine->lineNumber<<") "<<tempLine->text<<endl;
                tempLine = tempLine->nextLine;
            }
            break;
        }
    }

    if(!tempPage){
        cout<<"PAGE DOESN'T EXIST"<<endl;
    }
}

void TextEditor::saveFile(){
    PageNode* tempPage = head;
    ofstream myfile;
    myfile.open ("Output.txt");
    while(tempPage){
        myfile<<"<----- Page Number "<< tempPage->pageNumber <<" ----->"<<"\n";
        LineNode* tempLine = tempPage->nextPLine;
        while(tempLine){
            myfile <<tempLine->lineNumber<<") "<<tempLine->text<<"\n";
            tempLine=tempLine->nextLine;
        }
        tempPage = tempPage->nextPage;
    }
    myfile.close();

}

void TextEditor::printAll(){
    PageNode* tempPage = head;
    while(tempPage){
        cout<<"<----- Page Number "<< tempPage->pageNumber <<" ----->"<<endl;
        LineNode* tempLine = tempPage->nextPLine;
        while(tempLine){
            cout<<tempLine->lineNumber<<") "<<tempLine->text<<endl;
            tempLine = tempLine->nextLine;
        }
        cout<<"<-----End of Page "<< tempPage->pageNumber <<" ----->"<<endl;
        tempPage = tempPage->nextPage;      
    }
}

void options(){
    cout << "<----------------Text Editor------------->\n";
    cout << "1 : Insert text into line N\n";
    cout << "2 : Delete line N\n";
    cout << "3 : Move line M into line N\n";
    cout << "4 : Replace Text in line N\n";
    cout << "5 : Print All\n";
    cout << "6 : Save into a .txt file\n";
    cout << "7 : UNDO\n";
    cout << "8 : Print the page\n";
    cout << "9 : Exit the Editor\n";
}


int main(){
    TextEditor editor;
    
    int choice,page,page2,line,line2;
    
    string text;
    
    while(true){
        
        options();
        
        cin>>choice;
        
        switch(choice)
        
        {
            case 1:
                cout<<"Enter the page number : ";
                cin>>page;
                cout<<"Enter the line number : ";
                cin>>line;
                cout<<"Enter the text : ";
                cin.ignore();
                getline(cin,text);
                editor.insertLine(page,line,text);
                editor.saveFile();
                break;
            case 2:
                cout<<"Enter the page number : ";
                cin>>page;
                cout<<"Enter the line number : ";
                cin>>line;
                editor.deleteLine(editor.searchPage(page),line);
                editor.saveFile();
                break;
            case 3:
                cout<<"Enter the page number : ";
                cin>>page;
                cout<<"Enter the line number : ";
                cin>>line;
                cout<<"Enter the page number : ";
                cin>>page2;
                cout<<"Enter the line number : ";
                cin>>line2;
                editor.addToLine1(page,line,page2,line2);
                editor.saveFile();
                break;
            case 4:
                cout<<"Enter the page number : ";
                cin>>page;
                cout<<"Enter the line number : ";
                cin>>line;
                cout<<"Enter the text : ";
                cin.ignore();
                getline(cin,text);
                editor.insertLine(page,line,text);
                editor.saveFile();
                break;
            case 5:
                editor.printAll();
                break;
            case 6:
                editor.saveFile();
                break;
            case 7:
                editor.undo();
                editor.saveFile();
                break;
            case 8:
                cout<<"Enter the page number : ";
                cin>>page;
                editor.printPage(page);
                break;
            case 9:
                exit(1);
                break;
            default:
                cout<<"Invalid Option"<<endl;
                break;
        }
    }
    return 0;
}
