-- Contact Info
--
select 
	M.memberID,
	REPLACE(CONCAT(M.firstName,  ' ', IFNULL(M.midName,''),  ' ', M.lastName),'  ',' ') as 'FullName',
	M.sex,  
	CT.contactType,
	C.memberContact,
	ST.statusType
from 
cabrew.members M
	inner join cabrew.memberContacts C on M.memberID = C.memberID_fk
	inner join cabrew.statusTypes ST on M.statusTypeID_fk = ST.statusTypeID
	inner join cabrew.contactTypes CT on C.contactTypeID_fk = CT.contactTypeID
where 
	M.firstName = 'Brad' or M.lastName = 'Waterson' or lastName = 'Burrage' or firstName = 'Josh'
order by 
	M.lastName, 
	M.firstName
;

-- Date Info
-- 
select 
	D.memberDateID,
	REPLACE(CONCAT(M.firstName,  ' ', IFNULL(M.midName,''),  ' ', M.lastName),'  ',' ') as 'FullName',
	NT.noteType,
	D.memberDate, 
	D.dateDetail
from 
cabrew.members M
	inner join cabrew.memberDates D on M.memberID = D.memberID_fk
	inner join cabrew.dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
where 
	M.firstName = 'Brad' or M.lastName = 'Waterson' or lastName = 'Burrage' or firstName = 'Josh'
order by 
	M.lastName, 
	M.firstName,
	D.memberDate
;

-- Note Info
-- 
select 
	N.memberNoteID,
	REPLACE(CONCAT(M.firstName,  ' ', IFNULL(M.midName,''),  ' ', M.lastName),'  ',' ') as 'FullName',
	NT.noteType,
	N.memberNote
from 
cabrew.members M
	inner join cabrew.memberNotes N on M.memberID = N.memberID_fk
	inner join cabrew.noteTypes NT on N.noteTypeID_fk = NT.noteTypeID
order by 
	M.lastName, 
	M.firstName,
	NT.noteType
;


-- Needs to Pay
-- 
select 
	REPLACE(CONCAT(M.firstName,  ' ', IFNULL(M.midName,''),  ' ', M.lastName),'  ',' ') as 'FullName',
	C.memberContact,
	max(D.memberDate) as 'Last Payment',
	floor(datediff(now(), max(D.memberDate))/30.4) as 'MonthsSincePayment'
from 
cabrew.members M
	inner join cabrew.memberDates D on M.memberID = D.memberID_fk
	inner join cabrew.dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
	inner join cabrew.memberContacts C on M.memberID = C.memberID_fk
	inner join cabrew.contactTypes CT on C.contactTypeID_fk = CT.contactTypeID 
where 
	DT.dateTypeID = 3
	and CT.contactTypeID = 2
	and M.statusTypeID_fk = 1
group by 
	REPLACE(CONCAT(M.firstName,  ' ', IFNULL(M.midName,''),  ' ', M.lastName),'  ',' '),
	C.memberContact
having 
	datediff(now(), max(D.memberDate))/30 > 12
order by 
	datediff(now(), max(D.memberDate)),
	M.lastName, 
	M.firstName,
	D.memberDate
;




-- Ad Hoc
-- 
select * from cabrew.statusTypes;
select * from cabrew.dateTypes;
select * from cabrew.noteTypes;
select * from cabrew.memberDates;
select * from cabrew.members order by lastName;

--INSERT INTO cabrew.memberDates (memberDateID, memberID_fk, dateTypeID_fk, memberDate, lastChangeDate) VALUES (145,27,3,'2014-11-13',now());



-- New Data
-- 
INSERT INTO cabrew.statusTypes(statusTypeID, statusType) VALUES (1, 'Active');
INSERT INTO cabrew.statusTypes(statusTypeID, statusType) VALUES (2, 'Inactive');
INSERT INTO cabrew.statusTypes(statusTypeID, statusType) VALUES (3, 'Guest');
INSERT INTO cabrew.statusTypes(statusTypeID, statusType) VALUES (4, 'Archive');
INSERT INTO cabrew.statusTypes(statusTypeID, statusType) VALUES (5, 'Comp');
select * from cabrew.statusTypes;

INSERT INTO cabrew.contactTypes(contactTypeID,contactType) VALUES(1,'phone');
INSERT INTO cabrew.contactTypes(contactTypeID,contactType) VALUES(2,'email');
select * from cabrew.contactTypes;

