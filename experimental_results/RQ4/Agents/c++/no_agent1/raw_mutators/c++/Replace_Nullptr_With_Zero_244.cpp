//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Nullptr_With_Zero_244
 */ 
class MutatorFrontendAction_244 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(244)

private:
    class MutatorASTConsumer_244 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_244(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_nullptr_with_zero_244.h"

// ========================================================================================================
#define MUT244_OUTPUT 1

void MutatorFrontendAction_244::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *NL = Result.Nodes.getNodeAs<clang::CXXNullPtrLiteralExpr>("NullptrLiteral")) {
      // Filter nodes in header files
      if (!NL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NL->getLocation()))
        return;

      // Get the source code text of target node
      auto sourceRange = NL->getSourceRange();
      auto text = "0";

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), text);
    }
}
  
void MutatorFrontendAction_244::MutatorASTConsumer_244::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    StatementMatcher matcher = cxxNullPtrLiteralExpr().bind("NullptrLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}