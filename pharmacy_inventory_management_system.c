//                                                   ||SHREE||

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define MEDICINE_NAME_LEN 100
#define SUPPLIER_NAME_LEN 100

typedef enum{FAILURE,SUCCESS} status_code;

typedef struct unique_medicine
{
    int medication_id;
    struct unique_medicine *next;
}unique_node;

typedef struct medicine_details
{
    char medicine_name[MEDICINE_NAME_LEN];
    int batch;
    int supplier_id;
    int quantity;
    int price;
    int expiry;
    int medication_id;
    int total_sales;    //total sale per batch per medicine
    struct medicine_details *next;
}med_node;

typedef struct medicine_reorder_details
{
    char medicine_name[MEDICINE_NAME_LEN];
    int medication_id;
    int reorder;         // between 10 to 100
    int total_quantity;
    struct medicine_reorder_details *next;
}reorder_node;

typedef struct supplier_informations
{
    int supplier_id;
    char supplier_name[SUPPLIER_NAME_LEN];
    int contact_number;  
    int turnover;
    int supplier_unique_medicine;
    unique_node *un;
    struct supplier_informations *next;
}supplier_node;

status_code create_med_node(med_node **lptr,char medicine_name[],int batch,int supplier_id,int quantity,int price,int expiry,int medication_id)
{
    status_code SC=SUCCESS;
    med_node *nptr;
    nptr=(med_node*)malloc(sizeof(med_node));
    if(nptr==NULL)
    {
        SC=FAILURE;
    }
    else
    {
        strcpy(nptr->medicine_name,medicine_name);
        nptr->batch=batch;
        nptr->supplier_id=supplier_id;
        nptr->quantity=quantity;
        nptr->price=price;
        nptr->expiry=expiry;
        nptr->medication_id=medication_id;
        nptr->total_sales=0;
        nptr->next=NULL;
    }
    *lptr=nptr;
    return SC;
}
status_code create_reorder_node(reorder_node **lptr,char medicine_name[],int medication_id,int reorder,int total_quantity)
{
    status_code SC=SUCCESS;
    reorder_node *nptr;
    nptr=(reorder_node*)malloc(sizeof(reorder_node));
    if(nptr==NULL)
    {
        SC=FAILURE;
    }
    else
    {
        strcpy(nptr->medicine_name,medicine_name);
        nptr->medication_id=medication_id;
        nptr->reorder=reorder;
        nptr->total_quantity=total_quantity;
        nptr->next=NULL;
    }
    *lptr=nptr;
    return SC;
}
status_code create_supplier_node(supplier_node **lptr,int supplier_id,char supplier_name[],int contact_number)
{
    status_code SC=SUCCESS;
    supplier_node *nptr;
    nptr=(supplier_node*)malloc(sizeof(supplier_node));
    if(nptr==NULL)
    {
        SC=FAILURE;
    }
    else
    {
        nptr->supplier_id=supplier_id;
        strcpy(nptr->supplier_name,supplier_name);
        nptr->contact_number=contact_number;
        nptr->turnover=0;
        nptr->supplier_unique_medicine=0;
        (nptr->un)=(unique_node *)malloc(sizeof(unique_node));
        (nptr->un)->medication_id=-999;
        (nptr->un)->next=NULL;
        nptr->next=NULL;
    }
    *lptr=nptr;
    return SC;
}

status_code insert_update_medication_db(med_node **head,char medicine_name[],int batch,int supplier_id,int quantity,int price,int expiry,int medication_id)
{
    status_code SC=SUCCESS;
    int found=0;
    med_node *nptr;
    med_node *tptr;
    nptr=tptr=*head;
    while(!found && nptr!=NULL)
    {
        if(nptr->medication_id==medication_id && nptr->batch==batch)
        {
            found=1;
        }
        else
        {
            tptr=nptr;
            nptr=nptr->next;
        }
    }
    if(found==1)
    {
        nptr->quantity=quantity; //only allowed to change quantity
    }
    else
    {
        med_node *ptr=NULL;
        SC=create_med_node(&ptr,medicine_name,batch,supplier_id,quantity,price,expiry,medication_id);
        if(SC==SUCCESS)
        {
            if(tptr!=NULL)
            {
                tptr->next=ptr;
            }
            else
            {
                (*head)=ptr;
            }
        }
    }
    return SC;
}

bool is_aldready_in_supplier_db(supplier_node *head,int supplier_id)
{
    bool ans=0;
    while(head!=NULL && !ans)
    {
        if(head->supplier_id==supplier_id)
        {
            ans=1;
        }
        else
        {
            head=head->next;
        }
    }
    return ans;
}
bool is_aldready_in_reorder_database(reorder_node *head,int medication_id)
{
    bool ans=0;
    while(head!=NULL && !ans)
    {
        if(head->medication_id==medication_id)
        {
            ans=1;
        }
        else
        {
            head=head->next;
        }
    }
    return ans;
}

