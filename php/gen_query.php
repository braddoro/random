	<?php
/* ----------------
-- Field Types -- 
-- CF_MENU		= 1
-- CF_RADIO		= 2
-- CF_INT		= 3
-- CF_DATETIME	= 4
-- CF_TEXT		= 5
-- CF_TEXTAREA	= 6
-- CF_DATE		= 7
-- Field Types --
---------------- */
ini_set('display_errors', 'On');
error_reporting(E_ALL | E_STRICT);
date_default_timezone_set("America/New_York");
require_once("..\inc\generic_sql.php");
$objSQL = new c_generic_sql();
$i_type = 1;
$s_title = "";
$s_sql = "select L3.label, DATE_FORMAT(I.created,'%m/%d/%Y') as 'work_day', A.acct_id, CONCAT(A.first_name,' ',A.last_name) as 'name_full', count(distinct I.i_id) as 'total' from incidents I inner join accounts A on A.acct_id = I.created_by and I.source_lvl2 <> 1017 and I.created > '2010-06-23' and I.created <= '2010-06-23 23:59:59' inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 and L3.label_id in (101075,101159,101074,304760,155803,155804,101082) group by L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name);"; 
$s_sql = "select sessionID, sessionSeqNum, startDateTime, contactType, contactDisposition, dispositionReason, connectTime, badCallTag, transfer, redirect, conference, flowout from dbo.ContactCallDetail CCD where sessionID in ( select sessionID from dbo.ContactCallDetail where startDatetime > '2010-07-01' group by sessionID having sum(case when contactDisposition = 1 then 1 else 0 end) > 1 ) order by sessionID, sessionSeqNum, startDateTime "; 
$s_sql = "select sessionID, sessionSeqNum, startDateTime, contactType, contactDisposition, dispositionReason, connectTime, badCallTag, transfer, redirect, conference, flowout from dbo.ContactCallDetail CCD where sessionID in ( select sessionID from dbo.ContactCallDetail where startDatetime > '2010-07-02' group by sessionID having sum(case when contactDisposition = 1 then 1 else 0 end) > 1 ) order by sessionID, sessionSeqNum, startDateTime "; 
$s_sql = "select RES.resourceName, ASD.agentID, ASD.eventDateTime, ASD.eventType, ASD.reasonCode from dbo.AgentStateDetail ASD inner join dbo.resource RES on ASD.agentID = RES.resourceID where eventType in (1,7) and eventDateTime between '2010-07-01' and '2010-07-01 23:59:59' and agentID in ( select agentID from dbo.AgentStateDetail where eventDateTime between '2010-07-01' and '2010-07-01 23:59:59' and eventType in (1,7) and agentID in (select resourceID from dbo.resource where resourceName in ('Mike O''Shea','Corey McClelland','Paul Hassinger','Jon Burnley','Pam Lovitt','Sharon Gibilisco','Ben Lane','Sara Halvorsen','Amanda VanDyke')) group by agentID having count(agentID) in (1,3,5,7,9,11,13,15,17,19,21) ) order by RES.resourceName, ASD.eventDateTime "; 
$s_sql = "select p.product_id, p.seq, p.c\$parent_id, p.c\$child_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.org_id, o.c\$lat, o.c\$lon, a.street, a.city, a.state, a.postal_code, c\$sd_brand_blackmax, c\$sd_brand_huskee, c\$sd_brand_husq, c\$sd_brand_mcc, c\$sd_brand_poulan, c\$sd_brand_poulan_pro, c\$sd_brand_redmax, c\$sd_brand_we, c\$sd_flag_preferred, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_group_comm, c\$sd_group_handheld, c\$sd_group_mowers, c\$sd_group_snow, c\$sd_group_tractor, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless, c\$sd_trans_techums FROM sa_products p inner join orgs o on (o.org_id = p.c\$child_id) inner join (select org_id, street, city, label as 'state', postal_code, prov_id, country_id from org_addrs a left join labels l on a.prov_id = l.label_id where a.oat_id = 2 and l.tbl = 48) a on o.org_id = a.org_id WHERE '1' = '1' AND p.c\$RecordType = 1398 AND p.c\$parent_id = 118837"; 
$s_sql = "select distinct o.org_id, o.c\$lat as 'lat', o.c\$lon as 'lon', case when sa.child_id > 0 then 'yes' else 'no' end as 'child_id', o.c\$cert_advanced_exp_date, o.c\$cert_basic_exp_date, o.c\$account_number, o.name, a.street, a.city, a.postal_code, a.prov_id as 'state', o.c\$industry as 'dealertype', o.c\$good_receiving_address as 'gra' from orgs o inner join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id left join (select distinct c\$child_id as 'child_id' from sa_products where c\$recordtype = 1398 and seq > 0) sa on o.org_id = sa.child_id where o.c\$account_active_yes_no = 1 and o.name like 'aspl%' order by o.name"; 
$s_sql = "select sessionID, sessionSeqNum, startdatetime, CSQName, resourceID, resourceName, contactType, contactDisposition, transfer, redirect, flowout, metServiceLevel, callServiceLevelMet, connectTime, queueTime, ringTime, talkTime, holdTime, workTime from agent.dbo.cc_reporting_data_summary where startDatetime > getdate()-7 and (sessionID in (select sessionID from agent.dbo.cc_reporting_data_summary where startDatetime > getdate()-7 group by sessionID having sum(case when contactDisposition = 1 then 1 else 0 end) > 1)) order by sessionID, sessionSeqNum, startdatetime";
$s_sql = "select oa.street, oa.city, oa.country_id, oa.prov_id, oa.postal_code, o.org_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.c\$mbdm_name, o.c\$cbdm_name from orgs o left join org_addrs oa on o.org_id = oa.org_id and oa.oat_id = 2 where o.c\$account_active_yes_no = 1 limit 5 ";
$s_sql = "select org_id, name, c\$lat, c\$lon from orgs where c\$lat > 0 order by name limit 500";
$s_sql = "select * from org_addrs limit 5";
$s_sql = "select product_id, folder_id, id, c\$recordtype, c\$parent_id, c\$child_id, seq, disabled, oa_exclude, yes_cnt, cnt, updated, c\$group, c\$sd_flag_preferred, c\$sd_flag_primary, c\$sd_flag_probation from sa_products order by c\$parent_id, c\$child_id, seq";
$s_sql = 'select o.org_id, o.c$account_number, o.name, a.street, a.city, a.prov_id, a.postal_code, a.country_id, o.c$husq_mbdm_territory_num, o.c$husq_cbdm_territory_num, o.c$company_code, o.c$mbdm_name, o.c$cbdm_name, o.c$account_active_yes_no, l.label, o.c$good_receiving_address FROM orgs o left join org_addrs a on o.org_id = a.org_id left join labels l on a.prov_id = l.label_id WHERE l.tbl = 48 and a.oat_id = 2 and o.c$company_id = 899 and o.c$authority_code IS NOT NULL and (o.c$mass_merchant IS NULL OR o.c$mass_merchant = 0) order by o.name, a.city limit 500;';
$s_sql = "select distinct o.org_id, o.name, c\$lat as 'lat', c\$lon as 'lon', a.street, a.city, a.prov_id, a.postal_code, a.state, a.country_id, o.c\$account_number, o.c\$good_receiving_address, o.c\$company_code, o.c\$mbdm_name, o.c\$cbdm_name, o.c\$husq_mbdm_territory_num, o.c\$husq_cbdm_territory_num, o.c\$cert_advanced_exp_date, o.c\$cert_basic_exp_date, case when sa.child_id > 0 then 1 else 0 end as 'child_id' from orgs o inner join (select org_id, street, city, label as 'state', postal_code, prov_id, country_id from org_addrs a left join labels l on a.prov_id = l.label_id where a.oat_id = 2 and l.tbl = 48) a on o.org_id = a.org_id left join (select distinct c\$child_id as 'child_id' from sa_products where c\$recordtype = 1398 and seq > 0) sa on o.org_id = sa.child_id where o.c\$account_active_yes_no = 1 and o.name like 'gordon%' order by o.name limit 1000";
$s_sql = "select distinct o.org_id, c\$lat as 'lat', c\$lon as 'lon', case when sa.child_id > 0 then 'yes' else 'no' end as 'child_id', o.c\$cert_advanced_exp_date, o.c\$cert_basic_exp_date, o.c\$account_number, o.name, a.street, a.city, a.postal_code, a.state from orgs o inner join (select org_id, street, city, label as 'state', postal_code, prov_id, country_id from org_addrs a left join labels l on a.prov_id = l.label_id where a.oat_id = 2 and l.tbl = 48) a on o.org_id = a.org_id left join (select distinct c\$child_id as 'child_id' from sa_products where c\$recordtype = 1398 and seq > 0) sa on o.org_id = sa.child_id where o.c\$account_active_yes_no = 1 and o.name like '%golf%' order by o.name limit 1000"; 
$s_sql = "select o.c\$account_active_yes_no, o.name, o.c\$account_number as 'acct_num', o.c\$good_receiving_address as 'gra', o.c\$lat, o.c\$lon, o.c\$needs_service_dealer from orgs o where o.org_id = 49379";
$s_sql = "select oat_id, street, city, country_id, prov_id, postal_code, l.label as 'state' from org_addrs a left join labels l on a.prov_id = l.label_id where a.oat_id = 2 and l.tbl = 48 and a.org_id = 49379 order by case oat_id when 2 then 0 else 1 end limit 1";
$s_sql = "select distinct label_id, label from labels L inner join orgs O on L.label_id = O.c\$industry and fld = 1 and tbl = 20 order by label_id";
$s_sql = "select label_id, label from labels where tbl = 48";
$s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by registration_date desc limit 20";
$s_sql = "select id, serial_number, model_number from ps_prodreg_contacts where c_id in (1539108,1539109,1539110,1539111,1539112,1539113)";
$s_sql = "select registration_date, SUBDATE(registration_date, INTERVAL 1 year) from ps_prodreg_contacts order by registration_date desc limit 20";
$s_sql = "select child_id from sa_products order by child_id";
$s_sql = "select * from ps_prodreg_contacts limit 10";
$s_sql = "select * from contacts limit 10";
$s_sql = "select R.id, C.c_id, ph_office, C.last_name, C.first_name, R.serial_number, R.model_number, R.registration_date from contacts C inner join ps_prodreg_contacts R on C.c_id = R.c_id and registered_by = 40 and R.registration_date >= '2010-08-25 14:26:36' order by registration_date desc limit 20"; 
$s_sql = "select top 10 * from agent.dbo.cc_reporting_data_summary";
$s_sql = "select * from accounts where last_name = 'hughes'";
$s_sql = "select * from ps_inv_summary limit 100";
$s_sql = "select accountNumber, org_id, invoiceNumber, name, state, model, serial, quantity, invoiceDate, invoiceMonth, lineNumber, dueDate, product_group from ps_inv_detail limit 1000";
$s_sql = "select distinct O.org_id, O.c\$account_number as 'account', O.c\$good_receiving_address as 'GRA', O.name, L2.label as 'industry', O.c\$org_phone as 'phone', A.street, A.city, L.label as 'state', A.postal_code, left(O.c\$cert_basic_exp_date,10) as 'basic', left(O.c\$cert_advanced_exp_date,10) as 'advanced' from sa_products P inner join orgs O on P.c\$child_id = O.org_id inner join org_addrs A on O.org_id = A.org_id and A.oat_id = 2 inner join labels L on A.prov_id = L.label_id and L.tbl = 48 inner join labels L2 on O.c\$industry = L2.label_id and L2.fld = 1 and L2.tbl = 20 order by O.name";
$s_sql = "select distinct O.org_id, O.c\$account_number as 'account', O.c\$good_receiving_address as 'GRA', O.name, L2.label as 'industry', O.c\$org_phone as 'phone', A.street, A.city, L.label as 'state', A.postal_code, left(O.c\$cert_basic_exp_date,10) as 'basic', left(O.c\$cert_advanced_exp_date,10) as 'advanced' from sa_products P inner join orgs O on P.c\$child_id = O.org_id inner join org_addrs A on O.org_id = A.org_id and A.oat_id = 2 inner join labels L on A.prov_id = L.label_id and L.tbl = 48 inner join labels L2 on O.c\$industry = L2.label_id and L2.fld = 1 and L2.tbl = 20 where O.c\$account_active_yes_no = 1 and P.c\$sd_brand_huskee = 1 order by O.name";
$s_sql = "select oat_id, street, city, country_id, prov_id, postal_code, l.label as 'state' from org_addrs a left join labels l on a.prov_id = l.label_id where a.oat_id = 2 and l.tbl = 48 and a.org_id = 50429 order by case oat_id when 2 then 0 else 1 end limit 1";
$s_sql = "select o.name, o.c\$account_number as 'acct_num', o.c\$good_receiving_address as 'gra', o.c\$lat, o.c\$lon, o.c\$needs_service_dealer from orgs o where o.org_id = 50429 and o.c\$account_active_yes_no = 1";
$s_sql = "select distinct label_id, label from labels L inner join orgs O on L.label_id = O.c\$industry and fld = 1 and tbl = 20 order by label_id";
$s_sql = "select * from labels order by tbl, label_id";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label from custom_fields C inner join labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 93 order by C.col_name;";
$s_sql = "select tbl, lang_id, fld, label_id, label from labels order by tbl, lang_id, fld, label_id";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 93 ORDER BY C.col_name;";
$s_sql = "select org_id, c\$account_number, c\$good_receiving_address, c\$financial_yr_to_date_sales, c\$financial_last_yr_sales, c\$date_customer_created from orgs limit 100";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 3 and C.data_type = 7 ORDER BY C.col_name;";
$s_sql = "select distinct c\$cert_basic_exp_date from orgs limit 10 ";
$s_sql = "select * from ac_columns limit 100";
$s_sql = "select org_id, c\$needs_service_dealer, c\$no_service_dealer_available from orgs where org_id in (92286)";
$s_sql = "select distinct product_id, seq from sa_products order by product_id, seq";
$s_sql = "select O.org_id, O.c\$needs_service_dealer, O.c\$no_service_dealer_available, P.product_id, P.seq from orgs O inner join sa_products P on O.org_id = P.c\$parent_id and O.c\$needs_service_dealer = 1";
$s_sql = "select distinct product_id, seq from sa_products order by product_id, seq";
$s_sql = "select count(*) from orgs where c\$needs_service_dealer = 1";
$s_sql = "select distinct c\$parent_id, c\$child_id, product_id, seq from sa_products order by c\$parent_id, c\$child_id, product_id, seq";
$s_sql = "select CONCAT('deleteMe(', product_id, ',', seq, ');') as 'script' from sa_products order by product_id, seq";
$s_sql = "select count(*) as 'total_of_all_records' from sa_products";
$s_sql = "select C.col_name FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 93 ORDER BY C.col_name;";
$s_sql = "select o.c\$sd_brand_blackmax, o.c\$sd_brand_huskee, o.c\$sd_brand_husq, o.c\$sd_brand_mcc, o.c\$sd_brand_poulan, o.c\$sd_brand_poulan_pro, o.c\$sd_brand_redmax, o.c\$sd_brand_we, o.c\$sd_engine_briggs, o.c\$sd_engine_honda, o.c\$sd_engine_kawisaki, o.c\$sd_engine_kohler, o.c\$sd_flag_preferred, o.c\$sd_flag_primary, o.c\$sd_flag_probation, o.c\$sd_group_comm, o.c\$sd_group_handheld, o.c\$sd_group_mowers, o.c\$sd_group_snow, c\$sd_group_tractor, o.c\$sd_trans_hydrogear, o.c\$sd_trans_peerless, o.c\$sd_trans_techums from orgs o where o.org_id = 29033"; 
$s_sql = "select p.product_id, p.seq, p.c\$parent_id, p.c\$child_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.org_id, o.c\$lat, o.c\$lon, a.street, a.city, a.prov_id, a.postal_code, o.c\$sd_brand_blackmax, o.c\$sd_brand_huskee, o.c\$sd_brand_husq, o.c\$sd_brand_mcc, o.c\$sd_brand_poulan, o.c\$sd_brand_poulan_pro, o.c\$sd_brand_redmax, o.c\$sd_brand_we, o.c\$sd_engine_briggs, o.c\$sd_engine_honda, o.c\$sd_engine_kawisaki, o.c\$sd_engine_kohler, o.c\$sd_flag_preferred, o.c\$sd_flag_primary, o.c\$sd_flag_probation, o.c\$sd_group_comm, o.c\$sd_group_handheld, o.c\$sd_group_mowers, o.c\$sd_group_snow, o.c\$sd_group_tractor, o.c\$sd_trans_hydrogear, o.c\$sd_trans_peerless, o.c\$sd_trans_techums from sa_products p left join orgs o on o.org_id = p.c\$child_id inner join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id where p.c\$RecordType = 1398 -- and p.c\$parent_id = 29033 "; 
$s_sql = "select * from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where l0.label < 111111111"; 
$s_sql = "select * from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where l0.label > 111111111 or "; 
$s_sql = "select * from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where not (l0.label < 111111111 or l0.label REGEXP '[A-Z][a-z][^.{4,10}]')"; 
$s_sql = "select * from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where not (l0.label REGEXP '^.{1,9}') order by length(l0.label)"; 
$s_sql = "select p.id, l0.label from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where (l0.label REGEXP '^.{1,9}') order by length(l0.label)"; 
$s_sql = "select p.id, l0.label, convert(l0.label,unsigned) as 'test' from products p left join labels l0 on p.id = l0.label_id and l0.tbl = 65 and l0.lang_id = 1 and l0.fld = 1 where (convert(l0.label,unsigned) < 111111111 or l0.label REGEXP '[A-Z][a-z]') order by length(l0.label)"; 
$s_sql = "select p.product_id, p.seq, p.c\$parent_id, p.c\$child_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.org_id, o.c\$lat, o.c\$lon, a.street, a.city, a.prov_id, a.postal_code, o1.c\$sd_brand_blackmax, o1.c\$sd_brand_huskee, o1.c\$sd_brand_husq, o1.c\$sd_brand_mcc, o1.c\$sd_brand_poulan, o1.c\$sd_brand_poulan_pro, o1.c\$sd_brand_redmax, o1.c\$sd_brand_we, o1.c\$sd_engine_briggs, o1.c\$sd_engine_honda, o1.c\$sd_engine_kawisaki, o1.c\$sd_engine_kohler, o1.c\$sd_flag_preferred, o1.c\$sd_flag_primary, o1.c\$sd_flag_probation, o1.c\$sd_group_comm, o1.c\$sd_group_handheld, o1.c\$sd_group_mowers, o1.c\$sd_group_snow, o1.c\$sd_group_tractor, o1.c\$sd_trans_hydrogear, o1.c\$sd_trans_peerless, o1.c\$sd_trans_techums from sa_products p left join orgs o on (o.org_id = p.c\$parent_id) left join orgs o1 on (p.c\$child_id = o1.org_id) inner join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id where p.c\$RecordType = 1398 and p.c\$child_id = 26659"; 
$s_sql = "select * from sa_products where c\$child_id = 26659"; 
$s_sql = "select p1.id, p1.prod_lvl1_id, p1.prod_lvl2_id, p1.prod_lvl3_id, p1.prod_lvl4_id, p1.prod_lvl5_id, p1.prod_lvl6_id, l0.label, l1.label, l2.label, l3.label, l4.label, l5.label, l6.label FROM products p1 left outer join labels l0 on (p1.prod_lvl1_id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) left outer join labels l1 on (p1.prod_lvl1_id = l1.label_id AND l1.tbl = 65 AND l1.lang_id = 1 AND l1.fld = 1) left outer join labels l2 on (p1.prod_lvl2_id = l2.label_id AND l2.tbl = 65 AND l2.lang_id = 1 AND l2.fld = 1) left outer join labels l3 on (p1.prod_lvl3_id = l3.label_id AND l3.tbl = 65 AND l3.lang_id = 1 AND l3.fld = 1) left outer join labels l4 on (p1.prod_lvl4_id = l4.label_id AND l4.tbl = 65 AND l4.lang_id = 1 AND l4.fld = 1) left outer join labels l5 on (p1.prod_lvl5_id = l5.label_id AND l5.tbl = 65 AND l5.lang_id = 1 AND l5.fld = 1) left outer join labels l6 on (p1.prod_lvl6_id = l6.label_id AND l6.tbl = 65 AND l6.lang_id = 1 AND l6.fld = 1) where l0.label > 111111111 or l1.label > 111111111 or l2.label > 111111111 or l3.label > 111111111 or l4.label > 111111111 or l5.label > 111111111 or l0.label > 111111111 "; 
$s_sql = "select * from products p left join labels l0 on (p.id = l0.label_id AND l0.tbl = 65 AND l0.lang_id = 1 AND l0.fld = 1) where (l0.label < 111111111 or l0.label REGEXP '[A-Z][a-z][^.{4,10}]')"; 
$s_sql = "select p.id, l0.label, convert(l0.label,unsigned) as 'test' from products p left join labels l0 on p.id = l0.label_id and l0.tbl = 65 and l0.lang_id = 1 and l0.fld = 1 where (convert(l0.label,unsigned) < 111111111 or l0.label REGEXP '[A-Z][a-z]') order by length(l0.label)"; 
$s_sql = "select c\$child_id, c\$sd_flag_preferred, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless from sa_products order by c\$child_id, c\$sd_flag_preferred, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless "; 
$s_sql = "select c\$child_id from sa_products group by c\$child_id, c\$sd_flag_preferred, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless having count(c\$child_id) > 1 "; 
$s_sql = "select distinct c\$child_id, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless from sa_products where c\$child_id in ( select c\$child_id from sa_products group by c\$child_id, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_trans_hydrogear, c\$sd_trans_peerless having count(c\$child_id) > 2 ) order by c\$child_id limit 10000 "; 
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 3 ORDER BY C.cf_id, C.col_name;"; 
$s_sql = "select count(*) FROM orgs o where o.c\$sd_flag_primary = 1"; 
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 3 ORDER BY C.cf_id, C.col_name;"; 
$s_sql = "select o.org_id, o.c\$sd_brand_blackmax, o.c\$sd_brand_huskee, o.c\$sd_brand_husq, o.c\$sd_brand_mcc, o.c\$sd_brand_poulan, o.c\$sd_brand_poulan_pro, o.c\$sd_brand_redmax, o.c\$sd_brand_we, o.c\$sd_flag_preferred, o.c\$sd_flag_primary, o.c\$sd_flag_probation, o.c\$sd_group_comm, o.c\$sd_group_handheld, o.c\$sd_group_mowers, o.c\$sd_group_snow, o.c\$sd_group_tractor, o.c\$sd_engine_briggs, o.c\$sd_engine_honda, o.c\$sd_engine_kawisaki, o.c\$sd_engine_kohler, o.c\$sd_trans_hydrogear, o.c\$sd_trans_peerless, o.c\$sd_trans_techums FROM orgs o where o.c\$sd_flag_primary = 1";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 93 ORDER BY C.col_name;";
$s_sql = "select c\$parent_id, c\$child_id, product_id, seq, c\$sd_flag_primary from sa_products where c\$parent_id = 118837";
$s_sql = "select * from sa_products order by c\$sd_flag_primary, c\$parent_id, c\$child_id";
$s_sql = "select op.c\$mbdm_name as 'Region', op.c\$cbdm_name as 'Territory', op.c\$company_code as 'Company_Code', op.org_id as 'Org_ID', op.name as 'Partner_Name', op.c\$account_number as 'Account_Number', op.c\$good_receiving_address as 'GRA', op.c\$org_phone as 'Org Phone', ap.street, ap.city, ap.prov_id, ap.postal_code, op.c\$lat as 'LAT', op.c\$lon as 'LON', op.c\$no_service_dealer_available as 'No_Service_Dealer_Available', oc.c\$mbdm_name as 'Region', oc.c\$cbdm_name as 'Territory', oc.c\$company_code as 'Company_Code', oc.org_id as 'Org_ID', oc.name as 'Partner_Name', oc.c\$account_number as 'Account_Number', oc.c\$good_receiving_address as 'GRA', oc.c\$org_phone as 'Org Phone', ac.street, ac.city, ac.prov_id, ac.postal_code, oc.c\$lat as 'LAT', oc.c\$lon as 'LON', oc.c\$cert_basic_exp_date as 'Basic_Exp_Date', oc.c\$cert_advanced_exp_date as 'Advanced_Exp_Date', case when oc.c\$sd_flag_probation = 1 then 'Yes' else 'No' end as 'On_Probation', case when p.c\$sd_flag_primary = 1 then 'Yes' else 'No' end as 'Is_Primary' from orgs op left join sa_products p on op.org_id = p.c\$parent_id left join orgs oc on p.c\$child_id = oc.org_id left join org_addrs ap on ap.oat_id = 2 and p.c\$parent_id = ap.org_id left join org_addrs ac on ac.oat_id = 2 and p.c\$child_id = ac.org_id where op.c\$needs_service_dealer = 1 and op.c\$account_active_yes_no = 1 and p.c\$recordtype = 1398 order by op.c\$mbdm_name, op.c\$cbdm_name, op.name, p.c\$sd_flag_primary desc, oc.name ";
$s_sql = "select op.c\$mbdm_name as 'Region', op.c\$cbdm_name as 'Territory', op.c\$company_code as 'Company_Code', op.org_id as 'Org_ID', op.name as 'Partner_Name', op.c\$account_number as 'Account_Number', op.c\$good_receiving_address as 'GRA', op.c\$org_phone as 'Org Phone', ap.street, ap.city, ap.prov_id, ap.postal_code, op.c\$lat as 'LAT', op.c\$lon as 'LON', op.c\$no_service_dealer_available as 'No_Service_Dealer_Available', oc.c\$mbdm_name as 'Region', oc.c\$cbdm_name as 'Territory', oc.c\$company_code as 'Company_Code', oc.org_id as 'Org_ID', oc.name as 'Partner_Name', oc.c\$account_number as 'Account_Number', oc.c\$good_receiving_address as 'GRA', oc.c\$org_phone as 'Org Phone', ac.street, ac.city, ac.prov_id, ac.postal_code, oc.c\$lat as 'LAT', oc.c\$lon as 'LON', oc.c\$cert_basic_exp_date as 'Basic_Exp_Date', oc.c\$cert_advanced_exp_date as 'Advanced_Exp_Date', case when oc.c\$sd_flag_probation = 1 then 'Yes' else 'No' end as 'On_Probation', case when p.c\$sd_flag_primary = 1 then 'Yes' else 'No' end as 'Is_Primary' from orgs op left join sa_products p on op.org_id = p.c\$parent_id left join orgs oc on p.c\$child_id = oc.org_id left join org_addrs ap on ap.oat_id = 2 and p.c\$parent_id = ap.org_id left join org_addrs ac on ac.oat_id = 2 and p.c\$child_id = ac.org_id where op.c\$needs_service_dealer = 1 order by op.c\$mbdm_name, op.c\$cbdm_name, op.name, p.c\$sd_flag_primary desc, oc.c\$mbdm_name, oc.c\$cbdm_name, oc.name limit 10 ";
$s_sql = "select op.c\$mbdm_name as 'Retailer_Region', op.c\$cbdm_name as 'Retailer_Territory', op.c\$company_code as 'Retailer_Company_Code', op.org_id as 'Retailer_Org_ID', op.name as 'Retailer_Name', op.c\$account_number as 'Retailer_Account_Number', op.c\$good_receiving_address as 'Retailer_GRA', op.c\$org_phone as 'Retailer_Org_Phone', ap.street as 'Retailer_Street', ap.city as 'Retailer_City', ap.prov_id as 'Retailer_Prov_ID', ap.postal_code as 'Retailer_Postal_Code', op.c\$lat as 'Retailer_LAT', op.c\$lon as 'Retailer_LON', op.c\$no_service_dealer_available as 'Retailer_No_Service_Dealer_Available', oc.c\$mbdm_name as 'Partner_Region', oc.c\$cbdm_name as 'Partner_Territory', oc.c\$company_code as 'Partner_Company_Code', oc.org_id as 'Partner_Org_ID', oc.name as 'Partner_Name', oc.c\$account_number as 'Partner_Account_Number', oc.c\$good_receiving_address as 'Partner_GRA', oc.c\$org_phone as 'Partner_Org_Phone', ac.street as 'Partner_Street', ac.city as 'Partner_City', ac.prov_id as 'Partner_Prov_ID', ac.postal_code as 'Partner_Postal_Code', oc.c\$lat as 'Partner_LAT', oc.c\$lon as 'Partner_LON', oc.c\$cert_basic_exp_date as 'Partner_Basic_Exp_Date', oc.c\$cert_advanced_exp_date as 'Partner_Advanced_Exp_Date', case when oc.c\$sd_flag_probation = 1 then 'Yes' else 'No' end as 'Partner_On_Probation', case when p.c\$sd_flag_primary = 1 then 'Yes' else 'No' end as 'Partner_Is_Primary' from orgs op left join sa_products p on op.org_id = p.c\$parent_id left join orgs oc on p.c\$child_id = oc.org_id left join org_addrs ap on ap.oat_id = 2 and p.c\$parent_id = ap.org_id left join org_addrs ac on ac.oat_id = 2 and p.c\$child_id = ac.org_id where op.c\$needs_service_dealer = 1 order by op.c\$mbdm_name, op.c\$cbdm_name, op.name, p.c\$sd_flag_primary desc, oc.c\$mbdm_name, oc.c\$cbdm_name, oc.name limit 1 ";
$s_sql = "select * from accounts where sessionid = 'J_ECzzWbE-'";
$s_sql = "select * from accounts where acct_id = 958";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 3 and C.cf_id > 1000 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select o.org_id, c\$account_active_yes_no, c\$needs_service_dealer, c\$no_service_dealer_available, c\$sd_brand_blackmax, c\$sd_brand_huskee, c\$sd_brand_husq, c\$sd_brand_mcc, c\$sd_brand_poulan, c\$sd_brand_poulan_pro, c\$sd_brand_redmax, c\$sd_brand_we, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_group_comm, c\$sd_group_handheld, c\$sd_group_mowers, c\$sd_group_snow, c\$sd_group_tractor, c\$sd_trans_hydrogear, c\$sd_trans_peerless, c\$sd_trans_techums from orgs o where o.org_id in (41957,118837)";
$s_sql = "select * from incidents limit 10;";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select C.cf_id, C.col_name FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select * FROM incidents limit 1;";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select * from labels where tbl = 20 order by label";
$s_sql = "select L3.label, DATE_FORMAT(I.created,'%m/%d/%Y') as 'work_day', A.acct_id, CONCAT(A.first_name,' ',A.last_name) as 'name_full', count(distinct I.i_id) as 'total' from incidents I inner join accounts A on A.acct_id = I.created_by and I.source_lvl2 <> 1017 and I.created > '2010-09-16' and I.created <= '2010-09-16 23:59:59' inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 and L3.label_id in (101075,101159,101074,304760,155803,155804,101082) group by L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name); "; 
$s_sql = "select * from accounts where last_name = 'ballard';";
$s_sql = "select case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end as 'label', DATE_FORMAT(T.entered,'%m/%d/%Y') as 'work_day', A.acct_id, CONCAT(A.first_name,' ',A.last_name) as 'name_full', count(distinct T.thread_id) as 'total' from threads T inner join incidents I on T.i_id = I.i_id and T.entered > '2010-09-16' and T.entered <= '2010-09-16 23:59:59' and T.entered > I.created inner join accounts A on A.acct_id = T.acct_id inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 and L3.label_id in (101075,101159,101074,304760,155803,155804,101082) group by case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name); "; $s_sql=" select p.product_id, p.seq, p.c\$parent_id, p.c\$child_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.org_id, o.c\$lat, o.c\$lon, a.street, a.city, a.prov_id, a.postal_code, o1.c\$sd_brand_blackmax, o1.c\$sd_brand_huskee, o1.c\$sd_brand_husq, o1.c\$sd_brand_mcc, o1.c\$sd_brand_poulan, o1.c\$sd_brand_poulan_pro, o1.c\$sd_brand_redmax, o1.c\$sd_brand_we, o1.c\$sd_engine_briggs, o1.c\$sd_engine_honda, o1.c\$sd_engine_kawisaki, o1.c\$sd_engine_kohler, o1.c\$sd_flag_preferred, p.c\$sd_flag_primary, o1.c\$sd_flag_probation, o1.c\$sd_group_comm, o1.c\$sd_group_handheld, o1.c\$sd_group_mowers, o1.c\$sd_group_snow, o1.c\$sd_group_tractor, o1.c\$sd_trans_hydrogear, o1.c\$sd_trans_peerless, o1.c\$sd_trans_techums from sa_products p left join orgs o on (o.org_id = p.c\$child_id) left join orgs o1 on (p.c\$child_id = o1.org_id) inner join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id where p.c\$RecordType = 1398 and p.c\$parent_id = 118837 order by p.c\$sd_flag_primary desc, o.name "; 
$s_sql = "select * FROM incidents limit 1;";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select * FROM custom_fields C where C.tbl = 1 and cf_id > 1000 and C.data_type = 1;";
$s_sql = "select tbl, label_id, lang_id, fld, label from labels L where label = 'Belarusian';";
$s_sql = "select tbl, label_id, lang_id, fld, label from labels L where tbl = 20;";
$s_sql = "select * FROM custom_fields C where C.cf_id = 1438;";
$s_sql = "select tbl, label_id, lang_id, fld, label from labels L where label = 'Belarusian';";
$s_sql = "select * FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and C.tbl = 1 and L.tbl = 15 -- and C.cf_id = 1067 and L.label_id = 1067 -- and L.fld = 1 -- and L.lang_id = 1 ORDER BY C.cf_id, C.col_name; "; 
$s_sql = "select cf_id, field_size, required, seq, default_value, tbl, col_name, data_type, indexed, notes, mask, cfgroup_id, attr, min_val, max_val, hms_state, hms_op, active FROM custom_fields C where C.tbl = 1 ;";
$s_sql = "select M.cf_id, L.label_id, L.label FROM labels L inner join menu_items M on M.id = L.label_id and L.tbl = 20 and cf_id in (1067,1068,1080) ORDER BY M.cf_id, L.label ";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label FROM custom_fields C INNER JOIN labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name;";
$s_sql = "select * from orgs limit 500";
/* ap.street, ap.city, -- ap.prov_id as 'state', ap.postal_code, on s1.c\$child_id = o1.org_id and c\$recordtype = 1398) sa1 on o.org_id = sa1.parent_id o.org_id, o.c\$mbdm_name as 'mbdm_name', o.c\$cbdm_name as 'cbdm_name', o.c\$company_code as 'company_code', o.name, o.c\$account_number as 'account_number', oc.cert_advanced_exp_date_c as 'cert_advanced_exp_date', o.c\$no_service_dealer_available as 'no_service_dealer_available', a.street, a.city, a.prov_id as 'state', a.postal_code, sa1.child_name, case when sa1.sd_flag_primary = 1 then 'Yes' else '' end as 'sd_flag_primary', case when oc.sd_flag_probation = 1 then 'Yes' else '' end as 'sd_flag_probation', o.c\$cert_basic_exp_date as 'cert_basic_exp_date', a2.street, a2.city, a2.prov_id as 'state', a2.postal_code, oc.account_number_c, oc.org_id as 'org_id_c', o.c\$good_receiving_address as 'gra', oc.gra_c, o.c\$org_phone as 'org_phone', oc.org_phone_c, o.c\$lat, o.c\$lon, oc.lat, oc.lon left join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id left join (select c\$parent_id as 'parent_id', c\$child_id as 'child_id', o1.name as 'child_name', s1.c\$sd_flag_primary as 'sd_flag_primary', o1.c\$sd_flag_probation as 'sd_flag_probation' from sa_products s1 inner join orgs o1 on s1.c\$child_id = o1.org_id and c\$recordtype = 1398) sa1 on o.org_id = sa1.parent_id left join (select org_id, c\$account_number as 'account_number_c', c\$cert_advanced_exp_date as 'cert_advanced_exp_date_c', c\$good_receiving_address as 'gra_c', c\$org_phone as 'org_phone_c', c\$lat as 'lat', c\$lon as 'lon', c\$sd_flag_probation as sd_flag_probation from orgs) oc on sa1.child_id = oc.org_id where o.c\$needs_service_dealer = 1 and o.c\$account_active_yes_no = 1 order by o.c\$husq_cbdm_territory_num, o.c\$husq_mbdm_territory_num, o.name, sa1.sd_flag_primary desc */
/* C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label, L.tbl, L.label_id */
// (length(l0.label) > 8 and l0.label > 111111111 and l0.label < 99999999999)
// and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 ORDER BY C.cf_id, C.col_name
// $s_sql="SELECT * FROM custom_fields C where C.tbl = 1 and cf_id > 1000 and C.data_type = 1;";
// $s_sql="select * from orgs where c\$account_active_yes_no = 1 and (c\$dis_cat_desc = 'HUSQVARNA SILVER LEVEL' or c\$dis_cat_desc = 'HUSQVARNA GOLD LEVEL' or c\$dis_cat_desc = 'HUSQVARNA PLATINUM LEVEL' or c\$dis_cat_desc = 'HUSQVARNA TITANIUM LEVEL' or c\$dis_cat_desc = 'HUSQVARNA CROWN LEVEL') and c\$sold_and_unpaid > 0 and c\$date_customer_created < '2010-09-22' order by c\$date_customer_created desc limit 500";
// O.name, replace(replace(O.c\$dis_cat_desc,'HUSQVARNA ',''),' LEVEL','') as 'level',
$s_sql=" select O.org_id, O.c\$account_number					as 'account_id', O.c\$good_receiving_address			as 'gra', O.c\$account_active_yes_no			as 'active', O.c\$dis_cat_desc					as 'discount_level', O.c\$date_customer_created			as 'date_created', O.c\$coop_funds_start				as 'coop_funds_start', O.c\$coop_funds_available			as 'coop_funds_left', O.c\$financial_yr_to_date_sales		as 'sales_ytd', O.c\$financial_last_yr_sales		as 'sales_last' from orgs O where O.c\$account_active_yes_no = 1 and O.c\$financial_last_yr_sales > 0 and O.c\$date_customer_created < '2010-09-22' and cast(O.c\$financial_last_yr_sales as signed) < cast(O.c\$financial_yr_to_date_sales as signed) and ( O.c\$dis_cat_desc = 'HUSQVARNA SILVER LEVEL'	or O.c\$dis_cat_desc = 'HUSQVARNA GOLD LEVEL'		or O.c\$dis_cat_desc = 'HUSQVARNA PLATINUM LEVEL'	or O.c\$dis_cat_desc = 'HUSQVARNA TITANIUM LEVEL'	or O.c\$dis_cat_desc = 'HUSQVARNA CROWN LEVEL'	or O.c\$dis_cat_desc = 'DIXON SILVER LEVEL' 		or O.c\$dis_cat_desc = 'DIXON TITANIUM LEVEL'		 ) order by cast(O.c\$financial_last_yr_sales as signed) desc, cast(O.c\$financial_yr_to_date_sales as signed) desc "; $s_sql = "select M.cf_id, L.label_id, L.label FROM labels L inner join menu_items M on M.id = L.label_id and L.tbl = 20 and cf_id in (368) ORDER BY M.cf_id, L.label ";
$s_sql = "select * from orgs limit 500";
$s_sql = "select * from opportunities limit 10";
$s_sql = "select * from products order by id desc limit 10";
$s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by registration_date desc limit 20";
$s_sql = "select id, serial_number, model_number from ps_prodreg_contacts where c_id in (1539108,1539109,1539110,1539111,1539112,1539113)";
$s_sql = "select registration_date, SUBDATE(registration_date, INTERVAL 1 year) from ps_prodreg_contacts order by registration_date desc limit 20";
$s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by registration_date desc limit 20";
$s_sql = "select * from orgs o where o.c\$account_number = '80001' and o.c\$good_receiving_address = '02701' order by o.c\$good_receiving_address";
//1268367
$s_sql = "select * from contacts where c_id = 40";
// where c_id = 1268367
$s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by id desc limit 2000";
$s_sql = "select * from ps_sales_commissions limit 100";
$s_sql = "select * from ps_prodreg_contacts limit 100";
$s_sql = "select * from ps_sales_details limit 100";
$s_sql = "select * from ps_inv_summary limit 100";
$s_sql = "select count(*) from ps_inv_detail;";
//$s_sql = "update ps_inv_detail set name = 'HUSQVARNA PLATINUM LEVEL', serial = '10001.01', invoiceDate = '2010-10-18 09:33:39' where org_id = 1 and model = 'settlement' and linenumber = '2008' and invoicemonth = '1'";
$s_sql = "select * from ps_inv_detail where org_id = 92516 and invoiceNumber = '3745394' and linenumber = '0001' ;";
$s_sql = "select count(*) as 'rows' from ps_inv_detail where model='settlement'";
$s_sql = "select * from ps_inv_detail order by invoicedate;";
$s_sql = "select org_id, count(*) from ps_inv_detail group by org_id having count(*) > 1 order by count(*) desc;";
$s_sql = "select org_id, lineNumber, invoiceMonth, name, serial, accountNumber, invoiceDate, dueDate from ps_inv_detail where dueDate < '2010-10-21';";
// dupe check
$s_sql = "select dueDate, org_id, lineNumber, invoiceMonth, count(*) from ps_inv_detail group by dueDate, org_id, lineNumber, invoiceMonth having count(*) > 1;";
// show
$s_sql = "select dueDate, lineNumber, org_id, invoiceMonth, name, serial, accountNumber, invoiceDate from ps_inv_detail where model= 'settlement' order by dueDate, org_id;";
$s_sql = "select o.* from ps_inv_detail p inner join orgs o on p. org_id = o.org_id where p.accountNumber= '' order by org_id;";
$s_sql = "select dueDate, lineNumber, org_id, invoiceMonth, name, serial, accountNumber, invoiceDate from ps_inv_detail where accountNumber= '' order by dueDate, org_id;";
$s_sql = "select dueDate, lineNumber, org_id, invoiceMonth, name, serial, accountNumber, invoiceDate from ps_inv_detail where model= 'settlement' order by dueDate, org_id;";
$s_sql = "select org_id, lineNumber, invoiceMonth, state, sum(serial), count(org_id) from ps_inv_detail where model= 'settlement' group by org_id, lineNumber, invoiceMonth order by org_id, lineNumber, invoiceMonth, state;";
$s_sql = "select org_id, lineNumber, invoiceMonth, state, name, serial, accountNumber, dueDate, invoiceDate from ps_inv_detail where model= 'settlement' order by lineNumber, invoiceMonth, state, org_id;";
$s_sql = "select org_id, lineNumber, invoiceMonth, state, sum(serial), count(org_id) from ps_inv_detail where model= 'settlement' group by org_id, lineNumber, invoiceMonth order by org_id, lineNumber, invoiceMonth, state;";
$s_sql = "select * from ps_inv_detail;";
$s_sql = "select c\$account_active_yes_no, c\$needs_service_dealer, c\$no_dealer, c\$sd_brand_blackmax, c\$sd_brand_huskee, c\$sd_brand_husq, c\$sd_brand_mcc, c\$sd_brand_poulan, c\$sd_brand_poulan_pro, c\$sd_brand_redmax, c\$sd_brand_we, c\$sd_engine_briggs, c\$sd_engine_honda, c\$sd_engine_kawisaki, c\$sd_engine_kohler, c\$sd_flag_primary, c\$sd_flag_probation, c\$sd_group_comm, c\$sd_group_handheld, c\$sd_group_mowers, c\$sd_group_snow, c\$sd_group_tractor, c\$sd_trans_hydrogear, c\$sd_trans_peerless, c\$sd_trans_techums, c\$no_service_dealer_available from orgs o where o.org_id = 118837";
$s_sql = "select * from orgs limit 500";
$s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by id desc limit 1000";
$s_sql = "select assgn_acct_id, count(*) from tasks where due_date between '2010-11-01' and '2010-12-01' group by assgn_acct_id having count(*) > 1 order by count(*) desc";
$s_sql = "select task_id, op_id, org_id, c_id, name, notes from tasks limit 50";
$s_sql = "select T.task_id, T.due_date, T.name, T.notes, C.last_name, C.first_name, C.email, C.ph_office from tasks T left join contacts C on T.c_id = C.c_id where T.assgn_acct_id = 80 and T.due_date between '2010-11-03 00:00:00' and '2010-11-03 23:59:59' order by T.due_date limit 100";
$s_sql = "select acct_id, display_name from accounts where c\$sales_active = 1 and login = 'bhughes'";
$s_sql = "select acct_id, display_name, login, c\$sales_password from accounts where acct_id = 108";
$s_sql = "select task_id, year(due_date) as 'year', month(due_date) as 'month', day(due_date) as 'day', hour(due_date) as 'hour' from tasks where assgn_acct_id = 80 and due_date between '2010-11-02 00:00:00' and '2010-11-08 23:59:59' group by year(due_date), month(due_date), day(due_date), hour(due_date) order by year(due_date), month(due_date), day(due_date), hour(due_date) "; $s_sql = "select * from ps_prodreg_contacts where registered_by = 40 order by id desc limit 1000";
$s_sql = "select T.task_id, T.due_date, T.name, T.notes, C.last_name, C.first_name, C.email, C.ph_office, O.name, O.c\$account_number, O.c\$lat, O.c\$lon	 from tasks T left join orgs O on T.org_id = O.org_id left join contacts C on T.c_id = C.c_id order by T.due_date limit 100";
$s_sql = "select * from orgs limit 10;";
$s_sql = "select task_id, year(due_date) as 'year', month(due_date) as 'month', day(due_date) as 'day', hour(due_date) as 'hour' from tasks where assgn_acct_id = 108 and due_date between '2010-11-04 00:00:00' and '2010-11-10 23:59:59' order by year(due_date), month(due_date), day(due_date), hour(due_date) ";
$s_sql = "select I.i_id, I.subject, A.display_name, I.created, I.status_id from incidents I left join accounts A on I.assgn_acct_id = A.acct_id where I.org_id = 202987 and created > '2010-10-06 09:10:14' order by created desc ";
//statuses. where statuses.tbl = 1
// .label_id
$s_sql = "select * from statuses where statuses.tbl = 1";
$s_sql = "select * from labels where tbl = 19";
$s_sql = " select I.i_id, I.subject, A.display_name, I.created, L.label, C.first_name, C.last_name from incidents I left join labels L on I.status_id = L.label_id and L.tbl = 19 left join accounts A on I.assgn_acct_id = A.acct_id left join contacts C on I.c_id = C.c_id where I.org_id = 202987 and I.created > '2010-10-06 09:10:14' order by created desc ";
//$s_sql = "select * from incidents where org_id = 202987";
$s_sql = "select org_id, count(*) from incidents where created > '2010-10-06 09:10:14' group by org_id having count(*) > 5 order by count(*) desc";
$s_sql = "select * from incidents where org_id = 202987";
$s_sql = "select * from incidents where order by len(c\$disposition) desc limit 10";
$s_sql = "select distinct c\$disposition from incidents order by c\$disposition desc limit 100";
$s_sql = "select * from threads T inner join contacts c on T.c_id = C.c_id where i_id = 2281887";
// check for duplicates
//$s_sql = "select model,lineNumber,invoiceMonth,org_id,state,serial,name,invoiceDate,dueDate from ps_inv_detail where org_id in (select org_id from ps_inv_detail group by org_id having count(*) > 1) order by org_id,lineNumber,invoiceMonth,state;";
// show totals
//$s_sql = "select org_id,lineNumber,invoiceMonth,sum(serial),count(org_id) from ps_inv_detail where model='settlement' group by org_id,lineNumber,invoiceMonth order by org_id,lineNumber,invoiceMonth;";
// show list
$s_sql = "select model,lineNumber,invoiceMonth,state,org_id,name,serial,invoiceDate,dueDate from ps_inv_detail order by lineNumber,invoiceMonth,state;";
$s_sql = "select * from labels where tbl = 142 and label_id not in (101075,101159,101074,304760,155803,155804,101082) and fld = 1 and label in ('Consumer Support','Technical Services')";
// and label in ('Consumer Support','Technical Services')
$s_sql = "select * from contacts limit 10";
$s_sql="select source_lvl1, count(*) from incidents group by source_lvl1 order by source_lvl1";
$s_sql="select source_lvl2, count(*) from incidents group by source_lvl2 order by source_lvl2";
$s_sql = "select * from labels where tbl = 142 and label_id not in (101075,101159,101074,304760,155803,155804,101082) and fld = 1 and label in ('Consumer Support','Technical Services')";
$s_sql=" select case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end as 'Group', I.source_lvl2, I.mailbox_id, case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end as 'label', DATE_FORMAT(T.entered,'%m/%d/%Y') as 'work_day', A.acct_id, CONCAT(A.first_name,' ',A.last_name) as 'name_full', count(distinct T.thread_id) as 'total' from threads T inner join incidents I on T.i_id = I.i_id and T.entered > '2010-11-08 00:00:00' and T.entered <= '2010-11-08 23:59:59' -- and T.entered > I.created inner join accounts A on A.acct_id = T.acct_id inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 and L3.label_id in (101075,101159,101074,304760,155803,155804,101082,101126,308682) -- ,101126,308682 group by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, I.source_lvl2, I.mailbox_id, case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, I.source_lvl2, I.mailbox_id, case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name); ";
$s_sql=" select case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end as 'Type', CONCAT(A.first_name,' ',A.last_name) as 'name_full', case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end as 'label', I.source_lvl2, I.mailbox_id, DATE_FORMAT(T.entered,'%m/%d/%Y') as 'work_day', A.acct_id, count(distinct T.thread_id) as 'total' from threads T inner join incidents I on T.i_id = I.i_id and T.entered > '2010-11-08 00:00:00' and T.entered <= '2010-11-08 23:59:59' -- and I.created > '2010-11-08 00:00:00' and I.created <= '2010-11-08 23:59:59' -- and T.entered > I.created inner join accounts A on A.acct_id = T.acct_id inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 -- and L3.label_id in (101075,101159,101074,304760,155803,155804,101082,101126,308682) -- ,101126,308682 -- where I.i_id not in (select i_id from threads where entered > '2010-11-08 00:00:00' and entered <= '2010-11-08 23:59:59') group by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, I.source_lvl2, I.mailbox_id, case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, CONCAT(A.first_name,' ',A.last_name) case when L3.label in ('Consumer Support','Technical Services') then case when I.org_id > 0 then 'Technical Services' else 'Consumer Support' end else L3.label end, I.source_lvl2, I.mailbox_id, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id, ; ";
//$s_sql = "select * from incidents limit 10";
//$s_sql = "select * from threads limit 10";
$s_sql=" select case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end as 'type', L3.label, DATE_FORMAT(I.created,'%m/%d/%Y') as 'work_day', A.acct_id, CONCAT(A.first_name,' ',A.last_name) as 'name_full', count(distinct I.i_id) as 'total' from incidents I inner join accounts A on A.acct_id = I.assgn_acct_id inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 where I.created > '2010-11-11 00:00:00' and I.created <= '2010-11-11:23:59:59' and I.i_id not in (select i_id from threads) group by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name) order by case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, L3.label, DATE_FORMAT(I.created,'%m/%d/%Y'), A.acct_id, CONCAT(A.first_name,' ',A.last_name); ";
$s_sql = " select CONCAT(A.first_name,' ',A.last_name) as 'name_full', L3.label, case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end as 'type', DATE_FORMAT(T.entered,'%m/%d/%Y') as 'work_day', A.acct_id, count(distinct T.thread_id) as 'total' from threads T inner join incidents I on T.i_id = I.i_id and T.entered > '2010-11-11 00:00:00' and T.entered <= '2010-11-11 23:59:59' inner join accounts A on A.acct_id = T.acct_id inner join labels L3 on A.group_id = L3.label_id and L3.tbl = 142 and L3.lang_id = 1 and L3.fld = 1 group by CONCAT(A.first_name,' ',A.last_name), case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, L3.label, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id order by CONCAT(A.first_name,' ',A.last_name), case when I.source_lvl2 = 1001 then 'Phone' when I.source_lvl2 = 6001 then 'Phone' when I.source_lvl2 = 9001 then 'Phone' when I.source_lvl2 = 10002 then 'Phone' when I.source_lvl2 = 1017 then 'Chat' when I.source_lvl2 = 3001 then 'Email' when I.source_lvl2 = 3005 then 'Email' when I.source_lvl2 = 3006 then 'Email' when I.source_lvl2 = 3008 then 'Email' when I.source_lvl2 = 3015 then 'Email' when I.source_lvl2 = 5002 then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id not in (13,17,18) then 'Email' when I.source_lvl2 = 5001 and I.mailbox_id in (13,17,18) then 'Fax' end, L3.label, DATE_FORMAT(T.entered,'%m/%d/%Y'), A.acct_id ; ";
$s_sql = "select * from accounts where last_name = 'mccormick' order by acct_id desc limit 10";
$s_sql = "select * from incidents where created_by = 837 or assgn_acct_id = 837 order by i_id desc limit 100";
$s_sql = "select accountNumber, org_id, invoiceNumber, name, state, model, serial, quantity, invoiceDate, invoiceMonth, lineNumber, dueDate, product_group from ps_inv_detail where dueDate = '2010-12-01'";
$s_sql = "select dueDate, sum(serial), count(*) as rows from ps_inv_detail group by dueDate order by dueDate";
$s_title = "Mapping Table";
$s_sql = "select * from sa_products where product_id >= 6250 order by c\$description";
//if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,false);}
/* Account List */
$s_title = "By Account List";
$s_sql = " select DISTINCT  case when P1.c\$description > '' then P1.c\$description else O.name end as 'Mapped', P1.c\$description as 'Mapping', O.name as 'OrgName', O.c\$account_number as 'AccountID' from orgs O left join sa_products P1  on O.c\$account_number = P1.id  and P1.c\$RecordType = 1498 where O.c\$needs_service_dealer = 1  and O.c\$account_active_yes_no = 1 order by case when P1.c\$description > '' then P1.c\$description else O.name end, P1.c\$description, O.name, O.c\$account_number limit 10000 "; //if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);}
/* By Region */
$s_title = "By Region";
$s_sql = " select O.c\$mbdm_name as 'Region', count(distinct O.org_id) as 'retailers',  count(distinct P0.c\$parent_id) as 'partners' from orgs O left join sa_products P0  on O.org_id = P0.c\$parent_id  and P0.c\$RecordType = 1398 left join sa_products P1  on O.c\$account_number = P1.id  and P1.c\$RecordType = 1498 where O.c\$needs_service_dealer = 1  and O.c\$account_active_yes_no = 1 group by O.c\$mbdm_name order by O.c\$mbdm_name limit 10000 "; //if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,false);}
/* By Account */
$s_title = "By Account";
$s_sql = " select case when P1.c\$description > '' then P1.c\$description else O.name end as 'Account', count(distinct O.org_id) as 'retailers', count(distinct P0.c\$parent_id) as 'partners' from orgs O left join sa_products P0 on O.org_id = P0.c\$parent_id and P0.c\$RecordType = 1398 left join sa_products P1 on O.c\$account_number = P1.id and P1.c\$RecordType = 1498 where O.c\$needs_service_dealer = 1 and O.c\$account_active_yes_no = 1 group by case when P1.c\$description > '' then P1.c\$description else O.name end order by case when P1.c\$description > '' then P1.c\$description else O.name end limit 10000 ";
//if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,false);}
/* By Region & Account */
$s_title = "By Region & Account";
$s_sql = " select O.c\$mbdm_name, case when P1.c\$description > '' then P1.c\$description else O.name end as 'Account',  count(distinct O.org_id) as 'retailers',  count(distinct P0.c\$parent_id) as 'partners' from orgs O left join sa_products P0  on O.org_id = P0.c\$parent_id  and P0.c\$RecordType = 1398 left join sa_products P1  on O.c\$account_number = P1.id  and P1.c\$RecordType = 1498 where O.c\$needs_service_dealer = 1  and O.c\$account_active_yes_no = 1 group by O.c\$mbdm_name, case when P1.c\$description > '' then P1.c\$description else O.name end  order by O.c\$mbdm_name, case when P1.c\$description > '' then P1.c\$description else O.name end  limit 10000 "; //if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,false);}
//	O.c\$mbdm_name as 'Region',
//	O.c\$husq_cbdm_territory_num as 'RegionID',
//if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,false);}
/*
2. exec dashboard
by region retailers with dealers versus without dealers by region/by terr

summary
region, total stores, stores with dealers,. stores without dealers, ratio

detail
by account (TBD)
region, total stores, stores with dealers,. stores without dealers, ratio

detail 2
by territory
account (TBD), total stores, stores with dealers,. stores without dealers, ratio
*/
//$s_sql = "select product_id, id, disabled, c\$description from sa_products where c\$recordtype = 1498 order by disabled, c\$description, id";
//c\$account_number = '13541'
$s_sql = " select c\$rex_last_updated_date, c\$husq_cbdm_territory_num as 'RegionID', c\$mbdm_name as 'Region', c\$cbdm_name as 'Territory', c\$husq_mbdm_territory_num as 'TerritoryID', name, c\$account_number, c\$good_receiving_address, c\$needs_service_dealer, c\$account_active_yes_no from orgs where c\$account_active_yes_no = 1 and c\$rex_last_updated_date < '2010-06-01' order by c\$rex_last_updated_date, c\$good_receiving_address, c\$husq_cbdm_territory_num, c\$husq_mbdm_territory_num ";
$s_sql = "select c\$account_number, c\$good_receiving_address from orgs where c\$account_active_yes_no = 1 and c\$rex_last_updated_date < '2010-06-01' order by c\$account_number, c\$good_receiving_address ";
/* By Region, Territory & Account */
$s_title = "By Region, Territory, & Account";
$s_sql = "select case O.c\$husq_cbdm_territory_num when '001' then 'NORTH EAST' when '002' then 'NORTH CENTRAL' when '003' then 'NORTH WEST' when '004' then 'SOUTH CENTRAL' when '005' then 'SOUTH EAST' when '006' then 'SOUTH WEST' when '007' then 'ISLAND' when '008' then 'CENTRAL' when '009' then 'GREAT LAKES' when '020' then 'GSA, RR, RENTAL & OTHER' when '021' then 'CORPORATE ACCOUNTS' when '022' then 'HOUSE ACCOUNTS' when '061' then 'RENTAL REPS' when '098' then 'OPEN - NOT USED' else O.c\$husq_cbdm_territory_num+' : Not Mapped' end as 'region_name_mapped', O.c\$cbdm_name as 'Territory', O.c\$husq_mbdm_territory_num as 'TerritoryID', case when P1.c\$description > '' then P1.c\$description else O.name end as 'Account', count(distinct O.org_id) as 'retailers', count(distinct P0.c\$parent_id) as 'partners' from orgs O left join sa_products P0 on O.org_id = P0.c\$parent_id and P0.c\$RecordType = 1398 left join sa_products P1 on O.c\$account_number = P1.id and P1.c\$RecordType = 1498 and P1.disabled = 0 where O.c\$needs_service_dealer = 1 and O.c\$account_active_yes_no = 1 group by case O.c\$husq_cbdm_territory_num when '001' then 'NORTH EAST' when '002' then 'NORTH CENTRAL' when '003' then 'NORTH WEST' when '004' then 'SOUTH CENTRAL' when '005' then 'SOUTH EAST' when '006' then 'SOUTH WEST' when '007' then 'ISLAND' when '008' then 'CENTRAL' when '009' then 'GREAT LAKES' when '020' then 'GSA, RR, RENTAL & OTHER' when '021' then 'CORPORATE ACCOUNTS' when '022' then 'HOUSE ACCOUNTS' when '061' then 'RENTAL REPS' when '098' then 'OPEN - NOT USED' else O.c\$husq_cbdm_territory_num+' : Not Mapped' end, O.c\$cbdm_name, O.c\$husq_mbdm_territory_num, case when P1.c\$description > '' then P1.c\$description else O.name end order by case O.c\$husq_cbdm_territory_num when '001' then 'NORTH EAST' when '002' then 'NORTH CENTRAL' when '003' then 'NORTH WEST' when '004' then 'SOUTH CENTRAL' when '005' then 'SOUTH EAST' when '006' then 'SOUTH WEST' when '007' then 'ISLAND' when '008' then 'CENTRAL' when '009' then 'GREAT LAKES' when '020' then 'GSA, RR, RENTAL & OTHER' when '021' then 'CORPORATE ACCOUNTS' when '022' then 'HOUSE ACCOUNTS' when '061' then 'RENTAL REPS' when '098' then 'OPEN - NOT USED' else O.c\$husq_cbdm_territory_num+' : Not Mapped' end, O.c\$cbdm_name, O.c\$husq_mbdm_territory_num, case when P1.c\$description > '' then P1.c\$description else O.name end limit 10000 ";
$s_sql = "select * from sa_products where c\$RecordType = 1398";
$s_sql = "select org_id from orgs where c\$account_active_yes_no = 1 and c\$rex_last_updated_date < '2010-12-27 00:00:00' order by org_id";
$s_sql = "select * from orgs where c\$account_number = '00431' and c\$good_receiving_address = '00001'";
$s_sql = "select * from orgs where c\$account_number = '03408' and c\$good_receiving_address = '00001'";
$s_sql = "select A.name, A.lang_id, L.tbl, L.label_id, L.lang_id, L.fld, L.label from labels L inner join languages A on L.lang_id = A.lang_id order by A.name, L.label";
//$s_sql = "select * from labels order by lang_id";
//$s_sql = "select interface_id, name from interfaces order by interface_id";
/*
A.name, A.lang_id, 
C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, 
L.tbl, L.label_id, L.lang_id, L.fld, L.label 
*/
$s_sql = "select * from custom_fields C inner join labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 93 inner join languages A on L.lang_id = A.lang_id order by C.col_name;";
$s_sql = "select acct_id, display_name, c\$mobile_session from accounts where login = 'dbreeden' and (c\$sales_password = 'password' or (rtrim(c\$sales_password) = '' and 'password' = 'password') )";
$s_sql = "select * from accounts where last_name = 'breeden' limit 100";
$s_sql = "select c\$sales_password, acct_id, login, first_name, last_name, password, email_address, profile_id, display_name, attr, group_id from accounts where last_name = 'breeden'";
// and 'password' = 'password'
// or (rtrim(c\$sales_password) = '')
// c\$sales_password = 'password'
// ('password' = 'password')
// (c\$sales_password = '')
$s_sql = "select acct_id, display_name, c\$mobile_session from accounts where login = 'dbreeden' and ((c\$sales_password = 'password') or ((c\$sales_password is null) or ((c\$sales_password = '') and ('password' = 'password'))))";
$s_sql = "select c\$sales_password, acct_id, login, first_name, last_name, password, email_address, profile_id, display_name, attr, group_id from accounts where last_name = 'breeden'";
$s_sql = "select o.org_id, o.c\$lat as 'lat', o.c\$lon as 'lon', o.c\$gis_precision as 'gis_precision', o.c\$geocode_date as 'geocode_date' from orgs o where o.org_id = 202925";
$s_sql = "select o.org_id, o.c\$lat as 'lat', o.c\$lon as 'lon', o.c\$gis_precision as 'gis_precision', o.c\$geocode_date as 'geocode_date' from orgs o where o.c\$gis_precision is not null order by o.c\$gis_precision";
$s_sql = "select o.org_id, o.c\$lat as 'lat', o.c\$lon as 'lon', o.c\$gis_precision as 'gis_precision', o.c\$geocode_date as 'gis_geo_date', a.street, a.city, l.label as 'state', a.postal_code from orgs o inner join org_addrs a on o.org_id = a.org_id and a.oat_id = 2 and a.country_id = 1 and c\$account_active_yes_no = 1 and (c\$lat is null or c\$lon is null) and (o.c\$gis_precision is null) and rtrim(a.postal_code) <> '' and rtrim(a.postal_code) <> '.' inner join labels l on a.prov_id = l.label_id and a.oat_id = 2 and l.tbl = 48 and a.country_id = 1 order by a.org_id ";
$s_sql = "select o.org_id, o.c\$lat as 'lat', o.c\$lon as 'lon', o.c\$gis_precision as 'gis_precision', o.c\$geocode_date as 'gis_geo_date', a.street, a.city, l.label as 'state', a.postal_code from orgs o inner join org_addrs a on o.org_id = a.org_id and a.oat_id = 2 and a.country_id = 1 and c\$account_active_yes_no = 1 and (c\$lat is null or c\$lon is null) and (o.c\$gis_precision is null or o.c\$gis_precision = -1) and rtrim(a.postal_code) <> '' and rtrim(a.postal_code) <> '.' inner join labels l on a.prov_id = l.label_id and a.oat_id = 2 and l.tbl = 48 and a.country_id = 1 order by a.org_id ";
/*
'Get Menu Items' as 'Description', 
M.cf_id, 
L.label_id, 
L.label 
where L.label_id = 300
*/
$s_sql = "select * from labels L inner join menu_items M on M.id = L.label_id inner join custom_fields C on L.label_id = C.cf_id where L.tbl = 2 order by M.cf_id, M.seq, L.label limit 1000 ";
/* BEGIN: This is the query to get the menu items for a given table. 20 
and L.tbl = 2
and C.tbl = 2 
*/

