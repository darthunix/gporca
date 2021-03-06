//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.

#ifndef GPOPT_CPhysicalParallelUnionAll_H
#define GPOPT_CPhysicalParallelUnionAll_H

#include "gpopt/operators/CPhysicalUnionAll.h"

namespace gpopt
{
	// Operator that implements logical union all, but creates a slice for each
	// child relation to maximize concurrency.
	// See gpopt::CPhysicalSerialUnionAll for its serial sibling.
	class CPhysicalParallelUnionAll : public CPhysicalUnionAll
	{
		private:
			// array of child hashed distributions -- used locally for distribution derivation
			CDistributionSpecArray *const m_pdrgpds;

		public:
			CPhysicalParallelUnionAll(CMemoryPool *mp, CColRefArray *pdrgpcrOutput, CColRef2dArray *pdrgpdrgpcrInput,
									  ULONG ulScanIdPartialIndex);

			virtual EOperatorId Eopid() const;

			virtual const CHAR *SzId() const;

			virtual CDistributionSpec *
			PdsRequired(CMemoryPool *mp, CExpressionHandle &exprhdl, CDistributionSpec *pdsRequired,
						ULONG child_index, CDrvdProp2dArray *pdrgpdpCtxt, ULONG ulOptReq) const;

			virtual
			CEnfdDistribution::EDistributionMatching Edm
				(
				CReqdPropPlan *, // prppInput
				ULONG,  // child_index
				CDrvdProp2dArray *, //pdrgpdpCtxt
				ULONG // ulOptReq
				);

			virtual ~CPhysicalParallelUnionAll();
	};
}

#endif //GPOPT_CPhysicalParallelUnionAll_H
