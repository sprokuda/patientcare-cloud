#pragma once


const char* De_FollowAppTest = " \
create or replace procedure \"dba\".\"de_FollowApp\"(                                       			\
    in @app_book_id              integer,                                                   			\
    in @app_date                 date,                                                      			\
    in @column_type_status_att   char(128) default 'Attended',                              			\
    in @column_type_status_stop  char(128) default 'No_PatientCare'			\
    )			\
    begin			\
			\
			\
declare @attend_status        char(128);			\
declare @no_FollowApp_status  char(128);			\
			\
declare @Treatment_Type_ID    smallint;			\
declare @Treatment_Type_Name  varchar(20);			\
declare @mcount               integer;			\
			\
declare local temporary table \"ltt_FollowApp\"(			\
    \"Patient_ID\"              varchar(30),			\
    \"Patient_Title\"           char(6),			\
    \"Patient_First_Name\"      varchar(30),			\
    \"Patient_Last_Name\"       varchar(30),			\
    \"Patient_DOB\"             date,			\
    \"Appointment_ID\"          integer,			\
    \"Appointment_Date\"        date,			\
    \"Appointment_Time\"        time,			\
    \"Appointment_Status\"      char(12),			\
    \"Db_Code\"                 integer,			\
    \"Pracitioner_ID\"          varchar(9),			\
    \"Pracitioner_Type_ID\"     smallint,			\
    \"Pracitioner_Title\"       char(6),			\
    \"Pracitioner_First_Name\"  varchar(20),			\
    \"Practitioner_Last_Name\"  varchar(22),			\
    \"Clinic_ID\"               varchar(20),			\
    \"Clinic_Name\"             varchar(50),			\
    \"Treatment_Type_ID\"       smallint,			\
    \"Treatment_Type_Name\"     varchar(20),			\
    \"Patient_Mobile\"          varchar(20),			\
    \"Patient_Email\"           varchar(60)) on commit delete rows;			\
			\
end; \
set @attend_status = null;			\
select			\
\"Column_Type_Status_Abbreviatio\"			\
into			\
@attend_status			\
from \"dba\".\"a_slot_column_status_types\"			\
where(Column_Type_Status_Description = @column_type_status_att);			\
if (coalesce(@attend_status, '') = '')  then			\
set @attend_status = char(255);			\
end if;			\
//			\
			\
set @no_FollowApp_status = null;			\
select			\
\"Column_Type_Status_Abbreviatio\"			\
into			\
@no_FollowApp_status			\
from \"dba\".\"a_slot_column_status_types\"			\
where(Column_Type_Status_Description = @column_type_status_stop);			\
if (coalesce(@no_FollowApp_status, '') = '')  then			\
set @no_FollowApp_status = char(255);			\
end if;			\
//			\
    \
//			\
select			\
\"Patient_ID\",			\
\"Patient_Title\",			\
\"Patient_First_Name\",			\
\"Patient_Last_Name\",			\
\"Patient_DOB\",			\
\"Appointment_ID\",			\
\"Appointment_Date\",			\
\"Appointment_Time\",			\
\"Appointment_Status\",			\
\"Db_Code\",			\
\"Pracitioner_ID\",			\
\"Pracitioner_Title\",			\
\"Pracitioner_First_Name\",			\
\"Practitioner_Last_Name\",			\
\"Clinic_ID\",			\
\"Clinic_Name\",			\
\"Treatment_Type_ID\",			\
\"Treatment_Type_Name\",			\
\"Patient_Mobile\",			\
\"Patient_Email\"			\
from \"ltt_FollowApp\"			\
where(\"Treatment_Type_ID\" = 9 or			\
    (\"Treatment_Type_ID\" <> 9 and \"Pracitioner_Type_ID\" = 1));			\
			\
end;";



