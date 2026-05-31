//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Keyword_In_Dependent_Nested_Name_233
 */ 
class MutatorFrontendAction_233 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(233)

private:
    class MutatorASTConsumer_233 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_233(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Template_Keyword_In_Dependent_Nested_Name_233.h"

// ========================================================================================================
#define MUT233_OUTPUT 1

void MutatorFrontendAction_233::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UE = Result.Nodes.getNodeAs<clang::UnaryExprOrTypeTraitExpr>("UnaryExpr")) {
      //Filter nodes in header files
      if (!UE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UE->getLocation()))
        return;
      //Get the source code text of target node
      auto exprText = stringutils::rangetoStr(*(Result.SourceManager), UE->getSourceRange());
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      size_t templatePos = exprText.find("template ");
      if (templatePos != std::string::npos) {
        exprText.erase(templatePos, 9); // Remove "template "
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(UE->getSourceRange()), exprText);
    }
}
  
void MutatorFrontendAction_233::MutatorASTConsumer_233::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = unaryExprOrTypeTraitExpr(hasType(templateSpecializationType())).bind("UnaryExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}