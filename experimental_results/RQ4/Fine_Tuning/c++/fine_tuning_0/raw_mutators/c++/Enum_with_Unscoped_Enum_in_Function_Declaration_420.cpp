//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_with_Unscoped_Enum_in_Function_Declaration_420
 */ 
class MutatorFrontendAction_420 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(420)

private:
    class MutatorASTConsumer_420 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_420(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Enum_with_Unscoped_Enum_in_Function_Declaration_420.h"

// ========================================================================================================
#define MUT420_OUTPUT 1

void MutatorFrontendAction_420::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration.insert(declaration.find("{") + 1,
                         "\n/*mut420*/enum {A=0,B=1,C=2}mut420=A;\n");
      declaration.insert(declaration.rfind("}"),
                         "\nmut420=B;switch(mut420){case A:break;case B:break;case C:break;default:break;}\nreturn mut420;\n");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_420::MutatorASTConsumer_420::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}