INSERT INTO cabrew.dateTypes (dateTypeID, dateType) VALUES(1, 'Join Date');
INSERT INTO cabrew.dateTypes (dateTypeID, dateType) VALUES(2, 'Birth Date');
INSERT INTO cabrew.dateTypes (dateTypeID, dateType) VALUES(3, 'Dues Paid');
select * from cabrew.dateTypes;

INSERT INTO cabrew.noteTypes (noteTypeID, noteType) VALUES(1,'Default');
select * from cabrew.noteTypes;

select * from cabrew.members;

INSERT INTO cabrew.members(memberID,statusTypeID_fk,lastName,firstName,sex,lastChangeDate) VALUES 
 (1,1,'Agee','Rusty','M',now()),
 (2,1,'Allgaier','Eric','M',now()),
 (3,1,'Alward','Hazen','M',now()),
 (4,1,'Alward','Josh','M',now()),
 (5,1,'Balderson','Ritchie','M',now()),
 (6,1,'Barnes','Brian','M',now()),
 (7,1,'Bean','Dennis','M',now()),
 (8,1,'Berube','Sebastian','M',now()),
 (9,1,'Blackman','Trent','M',now()),
 (10,1,'Boor','Luke','M',now()),
 (11,1,'Bowling','Dean','M',now()),
 (12,1,'Brandon','Michael','M',now()),
 (13,1,'Burrage','Robert','M',now()),
 (14,1,'Butler','Mike','M',now()),
 (15,1,'Clark','Chip','M',now()),
 (16,1,'Coffey','Matt ','M',now()),
 (17,1,'Collinson','Roger','M',now()),
 (18,1,'Conley','Richard','M',now()),
 (19,1,'Connor','Kevin','M',now()),
 (20,1,'Cook','Palmer','M',now()),
 (21,1,'Coroi','Dana','M',now()),
 (22,1,'Cottingham','Robin','M',now()),
 (23,1,'Craven','Ford','M',now()),
 (24,1,'Crinch','John','M',now()),
 (25,1,'Crump','Brandon','M',now()),
 (26,1,'Davidshofer','Don','M',now()),
 (27,1,'Davis','Josh','M',now()),
 (28,1,'Dempsey','Chad ','M',now()),
 (29,1,'Doherty','Shannon','M',now()),
 (30,1,'Dowling','Drew','M',now()),
 (31,1,'Earnhardt','Sharon','M',now()),
 (32,1,'Eaton','Darryl','M',now()),
 (33,1,'Eldreth','Norman','M',now()),
 (34,1,'Eury','John','M',now()),
 (35,1,'Faust','Ike','M',now()),
 (36,1,'Fisner','Bryan','M',now()),
 (37,1,'Ford','Suzie','M',now()),
 (38,1,'Ford','Todd','M',now()),
 (39,1,'Friedman','Todd','M',now()),
 (40,1,'Gainous','Crystal','M',now()),
 (41,1,'Garvin','Jason','M',now()),
 (42,1,'Godby','Rick','M',now()),
 (43,1,'Graham','Mark','M',now()),
 (44,1,'Harlter','Mark','M',now()),
 (45,1,'Hartwick','Kevin','M',now()),
 (46,1,'Hartwick','Nye','M',now()),
 (47,1,'Haught','Dan','M',now()),
 (48,1,'Henderson','Chadwick','M',now()),
 (49,1,'Higgins','Chris','M',now()),
 (50,1,'Hughes','Brad','M',now()),
 (51,1,'Indicott','Jennifer','M',now()),
 (52,1,'Jackson','Marty','M',now()),
 (53,1,'Jen','Patty','M',now()),
 (54,1,'Johnson','Gary','M',now()),
 (55,1,'Kelley','Justin','M',now()),
 (56,1,'Kelley','Ryan','M',now()),
 (57,1,'Kelley','Tim','M',now()),
 (58,1,'Kingery','Christian','M',now()),
 (59,1,'Kirkman','Larry','M',now()),
 (60,1,'Kolb','Drew','M',now()),
 (61,1,'Ledbetter','Brad','M',now()),
 (62,1,'Ledbetter','Matthew','M',now()),
 (63,1,'Lewis','Adam','M',now()),
 (64,1,'Lewis','Dick','M',now()),
 (65,1,'Love','Neil','M',now()),
 (66,1,'Lundy','John','M',now()),
 (67,1,'Manning','Nancy','M',now()),
 (68,1,'Martinez','Joe','M',now()),
 (69,1,'McCray','Kerry','M',now()),
 (70,1,'McRorie','Heather','M',now()),
 (71,1,'Mitchell','Thad','M',now()),
 (72,1,'Monte','Matthew','M',now()),
 (73,1,'Moore','Justin','M',now()),
 (74,1,'Muenster','Rudi','M',now()),
 (75,1,'Murray','Thomas','M',now()),
 (76,1,'Nuttall','Ron','M',now()),
 (77,1,'Oliver','John','M',now()),
 (78,1,'Omspach','Will','M',now()),
 (79,1,'Osborne','Dean','M',now()),
 (80,1,'Overcash','Clyde','M',now()),
 (81,1,'Paolino','Bob','M',now()),
 (82,1,'Paskiewicz','Joe','M',now()),
 (83,1,'Peterlin','Scott','M',now()),
 (84,1,'Piercy','Craig','M',now()),
 (85,1,'Pope','Robin','M',now()),
 (86,1,'Propst','Steve','M',now()),
 (87,1,'Propst','Tim','M',now()),
 (88,1,'Putnam','Jonathan','M',now()),
 (89,1,'Putney','James','M',now()),
 (90,1,'Radford','Jay','M',now()),
 (91,1,'Readling','Wallace','M',now()),
 (92,1,'Reed','Karley','M',now()),
 (93,1,'Roberts','Bart','M',now()),
 (94,1,'Ronemus','Hoge','M',now()),
 (95,1,'Sain','Gary','M',now()),
 (96,1,'Sanborn','Tony','M',now()),
 (97,1,'Schmiedeshoff','Elaine','M',now()),
 (98,1,'Schmiedeshoff','Roy','M',now()),
 (99,1,'Schonder','Brian','M',now()),
 (100,1,'Scott','Nancy','M',now()),
 (101,1,'Seng','Dale','M',now()),
 (102,1,'Sewell','Jim','M',now()),
 (103,1,'Shepard','Derek','M',now()),
 (104,1,'Shepard','Mari','M',now()),
 (105,1,'Sherrill','Lee','M',now()),
 (106,1,'Sinclair','John','M',now()),
 (107,1,'Skinner','Tom','M',now()),
 (108,1,'Small','Jeff','M',now()),
 (109,1,'Small','Katie','M',now()),
 (110,1,'Smith','Baydon','M',now()),
 (111,1,'Smith','Josh','M',now()),
 (112,1,'Smith','Suzanne','M',now()),
 (113,1,'Smoak','Leigh','M',now()),
 (114,1,'Smoak','Maggie','M',now()),
 (115,1,'Smoak','Michael','M',now()),
 (116,1,'Starr','Kevin','M',now()),
 (117,1,'Thomas','Michael','M',now()),
 (118,1,'Troutman','Eric','M',now()),
 (119,1,'Tucker','Art','M',now()),
 (120,1,'Tucker','Rebecca','M',now()),
 (121,1,'Uken','Don','M',now()),
 (122,1,'Upchurch','Andy','M',now()),
 (123,1,'Vanderveen','Arvind','M',now()),
 (124,1,'Vandorn','Chris','M',now()),
 (125,1,'Vesel','Mark','M',now()),
 (126,1,'Vils','Jared','M',now()),
 (127,1,'Wallace','Cricket','M',now()),
 (128,1,'Wallace','Scott','M',now()),
 (129,1,'Waterson','Jennifer','M',now()),
 (130,1,'Waterson','Luke','M',now()),
 (131,1,'Whalen','Trenton','M',now()),
 (132,1,'Wichnoski','Brunno','M',now()),
 (133,1,'Williamson','Murray','M',now()),
 (134,1,'unk','Phillip','M',now())