$s_sql = "select M.cf_id, M.seq, L.label, L.label_id, L.lang_id, L.fld, C.col_name FROM labels L inner join menu_items M on M.id = L.label_id and L.fld = 1 inner join custom_fields C on L.label_id = C.cf_id where C.cf_id = 300 ORDER BY M.cf_id, M.seq, L.label limit 10000";
$s_sql = "select M.cf_id, M.seq, C.data_type, L.label, L.label_id, L.lang_id from menu_items M inner join custom_fields C on M.cf_id = C.cf_id inner join labels L on M.id = L.label_id where C.tbl = 2 and L.fld = 1 and C.cf_id = 175 and C.active = 1 order by C.cf_id, M.seq limit 100";
$s_sql = "select C.tbl, C.cf_id, C.data_type, C.field_size, C.col_name, L.label from custom_fields C inner join labels L on C.cf_id = L.label_id and L.tbl = 15 and L.fld = 1 and L.lang_id = 1 and C.tbl = 1 and C.cf_id >= 1064 order by C.cf_id, C.col_name;";
$s_sql = "select distinct c\$husq_cbdm_territory_num from orgs where c\$company_code = 'USF' and c\$husq_cbdm_territory_num in ('01','02','03','04','05','06','07','08','001','002','003','004','005','006','007','008')";
$s_sql = "select org_id, c\$account_number, c\$good_receiving_address, c\$territory_number, c\$husq_cbdm_territory_num, c\$src_sales_channel from orgs where c\$company_code = 'USF' and c\$husq_cbdm_territory_num in ('01','02','03','04','05','06','07','08','001','002','003','004','005','006','007','008') order by org_id";
$s_title = "Sales Summary by Company Code and Record Type.";
$s_sql = "select company_code,record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details group by company_code, record_type order by company_code, record_type";
$s_sql = "select company_code,record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details group by company_code, record_type order by company_code, record_type";

