//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Constexpr_Deleted_Assignment_318
 */ 
class MutatorFrontendAction_318 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(318)

private:
    class MutatorASTConsumer_318 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_318(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Constexpr_Deleted_Assignment_318.h"

// ========================================================================================================
#define MUT318_OUTPUT 1

void MutatorFrontendAction_318::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("AssignmentOperator")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
        return;

      if (MT->isUserProvided() && MT->isCopyAssignmentOperator()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
        auto pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration = "constexpr " + declaration.substr(0, pos) + "= delete;";
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_318::MutatorASTConsumer_318::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(isCopyAssignmentOperator()).bind("AssignmentOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}