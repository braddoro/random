yearPointsselect * from corporations;
select * from members where firstname = 'Lee';
select * from members;
select * from memberDates;
select * from memberContacts;
select * from memberNotes;
select * from memberChairs;

select * from contactTypes;
select * from dateTypes;

select dateType, datePoints from dateTypes where datePoints > 0 order by dateType;

select * from noteTypes;
select * from statusTypes;
select * from chairTypes;

select 
	ST.statusTypeID,
	ST.statusType,
	count(*)
from 
	members M inner join statusTypes ST on M.statusTypeID_fk = ST.statusTypeID
group by
	ST.statusTypeID,
	ST.statusType
order by 
	ST.statusTypeID,
	ST.statusType
;

call email_address();

call clubOfficers();
call eventSummary('2015-01-01','2015-12-31');
call eventSummary2('2015-01-01','2015-12-31');
call getMembersByType(1);
call lastPaidStatus(10,15);
call lastPaidStatus(13,24);
call memberDetail(195);
call memberDetail2(195);
call memberList();
call pComingDue();
call pSignInSheet();
call yearPoints(2015);
call memberActivity(2015, 50);
call memberActivity(2015, NULL);
call pointsList();
CALL memberAwards(2015, 1);
CALL RookieOfTheYear(2015);

insert into dateTypes (dateType, datePoints) values('Barrel Project', 1);

select * from dateTypes;
select * from memberDates;

--update dateTypes set datePoints = 1 where dateTypeID = 19;
--update dateTypes set dateType = 'Serve as Officer', datePoints = 2 where dateTypeID = 21;


select dt.dateTypeID, d.memberdate, dt.dateType, count(*)
from memberDates d
inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
where dt.datePoints > 0
group by dt.dateTypeID, d.memberdate, dt.dateType
order by dt.dateTypeID, d.memberdate, dt.dateType
;


select
	m.firstName,
	CONCAT(m.firstName, ' ', m.lastName) as 'name',
	CONCAT(m.firstName, ' ', m.lastName, ' <', c.memberContact, '>') as 'full',
	CONCAT(c.memberContact,',') as 'email'
from members m
inner join memberContacts c
	on m.memberID = c.memberID_fk
where
	m.statusTypeID_fk in (1,3,5)
	and c.contactTypeID_fk = 2
order by 
	c.memberContact
;

select dt.dateTypeID, d.memberdate, dt.dateType, count(*)
from memberDates d
inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
where dt.datePoints > 0
group by dt.dateTypeID, d.memberdate, dt.dateType
order by dt.dateTypeID, d.memberdate, dt.dateType
;

-- Rookies
select
	CONCAT(m.firstName, ' ', m.lastName) as 'full',
	d.memberDate,
	dt.dateTypeID,
	m.statusTypeID_fk 
from members m
	inner join memberDates d on m.memberID = d.memberID_fk
	inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
where
	m.statusTypeID_fk = 1
	and dt.dateTypeID = 1
	and year(d.memberDate) = 2015
order by 
	m.memberID
;



-- Top Competitor
select 
	M.memberID,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ') as 'FullName', 
	sum(dt.datePoints) as 'totalPoints'
from 
	memberDates d
	inner join members M on M.memberID = d.memberID_fk
	inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
where
	year(d.memberDate) =  2015
	and M.statusTypeID_fk = 1
	-- and dt.dateTypeID in (14,16,18,19)
	and M.memberID in 
		(select
			m.memberID
		from members m
			inner join memberDates d on m.memberID = d.memberID_fk
			inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
		where
			m.statusTypeID_fk = 1
			and dt.dateTypeID = 1
			and year(d.memberDate) = 2015
		)
group by
	M.memberID,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ')
order by 
	sum(dt.datePoints) desc,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ')
;

-- Top Educator
select 
	M.memberID,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ') as 'FullName', 
	sum(dt.datePoints) as 'totalPoints'
from 
	memberDates d
	inner join members M on M.memberID = d.memberID_fk
	inner join dateTypes dt on d.dateTypeID_fk = dt.dateTypeID
	-- inner join awardGroups ag on dt.dateTypeID = ag.dateTypeID_fk
	-- inner join awardNames an on ag.awardNameID_fk = an.awardNameID
where
	year(d.memberDate) =  2015
	and M.statusTypeID_fk = 1
	-- and dt.dateTypeID in (8,10,20,15,17)
	and dt.dateTypeID = 13
	-- and ag.awardNameID_fk = 2
group by
	M.memberID,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ')
order by 
	sum(dt.datePoints) desc,
	REPLACE(CONCAT(M.firstName,' ',IFNULL(M.midName,''),' ',M.lastName),'  ',' ')
;

insert into cabrew.awardNames (awardName) values('a');

select awardNameID, awardName from awardNames;
select * from awardGroups;
update cabrew.awardNames set awardName = 'CABREW-kie' where awardNameID = 4;

--insert into cabrew.awardGroups (awardNameID_fk,dateTypeID_fk) 
(3,7),
(3,9);

select dateTypeID, dateType from dateTypes where datePoints > 0;

-- most improved
-- new
-- barrel
-- education
-- education host
-- brew session
-- brew session host
;

insert into awardGroups (awardNameID_fk, dateTypeID_fk) values 
(4,6),
(4,7),
(4,8),
(4,9),
(4,10),
(4,11),
(4,12);



INSERT INTO `cabrew`.`memberDates` (`memberID_fk`, `dateTypeID_fk`, `memberDate`)
VALUES (32, 11, '2015/07/08');

select * from awardGroups;
select * from awardNames;
select * from dateTypes;

update awardNames set awardName = 'Best Attendance' where awardNameID = 3;
update awardGroups set awardNameID_fk = 4 where awardGroupID = 12;

select 
	an.awardName, 
	dt.dateType
	-- dt.dateTypeID
from 
awardGroups ag inner join awardNames an on ag.awardNameID_fk = an.awardNameID
inner join dateTypes dt on ag.dateTypeID_fk = dt.dateTypeID
order by an.awardName, dt.dateType
;
