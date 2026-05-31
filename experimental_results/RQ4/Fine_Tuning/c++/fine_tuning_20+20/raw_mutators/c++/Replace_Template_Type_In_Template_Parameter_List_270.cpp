//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Type_In_Template_Parameter_List_270
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/replace_template_type_in_template_parameter_list_270.h"

// ========================================================================================================
#define MUT270_OUTPUT 1

void MutatorFrontendAction_270::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      text = "U";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), text);
    }
}
  
void MutatorFrontendAction_270::MutatorASTConsumer_270::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl().bind("TemplateType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}