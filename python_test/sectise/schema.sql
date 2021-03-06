drop batabase if exists awessome;
create database awesome;
use awesome;
grant select,insert,update,delete on awesome.* to 'www-data'@'localhost' identified by 'www-data'
create table users (     
    `id` varchar(50) not null,  
    `email` varchar(50) not null,  
    `password` varchar(50) not null,
    `admin` bool not null,   
    `name` varchar(50) not null, 
    `image` varchar(500) not null,   
    `created_at` real not null,   
    unique key `idx_email` (`email`),
    key `idx_created_at` (`created_at`),    
    primary key (`id`) 
    )
    engine=innodb default charset=utf8;
