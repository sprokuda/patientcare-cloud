#pragma once

const char* De_Clinician_Details = " \
create or replace procedure \"dba\".\"de_clinician_details\"(           \
in @practice_id smallint                                                \
)                                                                       \
begin                                                                   \
select                                                                  \
                                                                        \
\"Clinic_ID_1\" ||                                                      \
\"right\"(\"string\"('0000', \"staff\".\"member_id\"), 4)               \
as \"Pracitioner_ID\",                                                  \
(select                                                                 \
    \"pers_titles\".\"title_name\"                                      \
    from \"dba\".\"pers_titles\"                                        \
    where(\"pers_titles\".\"title_id\" = \"staff\".\"title_id\"))       \
    as \"Pracitioner_Title\",                                           \
    \"staff\".\"firstname\"                  as \"Pracitioner_First_Name\",                                             \
    \"staff\".\"surname\"                    as \"Practitioner_Last_Name\",                                             \
    \"staff\".\"email\"                      as \"Practitioner_Email\",                                                 \
    \"staff\".\"birthdate\"                  as \"Practitioner_DOB\",                                                   \
    \"trim\"(\"dba\".\"cd1_get_login\"(\"dba\".\"sf_get_param_value\"('SMS_SURGERY_ID', \"staff\".\"practice_id\")))    \
    as \"Clinic_ID_1\",                                                                                                 \
    \"practice_locations\".\"description\"   as \"Clinic_Name_1\"                                                       \
    from \"dba\".\"staff\"                                                                                              \
    inner join \"dba\".\"practice_locations\"                                                                           \
    on(\"staff\".\"practice_id\" = \"practice_locations\".\"practice_id\")                                              \
    where(\"staff\".\"is_active\" = 'Y' and                                                                             \
        \"staff\".\"member_type\" = 1 and                                                                               \
        \"practice_locations\".\"practice_id\" = @practice_id);                                                         \
end                                                                                                                     \
;";
