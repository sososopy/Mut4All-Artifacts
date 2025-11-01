//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Pointer_To_Member_Expressions_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Pointer_To_Member_Expressions_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PME = Result.Nodes.getNodeAs<clang::MemberExpr>("PointerToMemberExpr")) {
      //Filter nodes in header files
      if (!PME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PME->getExprLoc()))
        return;
      
      //Get the source code text of target node
      auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), PME->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "(*" + sourceText + ")";

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PME->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = memberExpr(hasObjectExpression(cxxThisExpr())).bind("PointerToMemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}