const char* De_FollowApp1 = " \
create or replace procedure \"dba\".\"de_FollowApp\"(                                               \
in @app_book_id              integer,                                               \
in @app_date                 date,                                               \
in @column_type_status_att   char(128) default 'Attended',                                               \
in @column_type_status_stop  char(128) default 'No_PatientCare'                                               \
)                                               \
begin                                               \
                                               \
                                               \
declare @attend_status        char(128);                                               \
declare @no_FollowApp_status  char(128);                                               \
                                               \
declare @Treatment_Type_ID    smallint;                                               \
declare @Treatment_Type_Name  varchar(20);                                               \
declare @mcount               integer;                                               \
                                               \
declare local temporary table \"ltt_FollowApp\"(                                               \
    \"Patient_ID\"              varchar(30),                                               \
    \"Patient_Title\"           char(6),                                               \
    \"Patient_First_Name\"      varchar(30),                                               \
    \"Patient_Last_Name\"       varchar(30),                                               \
    \"Patient_DOB\"             date,                                               \
    \"Appointment_ID\"          integer,                                               \
    \"Appointment_Date\"        date,                                               \
    \"Appointment_Time\"        time,                                               \
    \"Appointment_Status\"      char(12),                                               \
    \"Db_Code\"                 integer,                                               \
    \"Pracitioner_ID\"          varchar(9),                                               \
    \"Pracitioner_Type_ID\"     smallint,                                               \
    \"Pracitioner_Title\"       char(6),                                               \
    \"Pracitioner_First_Name\"  varchar(20),                                               \
    \"Practitioner_Last_Name\"  varchar(22),                                               \
    \"Clinic_ID\"               varchar(20),                                               \
    \"Clinic_Name\"             varchar(50),                                               \
    \"Treatment_Type_ID\"       smallint,                                               \
    \"Treatment_Type_Name\"     varchar(20),                                               \
    \"Patient_Mobile\"          varchar(20),                                               \
    \"Patient_Email\"           varchar(60)) on commit delete rows;                                               \
                                               \
set @attend_status = null;                                               \
select                                               \
\"Column_Type_Status_Abbreviatio\"                                               \
into                                               \
@attend_status                                               \
from \"dba\".\"a_slot_column_status_types\"                                               \
where(Column_Type_Status_Description = @column_type_status_att);                                               \
if (coalesce(@attend_status, '') = '')  then                                               \
set @attend_status = char(255);                                               \
end if;                                               \
//                                               \
                                               \
set @no_FollowApp_status = null;                                               \
select                                               \
\"Column_Type_Status_Abbreviatio\"                                               \
into                                               \
@no_FollowApp_status                                               \
from \"dba\".\"a_slot_column_status_types\"                                               \
where(Column_Type_Status_Description = @column_type_status_stop);                                               \
if (coalesce(@no_FollowApp_status, '') = '')  then                                               \
set @no_FollowApp_status = char(255);                                               \
end if;                                               \
//                                               \
for \"f\" as \"cur\" dynamic scroll cursor for                                               \
select distinct                                               \
\"a_appointments\".\"pat_id\"  @pat_id                                               \
from \"dba\".\"a_appointments\"                                               \
where(\"a_appointments\".\"app_book_id\" = @app_book_id and                                               \
    \"a_appointments\".\"app_date\" = @app_date and                                               \
    \"a_appointments\".\"pat_id\"      is not null and                                               \
                                               \
                                               \
    locate(cast(\"a_appointments\".\"status\" as binary),                                               \
        cast(@no_FollowApp_status as binary)) = 0)                                               \
    do                                               \
                                               \
        set @Treatment_Type_ID = null;                                               \
set @Treatment_Type_Name = null;                                               \
if exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '322', '88322', 'D322', 'S322',                                               \
            '323', '88323', 'D323', 'S323',                                               \
            '324', '88324', 'D324', 'S324',                                               \
            '88326',                                               \
            'D331', 'S331',                                               \
            '332', 'S332',                                               \
            '337', 'D337', 'S337',                                               \
            '338', 'D338', 'S338',                                               \
            '341', 'D341', 'S341',                                               \
            '343', 'S343',                                               \
            '344', 'S344',                                               \
            '345', 'S345',                                               \
            '351', '88351', 'D351', 'S351',                                               \
            '352', 'S352',                                               \
            '353', 'S353',                                               \
            '354', 'S354',                                               \
            '355', 'S355',                                               \
            '359', 'S359',                                               \
            '361', 'S361',                                               \
            '363', 'S363',                                               \
            '365', 'S365',                                               \
            '366', 'S366',                                               \
            '371', 'S371',                                               \
            '373', 'S373',                                               \
            '375', 'S375',                                               \
            '376', 'S376',                                               \
            '377', 'D377', 'S377',                                               \
            '378', 'D378', 'S378',                                               \
            '379', 'S379',                                               \
            '381', 'D381', 'S381',                                               \
            '382', 'S382',                                               \
            '384', '88384', 'D384', 'S384',                                               \
            '385', 'S385',                                               \
            '386', '88386', 'D386', 'S386',                                               \
            '387', '88387', 'D387', 'S387',                                               \
            '388', 'S388',                                               \
            '389', 'S389',                                               \
            '391', 'D391', 'S391',                                               \
            '392', '88392', 'D392', 'S392',                                               \
            '393', 'S393',                                               \
            '394', 'S394',                                               \
            '395', 'S395')))  then                                               \
    set @Treatment_Type_ID = 1;                                               \
set @Treatment_Type_Name = 'Oral Surgery';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '311', '88311', 'D311', 'S311',                                               \
            '314', '88314', 'D314', 'S314',                                               \
            '316', '88316')))  then                                               \
    set @Treatment_Type_ID = 2;                                               \
set @Treatment_Type_Name = 'Extraction';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '411', '88411', 'D411', 'S411',                                               \
            '412', '88412', 'D412', 'S412',                                               \
            '413',                                               \
            '414', '88414', 'D414', 'S414',                                               \
            '415', '88415', 'D415', 'S415',                                               \
            '455', '88455', 'D455', 'S455',                                               \
            '417', '88417', 'D417', 'S417',                                               \
            '419', '88419', 'D419', 'S419',                                               \
            '421', '88421', 'D421', 'S421',                                               \
            '431', 'D431', 'S431',                                               \
            '432', 'D432', 'S432',                                               \
            '437', 'D437', 'S437',                                               \
            '438', 'D438', 'S438')))  then                                               \
    set @Treatment_Type_ID = 3;                                               \
set @Treatment_Type_Name = 'Endodontics';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '661', 'D661', 'S661',                                               \
            '671', 'D671', 'S671',                                               \
            '672', 'D672', 'S672',                                               \
            '673', 'D673', 'S673',                                               \
            '684', 'S684',                                               \
            '688', 'S688',                                               \
            '691', 'S691')))  then                                               \
    set @Treatment_Type_ID = 4;                                               \
set @Treatment_Type_Name = 'Implants';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '88576',                                               \
            '586', 'D586', 'S586',                                               \
            '587', 'D587', 'S587',                                               \
            '588', 'D588', 'S588',                                               \
            '611', 'D611', 'S611',                                               \
            '613', 'D613', 'S613',                                               \
            '615', 'D615', 'S615',                                               \
            '618', 'D618', 'S618')))  then                                               \
";

