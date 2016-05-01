
	long list_size;

	typedef enum { false, true } bool;

	typedef struct Client {
		long id;
		long current_usage;
		bool above;
		int min;
		int max;
	};

	typedef struct Node{
    	Client c;
    	Node next;
    };

	Node *head;
	head = malloc(sizeof(Node));

	/**
	 *
	 */
	Client createClient(long ID, long curr, int MIN, int MAX){
		Client *c1 = malloc(sizeof(Client));
		c1->id = ID;
		c1->current_usage = curr;
		c1->min = MIN;
		c1->max = MAX;
		c1->above = false;
		return c1;
	}

	/**
	 * Given a linked list head pointer, compute
	 * and return the number of nodes in the list.
	 */
	int length(Node* head) {
		Node* current = head;
		int count = 0;
		while (current != NULL) {
			count++;
			current = current->next;
		}
		return count;
	}


	/**
	 * Adds a Client to the front of the list.
	 */
	void push(Node **head, Client c1){
		Node *newNode = malloc(sizeof(Node)); // create new Node.
		newNode->c = c1; // Set its data.
		newNode->next = *head; // Make it point to head, so it is now first.
		*head = newNode; // Make head first again.
	}


	/**
	 * Adds a Client to the end of a list.
	 */
	bool add(Client c){

	}

	/**
	 * Removes a client from the list at index i.
	 */
	bool remove(int i){

	}

	/**
	 * Inserts a Client into a list at index i.
	 */
	bool insert(Client c, int i){

	}

	/**
	 * Return a pointer to the client with "id."
	 */
	Client getClient(char id){

	}