status_code insert_update_supplier_db(supplier_node **head,int supplier_id,int contact_number,char supplier_name[])
{
    status_code SC=SUCCESS;
    supplier_node *nptr,*tptr;
    nptr=tptr=*head;
    int found=0;
    while(!found && nptr!=NULL)
    {
        if(nptr->supplier_id==supplier_id)
        {
            found=1;
        }
        else
        {
            tptr=nptr;
            nptr=nptr->next;
        }
    }
    if(found==1)          // in case of update update contact and name
    {
        strcpy(nptr->supplier_name,supplier_name);
        nptr->contact_number=contact_number;
    }
    else
    {
        supplier_node *ptr;
        SC=create_supplier_node(&ptr,supplier_id,supplier_name,contact_number);
        if(SC==SUCCESS)
        {
            if(tptr!=NULL) //can use tail here or maintain prev in above loop
            {
                tptr->next=ptr;
            }
            else
            {
                (*head)=ptr;
            }
        }
    }
    return SC;
}

status_code insert_update_reorder_db(reorder_node **head,int medication_id,int reorder,char medicine_name[])
{
    status_code SC=SUCCESS;
    int found=0;
    reorder_node *nptr,*tptr;
    nptr=tptr=*head;
    while(!found && nptr!=NULL)
    {
        if(nptr->medication_id==medication_id)
        {
            found=1;
        }
        else
        {
            tptr=nptr;
            nptr=nptr->next;
        }
    }
    if(found==1)
    {
        nptr->reorder=reorder;
    }
    else
    {
        reorder_node *ptr;
        SC=create_reorder_node(&ptr,medicine_name,medication_id,reorder,0);
        if(SC==SUCCESS)
        {
            if(tptr!=NULL)
            {
                tptr->next=ptr;
            }
            else
            {
                (*head)=ptr;
            }
        }
    }
    return SC;
}

status_code calculate_turnover(supplier_node *head,int price,int quantity,int supplier_id)
{
    status_code SC=SUCCESS;
    int found=0;
    while(!found && head!=NULL)
    {
        if(head->supplier_id==supplier_id)
        {
            head->turnover=head->turnover+quantity*price;
            found=1;
        }
        else
        {
            head=head->next;
        }
    }
    if(found==0)
    {
        SC=FAILURE;
    }
    return SC;
}
void calculate_unique_medicine(supplier_node *head,int supplier_id, int medication_id)
{
    int found=0;
    while(head!=NULL && !found)
    {
        if(head->supplier_id==supplier_id)
        {
            unique_node *current=(head->un);
            unique_node *prev=NULL;
            while(current!=NULL && !found)
            {
                if(current->medication_id==medication_id)
                {
                    found=1;
                }
                else
                {
                    prev=current;
                    current=current->next;
                }
            }
            if(found!=1)
            {
                prev->next=(unique_node *)malloc(sizeof(unique_node));
                prev=prev->next;
                prev->medication_id=medication_id;
                prev->next=NULL;
                head->supplier_unique_medicine++;
            }
        }
        head=head->next;
    }
}

void update_total_quantity(reorder_node *head,int medication_id,int quantity)
{
    int found=0;
    while(head!=NULL && !found)
    {
        if(head->medication_id==medication_id)
        {
            found=1;
        }
        else
        {
            head=head->next;
        }
    }
    if(found==1)
    {
        head->total_quantity=head->total_quantity+quantity;
    }
}

void stock_alert(reorder_node *head,int medication_id)
{
    int found=0;
    while(!found && head!=NULL)
    {
        if(head->medication_id==medication_id)
        {
            found=1;
        }
        else
        {
            head=head->next;
        }
    }
    if(found==1)
    {
        if(head->total_quantity<=head->reorder)
        {
            printf("\nMedicine with %d id has less quantity than reorder level please restock\n",medication_id);
        }
    }
}

int delete_medication_db(med_node **head,int batch,int medication_id)
{
    int return_value=0;
    int found;
    med_node *nptr=*head;
    med_node *prev=NULL;
    found=0;
    
    while(!found && nptr!=NULL)
    {
        if(nptr->medication_id==medication_id && nptr->batch==batch)
        {
            found=1;
            return_value=nptr->quantity;
        }
        else
        {
            prev=nptr;
            nptr=nptr->next;
        }
    }
    if(found==1)
    {
        if(prev!=NULL)
        {
            prev->next=nptr->next;
            nptr->next=NULL;
            free(nptr);
        }
        else
        {
            (*head)=(*head)->next;
            nptr->next=NULL;
            free(nptr);
        }
    }
    else
    {
        return_value=0;
    }
    return return_value;
}

