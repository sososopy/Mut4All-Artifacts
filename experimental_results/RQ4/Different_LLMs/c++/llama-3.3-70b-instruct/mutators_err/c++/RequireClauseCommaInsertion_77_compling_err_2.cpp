//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/AST/Decl.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * RequireClauseCommaInsertion_77
 */ 
class MutatorFrontendAction_77 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(77)

private:
    class MutatorASTConsumer_77 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_77(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/RequireClauseCommaInsertion_77.h"
#include "clang/AST/DeclCXX.h"

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) override {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Concepts")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Get the source code text of target node
      auto requiresClause = CT->getTemplateParameterList();
      if (!requiresClause)
        return;
      auto requiresClauseText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(requiresClause->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      auto mutatedRequiresClauseText = requiresClauseText;
      size_t pos = mutatedRequiresClauseText.find(';');
      if (pos != std::string::npos) {
        mutatedRequiresClauseText.insert(pos, ",");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(requiresClause->getSourceRange()), mutatedRequiresClauseText);
    }
}
  
void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}