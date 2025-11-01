// header file
#pragma once #include "Mutator_base.h"
/** * Replace_Member_Call_With_PointerToMember_5 */
class MutatorFrontendAction_5 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(5)
private:
    class MutatorASTConsumer_5 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_5(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;

    private:
        Rewriter &TheRewriter;
    };
    class Callback : public MatchFinder::MatchCallback
    {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);

    private:
        Rewriter &Rewrite;
        std::map<std::string, std::string> instrumentedDeclMap;
    };
};
// source file
#include "../include/Replace_Member_Call_With_PointerToMember_5.h"
// ========================================================================================================
#define MUT5_OUTPUT 1
void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result)
{
    if (auto *MC = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("memberCall"))
    {
        if (!MC || !Result.Context->getSourceManager().isWrittenInMainFile(MC->getBeginLoc()))
            return;
        if (const MemberExpr *Member = dyn_cast<MemberExpr>(MC->getCallee()))
        {
            // member call
            if (Member->isArrow())
                return;
            std::string memberName = Member->getMemberDecl()->getNameAsString();
            if (const DeclRefExpr *Object = dyn_cast<DeclRefExpr>(Member->getBase()->IgnoreImpCasts()))
            {
                if (const VarDecl *Var = dyn_cast<VarDecl>(Object->getDecl()))
                {
                    std::string instanceName = Var->getNameAsString();
                    std::string varDecl = stringutils::rangetoStr(*(Result.SourceManager), Var->getSourceRange());
                    auto varTypeLoc = Var->getTypeSourceInfo()->getTypeLoc();
                    auto varType = stringutils::rangetoStr(*(Result.SourceManager), varTypeLoc.getSourceRange());
                    if (const CXXRecordDecl *RecordDecl = Var->getType()->getAsCXXRecordDecl())
                    {
                        std::string recordName = RecordDecl->getNameAsString();
                        if (instrumentedDeclMap.find(varDecl) == instrumentedDeclMap.end())
                        {
                            auto content = stringutils::rangetoStr(*(Result.SourceManager), Var->getSourceRange());
                            std::string mut_memberCalleeName = "mut5_" + instanceName + "_" + memberName + "_ptr";
                            instrumentedDeclMap[varDecl] = mut_memberCalleeName;
                            content += "; /*mut5*/auto " + mut_memberCalleeName + " = " + varType + "::" + memberName;
                            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Var->getSourceRange()), content);
#if MUT5_OUTPUT
                            llvm::errs() << "MUT5: " << "instrument VarDecl" << content << " ";
#endif
                        }
                        std::string mut_memberCalleeName = instrumentedDeclMap[varDecl];
                        std::string callByDot = instanceName + "." + memberName;
                        std::string callByMutPtr = "/*mut5*/(" + instanceName + ".*" + mut_memberCalleeName + ")";
                        auto content = stringutils::rangetoStr(*(Result.SourceManager), MC->getSourceRange());
                        assert(content.find(callByDot, 0) != string::npos && "Mut5: can not find a member call by dot");
                        stringutils::strReplace(content, callByDot, callByMutPtr);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MC->getSourceRange()), content);
#if MUT5_OUTPUT
                        llvm::errs() << "MUT5: " << "instrument MemberExpr" << content << " ";
#endif
                    }
                }
            }
        }
    }
}
void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    auto matcher = cxxMemberCallExpr().bind("memberCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}