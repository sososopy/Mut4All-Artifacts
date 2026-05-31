//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Lambda_with_Parameter_Packs_452
 */ 
class MutatorFrontendAction_452 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(452)

private:
    class MutatorASTConsumer_452 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_452(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Nested_Lambda_with_Parameter_Packs_452.h"

// ========================================================================================================
#define MUT452_OUTPUT 1

void MutatorFrontendAction_452::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TU")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      string mutcode =
          "/*mut452*/\ntemplate <typename... Ts> void exampleFunc(Ts... "
          "params) {\n  (void)[&](auto... innerParams) {\n    ([&] { "
          "params; innerParams; }, ...);\n  }();\n}\n\nint main() {\n  "
          "exampleFunc();\n  return 0;\n}\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getBeginLoc(), 0, mutcode);
    }
}
  
void MutatorFrontendAction_452::MutatorASTConsumer_452::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = translationUnitDecl().bind("TU");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}