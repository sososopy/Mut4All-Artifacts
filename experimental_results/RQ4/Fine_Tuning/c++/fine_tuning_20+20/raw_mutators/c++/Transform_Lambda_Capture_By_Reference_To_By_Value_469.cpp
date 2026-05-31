//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Lambda_Capture_By_Reference_To_By_Value_469
 */ 
class MutatorFrontendAction_469 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(469)

private:
    class MutatorASTConsumer_469 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_469(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/transform_lambda_capture_by_reference_to_by_value_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LE->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("[&") != string::npos)
        content.replace(content.find("[&"), 2, "[=/*mut469*/");
      else if (content.find("[&]") != string::npos)
        content.replace(content.find("[&]"), 3, "[=/*mut469*/]");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasDescendant(lambdaExpr(hasAnyParameter(hasType(autoType())), hasType(autoType())))).bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}