;
select * from cabrew.memberContacts;


INSERT INTO cabrew.memberContacts
(
memberContactID, memberID_fk, contactTypeID_fk, memberContact, lastChangeDate) VALUES
 (1,10,2,'84.coolhand.luke@gmail.com',now()), 
 (2,119,2,'adtuckerii@gmail.com',now()), 
 (3,107,2,'agrimefighter@yahoo.com',now()), 
 (4,128,2,'alelover@gmail.com',now()), 
 (5,122,2,'andyupchurch1@gmail.com',now()), 
 (6,110,2,'baydon@carolina.rr.com',now()), 
 (7,16,2,'beaconhillsbrewhouse@gmail.com',now()), 
 (8,8,2,'berube.sebastian@gmail.com',now()), 
 (9,50,2,'braddoro@gmail.com',now()), 
 (10,61,2,'brad.ledbetter@gmail.com',now()), 
 (11,60,2,'brewmasterdrew@gmail.com',now()), 
 (12,31,2,'captainkellia@gmail.com',now()), 
 (13,121,2,'carolinabrewsupply@gmail.com',now()), 
 (14,15,2,'chip60clark@gmail.com',now()), 
 (15,58,2,'ckingery1@gmail.com',now()), 
 (16,127,2,'cricketsthreads@gmail.com',now()), 
 (17,24,2,'crnich@hotmail.com',now()), 
 (18,25,2,'crumptowers@yaoo.com',now()), 
 (19,40,2,'crystal.gainous@gmail.com',now()), 
 (20,9,2,'ctblackman@vnet.com',now()), 
 (21,124,2,'c.vandom@yahoo.com',now()), 
 (22,101,2,'dale@sengsational.com',now()), 
 (23,32,2,'darryl8n@gmail.com',now()), 
 (24,11,2,'dean@poolsbyaloha.com',now()), 
 (25,7,2,'dennis.bean24@gmail.com',now()), 
 (26,103,2,'derekshepard@ctc.net',now()), 
 (27,30,2,'drewhead@drewhead.org',now()), 
 (28,2,2,'eric_allgaier@hotmail.com',now()), 
 (29,97,2,'E-trader@carolina.rr.com',now()), 
 (30,98,2,'E-trader@carolina.rr.com',now()), 
 (31,51,2,'fercott@hotmail.com',now()), 
 (32,75,2,'fxtman@gmail.com',now()), 
 (33,95,2,'garysai@windstream.net',now()), 
 (34,116,2,'graphicsbystarr@yahoo.com',now()), 
 (35,43,2,'gts_mark@att.net',now()), 
 (36,3,2,'hazen@alwardmasonry.com',now()), 
 (37,94,2,'hronemus@bellsbeer.com',now()), 
 (38,52,2,'jackson.marty42@gmail.com',now()), 
 (39,108,2,'jeffrey.smalljr@gmail.com',now()), 
 (40,102,2,'jim.sewell@paktech-opi.com',now()), 
 (41,34,2,'johneury@yahoo.com',now()), 
 (42,4,2,'joosgalward@aim.com',now()), 
 (43,27,2,'joshie.squashy@gmail.com',now()), 
 (44,89,2,'jputney@vnet.net',now()), 
 (45,106,2,'jsin62@mi-connection.com',now()), 
 (46,77,2,'jtollie@hotmail.com',now()), 
 (47,73,2,'justin@sublmnldesign.com',now()), 
 (48,126,2,'j_vils@yahoo.com',now()), 
 (49,92,2,'karley.reed@gmail.com',now()), 
 (50,109,2,'katiesmall11@gmail.com',now()), 
 (51,57,2,'kelley25@bellsouth.net',now()), 
 (52,69,2,'kerry.mccray@yahoo.com',now()), 
 (53,134,2,'kingphillip25@gmail.com',now()), 
 (54,105,2,'leesherrill6@gmail.com',now()), 
 (55,113,2,'leighsmoak@gmail.com',now()), 
 (56,26,2,'lupulin_1@yahoo.com',now()), 
 (57,66,2,'mail.john.lundy@gmail.com',now()), 
 (58,114,2,'margaretsmoak@gmail.com',now()), 
 (59,104,2,'marishepard@ctc.net',now()), 
 (60,44,2,'mark.harlter@gmail.com',now()), 
 (61,72,2,'matthewmonte@gmail.com',now()), 
 (62,12,2,'mbrandon@windstream.net',now()), 
 (63,133,2,'mbwilliamson@carolina.rr.com',now()), 
 (64,115,2,'michael.smoak@gmail.com',now()), 
 (65,85,2,'moroja71@gmail.com',now()), 
 (66,62,2,'m.r.ledbetter@gmail.com',now()), 
 (67,68,2,'mtzajej@gmail.com',now()), 
 (68,100,2,'nancy@twobeernutz.com',now()), 
 (69,65,2,'neil.love88@yahoo.com',now()), 
 (70,67,2,'ntmanning@windstream.net',now()), 
 (71,123,2,'overthanearv@yahoo.com',now()), 
 (72,53,2,'patty.jen@hotmail.com',now()), 
 (73,17,2,'r2collinson@hotmail.com',now()), 
 (74,18,2,'r.conley@windstream.com',now()), 
 (75,79,2,'rdo4info@gmail.com',now()), 
 (76,120,2,'rebeccawtucker@gmail.com',now()), 
 (77,13,2,'reburrage@carolina.rr.com',now()), 
 (78,42,2,'rgodby@gmail.com',now()), 
 (79,93,2,'robertsgeorgeb@gmail.com',now()), 
 (80,22,2,'robincottingham@bellsouth.net',now()), 
 (81,74,2,'rudi00@yahoo.com',now()), 
 (82,1,2,'rusty@twobeernutz.com',now()), 
 (83,59,2,'smashkirkman@gmail.com',now()), 
 (84,111,2,'Smithjm8@appstate.edu',now()), 
 (85,112,2,'snordansmith@yahoo.com',now()), 
 (86,23,2,'spittinrivers@gmail.com',now()), 
 (87,86,2,'stevep2@vnet.net',now()), 
 (88,71,2,'teehad78@gmail.com',now()), 
 (89,39,2,'tfriedman@tiaa-cref.org',now()), 
 (90,131,2,'tkwhalen@aol.com',now()), 
 (91,96,2,'tony210@ctc.net',now()), 
 (92,83,2,'vdub4life@ymail.com',now()), 
 (93,125,2,'Veselmark@gmail.com',now()), 
 (94,19,2,'war_potato@hotmail.com',now()), 
 (95,129,2,'waterson.jenn@gmail.com',now()), 
 (96,130,2,'waterson.luke@gmail.com',now()), 
 (97,91,2,'wreading@ups.com',now()); 

