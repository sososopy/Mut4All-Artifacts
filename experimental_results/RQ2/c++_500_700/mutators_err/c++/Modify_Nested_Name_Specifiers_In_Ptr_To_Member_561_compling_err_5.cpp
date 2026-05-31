//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_nested_name_specifiers_in_ptr_to_member_561
 */ 
class MutatorFrontendAction_561 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(561)

private:
    class MutatorASTConsumer_561 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_561(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/modify_nested_name_specifiers_in_ptr_to_member_561.h"

// ========================================================================================================
#define MUT561_OUTPUT 1

void MutatorFrontendAction_561::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PME = Result.Nodes.getNodeAs<clang::MemberExpr>("PtrToMemberExpr")) {
        if (!PME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PME->getBeginLoc()))
            return;

        auto memberExprText = stringutils::rangetoStr(*(Result.SourceManager), PME->getSourceRange());
        
        if (memberExprText.find("::") != std::string::npos) {
            std::string mutatedText = "OtherNamespace::" + memberExprText;
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(PME->getSourceRange()), mutatedText);
        }
    }
}

void MutatorFrontendAction_561::MutatorASTConsumer_561::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = memberExpr(hasObjectExpression(ignoringImpCasts(declRefExpr(to(varDecl(hasType(pointerType(pointee(recordType()))))))));
    matcher = matcher.bind("PtrToMemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}