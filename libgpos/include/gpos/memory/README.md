==============================================================
Ref counts protocol in CCache
--------------------------------------------------------------

In this document, we introduce a set of rules that CCache and its customers should follow. In a nutshell, customers are responsible for creating an object and inserting it to CCache. During a lookup operation, CCache is responsible for increasing objects’ ref counts and customers can only decrease them after objects’ use. 

In particular, a customer (e.g., ORCA) creates an instance of CCache, which will be used for storing data that it needs and may reuse. Customer always perform a Lookup operation for retrieving data that are designed to be kept in CCache temporarily. Let us assume that a customer needs object obj and performs a Lookup operation on local instance of CCache. If obj is not in CCache, then obj should be created and inserted to CCache. During creation phase, customer retrieves obj’s actual data from permanent storage, creates obj, whose ref count is set to one, and keeps the ownership of obj. CCache is responsible for inserting obj to the cache. Specifically, it creates a new cache entry that points to the object, takes the ownership of obj by increasing obj’s ref count by one, and inserts obj to the cache. If obj is a CCache resident, CCache will simply increase obj's ref count. In both cases, customer should always release obj after the use (e.g., at the end of query optimization phase). Sebsequently, if all customers of obj have released it properly, then obj’s ref count should be one, which represents CCache’s ownership, since there is still a cache entry that points to the object. Objects with ref count equal to one are eligible for eviction. 

Finally, multiple intermediate steps that add a reference and release an object may occur between the steps described above. For example, if a customer creates an object (or looks for an object, which is in CCache already), then CCache (CCacheAccessor) increases object’s ref count. Object’s ref count is decreased when CCacheAccessor goes out of scope. A customer can also add other “hidden” references. In particular, during object’s translation, orca increases the ref count and releases it at the end of translation. 