$s_sql = "select company_code, record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details group by company_code, record_type order by company_code, record_type";
$s_title = "Sales Details by Company Code, and Record Type.";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

$s_sql = "select * from ps_sales_details order by id desc limit 50";
$s_title = "Last 50 IDs";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

// 118875,119709,119783
$s_sql = "select org_id, c\$company_code, c\$account_number, c\$good_receiving_address, c\$territory_number, c\$husq_cbdm_territory_num as 'region_number', c\$src_sales_channel as 'territory_number' from orgs 
where org_id in (118875,119709,119783)
order by org_id";
$s_title = "Orgs by org id.";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);


$s_sql = "select company_code, region_number, record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details group by company_code, region_number, record_type order by company_code, region_number, record_type";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

$s_sql = "select company_code, region_number, gra, record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details group by company_code, region_number, gra, record_type order by company_code, region_number, gra, record_type";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

$s_sql = "select org_id, company_code, gra, account_number, region_number, record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details 
where org_id = 51244
group by org_id, company_code, gra, account_number, region_number, record_type 
order by org_id, company_code, gra, account_number, region_number, record_type
";
$s_title = "ID.";
$s_sql = "select * from ps_sales_details where id = 22961609 order by id";

/*
$s_key = $CompanyCode."_".$AccountNumber."_".trim($GRA)."_".$RegionNumber_key."_".$SalesChannel_key;
*/

