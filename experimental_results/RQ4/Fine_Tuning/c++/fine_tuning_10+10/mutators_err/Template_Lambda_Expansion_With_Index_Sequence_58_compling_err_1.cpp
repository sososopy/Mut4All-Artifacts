//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Lambda_Expansion_With_Index_Sequence_58
 */ 
class MutatorFrontendAction_58 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(58)

private:
    class MutatorASTConsumer_58 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_58(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Lambda_Expansion_With_Index_Sequence_58.h"

// ========================================================================================================
#define MUT58_OUTPUT 1

void MutatorFrontendAction_58::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto lambdaSource = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newTemplateParam = ", typename T";
      std::string newOperation = "+ sizeof(T)";
      size_t pos = lambdaSource.find("std::index_sequence<Is...>");
      if (pos != std::string::npos) {
          lambdaSource.insert(pos + 21, newTemplateParam);
          size_t foldPos = lambdaSource.find("N<Is>", pos);
          if (foldPos != std::string::npos) {
              lambdaSource.replace(foldPos, 5, "N<Is " + newOperation + ">");
          }
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambdaSource);
    }
}
  
void MutatorFrontendAction_58::MutatorASTConsumer_58::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasDescendant(callExpr(callee(functionDecl(hasName("std::index_sequence")))))).bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}