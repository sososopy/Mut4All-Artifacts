//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Invalid_Type_Compatibility_Check_238
 */ 
class MutatorFrontendAction_238 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(238)

private:
    class MutatorASTConsumer_238 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_238(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Invalid_Type_Compatibility_Check_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isUnion() || RD->isLambda())
            return;
        SourceLocation endLoc = RD->getEndLoc();
        if (endLoc.isInvalid())
            return;
        std::string assertion = "_Static_assert(!(__builtin_types_compatible_p(typeof(*unknown), int)));\n";
        Rewrite.InsertTextBefore(endLoc, assertion);
    }
}

void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}