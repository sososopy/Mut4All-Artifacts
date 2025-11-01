//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Trailing_Return_Type_Unrelated_423
 */ 
class MutatorFrontendAction_423 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(423)

private:
    class MutatorASTConsumer_423 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_423(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Trailing_Return_Type_Unrelated_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionWithTrailingReturn")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getReturnType().getTypePtr()->isDependentType()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string newReturnType = "int"; // Change to an unrelated type
        size_t arrowPos = declaration.find("->");
        if (arrowPos != std::string::npos) {
          size_t endPos = declaration.find(";", arrowPos);
          if (endPos != std::string::npos) {
            declaration.replace(arrowPos + 2, endPos - arrowPos - 2, newReturnType);
          }
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(returns(hasTrailingReturnType())).bind("functionWithTrailingReturn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}