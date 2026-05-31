//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Parameter_Type_387
 */ 
class MutatorFrontendAction_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(387)

private:
    class MutatorASTConsumer_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_387(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/remove_template_parameter_type_387.h"

// ========================================================================================================
#define MUT387_OUTPUT 1

void MutatorFrontendAction_387::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (content.find("template") != string::npos)
        content.erase(content.find("template"),
                      content.find('>') - content.find("template") + 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_387::MutatorASTConsumer_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}