//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Incomplete_Type_With_Member_Pointer_258
 */ 
class MutatorFrontendAction_258 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(258)

private:
    class MutatorASTConsumer_258 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_258(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/introduce_incomplete_type_with_member_pointer_258.h"

// ========================================================================================================
#define MUT258_OUTPUT 1

void MutatorFrontendAction_258::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *E = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!E || !Result.Context->getSourceManager().isWrittenInMainFile(
                     E->getExprLoc()))
        return;

      //Get the source code text of target node
      auto exprText = Lexer::getSourceText(CharSourceRange::getTokenRange(E->getSourceRange()), 
                                           *Result.SourceManager, 
                                           Result.Context->getLangOpts());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = "static_cast<IncompleteType>(" + exprText.str() + ")";
      std::string incompleteTypeDecl = "using IncompleteType = decltype(auto);";

      //Insert the incomplete type alias declaration
      SourceLocation funcStartLoc = E->getExprLoc();
      Rewrite.InsertTextBefore(funcStartLoc, incompleteTypeDecl + "\n");

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(E->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_258::MutatorASTConsumer_258::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = memberExpr(hasObjectExpression(hasType(pointerType()))).bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}