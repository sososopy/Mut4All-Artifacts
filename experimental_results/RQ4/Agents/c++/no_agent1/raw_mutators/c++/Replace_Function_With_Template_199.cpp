//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Template_199
 */ 

class MutatorFrontendAction_199 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(199)
private:
    class MutatorASTConsumer_199 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_199(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_with_template_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto funcDeclaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string templatePrefix = "template<typename T> ";
        std::string newFuncDeclaration = templatePrefix + funcDeclaration;
        newFuncDeclaration.insert(newFuncDeclaration.find("{") + 1, "\n/*mut199*/T dummyVar; // Template variable\n");
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFuncDeclaration);
      }
    }
}
  
void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}