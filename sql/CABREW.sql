CALL cabrew.memberDetail2(119);
CALL cabrew.yearPoints(50);
CALL cabrew.lastPaidStatus(11,16);
CALL cabrew.yearPoints2(2016);
CALL cabrew.RookieOfTheYear(2016);
CALL cabrew.memberAwards(2016, 2);
CALL cabrew.awardList();
CALL cabrew.memberAwards(2016, 4);
CALL cabrew.memberActivity(2016, 119);
CALL cabrew.clubOfficers();
CALL `cabrew`.`eventSummary`('2015-01-01', '2015-12-31');
CALL `cabrew`.`eventSummary2`('2015-01-01', '2015-12-31');
CALL `cabrew`.`getMembersByType`(1);
CALL `cabrew`.`yearStats`(2015);

select count(*) from members where statusTypeID_fk = 1; 

select dateTypeID, dateType, datePoints from dateTypes where datePoints > 0 and active = 'Y' order by dateType;

SELECT * FROM cabrew.awardGroups;

SELECT * FROM members where lastName = 'Purvis';
select 
	M.memberID,
	M.firstName,
	M.midName,
	M.lastName,
	M.sex,  
	DT.dateType,
	D.memberDate, 
	D.dateDetail
from members M
	left join memberDates D on M.memberID = D.memberID_fk
	left join dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
where M.memberID = 196
group by 
	M.memberID,
	M.firstName,
	M.midName,
	M.lastName,
	M.sex,  
	DT.dateType,
	D.memberDate, 
	D.dateDetail
order by memberDate
;



SELECT AG.*, DT.dateType 
FROM cabrew.awardGroups AG
inner join cabrew.dateTypes DT
on AG.dateTypeID_fk = DT.dateTypeID
order by AG.awardNameID_fk;

select
	M.memberID,
	M.firstName,
	M.lastName,
	DT.dateType
from members M
	left join memberDates D on M.memberID = D.memberID_fk
	left join dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
where DT.dateTypeID in (18)
group by
	M.memberID,
	M.firstName,
	M.lastName,
	DT.dateType
order by 
	M.firstName,
	M.lastName,
	DT.dateType
;



select
	M.memberID,
	M.firstName,
	M.lastName,
	M.statusTypeID_fk,
	DT.dateType,
	D.memberDate
from members M
	left join memberDates D on M.memberID = D.memberID_fk
	left join dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
where DT.dateTypeID in (18)
order by 
	M.firstName,
	M.lastName,
	DT.dateType 
;


select
	M.memberID,
	M.firstName,
	M.lastName,
	DT.dateType
from members M
	left join memberDates D on M.memberID = D.memberID_fk
	left join dateTypes DT on D.dateTypeID_fk = DT.dateTypeID
where DT.dateTypeID in (11)
group by
	M.memberID,
	M.firstName,
	M.lastName,
	DT.dateType
order by 
	M.firstName,
	M.lastName,
	DT.dateType 
;