$s_rn = "2";
$s_gr = "3";
$s_an = "23530";
$s_cc = "USF";
/*
USF_23530_3_2_X
USF_28283_1_2_X
USF_28283_3_2_X
USF_33741_1_2_X
*/

$s_sql = "select org_id, c\$company_code, c\$account_number, c\$good_receiving_address, c\$territory_number, c\$husq_cbdm_territory_num as 'region_number', c\$src_sales_channel as 'territory_number' from orgs 
where c\$company_code = '$s_cc'
and c\$account_number like '%$s_an%'
and c\$husq_cbdm_territory_num like '%$s_rn%'
and c\$good_receiving_address like '%$s_gr%'
order by org_id";
$s_title = "Orgs by org id.";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

$s_sql = "select org_id, company_code, gra, account_number, region_number, record_type, count(*) as 'rows', sum(convert(ytd,decimal(12,2))) as 'ytd', min(update_date) as 'min', max(update_date) as 'max' from ps_sales_details 
where company_code = '$s_cc'
and account_number like '%$s_an%'
and region_number like '%$s_rn%'
and gra like '%$s_gr%' 
group by org_id, company_code, gra, account_number, region_number, record_type 
order by org_id, company_code, gra, account_number, region_number, record_type
";
$s_title = "Sales Details.";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);

