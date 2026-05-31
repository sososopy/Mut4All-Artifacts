//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Conversion_Function_Call_Mutation_50
 */ 
class MutatorFrontendAction_50 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(50)

private:
    class MutatorASTConsumer_50 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_50(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Conversion_Function_Call_Mutation_50.h"

// ========================================================================================================
#define MUT50_OUTPUT 1

void MutatorFrontendAction_50::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMemberCallExpr>("ConversionCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getExprLoc()))
        return;

      //Get the source code text of target node
      auto callExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedExpr = "true ? " + callExpr + " : static_cast<double>(" + callExpr + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), mutatedExpr);
    }
}

void MutatorFrontendAction_50::MutatorASTConsumer_50::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxMemberCallExpr(
                       callee(cxxMethodDecl(hasName("operator T"))),
                       hasAncestor(classTemplateSpecializationDecl())
                   ).bind("ConversionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}