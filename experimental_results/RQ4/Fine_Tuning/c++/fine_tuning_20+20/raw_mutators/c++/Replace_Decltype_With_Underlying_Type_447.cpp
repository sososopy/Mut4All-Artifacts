//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Decltype_With_Underlying_Type_447
 */ 
class MutatorFrontendAction_447 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(447)

private:
    class MutatorASTConsumer_447 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_447(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_decltype_with_underlying_type_447.h"

// ========================================================================================================
#define MUT447_OUTPUT 1

void MutatorFrontendAction_447::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeType>("decltype")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto decltype_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      llvm::outs() << decltype_str << '\n';
      auto underlying = MT->getUnderlyingType();
      auto underlying_str =
          underlying.getAsString(Result.Context->getPrintingPolicy());
      llvm::outs() << underlying_str << '\n';
      //Perform mutation on the source code text by applying string replacement
      decltype_str.replace(decltype_str.find("decltype"), 8, underlying_str);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          decltype_str);
    }
}
  
void MutatorFrontendAction_447::MutatorASTConsumer_447::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decltypeType().bind("decltype");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}