INSERT INTO cabrew.memberContacts
(
memberContactID, memberID_fk, contactTypeID_fk, memberContact, lastChangeDate) VALUES
 (99,40,1,'229.221.6191',now()), 
 (100,10,1,'254.291.3159',now()), 
 (101,2,1,'267.987.7034',now()), 
 (102,76,1,'309.531.4559',now()), 
 (103,125,1,'330-421-7319',now()), 
 (104,102,1,'541.510.6306',now()), 
 (105,126,1,'561-758-2320',now()), 
 (106,62,1,'704-209-9909',now()), 
 (107,31,1,'704-213-1425',now()), 
 (108,83,1,'704.213.8468',now()), 
 (109,24,1,'704.231.2336',now()), 
 (110,53,1,'704-241-1421',now()), 
 (111,100,1,'704.281.3452',now()), 
 (112,101,1,'704.287.9661',now()), 
 (113,71,1,'704.361.5452',now()), 
 (114,19,1,'704.400.5307',now()), 
 (115,72,1,'704-408-4103',now()), 
 (116,110,1,'704.425.2579',now()), 
 (117,66,1,'704.454.3263',now()), 
 (118,1,1,'704.455.6132',now()), 
 (119,77,1,'704-467-1063',now()), 
 (120,12,1,'704.467.3053',now()), 
 (121,133,1,'704-467-5569',now()), 
 (122,57,1,'704.469.7529',now()), 
 (123,85,1,'704-490-6048',now()), 
 (124,113,1,'704.560.8046',now()), 
 (125,115,1,'704.560.8046',now()), 
 (126,74,1,'704-562-4351',now()), 
 (127,114,1,'704.572.0014',now()), 
 (128,130,1,'704.575.8482',now()), 
 (129,129,1,'704.575.9829',now()), 
 (130,121,1,'704.576.2895',now()), 
 (131,127,1,'704.582.9494',now()), 
 (132,22,1,'704-619-8284',now()), 
 (133,42,1,'704-633-2888',now()), 
 (134,131,1,'704-641-7770',now()), 
 (135,119,1,'704-642-7797',now()), 
 (136,23,1,'704.652.2422',now()), 
 (137,16,1,'704.668.6274',now()), 
 (138,34,1,'704-699-6306',now()), 
 (139,123,1,'704-699-9679',now()), 
 (140,105,1,'704.701.4179',now()), 
 (141,52,1,'704-701-7192',now()), 
 (142,7,1,'704.707.5394',now()), 
 (143,109,1,'704.743.8577',now()), 
 (144,59,1,'704.785.4205',now()), 
 (145,15,1,'704.791.2511',now()), 
 (146,86,1,'704.791.5657',now()), 
 (147,112,1,'704.793.3917',now()), 
 (148,103,1,'704.794.6293',now()), 
 (149,104,1,'704.794.6293',now()), 
 (150,79,1,'704-796-2021',now()), 
 (151,70,1,'704.796.3326',now()), 
 (152,9,1,'704-7967012',now()), 
 (153,73,1,'704.807.2389',now()), 
 (154,67,1,'704-839-1805',now()), 
 (155,75,1,'704-840-6033',now()), 
 (156,99,1,'704.904.9210',now()), 
 (157,43,1,'704.905.1681',now()), 
 (158,128,1,'704.941.8586',now()), 
 (159,68,1,'704.989.4378',now()), 
 (160,91,1,'704.996.9931',now()), 
 (161,61,1,'801-686-8643',now()), 
 (162,65,1,'828-773-7961',now()), 
 (163,51,1,'910-231-0139',now()), 
 (164,27,1,'919.271.9384',now()), 
 (165,93,1,'919.412.8921',now()), 
 (166,8,1,'980-226-7906',now()), 
 (167,50,1,'980.521.0162',now()), 
 (168,116,1,'980-621-3858',now()), 
 (169,95,1,'980-622-0520',now()), 
 (170,108,1,'980.622.1438',now()), 
 (171,134,1,'980-622-9213',now()); 


