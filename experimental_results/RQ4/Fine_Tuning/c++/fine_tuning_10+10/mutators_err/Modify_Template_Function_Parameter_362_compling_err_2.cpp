//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Function_Parameter_362
 */ 
class MutatorFrontendAction_362 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(362)

private:
    class MutatorASTConsumer_362 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_362(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Function_Parameter_362.h"

// ========================================================================================================
#define MUT362_OUTPUT 1

void MutatorFrontendAction_362::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      // Ensure the function is a template and has an operator overload
      if (MT->getDescribedFunctionTemplate() && MT->isOverloadedOperator()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string newParam = ", int extraParam = 0";
        size_t pos = declaration.find(')');
        if (pos != std::string::npos) {
          declaration.insert(pos, newParam);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_362::MutatorASTConsumer_362::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasOverloadedOperatorName()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}