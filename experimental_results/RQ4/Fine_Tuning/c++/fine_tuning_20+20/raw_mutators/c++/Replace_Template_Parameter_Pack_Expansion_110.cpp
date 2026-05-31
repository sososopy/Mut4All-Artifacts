//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_Pack_Expansion_110
 */ 
class MutatorFrontendAction_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)

private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_template_parameter_pack_expansion_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_110::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::PackExpansionExpr>("PackExpansion")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      content += ',' + content + ',' + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), content);
    }
}
  
void MutatorFrontendAction_110::MutatorASTConsumer_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = packExpansionExpr().bind("PackExpansion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}