const char* De_FollowApp2 = "\
    set @Treatment_Type_ID = 5;                                               \
set @Treatment_Type_Name = 'Crown & Bridge';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '711', 'D711', 'S711',                                               \
            '712', 'D712', 'S712',                                               \
            '719', 'D719', 'S719',                                               \
            '721', '88721', 'D721', 'S721',                                               \
            '722', '88722', 'D722', 'S722',                                               \
            '727', 'D727', 'S727',                                               \
            '728', 'D728', 'S728')))  then                                               \
    set @Treatment_Type_ID = 6;                                               \
set @Treatment_Type_Name = 'Prosthodontics';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '811', 'D811', 'S811',                                               \
            '821', 'D821', 'S821',                                               \
            '823', 'D823', 'S823',                                               \
            '824',                                               \
            '829', 'D829', 'S829',                                               \
            '831', 'D831', 'S831',                                               \
            '881', 'D881', 'S881')))  then                                               \
    set @Treatment_Type_ID = 7;                                               \
set @Treatment_Type_Name = 'Orthodontics';                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '117', 'D117', 'S117',                                               \
            '118',                                               \
            '119')))  then                                               \
    set @Treatment_Type_ID = 8;                                               \
set @Treatment_Type_Name = 'Bleaching';                                               \
                                               \
elseif exists(select top 1                                               \
    1                                               \
    from \"dba\".\"treat\",                                               \
    \"dba\".\"procedures\"                                               \
    where(\"treat\".\"item_id\" = \"procedures\".\"item_id\" and                                               \
        \"treat\".\"treat_date\" = @app_date and                                               \
        \"treat\".\"patient_id\" = @pat_id and                                               \
        \"treat\".\"ref_status\" is null and                                               \
        \"procedures\".\"item\" in(                                               \
            '114', 'D114', 'S114',                                               \
            '115', 'D115', 'S115',                                               \
            '88114',                                               \
            '88115')))  then                                               \
    set @Treatment_Type_ID = 9;                                               \
set @Treatment_Type_Name = 'Hygiene';                                               \
                                               \
else                                               \
set @Treatment_Type_ID = 10;                                               \
set @Treatment_Type_Name = 'Default';                                               \
end if;                                               \
                                               \
insert                                               \
into \"ltt_FollowApp\"(                                               \
    \"Patient_ID\",                                               \
    \"Patient_Title\",                                               \
    \"Patient_First_Name\",                                               \
    \"Patient_Last_Name\",                                               \
    \"Patient_DOB\",                                               \
    \"Appointment_ID\",                                               \
    \"Appointment_Date\",                                               \
    \"Appointment_Time\",                                               \
    \"Appointment_Status\",                                               \
    \"Db_Code\",                                               \
    \"Pracitioner_ID\",                                               \
    \"Pracitioner_Type_ID\",                                               \
    \"Pracitioner_Title\",                                               \
    \"Pracitioner_First_Name\",                                               \
    \"Practitioner_Last_Name\",                                               \
    \"Clinic_ID\",                                               \
    \"Clinic_Name\",                                               \
    \"Treatment_Type_ID\",                                               \
    \"Treatment_Type_Name\",                                               \
    \"Patient_Mobile\",                                               \
    \"Patient_Email\")                                               \
                                                                                              \
    select                                               \
    \"patients\".\"patients_cart_num\"      as \"Patient_ID\",                                               \
    \"pers_titles\".\"title_name\"          as \"Patient_Title\",                                               \
    \"patients\".\"firstname\"              as \"Patient_First_Name\",                                               \
    \"patients\".\"surname\"                as \"Patient_Last_Name\",                                               \
    \"patients\".\"DOB\"                    as \"Patient_DOB\",                                               \
    \"D\".\"appoint_id\"                    as \"Appointment_ID\",                                               \
    \"D\".\"app_date\"                      as \"Appointment_Date\",                                               \
    \"D\".\"app_start\"                     as \"Appointment_Time\",                                               \
    \"D\".\"app_status\"                    as \"Appointment_Status\",                                               \
    \"patients\".\"Patient_ID\"             as \"Db_Code\",                                               \
                                               \
    \"Clinic_ID\" ||                                               \
    \"right\"(\"string\"('0000', \"D\".\"doct_id\"), 4)                                               \
    as \"Pracitioner_ID\",                                               \
    \"staff\".\"prof_id\"                   as \"Pracitioner_Type_ID\",                                               \
    (select                                               \
        \"pers_titles\".\"title_name\"                                               \
        from \"dba\".\"pers_titles\"                                               \
        where(\"pers_titles\".\"title_id\" = \"staff\".\"title_id\"))                                               \
    as \"Pracitioner_Title\",                                               \
    \"staff\".\"firstname\"                 as \"Pracitioner_First_Name\",                                               \
    \"staff\".\"surname\"                   as \"Practitioner_Last_Name\",                                               \
    \"trim\"(\"dba\".\"cd1_get_login\"(\"dba\".\"sf_get_param_value\"('SMS_SURGERY_ID', \"D\".\"pract_id\")))                                               \
    as \"Clinic_ID\",                                               \
    \"practice_locations\".\"description\"  as \"Clinic_Name\",                                               \
    @Treatment_Type_ID                  as \"Treatment_Type_ID\",                                               \
    @Treatment_Type_Name                as \"Treatment_Type_Name\",                                               \
    coalesce(\"send_appt_to_patients\".\"mobile\", \"patients\".\"mobile\")                                               \
    as \"Patient_Mobile\",                                               \
    coalesce(\"send_appt_to_patients\".\"email\", \"patients\".\"email\")                                               \
    as \"Patient_Email\"                                               \
    from \"dba\".\"patients\"                                               \
    inner join(                                               \
        select                                               \
        \"a_appointments\".\"appoint_id\",                                               \
        \"a_appointments\".\"app_date\",                                               \
        \"a_appointments\".\"pat_id\",                                               \
        \"a_appointments\".\"doct_id\",                                               \
        (case                                               \
                                               \
            locate(cast(\"a_appointments\".\"status\" as binary),                                               \
                cast(@attend_status as binary))                                               \
            when  0 then 'Not Attended'                                               \
else  'Attended'                                               \
end)                          as \"app_status\",                                               \
min(\"a_appointments\".\"start\")  as \"app_start\",                                               \
\"app_books\".\"practice_id\"      as \"pract_id\"                                               \
from \"dba\".\"a_appointments\"                                               \
inner join \"dba\".\"app_books\"                                               \
on(\"a_appointments\".\"app_book_id\" = \"app_books\".\"app_book_number\")                                               \
where(\"a_appointments\".\"app_book_id\" = @app_book_id and                                               \
    \"a_appointments\".\"app_date\" = @app_date and                                               \
    \"a_appointments\".\"pat_id\"      is not null and                                               \
    \"a_appointments\".\"pat_id\" = @pat_id and                                               \
                                               \
                                               \
    locate(cast(\"a_appointments\".\"status\" as binary),                                               \
        cast(@no_FollowApp_status as binary)) = 0)                                               \
        group by                                               \
        \"a_appointments\".\"appoint_id\",                                               \
        \"a_appointments\".\"app_date\",                                               \
        \"a_appointments\".\"pat_id\",                                                                                              \
        \"a_appointments\".\"doct_id\",                                               \
        \"app_status\",                                               \
        \"pract_id\") D                                               \
    on(\"patients\".\"patient_id\" = \"D\".\"pat_id\")                                               \
    left outer join \"dba\".\"staff\"                                               \
    on(\"D\".\"doct_id\" = \"staff\".\"member_id\")                                               \
    inner join \"dba\".\"practice_locations\"                                               \
    on(\"D\".\"pract_id\" = \"practice_locations\".\"practice_id\")                                               \
    left outer join \"dba\".\"pers_titles\"                                               \
    on(\"patients\".\"title_id\" = \"pers_titles\".\"title_id\")                                               \
    left outer join \"dba\".\"patients\" \"send_appt_to_patients\"                                               \
    on(\"send_appt_to_patients\".\"patient_id\" = \"patients\".\"send_appt_to_pat_id\");                                               \
end for;                                               \
//                                               \
select                                                                                        \
\"Patient_ID\",                                               \
\"Patient_Title\",                                               \
\"Patient_First_Name\",                                               \
\"Patient_Last_Name\",                                               \
\"Patient_DOB\",                                               \
\"Appointment_ID\",                                               \
\"Appointment_Date\",                                               \
\"Appointment_Time\",                                               \
\"Appointment_Status\",                                               \
\"Db_Code\",                                               \
\"Pracitioner_ID\",                                               \
\"Pracitioner_Title\",                                               \
\"Pracitioner_First_Name\",                                               \
\"Practitioner_Last_Name\",                                               \
\"Clinic_ID\",                                               \
\"Clinic_Name\",                                               \
\"Treatment_Type_ID\",                                               \
\"Treatment_Type_Name\",                                               \
\"Patient_Mobile\",                                               \
\"Patient_Email\"                                               \
from \"ltt_FollowApp\"                                               \
where(\"Treatment_Type_ID\" = 9 or                                               \
    (\"Treatment_Type_ID\" < > 9 and \"Pracitioner_Type_ID\" = 1));                                               \
                                               \
end                                               \
;                                               \
";