// EXTRACT(DAY FROM update_date)
$s_title = "Sales Details by Company Code and Record Type.";
$s_sql = "
select 
	company_code, 
	record_type, 
	count(*) as 'rows', 
	sum(convert(lytd,decimal(12,2))) as 'lytd',
	sum(convert(ytd,decimal(12,2))) as 'ytd',
	min(update_date) as 'min',
	max(update_date) as 'max'
from 
	ps_sales_details 
where 
	company_code <> 'foo'
group by 
	company_code, 
	record_type 
order by 
	company_code, 
	record_type 
";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",true,false,false,true,false);

//USF_24399_2_X_X_2607.19 | 50988 |

$s_sql = "
select 
	*
from 
	ps_sales_details 
where 
	company_code = 'USF'
	and ytd = '362.3'
	and region_number = 1
	and record_type = 1
order by 
	org_id
";

$s_sql = "
select p.product_id, p.seq, p.c\$parent_id, p.c\$child_id, o.name, o.c\$account_number, o.c\$good_receiving_address, o.org_id, o.c\$lat, o.c\$lon, a.street, a.city, a.prov_id, a.postal_code, 
	o1.c\$sd_brand_blackmax, o1.c\$sd_brand_huskee, o1.c\$sd_brand_husq, o1.c\$sd_brand_mcc, o1.c\$sd_brand_poulan, o1.c\$sd_brand_poulan_pro, o1.c\$sd_brand_redmax, o1.c\$sd_brand_we, 
	o1.c\$sd_engine_briggs, o1.c\$sd_engine_honda, o1.c\$sd_engine_kawisaki, o1.c\$sd_engine_kohler, o1.c\$sd_flag_preferred, p.c\$sd_flag_primary, o1.c\$sd_flag_probation, 
	o1.c\$sd_group_comm, o1.c\$sd_group_handheld, o1.c\$sd_group_mowers, o1.c\$sd_group_snow, o1.c\$sd_group_tractor, o1.c\$sd_trans_hydrogear, o1.c\$sd_trans_peerless, o1.c\$sd_trans_techums,
	o.c\$org_phone
	from sa_products p 
	left join orgs o
		on  (o.org_id = p.c\$parent_id) 
	left join orgs o1
		on (p.c\$child_id = o1.org_id)
	inner join org_addrs a on a.oat_id = 2 and o.org_id = a.org_id
	where p.c\$RecordType = 1398
	and  p.c\$child_id  = 34885
	order by p.c\$sd_flag_primary desc, o.name
