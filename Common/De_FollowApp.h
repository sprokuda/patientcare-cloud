#pragma once


const char* De_FollowAppTest = " \
create or replace procedure \"dba\".\"de_FollowApp\"(                                       			\
    in @app_book_id              integer,                                                   			\
    in @app_date                 date,                                                      			\
    in @column_type_status_att   char(128) default 'Attended',                              			\
    in @column_type_status_stop  char(128) default 'No_PatientCare'			\
    )			\
    begin			\
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
set @attend_status = null;                                               \
\
end;";

const char* De_FollowApp0403_1 = "create or replace procedure \"dba\".\"de_FollowApp\"(                \n\n in @app_book_id              integer,\n\n in @app_date                 date,\n\nin @column_type_status_att   char(128) default 'Attended',      \n\n in @column_type_status_stop  char(128) default 'No_PatientCare' \n\n)\n\nbegin\n\n \n\n \n\n  declare @attend_status        char(128);\n\n  declare @no_FollowApp_status  char(128);\n\n \n\n  declare @Treatment_Type_ID    smallint;\n\n  declare @Treatment_Type_Name  varchar(20);\n\n  declare @mcount               integer;\n\n  \n\n  declare local temporary table \"ltt_FollowApp\"(\n\n    \"Patient_ID\"              varchar(30),\n\n    \"Patient_Title\"           char(6),\n\n    \"Patient_First_Name\"      varchar(30),\n\n    \"Patient_Last_Name\"       varchar(30),\n\n    \"Patient_DOB\"             date,\n\n    \"Appointment_ID\"          integer,\n\n    \"Appointment_Date\"        date,\n\n    \"Appointment_Time\"        time,\n\n    \"Appointment_Status\"      char(12),\n\n    \"Db_Code\"                 integer,\n\n    \"Pracitioner_ID\"          varchar(9),\n\n    \"Pracitioner_Type_ID\"     smallint,\n\n    \"Pracitioner_Title\"       char(6),\n\n    \"Pracitioner_First_Name\"  varchar(20),\n\n    \"Practitioner_Last_Name\"  varchar(22),\n\n    \"Clinic_ID\"               varchar(20),\n\n    \"Clinic_Name\"             varchar(50),\n\n    \"Treatment_Type_ID\"       smallint,\n\n    \"Treatment_Type_Name\"     varchar(20),\n\n    \"Patient_Mobile\"          varchar(20),\n\n    \"Patient_Email\"           varchar(60)) on commit delete rows;\n\n \n\n  set @attend_status = null;\n\n  select\n\n    \"Column_Type_Status_Abbreviatio\"\n\n   into\n\n    @attend_status\n\n   from \"dba\".\"a_slot_column_status_types\"\n\n   where (Column_Type_Status_Description = @column_type_status_att);\n\n  if (coalesce(@attend_status, '') = '')  then\n\n   set @attend_status = char(255);\n\n  end if;\n\n  //\n\n \n\n  set @no_FollowApp_status = null;\n\n  select\n\n    \"Column_Type_Status_Abbreviatio\"\n\n   into\n\n    @no_FollowApp_status\n\n   from \"dba\".\"a_slot_column_status_types\"\n\n   where (Column_Type_Status_Description = @column_type_status_stop);\n\n  if (coalesce(@no_FollowApp_status, '') = '')  then\n\n   set @no_FollowApp_status = char(255);\n\n  end if;\n\n  //\n\n  for \"f\" as \"cur\" dynamic scroll cursor for\n\n   select distinct\n\n     \"a_appointments\".\"pat_id\"  @pat_id\n\n    from \"dba\".\"a_appointments\"\n\n    where (\"a_appointments\".\"app_book_id\" = @app_book_id  and\n\n           \"a_appointments\".\"app_date\"    = @app_date     and\n\n           \"a_appointments\".\"pat_id\"      is not null     and \n\n          \n\n                                                                          \n\n           locate(cast(\"a_appointments\".\"status\" as binary),            \n\n                  cast(@no_FollowApp_status as binary)) = 0)\n\n   do\n\n  \n\n    set @Treatment_Type_ID   = null;\n\n    set @Treatment_Type_Name = null;\n\n    if exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '322','88322','D322','S322',\n\n                                   '323','88323','D323','S323',\n\n                                   '324','88324','D324','S324',\n\n                                         '88326',\n\n                                                 'D331','S331',\n\n                                   '332',               'S332',\n\n                                   '337',        'D337','S337',\n\n                                   '338',        'D338','S338',\n\n                                   '341',        'D341','S341',\n\n                                   '343',               'S343',\n\n                                   '344',               'S344',\n\n                                   '345',               'S345',\n\n                                   '351','88351','D351','S351',\n\n                                   '352',               'S352',\n\n                                   '353',               'S353',\n\n                                   '354',               'S354',\n\n                                   '355',               'S355',\n\n                                   '359',               'S359',\n\n                                   '361',               'S361',\n\n                                   '363',               'S363',\n\n                                   '365',               'S365',\n\n                                   '366',               'S366',\n\n                                   '371',               'S371',\n\n                                   '373',               'S373',\n\n                                   '375',               'S375',\n\n                                   '376',               'S376',\n\n                                   '377',        'D377','S377',\n\n                                   '378',        'D378','S378',\n\n                                   '379',               'S379',\n\n                                   '381',        'D381','S381',\n\n                                   '382',               'S382',\n\n                                   '384','88384','D384','S384',\n\n                                   '385',               'S385',\n\n                                   '386','88386','D386','S386',\n\n                                   '387','88387','D387','S387',\n\n                                   '388',               'S388',\n\n                                   '389',               'S389',\n\n                                   '391',        'D391','S391',\n\n                                   '392','88392','D392','S392',\n\n                                   '393',               'S393',\n\n                                   '394',               'S394',\n\n                                   '395',               'S395')))  then\n\n      set @Treatment_Type_ID   = 1;\n\n      set @Treatment_Type_Name = 'Oral Surgery';\n\n     elseif exists (select top 1\n\n                1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '311','88311','D311','S311',\n\n                                   '314','88314','D314','S314',\n\n                                   '316','88316')))  then\n\n      set @Treatment_Type_ID   = 2;\n\n      set @Treatment_Type_Name = 'Extraction';\n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '411','88411','D411','S411',\n\n                                   '412','88412','D412','S412',\n\n                                   '413',\n\n                                   '414','88414','D414','S414',\n\n                                   '415','88415','D415','S415',\n\n                                   '455','88455','D455','S455',\n\n                                   '417','88417','D417','S417',\n\n                                   '419','88419','D419','S419',\n\n                                   '421','88421','D421','S421',\n\n                                   '431',        'D431','S431',\n\n                                   '432',        'D432','S432',\n\n                                   '437',        'D437','S437',\n\n                                   '438',        'D438','S438')))  then\n\n      set @Treatment_Type_ID   = 3;\n\n      set @Treatment_Type_Name = 'Endodontics';\n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '661',        'D661','S661',\n\n                                   '671',        'D671','S671',\n\n                                   '672',        'D672','S672',\n\n                                   '673',        'D673','S673',\n\n                                   '684',               'S684',\n\n                                   '688',               'S688',\n\n                                   '691',               'S691')))  then\n\n";
const char* De_FollowApp0403_2 = "set @Treatment_Type_ID = 4; \n\n      set @Treatment_Type_Name = 'Implants'; \n\n     elseif exists(select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                         '88576',\n\n                                   '586',        'D586','S586',\n\n                                   '587',        'D587','S587',\n\n                                   '588',        'D588','S588',\n\n                                   '611',        'D611','S611',\n\n                                   '613',        'D613','S613',\n\n                                   '615',        'D615','S615',\n\n                                   '618',        'D618','S618')))  then\n\n      set @Treatment_Type_ID   = 5;\n\n      set @Treatment_Type_Name = 'Crown & Bridge';\n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '711',        'D711','S711',\n\n                                   '712',        'D712','S712',\n\n                                   '719',        'D719','S719',\n\n                                   '721','88721','D721','S721',\n\n                                   '722','88722','D722','S722',\n\n                                   '727',        'D727','S727',\n\n                                   '728',        'D728','S728')))  then\n\n      set @Treatment_Type_ID   = 6;\n\n      set @Treatment_Type_Name = 'Prosthodontics';\n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '811',        'D811','S811',\n\n                                   '821',        'D821','S821',\n\n                                   '823',        'D823','S823',\n\n                                   '824',\n\n                                   '829',        'D829','S829',\n\n                                   '831',        'D831','S831',\n\n                                   '881',        'D881','S881')))  then\n\n      set @Treatment_Type_ID   = 7;\n\n      set @Treatment_Type_Name = 'Orthodontics';\n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '117',        'D117','S117',\n\n                                   '118',\n\n                                   '119')))  then\n\n      set @Treatment_Type_ID   = 8;\n\n      set @Treatment_Type_Name = 'Bleaching';\n\n  \n\n     elseif exists (select top 1\n\n                 1\n\n                from \"dba\".\"treat\",\n\n                     \"dba\".\"procedures\"\n\n                where (\"treat\".\"item_id\"    = \"procedures\".\"item_id\"  and\n\n                       \"treat\".\"treat_date\" = @app_date               and\n\n                       \"treat\".\"patient_id\" = @pat_id                 and\n\n                       \"treat\".\"ref_status\" is null                   and\n\n                       \"procedures\".\"item\" in (\n\n                                   '114',        'D114','S114',\n\n                                   '115',        'D115','S115',\n\n                                   '88114',\n\n                                   '88115')))  then\n\n      set @Treatment_Type_ID   = 9;\n\n      set @Treatment_Type_Name = 'Hygiene';\n\n  \n\n     else\n\n      set @Treatment_Type_ID   = 10;\n\n      set @Treatment_Type_Name = 'Default';\n\n    end if;\n\n \n\n    insert\n\n     into \"ltt_FollowApp\"(\n\n      \"Patient_ID\",\n\n      \"Patient_Title\",\n\n      \"Patient_First_Name\",\n\n      \"Patient_Last_Name\",\n\n      \"Patient_DOB\",\n\n      \"Appointment_ID\",\n\n      \"Appointment_Date\",\n\n      \"Appointment_Time\",\n\n      \"Appointment_Status\",\n\n      \"Db_Code\",               \n\n      \"Pracitioner_ID\",\n\n      \"Pracitioner_Type_ID\",\n\n      \"Pracitioner_Title\",\n\n      \"Pracitioner_First_Name\",\n\n      \"Practitioner_Last_Name\",\n\n      \"Clinic_ID\",\n\n      \"Clinic_Name\",\n\n      \"Treatment_Type_ID\",\n\n      \"Treatment_Type_Name\",\n\n      \"Patient_Mobile\",\n\n      \"Patient_Email\")\n\n \n\n    select \n\n      \"patients\".\"patients_cart_num\"      as \"Patient_ID\",\n\n      \"pers_titles\".\"title_name\"          as \"Patient_Title\",\n\n      \"patients\".\"firstname\"              as \"Patient_First_Name\",\n\n      \"patients\".\"surname\"                as \"Patient_Last_Name\" ,\n\n      \"patients\".\"DOB\"                    as \"Patient_DOB\",\n\n      \"D\".\"appoint_id\"                    as \"Appointment_ID\",\n\n      \"D\".\"app_date\"                      as \"Appointment_Date\",\n\n      \"D\".\"app_start\"                     as \"Appointment_Time\",\n\n      \"D\".\"app_status\"                    as \"Appointment_Status\",\n\n      \"patients\".\"Patient_ID\"             as \"Db_Code\",\n\n     \n\n      \"Clinic_ID\" ||                                  \n\n      \"right\"(\"string\"('0000',\"D\".\"doct_id\"), 4)\n\n                                          as \"Pracitioner_ID\",   \n\n      \"staff\".\"prof_id\"                   as \"Pracitioner_Type_ID\", \n\n      (select\n\n         \"pers_titles\".\"title_name\"\n\n        from \"dba\".\"pers_titles\"\n\n        where (\"pers_titles\".\"title_id\" = \"staff\".\"title_id\"))\n\n                                          as \"Pracitioner_Title\",\n\n      \"staff\".\"firstname\"                 as \"Pracitioner_First_Name\",\n\n      \"staff\".\"surname\"                   as \"Practitioner_Last_Name\",\n\n      \"trim\"(\"dba\".\"cd1_get_login\"(\"dba\".\"sf_get_param_value\"('SMS_SURGERY_ID', \"D\".\"pract_id\")))\n\n                                          as \"Clinic_ID\",         \n\n      \"practice_locations\".\"description\"  as \"Clinic_Name\",\n\n      @Treatment_Type_ID                  as \"Treatment_Type_ID\",\n\n      @Treatment_Type_Name                as \"Treatment_Type_Name\",\n\n      coalesce(\"send_appt_to_patients\".\"mobile\", \"patients\".\"mobile\") \n\n                                          as \"Patient_Mobile\",\n\n      coalesce(\"send_appt_to_patients\".\"email\", \"patients\".\"email\")   \n\n                                          as \"Patient_Email\"\n\n     from \"dba\".\"patients\"\n\n          inner join (\n\n                select\n\n                  \"a_appointments\".\"appoint_id\",\n\n                  \"a_appointments\".\"app_date\",\n\n                  \"a_appointments\".\"pat_id\",\n\n                  \"a_appointments\".\"doct_id\",\n\n                  (case\n\n                   \n\n                    locate(cast(\"a_appointments\".\"status\" as binary),    \n\n                           cast(@attend_status as binary))\n\n                    when  0 then 'Not Attended'\n\n                    else  'Attended'\n\n                   end)                          as \"app_status\",\n\n                  min(\"a_appointments\".\"start\")  as \"app_start\",\n\n                  \"app_books\".\"practice_id\"      as \"pract_id\"\n\n                 from \"dba\".\"a_appointments\"\n\n                      inner join \"dba\".\"app_books\"\n\n                       on (\"a_appointments\".\"app_book_id\" = \"app_books\".\"app_book_number\")\n\n                 where (\"a_appointments\".\"app_book_id\" = @app_book_id  and\n\n                        \"a_appointments\".\"app_date\"    = @app_date     and\n\n                        \"a_appointments\".\"pat_id\"      is not null     and \n\n                        \"a_appointments\".\"pat_id\"      = @pat_id       and\n\n                       \n\n                                                                         \n\n                        locate(cast(\"a_appointments\".\"status\" as binary),\n\n                               cast(@no_FollowApp_status as binary)) = 0)\n\n                 group by\n\n                   \"a_appointments\".\"appoint_id\",\n\n                   \"a_appointments\".\"app_date\",\n\n                   \"a_appointments\".\"pat_id\",\n\n                   \"a_appointments\".\"doct_id\",\n\n                   \"app_status\",\n\n                   \"pract_id\") D\n\n         on (\"patients\".\"patient_id\" = \"D\".\"pat_id\")\n\n        left outer join \"dba\".\"staff\"\n\n         on (\"D\".\"doct_id\" = \"staff\".\"member_id\")\n\n        inner join \"dba\".\"practice_locations\"\n\n         on (\"D\".\"pract_id\" = \"practice_locations\".\"practice_id\")\n\n        left outer join \"dba\".\"pers_titles\"\n\n         on (\"patients\".\"title_id\" = \"pers_titles\".\"title_id\")\n\n        left outer join \"dba\".\"patients\" \"send_appt_to_patients\"\n\n         on (\"send_appt_to_patients\".\"patient_id\" = \"patients\".\"send_appt_to_pat_id\");\n\n  end for;\n\n  //\n\n  select\n\n    \"Patient_ID\",\n\n    \"Patient_Title\",\n\n    \"Patient_First_Name\",\n\n    \"Patient_Last_Name\",\n\n    \"Patient_DOB\",\n\n    \"Appointment_ID\",\n\n    \"Appointment_Date\",\n\n    \"Appointment_Time\",\n\n    \"Appointment_Status\",\n\n    \"Db_Code\",\n\n    \"Pracitioner_ID\",\n\n    \"Pracitioner_Title\",\n\n    \"Pracitioner_First_Name\",\n\n    \"Practitioner_Last_Name\",\n\n    \"Clinic_ID\",\n\n    \"Clinic_Name\",\n\n    \"Treatment_Type_ID\",\n\n    \"Treatment_Type_Name\",\n\n    \"Patient_Mobile\",\n\n    \"Patient_Email\"\n\n   from \"ltt_FollowApp\"\n\n   where (\"Treatment_Type_ID\" = 9  or                                 \n\n          (\"Treatment_Type_ID\" <> 9  and  \"Pracitioner_Type_ID\" = 1));\n\n\nend\n\n;";


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
    (\"Treatment_Type_ID\" <> 9 and \"Pracitioner_Type_ID\" = 1));                                               \
                                               \
end                                               \
;                                               \
";