void search_medication_by_name(med_node *head,char medicine_name[])
{
    int found=0;
    while(head!=NULL)
    {
        if(strcmp(head->medicine_name,medicine_name)==0)
        {   
            printf("\n");
            printf("Medicine name :%s \n",head->medicine_name);
            printf("Medicine batch number : %d \n",head->batch);
            printf("Supplier_id of supplier for this medicine :%d \n",head->supplier_id);
            printf("Quantity available :%d \n",head->quantity);
            printf("Price at which it was purchased :%d \n",head->price);
            printf("Expiry of this medication in DDMMYYYY :%d \n",head->expiry);
            printf("Medication_id :%d \n",head->medication_id);
            printf("Total sales made of this medication from this batch :%d \n",head->total_sales);
            found=1;
        }
        head=head->next;       
    }
    if(!found)
    {
        printf("\nNO SUCH MEDICATION EXIST IN DB");
    }
        
}
status_code check_to_update(med_node *head,int medication_id,int batch)
{
    status_code SC=FAILURE;
    while(head!=NULL)
    {
        if(head->medication_id==medication_id && head->batch==batch)
        {   
            SC=SUCCESS;
        }
        head=head->next;       
    }
    return SC;
}
void search_medication_by_supplier_id(med_node *head,int supplier_id)
{
    int found=0;
    while(head!=NULL)
    {
        if(head->supplier_id==supplier_id)
        {   
            found=1;
            printf("\n");
            printf("\nMedicine name :%s \n",head->medicine_name);
            printf("Medicine batch number : %d \n",head->batch);
            printf("Quantity available :%d \n",head->quantity);
            printf("Price at which it was purchased :%d \n",head->price);
            printf("Expiry of this medication in DDMMYYYY :%d \n",head->expiry);
            printf("Medication_id :%d \n",head->medication_id);
            printf("Total sales made of this medication from this batch :%d \n",head->total_sales);
        }
        head=head->next;
    }
    if(!found)
    {
        printf("\nTHIS SUPPLIER HAS NOT SUPPLIED ANY MEDICINE");
    }        
}


void extract_date(int expiry,int *day,int *month,int *year)
{
    *day=expiry/1000000;
    *month=(expiry/10000)%100;
    *year=expiry%10000;
}
int is_within_one_month(int current_day,int current_month,int current_year,int expiry_day,int expiry_month,int expiry_year)
{
    int ans=0;
    if (expiry_year==current_year)
    {
        if(expiry_month==current_month)
        {
            ans=1;
        }
        else if(expiry_month==current_month+1)
        {
            if(current_day>=expiry_day)
            {
                ans=1;
            }
        }
    }
    else if(current_month==12 && expiry_month==1 && expiry_year==current_year+1)
    {
        if(current_day>=expiry_day)
        {
            ans=1;
        }
    }
    return ans;
}
void check_expiration_dates(med_node *head)
{
    int current_day,current_month,current_year;
    printf("\nEnter Todays day in DD format: ");
    scanf("%d",&current_day);
    printf("Enter Todays month in MM format: ");
    scanf("%d",&current_month);
    printf("Enter Todays year in YYYY format: ");
    scanf("%d",&current_year);

    while(head!=NULL)
    {
        int expiry_day,expiry_month,expiry_year;
        extract_date(head->expiry,&expiry_day,&expiry_month,&expiry_year);

        if(is_within_one_month(current_day,current_month,current_year,expiry_day,expiry_month,expiry_year)) 
        {
            printf("Alert: Medication %s (Batch %d) is approaching expiry on %02d-%02d-%04d.\n",head->medicine_name,head->batch,expiry_day,expiry_month,expiry_year);
        }
        head=head->next;
    }
}
int compare_dates(int expiry1,int expiry2)
{
    int day1,month1,year1;
    int day2,month2,year2;
    int ans;
    extract_date(expiry1,&day1,&month1,&year1);
    extract_date(expiry2,&day2,&month2,&year2);
    if (year1!=year2)
    {
        ans=year1-year2;
    }
    else if(month1!=month2) 
    {
        ans=month1-month2;
    }
    else
    {
        ans=day1-day2;
    }
    return ans;
}
med_node *divide_med_db(med_node *lptr)
{
    med_node *nptr,*fast,*slow;
    slow=lptr;
    fast=lptr->next->next;
    while(fast!=NULL)
    {
        slow=slow->next;
        fast=fast->next;
        if(fast!=NULL)
        {
            fast=fast->next;
        }
    }
    nptr=slow->next;
    slow->next=NULL;
    return nptr;
}
med_node *merge_med_db(med_node *list1,med_node *list2)
{
    med_node *result,*ptr1,*ptr2,*tail;
    ptr1=list1;
    ptr2=list2;
    if(compare_dates(list1->expiry,list2->expiry)<0)
    {
        result=tail=list1;
        ptr1=ptr1->next;
    }
    else
    {
        result=tail=list2;
        ptr2=ptr2->next;
    }
    while((ptr1!=NULL)&&(ptr2!=NULL))
    {
        if(compare_dates(ptr1->expiry,ptr2->expiry)<0)
        {
            tail->next=ptr1;
            tail=tail->next;
            ptr1=ptr1->next;
        }
        else
        {
            tail->next=ptr2;
            tail=tail->next;
            ptr2=ptr2->next;
        }
    }
    if(ptr1!=NULL)
    {
        tail->next=ptr1;
    }
    else
    {
        tail->next=ptr2;
    }
    return result;
}
med_node *merge_sort_by_expiry(med_node *lptr)
{
    med_node *nptr;
    med_node *list_ptr=lptr;
    if(lptr!=NULL && lptr->next!=NULL)
    {
        nptr=divide_med_db(list_ptr);
        list_ptr=merge_sort_by_expiry(list_ptr);
        nptr=merge_sort_by_expiry(nptr);
        lptr=merge_med_db(list_ptr,nptr);
    }
    return lptr;
}

