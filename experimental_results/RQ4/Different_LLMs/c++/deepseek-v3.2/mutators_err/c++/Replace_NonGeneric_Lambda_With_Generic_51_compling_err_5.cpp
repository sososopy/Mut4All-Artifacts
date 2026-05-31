//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_NonGeneric_Lambda_With_Generic_51
 */ 
class MutatorFrontendAction_51 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(51)

private:
    class MutatorASTConsumer_51 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_51(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_51.h"

// ========================================================================================================
#define MUT51_OUTPUT 1

void MutatorFrontendAction_51::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Check if lambda already has template parameters (is generic)
      if (MT->getTemplateParameterList())
        return;
      
      // Ensure lambda is in a capture context (has a capture default or specific captures)
      if (!MT->hasCaptures() && MT->getCaptureDefault() == clang::LCD_None)
         return;
      
      //Get the source code text of target node
      auto lambdaRange = CharSourceRange::getTokenRange(MT->getSourceRange());
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      
      // Find the position to insert template parameter list
      size_t insertPos = lambdaText.find('[');
      if (insertPos == std::string::npos)
        return;
      
      // Find the end of capture list
      size_t captureEnd = lambdaText.find(']', insertPos);
      if (captureEnd == std::string::npos)
        return;
      
      // Check for mutable specifier
      size_t mutablePos = lambdaText.find("mutable", captureEnd);
      size_t paramStart = captureEnd + 1;
      if (mutablePos != std::string::npos && mutablePos < lambdaText.find('(', captureEnd)) {
        paramStart = mutablePos + 7; // length of "mutable"
      }
      
      // Find parameter list start
      size_t paramListStart = lambdaText.find('(', paramStart);
      if (paramListStart == std::string::npos)
        return;
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = lambdaText;
      mutatedText.insert(paramListStart, "template<typename T> ");
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(lambdaRange, mutatedText);
    }
}
  
void MutatorFrontendAction_51::MutatorASTConsumer_51::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}