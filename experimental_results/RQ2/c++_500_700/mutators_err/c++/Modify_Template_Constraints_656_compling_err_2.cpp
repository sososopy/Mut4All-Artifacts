//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_constraints_656
 */ 
class MutatorFrontendAction_656 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(656)

private:
    class MutatorASTConsumer_656 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_656(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_constraints_656.h"

// ========================================================================================================
#define MUT656_OUTPUT 1

void MutatorFrontendAction_656::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (const auto *Constraints = FTD->getTemplateParameters()->getRequiresClause()) {
        auto originalConstraints = stringutils::rangetoStr(*(Result.SourceManager), Constraints->getSourceRange());
        
        std::string modifiedConstraints = "IsSame<T, Empty> && " + originalConstraints;

        Rewrite.ReplaceText(Constraints->getSourceRange(), modifiedConstraints);
      }
    }
}
  
void MutatorFrontendAction_656::MutatorASTConsumer_656::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateParameterList(hasRequiresClause()))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}