supplier_node *divide_supplier_db(supplier_node *lptr)
{
    supplier_node *nptr,*fast,*slow;
    slow=lptr;
    fast=lptr->next->next;
    while(fast!=NULL)
    {
        slow=slow->next;
        fast=fast->next;
        if(fast!=NULL)
        {
            fast=fast->next;
        }
    }
    nptr=slow->next;
    slow->next=NULL;
    return nptr;
}
supplier_node *merge_supplier_db(supplier_node *list1,supplier_node *list2)
{
    supplier_node *result,*ptr1,*ptr2,*tail;
    ptr1=list1;
    ptr2=list2;
    if(list1->turnover > list2->turnover)
    {
        result=tail=list1;
        ptr1=ptr1->next;
    }
    else
    {
        result=tail=list2;
        ptr2=ptr2->next;
    }
    while((ptr1!=NULL)&&(ptr2!=NULL))
    {
        if(ptr1->turnover > ptr2->turnover)
        {
            tail->next=ptr1;
            tail=tail->next;
            ptr1=ptr1->next;
        }
        else
        {
            tail->next=ptr2;
            tail=tail->next;
            ptr2=ptr2->next;
        }
    }
    if(ptr1!=NULL)
    {
        tail->next=ptr1;
    }
    else
    {
        tail->next=ptr2;
    }
    return result;
}
supplier_node *merge_sort_by_turnover(supplier_node *lptr)
{
    supplier_node *nptr;
    supplier_node *list_ptr=lptr;
    if(lptr!=NULL && lptr->next!=NULL)
    {
        nptr=divide_supplier_db(list_ptr);
        list_ptr=merge_sort_by_turnover(list_ptr);
        nptr=merge_sort_by_turnover(nptr);
        lptr=merge_supplier_db(list_ptr,nptr);
    }
    return lptr;
}

supplier_node *merge_supplier_db_unique_medicine(supplier_node *list1,supplier_node *list2)
{
    supplier_node *result,*ptr1,*ptr2,*tail;
    ptr1=list1;
    ptr2=list2;
    if(list1->supplier_unique_medicine > list2->supplier_unique_medicine)
    {
        result=tail=list1;
        ptr1=ptr1->next;
    }
    else
    {
        result=tail=list2;
        ptr2=ptr2->next;
    }
    while((ptr1!=NULL)&&(ptr2!=NULL))
    {
        if(ptr1->supplier_unique_medicine > ptr2->supplier_unique_medicine)
        {
            tail->next=ptr1;
            tail=tail->next;
            ptr1=ptr1->next;
        }
        else
        {
            tail->next=ptr2;
            tail=tail->next;
            ptr2=ptr2->next;
        }
    }
    if(ptr1!=NULL)
    {
        tail->next=ptr1;
    }
    else
    {
        tail->next=ptr2;
    }
    return result;
}
supplier_node *merge_sort_by_unique_medicine(supplier_node *lptr)
{
    supplier_node *nptr;
    supplier_node *list_ptr=lptr;
    if(lptr!=NULL && lptr->next!=NULL)
    {
        nptr=divide_supplier_db(list_ptr);
        list_ptr=merge_sort_by_unique_medicine(list_ptr);
        nptr=merge_sort_by_unique_medicine(nptr);
        lptr=merge_supplier_db_unique_medicine(list_ptr,nptr);
    }
    return lptr;
}


void sales(med_node *med_head1,reorder_node *reorder_head1,int quantity,int selling_price,int medication_id,int batch)
{
    int found=0;
    
    while(!found && med_head1!=NULL)
    {
        if(med_head1->medication_id==medication_id && med_head1->batch==batch)
        {
            found=1;
        }
        else
        {
            med_head1=med_head1->next;
        }
    }
    int flag=0;
    if(found==1 && med_head1->quantity-quantity>=0)
    {
        med_head1->quantity=med_head1->quantity-quantity;
        med_head1->total_sales=med_head1->total_sales+(selling_price*quantity);
        while(reorder_head1!=NULL && !flag)
        {
            if(reorder_head1->medication_id==medication_id)
            {
                reorder_head1->total_quantity=reorder_head1->total_quantity-quantity;
                flag=1;
            }
            else
            {
                reorder_head1=reorder_head1->next;
            }
        }
    }
    else
    {
        if(found==0)
        {
            printf("FAILURE : There does not exist medicine with %d id in %d batch\n",medication_id,batch);
        }
        else if(med_head1->quantity-quantity<0)
        {
            printf("FAILURE : INSUFFICIENT STOCK CANNOT BE UPDATED\n");
        }
    }
}

