```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_pseudo_destructor_expression_16
 */ 
class MutatorFrontendAction_16 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(16)

private:
    class MutatorASTConsumer_16 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_16(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/modify_pseudo_destructor_expression_16.h"

// ========================================================================================================
#define MUT16_OUTPUT 1

void MutatorFrontendAction_16::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("PseudoDestructorExpr")) {
      // Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;
      
      // Get the source code text of target node
      auto exprSourceRange = MT->getSourceRange();
      auto exprSourceText = stringutils::rangetoStr(*(Result.SourceManager), exprSourceRange);

      // Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = "(*ptr).~NonExistentType()";
      if (exprSourceText.find("~") != std::string::npos) {
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(exprSourceRange, mutatedExpr);
      }
    }
}
  
void MutatorFrontendAction_16::MutatorASTConsumer_16::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxMemberCallExpr(
                      callee(cxxPseudoDestructorExpr())
                   ).bind("PseudoDestructorExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}
```