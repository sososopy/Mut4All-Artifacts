//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Constexpr_Deleted_Method_15
 */ 
class MutatorFrontendAction_15 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(15)
    
    private:
      class MutatorASTConsumer_15 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_15(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_constexpr_deleted_method_15"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      //Get the source code text of target node
      auto methods = DL->methods();
      for (auto method : methods) {
        if (method->isStatic())
          continue;
        if (method->isDeleted())
          continue;
        if (method->isDefaulted())
          continue;
        if (method->isConstexpr())
          continue;
        if (method->isUserProvided() == false)
          continue;
        auto method_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                                   method->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        method_decl = "constexpr " + method_decl + "=delete";
        llvm::outs() << method_decl << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), method_decl);
      }
    }
  }

void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}