void search_supplier(supplier_node *head,int supplier_id)
{
    int found=0;
    int j=0;
    while(head!=NULL && !found)
    {
        if(head->supplier_id==supplier_id)
        {
            found=1;
        }
        else
        {
            head=head->next;
        }
    }
    if(found==1)
    {
        printf("\nSupplier Name is : %s\n",head->supplier_name);
        printf("The contact number is :%d\n",head->contact_number);
        printf("The turnover of this supplier is :%d\n",head->turnover);
        printf("The number of unique medicine provided are :%d\n",head->supplier_unique_medicine);
        printf("medication ids provided by this supplier are : ");
        unique_node *ptr=(head->un)->next;
        while(ptr!=NULL)
        {   
            printf("%d  ",ptr->medication_id);
            ptr=ptr->next;
        }
        printf("\n");
    }
    else
    {
        printf("NO SUCH SUPPLIER EXISTS\n");
    }
}

void print_med_node(med_node *head)
{
    while(head!=NULL)
    {
        printf("Medicine name :%s \n",head->medicine_name);
        printf("Medicine batch number : %d \n",head->batch);
        printf("Supplier_id of supplier for this medicine :%d \n",head->supplier_id);
        printf("Quantity available :%d \n",head->quantity);
        printf("Price at which it was purchased :%d \n",head->price);
        printf("Expiry of this medication in DDMMYYYY :%d \n",head->expiry);
        printf("Medication_id :%d \n",head->medication_id);
        printf("Total sales made of this medication from this batch :%d \n",head->total_sales);
        printf("\n\n");
        head=head->next;
    }
}
void print_reorder_node(reorder_node *head)
{
    while(head!=NULL)
    {
        printf("Medicine Name: %s\n", head->medicine_name);
        printf("Medication ID: %d\n", head->medication_id);
        printf("Reorder Level: %d\n", head->reorder);
        printf("Total Quantity: %d\n", head->total_quantity);
        printf("\n\n");
        head=head->next;
    }
}
void print_supplier_node(supplier_node *head)
{
    while(head!=NULL)
    {
        printf("Supplier ID: %d\n", head->supplier_id);
        printf("Supplier Name: %s\n", head->supplier_name);
        printf("Contact Number: %d\n", head->contact_number);
        printf("Turnover: %d\n", head->turnover);
        printf("Number of Unique Medicines Supplied: %d\n", head->supplier_unique_medicine);
                        
        printf("List of Unique Medicines:");
        unique_node *ptr=(head->un)->next;
        while(ptr!=NULL)
        {   
            printf("   - Medicine ID: %d\n",ptr->medication_id);
            ptr=ptr->next;
        }
        printf("\n\n");
        head=head->next;
    }
}


void save_data(med_node *med_head,reorder_node *reorder_head,supplier_node *supplier_head)
{
    FILE *fp1;
    fp1=fopen("medicine_database.txt","w");
    if(fp1!=NULL)
    {
        while(med_head!=NULL)
        {
            fprintf(fp1,"%s %d %d %d %d %d %d %d\n",med_head->medicine_name,med_head->batch,med_head->supplier_id,med_head->quantity,med_head->price,med_head->expiry,med_head->medication_id,med_head->total_sales);
            med_head=med_head->next;
        }
    }
    else
    {
        printf("\n ERROR IN OPENING MEDICINE DATABASE\n");
    }
    fclose(fp1);

    FILE *fp2;
    fp2=fopen("reorder_database.txt","w");
    if(fp2!=NULL)
    {
        while(reorder_head!=NULL)
        {
            fprintf(fp2,"%s %d %d %d\n",reorder_head->medicine_name,reorder_head->medication_id,reorder_head->reorder,reorder_head->total_quantity);
            reorder_head=reorder_head->next;
        }
    }
    else
    {
        printf("\n ERROR IN OPENING REORDER DATABASE\n");
    }
    fclose(fp2);

    FILE *fp3;
    fp3=fopen("supplier_database.txt","w");
    if(fp3!=NULL)
    {
        while(supplier_head!=NULL)
        {
            fprintf(fp3, "%d %s %d %d %d\n",supplier_head->supplier_id,supplier_head->supplier_name,supplier_head->contact_number,supplier_head->turnover,supplier_head->supplier_unique_medicine);
            unique_node *un =supplier_head->un;
            while(un!=NULL)
            {
                fprintf(fp3,"%d ",un->medication_id);
                un=un->next;
            }
            fprintf(fp3, "\n");
            supplier_head=supplier_head->next;
        }
    }
    else
    {
        printf("\nERROR IN OPENING SUPPLIER DATABASE\n");
    }
    fclose(fp3);
}

