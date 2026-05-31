//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constexpr_To_Deleted_Function_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)

private:
    class MutatorASTConsumer_15 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Constexpr_To_Deleted_Function_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("deletedFunc")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Get the source code text of the target node
        auto SM = Result.SourceManager;
        auto declRange = FD->getSourceRange();
        string declText = stringutils::rangetoStr(*SM, declRange);

        // Perform mutation: insert constexpr before = delete
        size_t pos = declText.find("= delete");
        if (pos != string::npos) {
            declText.insert(pos, "constexpr ");
            Rewrite.ReplaceText(declRange, declText);
        }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify deleted member functions
    DeclarationMatcher matcher = cxxConstructorDecl(isDeleted()).bind("deletedFunc");
    matcher = matcher || cxxDestructorDecl(isDeleted()).bind("deletedFunc");
    matcher = matcher || cxxMethodDecl(isDeleted()).bind("deletedFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}