//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_Clause_In_Out_Of_Line_Definition_107
 */ 
class MutatorFrontendAction_107 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(107)

private:
    class MutatorASTConsumer_107 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_107(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Replace_Requires_Clause_In_Out_Of_Line_Definition_107.h"

// ========================================================================================================
#define MUT107_OUTPUT 1

void MutatorFrontendAction_107::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("outOfLineFunc")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isOutOfLine())
        return;
      if (!FD->isTemplateInstantiation())
        return;
      const FunctionDecl *TemplatedFD = FD->getTemplateInstantiationPattern();
      if (!TemplatedFD)
        return;
      const Expr *RequiresExpr = TemplatedFD->getTrailingRequiresClause();
      if (!RequiresExpr)
        return;
      //Get the source code text of target node
      std::string RequiresText = stringutils::rangetoStr(*(Result.SourceManager), RequiresExpr->getSourceRange());
      if (RequiresText.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      std::string NewRequiresText;
      if (choice == 0) {
        NewRequiresText = "((" + RequiresText.substr(9, RequiresText.length() - 9) + "))";
      } else {
        NewRequiresText = "(!(" + RequiresText.substr(9, RequiresText.length() - 9) + "))";
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RequiresExpr->getSourceRange()), NewRequiresText);
    }
}
  
void MutatorFrontendAction_107::MutatorASTConsumer_107::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isOutOfLine(), isTemplateInstantiation(), hasTrailingReturn()).bind("outOfLineFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}