med_node *read_medicine(med_node *med_head)
{
    FILE *fp;
    fp=fopen("medicine_database.txt","r");
    med_node *new_node,*tail;
    tail=med_head;
    char medicine_name[MEDICINE_NAME_LEN];
    int batch,supplier_id,quantity,price,expiry,medication_id,total_sales;
    if(fp!=NULL)
    {
        while(fscanf(fp,"%s %d %d %d %d %d %d %d\n",medicine_name,&batch,&supplier_id,&quantity,&price,&expiry,&medication_id,&total_sales)==8)
        {
            new_node=(med_node *)malloc(sizeof(med_node));
            strcpy(new_node->medicine_name,medicine_name);
            new_node->batch=batch;
            new_node->supplier_id=supplier_id;
            new_node->quantity=quantity;
            new_node->price=price;
            new_node->expiry=expiry;
            new_node->medication_id=medication_id;
            new_node->total_sales=total_sales;
            new_node->next=NULL;

            if(med_head==NULL)
            {
                med_head=new_node;
                tail=med_head;
            }
            else
            {
                tail->next=new_node;
                tail=tail->next;
            }
        }
    }
    else
    {
        printf("\nUNABLE TO READ MEDICINE DATABASE\n");
    }
    fclose(fp);
    return med_head;
}
reorder_node *read_reorder(reorder_node *reorder_head)
{
    FILE *fp;
    fp=fopen("reorder_database.txt","r");
    reorder_node *new_node,*tail;
    tail=reorder_head;
    char medicine_name[MEDICINE_NAME_LEN];
    int medication_id,reorder,total_quantity;
    if(fp!=NULL)
    {
        while(fscanf(fp,"%s %d %d %d\n",medicine_name,&medication_id,&reorder,&total_quantity)==4)
        {
            new_node=(reorder_node *)malloc(sizeof(reorder_node));
            strcpy(new_node->medicine_name,medicine_name);
            new_node->medication_id=medication_id;
            new_node->reorder=reorder;
            new_node->total_quantity=total_quantity;
            new_node->next=NULL;

            if(reorder_head==NULL)
            {
                reorder_head=new_node;
                tail=reorder_head;
            }
            else
            {
                tail->next=new_node;
                tail=tail->next;
            }
        }
    }
    else
    {
        printf("\nUNABLE TO READ REORDER DATABASE\n");
    }
    fclose(fp);
    return reorder_head;
}
supplier_node *read_supplier(supplier_node *supplier_head)
{
    FILE *fp;
    fp=fopen("supplier_database.txt","r");
    supplier_node *new_node,*tail;
    tail=supplier_head;
    int supplier_id,contact_number,turnover,supplier_unique_medicine,medication_id;
    char supplier_name[SUPPLIER_NAME_LEN];
    if(fp!=NULL)
    {
        while(fscanf(fp,"%d %s %d %d %d\n",&supplier_id,supplier_name,&contact_number,&turnover,&supplier_unique_medicine)==5)
        {
            new_node=(supplier_node*)malloc(sizeof(supplier_node));
            strcpy(new_node->supplier_name,supplier_name);
            new_node->supplier_id=supplier_id;
            new_node->contact_number=contact_number;
            new_node->turnover=turnover;
            new_node->supplier_unique_medicine=supplier_unique_medicine;
            new_node->un=NULL;
            int count=0;
            unique_node *uptr,*tptr;
            while(count<=supplier_unique_medicine)
            {
                uptr=(unique_node*)malloc(sizeof(unique_node));
                fscanf(fp,"%d ",&medication_id);
                uptr->medication_id=medication_id;
                uptr->next=NULL;
                if(new_node->un==NULL)
                {
                    new_node->un=uptr;
                    tptr=new_node->un;
                }
                else
                {
                    tptr->next=uptr;
                    tptr=tptr->next;
                }
                count++;
            }
            new_node->next=NULL;

            if(supplier_head==NULL)
            {
                supplier_head=new_node;
                tail=supplier_head;
            }
            else
            {
                tail->next=new_node;
                tail=tail->next;
            }
        }
    }
    else
    {
        printf("\nUNABLE TO READ SUPPLIER DATABASE\n");
    }
    fclose(fp);
    return supplier_head;
}

