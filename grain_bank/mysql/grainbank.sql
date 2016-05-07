create database grain_bank;

CREATE TABLE grain_bank.grain_user (
  userID int(11) NOT NULL AUTO_INCREMENT,
  userName varchar(100) NOT NULL,
  active int(11) NOT NULL,
  addedDate timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (userID),
  UNIQUE KEY grain_user_UNIQUE (userName)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

INSERT INTO `grain_bank`.`grain_user`
(userName, active)
VALUES('Brad Hughes', 'Y');

select * from grain_bank.grain_user;
select * from grain_bank.grain_type;
select * from grain_bank.grain_inventory;
SET SQL_SAFE_UPDATES=1;
update grain_bank.grain_inventory set userID_fk = 1 where userID_fk = 0;

INSERT INTO grain_bank.grain_type (grain_type, addedDate) values('Best Malz Red X', '2015-03-13');
INSERT INTO grain_bank.grain_type (grain_type, addedDate) values('Weyermann Pale Ale Malt', '2015-04-20');
INSERT INTO grain_bank.grain_type (grain_type, addedDate) values('Castle Pilsen', '2015-04-20');
INSERT INTO grain_bank.grain_type (grain_type, addedDate) values('Bairds Marris Otter Pale Malt', '2015-04-20');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (1, 55, '2015-03-13');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (1, -23, '2015-03-13');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (2, 55, '2015-04-20');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (3, 55, '2015-04-20');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (4, 50, '2015-04-20');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (4, -10, '2015-04-20');

INSERT INTO grain_bank.grain_inventory (grainID_fk, transactionAmount, transactionDate)
VALUES (3, 18, '2015-05-08');

update grain_bank.grain_inventory set transactionAmount = -18 where grainInventoryID = 7;

call grain_bank.grainBalance(1);
call grain_bank.transactionDetail(1);

select 
	t.grain_type, 
	i.transactionAmount,
	i.transactionDate
from grain_bank.grain_type t
left join grain_bank.grain_inventory i
	on t.grainID = i.grainID_fk
order by 
	i.transactionDate,
	i.transactionAmount desc,
	t.grain_type;


select t.grain_type,  i.transactionAmount, i.transactionDate from grain_bank.grain_type t left join grain_bank.grain_inventory i on t.grainID = i.grainID_fk order by t.grain_type, i.transactionAmount desc;

select t.grain_type,  i.transactionAmount, DATE_FORMAT(i.transactionDate,'%m/%d/%Y') as 'transactionDate' from grain_bank.grain_type t left join grain_bank.grain_inventory i on t.grainID = i.grainID_fk order by t.grain_type, i.transactionAmount desc;

select * from grain_bank.grain_type;

select t.grain_type, sum(i.transactionAmount) as 'balance', DATE_FORMAT(max(i.transactionDate),'%m/%d/%Y') as 'lastChange' 
from grain_bank.grain_type t left join grain_bank.grain_inventory i on t.grainID = i.grainID_fk 
group by t.grain_type order by t.grain_type;

select *, DATE_FORMAT(transactionDate,'%m/%d/%Y') as 'transactionDate' from grain_bank.grain_inventory;

update grain_bank.grain_inventory set orderID = 1067 where grainInventoryID = 1;