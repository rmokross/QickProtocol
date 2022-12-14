
CREATE TABLE IF NOT EXISTS 
customer(
	customerID INT NOT NULL PRIMARY KEY, 
	name varchar(100) NOT NULL, 
	status BOOL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS 
project(
	projectID NOT NULL PRIMARY KEY, 
	customerID INT, 
	name varchar(100) NOT NULL, 
	description varchar(2000),
	start_date datetime default current_timestamp, 
	end_date datetime, 
	status BOOL DEFAULT 1, 
	protocol_dir varchar(200), 
	FOREIGN KEY (customerID) REFERENCES customer (customerID)
);

CREATE TABLE IF NOT EXISTS 
protocol(
	protocolID INT NOT NULL PRIMARY KEY, 
	protocol_type varchar(100) NOT NULL, 
	create_dt datetime default current_timestamp, 
	title varchar(100), summary varchar(10000), 
	projectID INT, 
	cratch_pad varchar(15000), 
	FOREIGN KEY (projectID) REFERENCES project (projectID)
);

CREATE TABLE IF NOT EXISTS 
todo(
	todoID INT NOT NULL PRIMARY KEY, 
	protocolID INT NOT NULL, 
	status BOOL DEFAULT 1, 
	name varchar(100), 
	FOREIGN KEY (protocolID) REFERENCES protocol (protocolID)
);