int main()
{
    med_node *med_head=NULL;
    reorder_node *reorder_head=NULL;
    supplier_node *supplier_head=NULL;
    /*read initial data from file*/
    med_head=read_medicine(med_head);
    reorder_head=read_reorder(reorder_head);
    supplier_head=read_supplier(supplier_head);

    int i,n;
    char medicine_name[MEDICINE_NAME_LEN];
    int batch,supplier_id,quantity,price,expiry,medication_id,reorder;                                         
    char supplier_name[SUPPLIER_NAME_LEN];
    int contact_number,total_sales,turnover,supplier_unique_medicine,selling_price;
    int checker;
    int choice=1;

    /*temproary variables*/
    supplier_node *temp=supplier_head;
    int count=0;

    while(choice!=0)
    {
        printf("\n--------------------------------------------------\n");
        printf("\nPharmacy Inventory Management\n");
        printf("1. Add New Medication\n");
        printf("2. Update Medication Details\n");
        printf("3. Delete Medication\n");
        printf("4. Search Medication\n");
        printf("5. Check Expiration Dates\n");
        printf("6. Sort According to Expiry\n");
        printf("7. Sales Tracking(To sell medicines)\n");
        printf("8. Supplier Management\n");
        printf("9. All Rounder Suppliers\n");
        printf("10. Suppliers with largest turnover\n");
        printf("11. Print Medication DB\n");
        printf("12. Print Reorder DB\n");
        printf("13. Print Supplier DB\n");
        printf("0. END OPERATIONS\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("\n--------------------------------------------------\n");

        switch(choice)
        {
            case 1:
                //to add new medication will also ask from which supplier they took medicine and reorder level 
                printf("How many medicine will you like to add in Database :");
                scanf("%d",&n);
                for(i=0;i<n;i++)
                {
                    printf("\nEnter medicine name : ");
                    scanf("%s",medicine_name);
                    printf("Enter the medication id of given medicine : ");
                    scanf("%d",&medication_id);
                    printf("Enter the batch of medicine : ");
                    scanf("%d",&batch);
                    printf("Enter Quantity of given medicine : ");
                    scanf("%d",&quantity);
                    printf("Enter price at which medicine was purchased from given supplier: ");
                    scanf("%d",&price);
                    printf("Enter the expiry date of medicine in DDMMYYYY: ");
                    scanf("%d",&expiry);
                    printf("Enter Supplier id of Supplier : ");
                    scanf("%d",&supplier_id);
                    if(is_aldready_in_reorder_database(reorder_head,medication_id)==0)
                    {   
                        reorder=0;
                        while(!(reorder<100 && reorder>10))
                        {
                            printf("Enter The Reorder level for this medicine ranging from 10 to 100 only: ");
                            scanf("%d",&reorder);
                        }
                        insert_update_reorder_db(&reorder_head,medication_id,reorder,medicine_name);
                    }
                    if(is_aldready_in_supplier_db(supplier_head,supplier_id)==0)
                    {
                        printf("Enter the name of Supplier : ");
                        scanf("%s",supplier_name);
                        printf("Enter the contact information of Supplier: ");
                        scanf("%d",&contact_number);
                        insert_update_supplier_db(&supplier_head,supplier_id,contact_number,supplier_name);
                    }
                    insert_update_medication_db(&med_head,medicine_name,batch,supplier_id,quantity,price,expiry,medication_id);
                    calculate_turnover(supplier_head,price,quantity,supplier_id);
                    calculate_unique_medicine(supplier_head,supplier_id,medication_id);
                    update_total_quantity(reorder_head,medication_id,quantity);
                    printf("\n");
                    stock_alert(reorder_head,medication_id);
                }
                break;
            case 2:
                printf("Enter Medication ID and Batch number of Medicine to be updated\n");
                printf("Enter the Medication ID of given Medicine : ");
                scanf("%d",&medication_id);
                printf("Enter the Batch of Medicine : ");
                scanf("%d",&batch);
                printf("Enter the New Quantity of this medicine : ");
                scanf("%d",&quantity);                                         //this updation doesnt change total quantity
                if(check_to_update(med_head,medication_id,batch)==1)
                {
                    if(insert_update_medication_db(&med_head,medicine_name,batch,supplier_id,quantity,price,expiry,medication_id))
                    {
                        printf("Updated Successfully");
                    }
                    else
                    {
                        printf("Unable to Update Please Try again");
                    }
                }
                else
                {
                    printf("No such Medicine present in Database");
                }
                break;
            case 3:
                printf("Enter Medication ID and Batch number of Medicine to be Deleted\n");
                printf("Enter the Medication ID of Given Medicine : ");
                scanf("%d",&medication_id);
                printf("Enter the batch of Medicine : ");
                scanf("%d",&batch);
                checker=delete_medication_db(&med_head,batch,medication_id);
                    if(checker==0)
                    {
                        printf("NO SUCH MEDICATION EXIST IN GIVEN BATCH\n");
                    }
                    else
                    {
                        printf("DELETED SUCCESFULLY\n");
                        update_total_quantity(reorder_head,medication_id,(-1*checker));
                    }
                    stock_alert(reorder_head,medication_id);
                break;
            case 4:
                printf("Enter 1 for search by Medicine name\nEnter 2 for search by Supplier ID\n");
                scanf("%d",&checker);
                if(checker==1)
                {
                    printf("Enter Medicine name :");
                    scanf("%s",medicine_name);
                    search_medication_by_name(med_head,medicine_name);
                }
                else if(checker==2)
                {
                    printf("Enter Supplier ID :");
                    scanf("%d",&supplier_id);
                    search_medication_by_supplier_id(med_head,supplier_id);
                }
                else
                {
                    printf("\nINVALID INPUT");
                }
                break;
            case 5://it prints all those medicine which will expire in next month
                printf("The medicines which will expire within next month are :");
                check_expiration_dates(med_head);
                break;
            case 6:
                med_head=merge_sort_by_expiry(med_head);
                break;
            case 7:
                printf("\nEnter the medication id of medicine sold : ");
                scanf("%d",&medication_id);
                printf("Enter the batch from which this medicine was sold : ");
                scanf("%d",&batch);
                printf("Enter Quantity of sold medicine : ");
                scanf("%d",&quantity);
                printf("Enter price at which medicine was sold: ");
                scanf("%d",&selling_price);
                sales(med_head,reorder_head,quantity,selling_price,medication_id,batch);
                stock_alert(reorder_head,medication_id);
                break;
            case 8:
                printf("\nEnter 1 to add/update new supplier\nEnter 2 to search for supplier : \n");
                scanf("%d",&checker);
                if(checker==1)
                {
                    printf("Enter the Name of this Supplier : ");
                    scanf("%s",supplier_name);
                    printf("Enter the Contact Information : ");
                    scanf("%d",&contact_number);
                    printf("Enter Supplier ID : ");
                    scanf("%d",&supplier_id);
                    checker=insert_update_supplier_db(&supplier_head,supplier_id,contact_number,supplier_name);
                    if(checker==0)
                    {
                        printf("\nFAILURE : NO FREE SPACE\n");
                    }
                }
                else if(checker==2)
                {
                    printf("Enter Supplier id of Supplier : ");
                    scanf("%d",&supplier_id);
                    search_supplier(supplier_head,supplier_id);
                }
                break;
            
            case 9:
                supplier_head=merge_sort_by_unique_medicine(supplier_head);
                
                printf("\nThe best ALL ROUNDER supplier is :\n");
                printf("Supplier ID: %d\n",supplier_head->supplier_id);
                printf("Supplier Name: %s\n",supplier_head->supplier_name);
                printf("Contact Number: %d\n",supplier_head->contact_number);
                printf("Number of Unique Medicines Supplied: %d\n",supplier_head->supplier_unique_medicine);
                printf("\n");
                printf("Top 10 ALL ROUNDER suppliers are \n");
                temp=supplier_head;
                count=0;
                while(temp!=NULL && count<10)
                {
                    printf("Supplier ID: %d\n",temp->supplier_id);
                    printf("Supplier Name: %s\n",temp->supplier_name);
                    printf("Contact Number: %d\n",temp->contact_number);
                    printf("Number of Unique Medicines Supplied: %d\n",temp->supplier_unique_medicine);
                    printf("\n");
                    count++;
                    temp=temp->next;
                }
                break;
            case 10:
                supplier_head=merge_sort_by_turnover(supplier_head);
                
                printf("\nThe supplier with largest turnover is :\n");
                printf("Supplier ID: %d\n",supplier_head->supplier_id);
                printf("Supplier Name: %s\n",supplier_head->supplier_name);
                printf("Contact Number: %d\n",supplier_head->contact_number);
                printf("Turnover: %d\n",supplier_head->turnover);
                printf("\n");
                printf("Top 10 suppliers with largest turnover are : \n");
                temp=supplier_head;
                count=0;
                while(temp!=NULL && count<10)
                {
                    printf("Supplier ID: %d\n",temp->supplier_id);
                    printf("Supplier Name: %s\n",temp->supplier_name);
                    printf("Contact Number: %d\n",temp->contact_number);
                    printf("Turnover: %d\n",temp->turnover);
                    printf("\n");
                    count++;
                    temp=temp->next;
                }
                break;
            case 11:
                printf("\n");
                print_med_node(med_head);
                break;
            case 12:
                printf("\n");
                print_reorder_node(reorder_head);
                break;
            case 13:
                printf("\n");
                print_supplier_node(supplier_head);
                break;
            case 0:
                save_data(med_head,reorder_head,supplier_head);
                med_node *free_m;
                while(med_head!=NULL)
                {
                    free_m=med_head;
                    med_head=med_head->next;
                    free(free_m);
                }
                reorder_node *free_r;
                while(reorder_head!=NULL)
                {
                    free_r=reorder_head;
                    reorder_head=reorder_head->next;
                    free(free_r);
                }
                supplier_node *free_s;
                unique_node *free_u;
                while(supplier_head!=NULL)
                {
                    free_s=supplier_head;
                    supplier_head=supplier_head->next;
                    while(free_s->un!=NULL)
                    {
                        free_u=free_s->un;
                        free_s->un=free_s->un->next;
                        free(free_u);
                    }
                    free(free_s);
                }
                printf("Saving Data .....\n");
                printf("Saved Succesfully\n");
                printf("ThankYou\n");
                break;
            default :
                printf("Choose valid option\n");
                break;
        }
    }
}