INSERT INTO cabrew.memberDates(memberDateID, memberID_fk, dateTypeID_fk, memberDate, dateDetail, lastChangeDate) VALUES;


INSERT INTO cabrew.memberDates(memberDateID, memberID_fk, dateTypeID_fk, memberDate, lastChangeDate) VALUES
 (1,54,3,'2012-01-10',now()), 
 (2,2,3,'2012-01-12',now()), 
 (3,21,3,'2012-01-12',now()), 
 (4,23,3,'2012-01-12',now()), 
 (5,24,3,'2012-01-12',now()), 
 (6,26,3,'2012-01-12',now()), 
 (7,57,3,'2012-01-12',now()), 
 (8,66,3,'2012-01-12',now()), 
 (9,76,3,'2012-01-12',now()), 
 (10,86,3,'2012-01-12',now()), 
 (11,93,3,'2012-01-12',now()), 
 (12,95,3,'2012-01-12',now()), 
 (13,99,3,'2012-01-12',now()), 
 (14,107,3,'2012-01-12',now()), 
 (15,111,3,'2012-01-12',now()), 
 (16,129,3,'2012-01-12',now()), 
 (17,130,3,'2012-01-12',now()), 
 (18,1,3,'2012-02-09',now()), 
 (19,60,3,'2012-02-09',now()), 
 (20,79,3,'2012-02-09',now()), 
 (21,100,3,'2012-02-09',now()), 
 (22,103,3,'2012-02-09',now()), 
 (23,104,3,'2012-02-09',now()), 
 (24,13,3,'2012-02-14',now()), 
 (25,15,3,'2012-03-08',now()), 
 (26,16,3,'2012-03-08',now()), 
 (27,27,3,'2012-03-08',now()), 
 (28,55,3,'2012-03-08',now()), 
 (29,56,3,'2012-03-08',now()), 
 (30,83,3,'2012-03-08',now()), 
 (31,101,3,'2012-03-08',now()), 
 (32,116,3,'2012-03-08',now()), 
 (33,40,3,'2012-04-12',now()), 
 (34,28,3,'2012-06-14',now()), 
 (35,125,3,'2012-08-09',now()), 
 (36,97,3,'2012-09-13',now()), 
 (37,98,3,'2012-09-13',now()), 
 (38,132,3,'2012-02-01',now()), 
 (39,105,3,'2012-01-01',now()); 
 
