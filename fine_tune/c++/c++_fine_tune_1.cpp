// header file
#pragma once #include "Mutator_base.h"
/** * Insert_Wrapper_Method_For_Static_NonVoid_NoParam_1 */
class MutatorFrontendAction_1 : public MutatorFrontendAction
{
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)
private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer
    {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
    };
};
// source file
#include "../include/Insert_Wrapper_Method_For_Static_NonVoid_NoParam_1 
// ========================================================================================================
#define MUT1_OUTPUT 1
void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result)
{
    SourceManager &SM = Rewrite.getSourceMgr();
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("hasStaticNonVoidMethod "))
    {
        if (!recordDecl)
            return;
        if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc()))
            return;
        if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
            return;
        if (!recordDecl->isCompleteDefinition())
            return;
        auto recordName = recordDecl->getNameAsString();
#if MUT1_OUTPUT llvm::errs() << "MUT1:  " << "Match record with static non void method:  " << recordName << "  ";
#endif string insText = "/*mut1*/ public:  ";
        bool insFlag = false;
        for (const auto *methodDecl : recordDecl->methods())
        {
            string methodName = methodDecl->getNameAsString();
            SourceRange returnTypeRange = methodDecl->getReturnTypeSourceRange();
            QualType returnType = methodDecl->getReturnType();
            string returnTypeName = stringutils::rangetoStr(SM, returnTypeRange);
            if (methodDecl->isImplicit())
                continue;
            bool isStatic = methodDecl->isStatic();
            bool notReturnsVoid = returnTypeName != "void ";
            assert(isStatic && notReturnsVoid && "Mut1: should be a static non void method  ");
            if (methodDecl->getNumParams() > 0)
                continue;
#if MUT1_OUTPUT llvm::errs() << "  " << "Static non void method with 0 param:  " << methodName << "  ";
#endif string insMethodType = "void ";
            string insMethodName = "mut1_ " + methodDecl->getNameAsString() + "_ " + returnTypeName + "_ " + stringutils::getTimestamp();
            string insMethodParams = "( " + returnTypeName + "= " + methodName + "() " + ") ";
            insText += "  " + insMethodType + "  " + insMethodName + insMethodParams + "{}  ";
            insFlag = true;
        }
        if (insFlag)
        {
            SourceLocation recordEndLoc = recordDecl->getEndLoc();
            Rewrite.InsertText(recordEndLoc, insText, false, true);
#if MUT1_OUTPUT llvm::errs() << "  " << "insert:  " << insText << "  ";
#endif
        }
    }
}
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context)
{
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(has(cxxMethodDecl(isStaticStorageClass(), unless(returns(asString("void ")))))).bind("hasStaticNonVoidMethod ");
    ;
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
