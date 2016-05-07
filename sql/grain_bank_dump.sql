-- CREATE DATABASE  IF NOT EXISTS `grain_bank` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `1873790_braddoro`;
-- MySQL dump 10.13  Distrib 5.5.40, for debian-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: grain_bank
-- ------------------------------------------------------
-- Server version	5.5.40-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `grain_inventory`
--

DROP TABLE IF EXISTS `grain_inventory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grain_inventory` (
  `grainInventoryID` int(11) NOT NULL AUTO_INCREMENT,
  `grainID_fk` int(11) unsigned NOT NULL,
  `userID_fk` int(11) NOT NULL,
  `orderID` int(11) DEFAULT NULL,
  `transactionAmount` int(11) NOT NULL,
  `transactionDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`grainInventoryID`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `grain_inventory`
--

LOCK TABLES `grain_inventory` WRITE;
/*!40000 ALTER TABLE `grain_inventory` DISABLE KEYS */;
INSERT INTO `grain_inventory` VALUES (1,1,1,1067,55,'2015-03-13 04:00:00'),(2,1,1,1067,-23,'2015-03-13 04:00:00'),(3,2,1,1111,55,'2015-04-20 04:00:00'),(4,3,1,1111,55,'2015-04-20 04:00:00'),(5,4,1,1111,50,'2015-04-20 04:00:00'),(6,4,1,1112,-10,'2015-04-20 04:00:00'),(7,3,1,1145,-18,'2015-05-08 04:00:00');
/*!40000 ALTER TABLE `grain_inventory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `grain_type`
--

DROP TABLE IF EXISTS `grain_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grain_type` (
  `grainID` int(11) NOT NULL AUTO_INCREMENT,
  `grain_type` varchar(100) NOT NULL,
  `addedDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`grainID`),
  UNIQUE KEY `grain_type_UNIQUE` (`grain_type`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `grain_type`
--

LOCK TABLES `grain_type` WRITE;
/*!40000 ALTER TABLE `grain_type` DISABLE KEYS */;
INSERT INTO `grain_type` VALUES (1,'Best Malz Red X','2015-03-13 04:00:00'),(2,'Weyermann Pale Ale Malt','2015-04-20 04:00:00'),(3,'Castle Pilsen','2015-04-20 04:00:00'),(4,'Bairds Marris Otter Pale Malt','2015-04-20 04:00:00');
/*!40000 ALTER TABLE `grain_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `grain_user`
--

DROP TABLE IF EXISTS `grain_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grain_user` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(100) NOT NULL,
  `active` char(1) NOT NULL,
  `addedDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`userID`),
  UNIQUE KEY `grain_user_UNIQUE` (`userName`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `grain_user`
--

LOCK TABLES `grain_user` WRITE;
/*!40000 ALTER TABLE `grain_user` DISABLE KEYS */;
INSERT INTO `grain_user` VALUES (5,'Brad Hughes','Y','2015-05-08 20:31:07');
/*!40000 ALTER TABLE `grain_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'grain_bank'
--
/*!50003 DROP PROCEDURE IF EXISTS `grainBalance` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `grainBalance`(in p_userID int)
BEGIN

select 
	t.grain_type, 
	sum(i.transactionAmount) as 'balance',
	max(transactionDate) as 'lastChange'
from grain_bank.grain_type t
left join grain_bank.grain_inventory i
	on t.grainID = i.grainID_fk
where
	i.userID_fk = p_userID
group by 
	t.grain_type
order by 
	t.grain_type;


END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `transactionDetail` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `transactionDetail`(in p_userID int)
BEGIN

select 
	t.grain_type, 
	i.transactionAmount,
	i.orderID,
	i.transactionDate
from grain_bank.grain_type t
left join grain_bank.grain_inventory i
	on t.grainID = i.grainID_fk
where 
	i.userID_fk = p_userID
order by 
	i.transactionDate,
	i.transactionAmount desc,
	t.grain_type;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-05-10 21:37:55
