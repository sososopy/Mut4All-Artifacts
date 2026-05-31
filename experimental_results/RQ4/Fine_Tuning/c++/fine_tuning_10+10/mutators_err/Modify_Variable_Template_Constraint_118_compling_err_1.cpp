//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Variable_Template_Constraint_118
 */ 
class MutatorFrontendAction_118 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(118)

private:
    class MutatorASTConsumer_118 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_118(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Variable_Template_Constraint_118.h"

// ========================================================================================================
#define MUT118_OUTPUT 1

void MutatorFrontendAction_118::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (auto *ReqClause = VD->getRequiresClause()) {
        auto originalConstraint = stringutils::rangetoStr(*(Result.SourceManager), ReqClause->getSourceRange());
        std::string mutatedConstraint = "requires(false)"; // Introducing an always false constraint
        std::string declaration = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
        size_t pos = declaration.find(originalConstraint);
        if (pos != std::string::npos) {
          declaration.replace(pos, originalConstraint.length(), mutatedConstraint);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_118::MutatorASTConsumer_118::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varTemplateDecl(hasRequiresClause()).bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}