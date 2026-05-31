//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Variable_In_Function_Body_342
 */ 
class MutatorFrontendAction_342 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(342)

private:
    class MutatorASTConsumer_342 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_342(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/insert_unused_variable_in_function_body_342.h"

// ========================================================================================================
#define MUT342_OUTPUT 1

void MutatorFrontendAction_342::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string unusedVar = "\nint unusedVar_342 = 0; /*mut342*/\n";
        functionBody.insert(functionBody.find("{") + 1, unusedVar);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_342::MutatorASTConsumer_342::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}