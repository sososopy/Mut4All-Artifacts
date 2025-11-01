//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Misuse_270
 */ 
class MutatorFrontendAction_270 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(270)

private:
    class MutatorASTConsumer_270 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_270(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Parameter_Misuse_270.h"

// ========================================================================================================
#define MUT270_OUTPUT 1

void MutatorFrontendAction_270::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TF = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!TF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TF->getLocation()))
        return;
      
      if (TF->getTemplatedDecl()->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TF->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("{");
        if (pos != std::string::npos) {
          std::string mutation = "\nclass MyClass : T // Missing body or semicolon\n";
          declaration.insert(pos + 1, mutation);
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TF->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_270::MutatorASTConsumer_270::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(cxxMethodDecl())).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}