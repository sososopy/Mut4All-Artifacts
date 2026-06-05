

    static const char* fnName = "NewLs::CpSelect - ";
    tbr_t  tbrStart, tbrEnd, timediff;

    tbrStart = tbrGet();

    McId         newCpId((McIdHandle)cpId);
    McId         newSelId((McIdHandle)selId);
    McCpId*      pCpId        = static_cast<McCpId*>(&newCpId);
    ComplexXCon* pXCon        = NULL;
    CtpCpBase*   pNewSel      = NULL;
    ComplexXCon* pNewXCon     = NULL;
    UniCp*       pCp          = NULL;

    rFailureReason = LS_SUCCESS;

    if (!pCpId)
    {
        MEMORY_LOG_MCHECK (getSwitchCoreMemoryLog(), pCpId, NULL,
               "!!!ERROR!!!: " << fnName << "null CP id");
        return false;
    }

    // Find the CP object
    pCp = FindCpFromHashList(*pCpId);
    if (!pCp)
    {
        LOGIC_SWITCH_ERRLOG_PRINTF (getSwitchCoreMemoryLog(),
                "%sCannot find CP 0x%llX", fnName, cpId);
        rFailureReason = END_POINT_NOT_FOUND;
        return false;
    }

    McCpProtectionApp protApp = pCp->GetProtApp();

    if (rxProtApp != protApp)
    {
        .
        .
        .
        return false;
    }

    pXCon = pCp->GetXCon();
    if (!pXCon)
    {
        char mcBuf[ 64 ];

        pCpId->toString (mcBuf, sizeof (mcBuf));
        LOGIC_SWITCH_ERRLOG_PRINTF (getSwitchCoreMemoryLog(),
                "%sCannot find XCon for CP: %s", fnName, mcBuf);
        rFailureReason = CROSS_CONNECT_NOT_FOUND;
        return false;
    }

    if (!squelch)
    {
        if (newSelId == McId::InvalidId)
        {
            rFailureReason = BAD_END_POINT_ID;
            return false;
        }

        //
        // Find the CTP or CP (if it exists) and verify that it belongs to pXCon
        //
        if (newSelId.IsCtpId())
        {
            McCtpId ctpId(newSelId);
            pNewSel = FindCtpFromHashList(ctpId);
        }
        else if (newSelId.IsCpId())
        {
            McCpId newCpId((McCpIdHandle) newSelId.GetId());
            pNewSel = FindCpFromHashList(newCpId);
        }
     if (pNewSel)
            pNewXCon = pNewSel->GetXCon();
    }
    else
    {
        char mcBuf[ 64 ];

        pCpId->toString (mcBuf, sizeof (mcBuf));
        getSwitchCoreMemoryLog()->PrintRamLog ("%sSetting %s to be squelched.",
                fnName, mcBuf);
    }

    if (squelch || (pNewSel && pNewXCon == pXCon))
    {
        if (pCp->GetSelectedCtpCp() == pNewSel
                || (isDefaultFromCac && pCp->GetDefaultSelector() == pNewSel))
        {
            .
            .
            .
            return false;   // already selected
        }

        // Update current selector on CP.
        pCp->SetSelectedCtpCp(pNewSel);
        .
        .
        .
    }


