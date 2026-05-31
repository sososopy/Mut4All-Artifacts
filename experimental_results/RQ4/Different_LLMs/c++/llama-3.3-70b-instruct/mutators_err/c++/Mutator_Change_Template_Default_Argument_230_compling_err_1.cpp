//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Template_Default_Argument_230
 */ 
class MutatorFrontendAction_Change_Template_Default_Argument_230 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Change_Template_Default_Argument_230)

private:
    class MutatorASTConsumer_Change_Template_Default_Argument_230 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Change_Template_Default_Argument_230(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Change_Template_Default_Argument_230.h"

// ========================================================================================================
#define MUT230_OUTPUT 1

void MutatorFrontendAction_Change_Template_Default_Argument_230::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string newDefaultArgument;
      if (MT->getTemplateParameters()->getParam(0)->hasDefaultArgument()) {
        std::string currentDefaultArgument = MT->getTemplateParameters()->getParam(0)->getDefaultArgument()->getSourceRange().getBegin().printToString(*Result.SourceManager);
        if (currentDefaultArgument == "int") {
          newDefaultArgument = "double";
        } else if (currentDefaultArgument == "double") {
          newDefaultArgument = "float";
        } else {
          newDefaultArgument = "int";
        }
      } else {
        newDefaultArgument = "int";
      }
      declaration.replace(declaration.find("typename T"), 10, "typename T = " + newDefaultArgument);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Change_Template_Default_Argument_230::MutatorASTConsumer_Change_Template_Default_Argument_230::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}