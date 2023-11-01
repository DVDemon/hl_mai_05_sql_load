use archdb;
create database my_database; 
show tables;
drop table User;
CREATE TABLE IF NOT EXISTS User 
    (`id` INT NOT NULL AUTO_INCREMENT,
    `first_name` VARCHAR(128),
    `last_name` VARCHAR(128),
    `email` VARCHAR(128),
    `title` VARCHAR(128) ,
    PRIMARY KEY (`id`));
describe User;

select * from User where id<10;
explain select * from User where id=10;
explain select * from User where first_name='Carl';
explain select * from User where id>10 and id<20;
explain select * from User where email='William_Norman4908@ckzyi.us';
alter table User modify first_name varchar(128);
alter table User modify last_name varchar(128);
create index ln_fn using btree on User(last_name,first_name);

explain select * from User where first_name like 'Elle%' and last_name like 'A%';

explain select * from User where last_name like 'Car%';
-- insert into User (first_name,last_name,email,title) values ('Иван','Иванов','ivanov@yandex.ru','господин');
-- SELECT LAST_INSERT_ID();
-- select * from User where id=LAST_INSERT_ID();
-- delete from User where id= 100001;
show index from User;
explain select * from User where first_name like 'Ca%' and last_name like 'V%';
explain format=json select * from User where first_name='Elle%' and last_name='A%';
create index fn_ln on User(first_name);
explain select * from User where first_name like 'Ca%' and last_name like 'V%';
drop index fn_ln on User;

drop table sales;
CREATE TABLE sales (
bill_no INT NOT NULL,
bill_date TIMESTAMP NOT NULL,
customer_id VARCHAR(15) NOT NULL,
amount DECIMAL(8, 2) NOT NULL
) PARTITION BY RANGE (UNIX_TIMESTAMP(bill_date)) (
PARTITION sales_2020_q1 VALUES LESS THAN (UNIX_TIMESTAMP('2020-04-01')),
PARTITION sales_2020_q2 VALUES LESS THAN (UNIX_TIMESTAMP('2020-07-01')),
PARTITION sales_2020_q3 VALUES LESS THAN (UNIX_TIMESTAMP('2020-10-01')),
PARTITION sales_2020_q4 VALUES LESS THAN (UNIX_TIMESTAMP('2025-01-01'))
);

create index bd on sales(bill_date);
insert into sales (bill_no,bill_date,customer_id,amount) VALUES (1, CURRENT_TIMESTAMP(),1,10);

explain partitions select * from sales where bill_date = UNIX_TIMESTAMP('2020-04-01');
explain partitions select * from sales where bill_date = CURRENT_TIMESTAMP();

SELECT * FROM information_schema.partitions WHERE TABLE_SCHEMA='archdb' AND TABLE_NAME = 'sales' AND PARTITION_NAME IS NOT NULL
