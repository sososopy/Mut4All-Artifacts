//header file
#pragma once
#include "Mutator_base.h"

/**
 * Static_Function_Declaration_In_Anonymous_Struct_159
 */ 
class MutatorFrontendAction_159 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(159)

private:
    class MutatorASTConsumer_159 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_159(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Static_Function_Declaration_In_Anonymous_Struct_159.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AnonStruct = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonStruct")) {
      //Filter nodes in header files
      if (!AnonStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AnonStruct->getLocation()))
        return;
      
      if (!AnonStruct->isAnonymousStructOrUnion())
        return;

      //Get the source code text of target node
      auto structText = stringutils::rangetoStr(*(Result.SourceManager), AnonStruct->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string staticFuncDecl = "static void functionName; /*mut159*/\n";
      if (structText.find('{') != std::string::npos) {
        structText.insert(structText.find('{') + 1, staticFuncDecl);
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AnonStruct->getSourceRange()), structText);
    }
}
  
void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(isAnonymousStructOrUnion()).bind("AnonStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}