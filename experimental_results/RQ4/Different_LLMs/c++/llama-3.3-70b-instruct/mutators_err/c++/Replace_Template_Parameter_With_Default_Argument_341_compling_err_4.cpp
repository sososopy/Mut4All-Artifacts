//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Parameter_With_Default_Argument_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_341>(TheRewriter);
    }

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
    Rewriter TheRewriter;
};

//source file
#include "../include/Replace_Template_Parameter_With_Default_Argument_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      for (auto param = templateParams->begin(); param != templateParams->end(); param++) {
        if (auto *tparam = dyn_cast<clang::TemplateTypeParmDecl>(*param)) {
          if (tparam->hasDefaultArgument()) {
            auto defaultArg = tparam->getDefaultArgument();
            //Perform mutation on the source code text by applying string replacement
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getSourceRange());
            declaration.replace(declaration.find(tparam->getNameAsString()), tparam->getNameAsString().length(), defaultArg->getType().getAsString());
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}