select * from cabrew.memberDates;


INSERT INTO cabrew.memberDates(memberDateID, memberID_fk, dateTypeID_fk, memberDate, lastChangeDate) VALUES
 (40,105,3,'2012-10-11',now()), 
 (41,50,3,'2012-10-11',now()), 
 (42,108,3,'2012-10-11',now()), 
 (43,109,3,'2012-10-11',now()), 
 (44,127,3,'2012-10-11',now()), 
 (45,128,3,'2012-10-11',now()), 
 (46,2,3,'2012-11-08',now()), 
 (47,57,3,'2012-11-08',now()), 
 (48,86,3,'2012-11-08',now()), 
 (49,95,3,'2012-11-08',now()), 
 (50,1,3,'2012-11-08',now()), 
 (51,60,3,'2012-11-08',now()), 
 (52,27,3,'2012-11-08',now()), 
 (53,83,3,'2012-11-08',now()), 
 (54,19,3,'2012-11-08',now()), 
 (55,68,3,'2012-11-08',now()), 
 (56,23,3,'2012-12-13',now()), 
 (57,66,3,'2012-12-13',now()), 
 (58,79,3,'2012-12-13',now()), 
 (59,101,3,'2012-12-13',now()), 
 (60,40,3,'2012-12-13',now()), 
 (61,30,3,'2012-12-13',now()), 
 (62,71,3,'2012-12-13',now()), 
 (63,91,3,'2012-12-13',now()), 
 (64,96,3,'2012-12-13',now()), 
 (65,7,3,'2012-12-23',now()), 
 (66,42,3,'2013-01-01',now()), 
 (67,12,3,'2013-01-10',now()), 
 (68,110,3,'2013-01-10',now()), 
 (69,118,3,'2013-02-01',now()), 
 (70,121,3,'2013-02-14',now()), 
 (71,97,3,'2013-04-11',now()), 
 (72,98,3,'2013-04-11',now()), 
 (73,9,3,'2013-04-11',now()), 
 (74,134,3,'2013-04-11',now()), 
 (75,34,3,'2013-05-09',now()), 
 (76,119,3,'2013-03-01',now()), 
 (77,74,3,'2013-06-13',now()), 
 (78,52,3,'2013-07-11',now()), 
 (79,61,3,'2013-07-11',now()), 
 (80,75,3,'2013-07-11',now()), 
 (81,88,3,'2013-06-01',now()), 
 (82,31,3,'2013-08-08',now()), 
 (83,72,3,'2013-08-08',now()), 
 (84,85,3,'2013-09-12',now()), 
 (85,22,3,'2013-09-28',now()), 
 (86,58,3,'2013-09-28',now()), 
 (87,67,3,'2013-09-28',now()), 
 (88,77,3,'2013-04-11',now()), 
 (89,13,3,'2013-01-01',now()), 
 (90,56,3,'1899-12-31',now()), 
 (91,54,3,'1899-12-31',now()), 
 (92,10,3,'2012-01-10',now()), 
 (93,43,3,'2012-01-10',now()), 
 (94,53,3,'2012-01-10',now()), 
 (95,59,3,'2012-01-10',now()), 
 (96,112,3,'2012-01-10',now()), 
 (97,113,3,'2012-01-10',now()), 
 (98,114,3,'2012-01-10',now()), 
 (99,115,3,'2012-01-10',now()), 
 (100,18,3,'2012-01-11',now()), 
 (101,73,3,'2012-01-11',now()), 
 (102,133,3,'2012-01-11',now()); 