";
//$s_sql = "select * from orgs limit 1";
//$s_sql = "select * from sa_products where c\$RecordType = 1398 and (c\$parent_id = 34885 or c\$child_id = 34885) order by product_id desc limit 100 ";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,true,true,true);
//

$s_sql = "
select
	P1.c\$description,
	O.name,
	count(distinct O.org_id) as 'retailers', 
	count(distinct P0.c\$parent_id) as 'partners'
from orgs O
left join sa_products P0 
	on O.org_id = P0.c\$parent_id 
	and P0.c\$RecordType = 1398
left join sa_products P1 
	on O.c\$account_number = P1.id 
	and P1.c\$RecordType = 1498
where O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
	and name like 'discount ag%'
group by
	P1.c\$description,
	O.name
order by
	P1.c\$description,
	O.name
";
/*
left join sa_products P0 
	on O.org_id = P0.c\$parent_id 
	and P0.c\$RecordType = 1398
	
	and name like 'discount ag%'
	case when P1.c\$description > '' then P1.c\$description else O.name end as 'Account',
having 
	count(distinct O.org_id) <> count(distinct P1.product_id) 
	
*/
$s_sql = "
select
	O.name,
	count(distinct O.org_id) as 'retailers', 
	count(distinct P1.product_id) as 'partners'
