```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Attribute_To_Lambda_Expression_483
 */ 
class MutatorFrontendAction_483 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(483)

private:
    class MutatorASTConsumer_483 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_483(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Attribute_To_Lambda_Expression_483.h"

// ========================================================================================================
#define MUT483_OUTPUT 1

void MutatorFrontendAction_483::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LT = Result.Nodes.getNodeAs<clang::LambdaExpr>(("Lambda"))) {
      //Filter nodes in header files
      if (!LT ||!Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   LT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<std::string> calling_conventions = {"aapcs", "aapcs-vfp", "intel"};
      std::string attribute = "__attribute__((pcs(\"" + calling_conventions[getrandom::getRandomIndex(calling_conventions.size())] + "\"))";
      declaration.insert(declaration.find(")"), " " + attribute);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_483::MutatorASTConsumer_483::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("Lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}