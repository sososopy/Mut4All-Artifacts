//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_OpenMP_Depend_Clause_348
 */ 
class MutatorFrontendAction_348 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(348)

private:
    class MutatorASTConsumer_348 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_348(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_OpenMP_Depend_Clause_348.h"

// ========================================================================================================
#define MUT348_OUTPUT 1

void MutatorFrontendAction_348::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::OMPDependClause>("DependClause")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto dep =
          stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      llvm::outs() << dep << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto varlist = MT->varlists();
      for (auto var : varlist) {
        auto name = stringutils::rangetoStr(*(Result.SourceManager),
                                            var->getSourceRange());
        dep.replace(dep.find(name), name.length(), name + "[0.5]");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getBeginLoc(),MT->getEndLoc()), dep);
    }
}
  
void MutatorFrontendAction_348::MutatorASTConsumer_348::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::ompDependClause().bind("DependClause");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}