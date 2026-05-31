//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Operator_Overload_With_Template_33
 */ 
class MutatorFrontendAction_33 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(33)

private:
    class MutatorASTConsumer_33 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_33(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_operator_overload_with_template_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Operator")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->isOverloadedOperator()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        std::string templateReplacement = "template<typename T> T operator+(const T& lhs, const T& rhs) { return lhs + rhs; }";
        declaration = "/*mut33*/" + templateReplacement;

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isOverloadedOperator()).bind("Operator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}