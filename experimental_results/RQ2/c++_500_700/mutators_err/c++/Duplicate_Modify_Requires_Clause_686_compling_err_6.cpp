//header file
#pragma once
#include "Mutator_base.h"

/**
 * duplicate_modify_requires_clause_686
 */ 
class MutatorFrontendAction_686 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(686)

private:
    class MutatorASTConsumer_686 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_686(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/duplicate_modify_requires_clause_686.h"

// ========================================================================================================
#define MUT686_OUTPUT 1

void MutatorFrontendAction_686::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithRequires")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplateSpecializationKind() != TSK_Undeclared) {
            auto requiresClause = FD->getTrailingRequiresClause();
            if (requiresClause) {
                std::string originalRequires = Lexer::getSourceText(
                    CharSourceRange::getTokenRange(requiresClause->getSourceRange()), 
                    *Result.SourceManager, 
                    Result.Context->getLangOpts()).str();

                std::string modifiedRequires = originalRequires + " && r.some_property()";

                std::string newFunctionDecl = Lexer::getSourceText(
                    CharSourceRange::getTokenRange(FD->getSourceRange()), 
                    *Result.SourceManager, 
                    Result.Context->getLangOpts()).str();

                size_t pos = newFunctionDecl.find(originalRequires);
                if (pos != std::string::npos) {
                    newFunctionDecl.replace(pos, originalRequires.length(), modifiedRequires);
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFunctionDecl);
            }
        }
    }
}
  
void MutatorFrontendAction_686::MutatorASTConsumer_686::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        isTemplateInstantiation(),
        hasRequiresClause(),
        hasAnyParameter(hasType(autoType()))
    ).bind("FunctionWithRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}