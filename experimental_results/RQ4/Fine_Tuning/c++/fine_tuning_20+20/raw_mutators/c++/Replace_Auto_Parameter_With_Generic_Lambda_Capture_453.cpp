//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Auto_Parameter_With_Generic_Lambda_Capture_453
 */ 
class MutatorFrontendAction_453 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(453)

private:
    class MutatorASTConsumer_453 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_453(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_auto_parameter_with_generic_lambda_capture_453.h"

// ========================================================================================================
#define MUT453_OUTPUT 1

void MutatorFrontendAction_453::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (definition.find("auto&&") != string::npos) {
        definition.replace(definition.find("auto&&"), 6, "[&](auto&&");
        definition.insert(definition.find('{'), "){return ");
        definition.insert(definition.rfind('}'), ";}");
        llvm::outs() << definition;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(FD->getSourceRange()), definition);
      }
    }
}
  
void MutatorFrontendAction_453::MutatorASTConsumer_453::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}