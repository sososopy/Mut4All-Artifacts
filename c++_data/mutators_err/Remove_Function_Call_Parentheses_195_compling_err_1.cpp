//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Function_Call_Parentheses_195
 */ 
class MutatorFrontendAction_195 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(195)

private:
    class MutatorASTConsumer_195 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_195(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Remove_Function_Call_Parentheses_195.h"

// ========================================================================================================
#define MUT195_OUTPUT 1

void MutatorFrontendAction_195::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("MemberTemplateCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      //Get the source code text of target node
      auto callee = MT->getCallee();
      if (auto *DRE = dyn_cast<DeclRefExpr>(callee)) {
        if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
          if (FD->isTemplateInstantiation()) {
            //Perform mutation on the source code text by applying string replacement
            auto calleeText = stringutils::rangetoStr(*(Result.SourceManager), DRE->getSourceRange());
            auto templateArgs = FD->getTemplateSpecializationArgsAsWritten()->getSourceRange();
            auto templateArgsText = stringutils::rangetoStr(*(Result.SourceManager), templateArgs);
            std::string mutatedText = calleeText + templateArgsText;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_195::MutatorASTConsumer_195::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxMemberCallExpr(callee(functionDecl(isTemplateInstantiation()))).bind("MemberTemplateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}