from orgs O
left join sa_products P1 
	on O.c\$account_number = P1.id 
	and P1.c\$RecordType = 1498
where O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
group by
	O.name
order by
	O.name
";

$s_title = "Orgs by org id.";


$s_sql = "
select
	O.name,
	sum(case when O.c\$needs_service_dealer = 1 then 1 else 0 end) as 'retailer',
	count(distinct O.org_id) as 'retailers', 
	count(distinct P1.c\$parent_id) as 'partners'
from orgs O
left join sa_products P1 
	on O.c\$account_number = P1.id 
	and P1.c\$RecordType = 1498
where O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
	and name like 'Family Farm%'
group by
	O.name
having 
	count(distinct O.org_id) <> count(distinct P1.product_id) 
order by
	O.name
";


$s_sql = "select org_id, name, c\$needs_service_dealer, c\$company_code, c\$account_number, c\$good_receiving_address, c\$territory_number, c\$husq_cbdm_territory_num as 'region_number', c\$src_sales_channel as 'territory_number', c\$account_active_yes_no as 'active' 
from orgs 

order by org_id
";

$s_sql = "
select
	O.name,
	O2.name
from orgs O
left join sa_products P1 
	on O.org_id = P1.c\$parent_id
	and P1.c\$RecordType = 1398
