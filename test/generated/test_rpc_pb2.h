// Generated by the protocol buffer compiler.  DO NOT EDIT!

#ifndef __TEST_RPC_PB2_H__
#define __TEST_RPC_PB2_H__

#include <cpb/cpb.h>


/* 'PhoneNumber.PhoneType' enumeration values */
#define TEST_PHONENUMBER_MOBILE 0
#define TEST_PHONENUMBER_HOME 1
#define TEST_PHONENUMBER_WORK 2

extern const struct cpb_msg_desc cpb_messages_test[];

/* Message descriptor pointers */
#define test_PhoneNumber (&cpb_messages_test[0])
#define test_Person (&cpb_messages_test[1])
#define test_LookupResult (&cpb_messages_test[2])
#define test_Name (&cpb_messages_test[3])

extern const struct cpb_field_desc cpb_fields_test_phonenumber[];

/* 'PhoneNumber' field descriptor pointers */
#define test_PhoneNumber_number (&cpb_fields_test_phonenumber[0])
#define test_PhoneNumber_type (&cpb_fields_test_phonenumber[1])

extern const struct cpb_field_desc cpb_fields_test_person[];

/* 'Person' field descriptor pointers */
#define test_Person_name (&cpb_fields_test_person[0])
#define test_Person_id (&cpb_fields_test_person[1])
#define test_Person_email (&cpb_fields_test_person[2])
#define test_Person_phone (&cpb_fields_test_person[3])

extern const struct cpb_field_desc cpb_fields_test_lookupresult[];

/* 'LookupResult' field descriptor pointers */
#define test_LookupResult_person (&cpb_fields_test_lookupresult[0])

extern const struct cpb_field_desc cpb_fields_test_name[];

/* 'Name' field descriptor pointers */
#define test_Name_name (&cpb_fields_test_name[0])

extern const struct cpb_service_desc cpb_services_test[];

/* Service descriptor pointers */
#define test_Search (&cpb_services_test[0])

extern const struct cpb_method_desc cpb_methods_test_search[];

/* 'Search' method descriptor pointers */
#define test_Search_search_by_name (&cpb_methods_test_search[0])

#endif /* __TEST_RPC_PB2_H__ */
