//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_RangeBasedForLoop_Initializer_With_Invalid_Ternary_Expression_331
 */ 
class MutatorFrontendAction_331 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(331)

private:
    class MutatorASTConsumer_331 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_331(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_RangeBasedForLoop_Initializer_With_Invalid_Ternary_Expression_331.h"

// ========================================================================================================
#define MUT331_OUTPUT 1

void MutatorFrontendAction_331::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXForRangeStmt>("ForRange")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto DLstr = stringutils::rangetoStr(*(Result.SourceManager),
                                           DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      DLstr.replace(DLstr.find(':'),
                    DLstr.find('{') - DLstr.find(':') - 1, "cond?1:0");
      DLstr.insert(DLstr.find('{'), ";");
      llvm::outs() << DLstr << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), DLstr);
    }
}
  
void MutatorFrontendAction_331::MutatorASTConsumer_331::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxForRangeStmt().bind("ForRange");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}