left join orgs O2
	on P1.c\$child_id = O2.org_id
where O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
	and O.name like 'wal-mart%'
order by
	O.name,
	O2.name
";




$s_sql = "
select
	O.name,
	sum(case when O.c\$needs_service_dealer = 1 then 1 else 0 end) as 'retailer',
	count(distinct P1.product_id) as 'partners'
from orgs O
left join sa_products P1 
	on O.org_id = P1.id 
	and P1.c\$RecordType = 1398
where O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
	and not (O.name like 'home depot%' or O.name like 'sears%' or O.name like 'fastenal%' or O.name = 'nothern tool and equipment')
group by
	O.name
having 
	sum(case when O.c\$needs_service_dealer = 1 then 1 else 0 end) <> count(distinct P1.product_id) 
order by
	O.name
";

//c$sd_flag_primary
$s_sql = "
select * from sa_products 
where c\$RecordType = 1398
order by product_id desc
limit 10
";


// left join orgs O2
	// on P.c\$child_id = O2.org_id
	// and P.c\$RecordType = 1398

$s_sql = "
select
	YEAR(P.updated) as 'Y',
	MONTH(P.updated) as 'M',
	count(*) as 'done'
from orgs O1
inner join sa_products P
	on O1.org_id = P.c\$parent_id
	and P.c\$RecordType = 1398
where
	O1.c\$needs_service_dealer = 1 
	and O1.c\$account_active_yes_no = 1
	and P.c\$child_id is not null
group by
	YEAR(P.updated),
	MONTH(P.updated)
order by
	YEAR(P.updated),
	MONTH(P.updated)
";
//CONCAT('runit(',O1.org_id,');') AS 'out'
$s_sql = "
select
count(*) as 'total'	
from orgs O1
where
	O1.c\$needs_service_dealer = 1 
	and O1.c\$account_active_yes_no = 1
	and (O1.name like 'Sears%' or O1.name like 'Fastenal%' or O1.name like 'NORTHERN TOOL%')
";
//O1.name like 'Home Depot%' or 

// O.c\$needs_service_dealer as 'needs_service_dealer', 
// O.c\$account_active_yes_no as 'account_active' 
/*
O2.name 						as 'org_name_part', 
O2.c\$company_code				as 'company_code_part', 
O2.c\$account_number			as 'account_number_part', 
O2.c\$good_receiving_address	as 'good_receiving_address_part', 
O2.c\$husq_cbdm_territory_num	as 'region_number_part', 
O2.c\$territory_number			as 'territory_number_part', 
O2.c\$src_sales_channel			as 'territory_number_part' 
left join orgs O2
on P.childID = O2.org_id
O.c\$territory_number			as 'territory_number', 
O.c\$src_sales_channel			as 'territory_number', 
P.parentID

	OS.c\$husq_cbdm_territory_num	as 'region_number',
	, 
	OP.c\$husq_cbdm_territory_num	as 'region_number'
*/
$s_title = "Service Dealers and Partners";
$s_sql = "
select
	OS.org_id, 
	OS.name 						as 'org_name', 
	A.city,
	L.label,
	OS.c\$company_code				as 'company_code', 
	OS.c\$account_number			as 'account_number', 
	OS.c\$good_receiving_address	as 'good_receiving_address', 
	case P.c\$sd_flag_primary
	when 1 then '<strong>Yes</strong>' 
	when 0 then 'No'
	else '' 
	end								as 'primary',	
	OP.org_id,
	OP.name 						as 'org_name', 
	OP.c\$company_code				as 'company_code', 
	OP.c\$account_number			as 'account_number', 
	OP.c\$good_receiving_address	as 'good_receiving_address'
from orgs OS
left join org_addrs A
	on OS.org_id = A.org_id
	and A.oat_id = 2
left join labels L
	on A.prov_id = L.label_id 
	and A.oat_id = 2
	and L.tbl = 48
left join sa_products P
	on OS.org_id = P.c\$parent_id
	and P.c\$RecordType = 1398
left join orgs OP
	on P.c\$child_id = OP.org_id
	and P.c\$RecordType = 1398
where 
	P.c\$parent_id is null
	and OS.c\$needs_service_dealer = 1 
	and OS.c\$account_active_yes_no = 1
order by
	OS.name,
	P.c\$sd_flag_primary desc, 
	OP.name
";
//$s_sql = "select * from sa_products where c\$RecordType = 1398 order by product_id limit 1";
$s_sql = "
select
	L.label,
	A.city,
	OS.c\$account_number as 'account_number',
	count(*) as 'accounts'
from orgs OS
left join org_addrs A
	on OS.org_id = A.org_id
	and A.oat_id = 2
left join labels L
	on A.prov_id = L.label_id 
	and A.oat_id = 2
	and L.tbl = 48
left join sa_products P
	on OS.org_id = P.c\$parent_id
	and P.c\$RecordType = 1398
where 
	P.c\$parent_id is null
	and OS.c\$needs_service_dealer = 1 
	and OS.c\$account_active_yes_no = 1
group by 
	L.label,
	A.city,
	OS.c\$account_number
order by
	count(*) desc,
	L.label,
	A.city,
	OS.c\$account_number
";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",true,false,false,true,false);
// $s_sql = "select city from org_addrs a left  a.oat_id = 2 ";
$s_title = "Service Dealers and Partners";
$s_sql = "
select
	O.org_id, 
	O.name 							as 'org_name', 
	A.city,
	O.c\$company_code				as 'company_code', 
	O.c\$account_number				as 'account_number', 
	O.c\$good_receiving_address		as 'good_receiving_address', 
	O.c\$husq_cbdm_territory_num	as 'region_number'
from orgs O
left join org_addrs A
	on O.org_id = A.org_id
	and A.oat_id = 2
left join
	(
	select distinct 
		c\$parent_id	as 'parentID',
		c\$RecordType	as 'rec_type',
		c\$child_id		as 'childID'
	from sa_products 
	where c\$RecordType = 1398
	) P
	on O.org_id = P.parentID
where 
	P.parentID is null
	and O.c\$needs_service_dealer = 1 
	and O.c\$account_active_yes_no = 1
order by
	O.name
";
//echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",true,false,false,true,false);

$s_title = "Sales Details by Company Code, Region Number, and Record Type.";
$s_sql = "
select 
	company_code, 
	record_type, 
	count(*) as 'rows', 
	sum(convert(lytd,decimal(12,2))) as 'lytd',
	sum(convert(ytd,decimal(12,2))) as 'ytd',
	min(update_date) as 'min',
	max(update_date) as 'max'
from 
	ps_sales_details 
where 
	company_code <> 'foo'
group by 
	company_code, 
	record_type 
order by 
	company_code, 
	record_type 
";
echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",true,false,false,true,false);

$i_type = 0;
$s_title = "";
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<title><?php echo $s_title; ?></title>
</head>
<body>
<?php
/*
HUSQVARNA CROWN LEVEL		20%
HUSQVARNA PLATINUM LEVEL	15%
HUSQVARNA GOLD LEVEL 		10%
HUSQVARNA TITANIUM LEVEL	8%
HUSQVARNA SILVER LEVEL		6%
*/
/* ----------- gen_query parameters -----------
$s_title 				Required
$i_type 				Required
$server 				Required
$username				Required
$password				Required
$dbname 				Required
$s_sql 					Required
$i_random				Default: 0		integer
$b_useKeywordChecking	Default: true 	boolean
$s_font_size			Default: .66em	string
$b_showRowNumber		Default: false 	boolean
$b_showSQL				Default: false 	boolean
$b_showFieldlist		Default: false 	boolean
$b_bottomHeader	 		Default: false 	boolean
$b_bare	 				Default: false 	boolean
$b_showFieldSize		Default: false 	boolean
----------- gen_query parameters ----------- */
if ($i_type == 1) {echo $objSQL->gen_query($s_title,1,"odbcnj01.rightnowtech.com","","","husqvarna",$s_sql,rand(11111,999999),false,".66em",false,false,false,true);}
if ($i_type == 2) {echo $objSQL->gen_query($s_title,2,"CHA-SSQL03,1433","data_writer","hWCiKNs1U4","agent",$s_sql);}
if ($i_type == 3) {echo $objSQL->gen_query($s_title,1,"Cisco","","","db_cra",$s_sql);}
?>
</body>
</html>