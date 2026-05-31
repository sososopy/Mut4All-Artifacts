//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_constexpr_function_with_deleted_assignment_637
 */ 
class MutatorFrontendAction_637 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(637)

private:
    class MutatorASTConsumer_637 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_637(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_constexpr_function_with_deleted_assignment_637.h"

// ========================================================================================================
#define MUT637_OUTPUT 1

void MutatorFrontendAction_637::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("AssignmentOperator")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        if (MT->isDeleted() && MT->isConstexpr()) {
            SourceLocation startLoc = MT->getBeginLoc();
            SourceLocation endLoc = MT->getEndLoc();
            std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), 
                                                            *Result.SourceManager, 
                                                            Result.Context->getLangOpts()).str();
            size_t pos = originalText.find("constexpr");
            if (pos != std::string::npos) {
                originalText.erase(pos, 9); // Remove "constexpr"
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), originalText);
        }
    }
}

void MutatorFrontendAction_637::MutatorASTConsumer_637::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isDeleted(), isConstexpr(), isCopyAssignmentOperator()).bind("AssignmentOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}