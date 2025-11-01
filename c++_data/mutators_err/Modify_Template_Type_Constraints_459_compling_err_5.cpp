//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Type_Constraints_459
 */ 
class MutatorFrontendAction_459 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(459)

private:
    class MutatorASTConsumer_459 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_459(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Type_Constraints_459.h"

// ========================================================================================================
#define MUT459_OUTPUT 1

void MutatorFrontendAction_459::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->getTemplateSpecializationKind() != TSK_Undeclared) {
            auto sourceRange = FD->getSourceRange();
            auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            if (sourceText.find("requires") != std::string::npos) {
                auto insertPos = sourceText.find("requires") + std::string("requires").length();
                auto additionalConstraint = " || IsSame<U, V>";

                sourceText.insert(insertPos, additionalConstraint);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
            }
        }
    }
}

void MutatorFrontendAction_459::MutatorASTConsumer_459::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;

    DeclarationMatcher matcher = functionDecl(hasRequiresClause()).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}