INSERT INTO cabrew.memberDates(memberDateID, memberID_fk, dateTypeID_fk, memberDate, lastChangeDate) VALUES
 (103,108,3,'2013-11-14',now()), 
 (104,57,3,'2013-11-14',now()), 
 (105,19,3,'2013-11-14',now()), 
 (106,91,3,'2013-11-14',now()), 
 (107,133,3,'2013-12-12',now()), 
 (108,95,3,'2013-12-12',now()), 
 (109,27,3,'2013-12-12',now()), 
 (110,42,3,'2013-12-12',now()), 
 (111,126,3,'2013-12-12',now()), 
 (112,13,3,'2014-01-01',now()), 
 (113,2,3,'2014-01-09',now()), 
 (114,60,3,'2014-01-09',now()), 
 (115,30,3,'2014-01-09',now()), 
 (116,71,3,'2014-01-09',now()), 
 (117,110,3,'2014-01-09',now()), 
 (118,34,3,'2014-01-09',now()), 
 (119,75,3,'2014-01-09',now()), 
 (120,72,3,'2014-01-09',now()), 
 (121,62,3,'2014-01-09',now()), 
 (122,53,3,'2014-02-20',now()), 
 (123,105,3,'2014-02-20',now()), 
 (124,86,3,'2014-02-20',now()), 
 (125,51,3,'2014-02-20',now()), 
 (126,61,3,'2014-04-08',now()), 
 (127,88,3,'2014-05-13',now()), 
 (128,130,3,'2014-05-13',now()), 
 (129,119,3,'2014-07-10',now()), 
 (130,123,3,'2014-07-10',now()), 
 (131,8,3,'2014-09-11',now()), 
 (132,65,3,'2014-09-11',now()), 
 (133,120,3,'2014-09-11',now()), 
 (134,44,3,'2014-09-24',now()), 
 (135,92,3,'2014-09-24',now()), 
 (136,17,3,'2014-09-27',now()), 
 (137,25,3,'2014-09-27',now()), 
 (138,32,3,'2014-09-27',now()), 
 (139,69,3,'2014-09-27',now()), 
 (140,122,3,'2014-09-27',now()), 
 (141,1,3,'2014-10-09',now()), 
 (142,131,3,'2014-10-09',now()), 
 (143,23,3,'2014-12-01',now()), 
 (144,40,3,'2014-12-01',now()); 

select * from cabrew.memberDates;

update cabrew.noteTypes set noteType = 'Join Source' where noteTypeID = 1;
insert into cabrew.noteTypes (noteTypeID, noteType) values(2,'Inactive Reason');

