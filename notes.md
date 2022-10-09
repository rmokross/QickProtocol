#### Project planning

QickProtocol is a tool which opens up a protocol template after providing small input from the user regarding the protocol type, customer, project and title. The tool will collect the information (ToDos, ideas and summary) and create a overall Todo List.


#### *1. Entity diagramm:*

#### 1.1 Entities

- Customer:
	* str	: custID (PK)
	* str	: name
	* bool	: active

- Project
	* str	: projectID	(PK)
	* str	: custID	(FK)
	* str	: description
	* str	: name
	* str	: todoID	(FK)
	* str	: ideaID	(FK)
	* date	: start_date
	* date	: end_date_planned
	* date	: end_date
	* str	: status
	* str	: protocol_pdf_path
 
- protocol
	* str	: protocolID	(PK)
	* str	: protocol_type
	* date	: date
	* str	: appointmentID	(FK)
	* str	: ideaID	(FK)
	* str	: title

- todos
	* str	: todosID	(PK)
	* str	: protocollID 	(FK)
	* bool	: done
	* str	: text

- idea
	* str	: ideaID	(PK)
	* str	: protocolID	(FK)
	* str	: text

- summary
	* str	: summaryID	(PK)
	* str	: protocolID	(FK)
	* str	: text