select * from cabrew.noteTypes;


INSERT INTO cabrew.memberNotes (memberNoteID, memberID_fk, noteTypeID_fk, noteDate, memberNote, lastChangeDate) VALUES
  (1,10,1, now(), 'Oktoberfest 2012', now()), 
 (2,18,1, now(), 'Runnin` Hot 2013', now()), 
 (3,43,1, now(), 'Oktoberfest 2012', now()), 
 (4,50,1, now(), 'Alt Bev - Matt Cofee', now()), 
 (5,53,1, now(), 'Oktoberfest 2012', now()), 
 (6,54,2, now(), 'moved to IBU', now()), 
 (7,56,2, now(), 'moved to beach 2013', now()), 
 (8,59,1, now(), 'Oktoberfest 2012', now()), 
 (9,60,2, now(), 'Moved to Mass 2014', now()), 
 (10,73,1, now(), 'Runnin` Hot 2013', now()), 
 (11,105,1, now(), 'Won Raffle', now()), 
 (12,112,1, now(), 'Oktoberfest 2012', now()), 
 (13,113,1, now(), 'Oktoberfest 2012', now()), 
 (14,114,1, now(), 'Oktoberfest 2012', now()), 
 (15,115,1, now(), 'Oktoberfest 2012', now()), 
 (16,126,2, now(), 'Moved to Green Bay', now()), 
 (17,132,1, now(), 'Beertopia', now()), 
 (18,133,1, now(), 'Runnin` Hot 2013', now()), 
 (19,110,2, now(), 'Moved to Georgetown SC', now()), 
 (20,111,1, now(), 'Thad Mitchell', now()), 
 (21,112,1, now(), 'Thad Mitchell', now());

select * from cabrew.memberNotes;



DELIMITER // 
CREATE PROCEDURE cabrew.GetAllProducts() 
BEGIN 
SELECT * FROM cabrew.members; 
END // 
DELIMITER ; 

call cabrew.GetAllProducts();

INSERT INTO memberContacts (
	memberID_fk,
	contactTypeID_fk,
	memberContact,
	lastChangeDate) values
 (129,3,'10347 Garrett Grigg Road, Charlotte NC 28262', now()),
 (130,3,'10347 Garrett Grigg Road, Charlotte NC 28262', now()),
 (134,3,'105 Edgewater Dr, Concord NC 28027', now()),
 (24,3,'119 Cliffwood Circle, Mooresville NC 28115', now()),
 (71,3,'1228 Piney church Road, Concord NC 28025', now()),
 (57,3,'1244 Edgewood Court, Salisbury NC 28147', now()),
 (79,3,'127 Stacybrook Dr, Concord NC 28025', now()),
 (15,3,'143 Glendale Avenue, Concord NC 28025', now()),
 (121,3,'162 Anniston Way, Davidson NC 28036', now()),
 (102,3,'1680 Irving Road, Eugene OR 97402', now()),
 (50,3,'2535 clover Rd NW Concord NC 28027', now()),
 (68,3,'2543 Clover Road, Concord NC 28027', now()),
 (97,3,'2615 Willis Dr, Harrisburg NC 28075', now()),
 (98,3,'2615 Willis Dr, Harrisburg NC 28075', now()),
 (9,3,'3042 Dewitch, Concord NC 28027', now()),
 (42,3,'307 North Rowan Ave, Spencer NC 28159', now()),
 (83,3,'3151 Zion Church Road, Concord NC 28025', now()),
 (30,3,'3519 Kendale Avenue, Concord NC 28027', now()),
 (2,3,'3820 Carolea Valley Drive, Concord NC 28027', now()),
 (101,3,'4320 Silvermere Way, Charlotte NC 28269', now()),
 (99,3,'4701 Lindstrom Drive, Charlotte NC 28226', now()),
 (66,3,'5272 Stonepath Court, Harrisburg NC 28075', now()),
 (23,3,'5789 Village Drive, Concord NC 28027', now()),
 (40,3,'5789 Village Drive, Concord NC 28027', now()),
 (86,3,'5950 US Hwy 6015, Concord NC 28025', now()),
 (77,3,'769 Mottshoe Dr SW, Concord NC 28027', now()),
 (95,3,'799 Rothmoor Dr NE, Concord NC 28025', now()),
 (27,3,'8 Old Farm Road, Salisbury NC 28147', now()),
 (70,3,'917 Klondale Avenue, Kannapolis NC 28081', now()),
 (93,3,'917 Klondale Avenue, Kannapolis NC 28081', now());
