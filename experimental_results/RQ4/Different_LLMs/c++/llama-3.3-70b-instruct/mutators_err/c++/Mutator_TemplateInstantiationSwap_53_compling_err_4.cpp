//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationSwap_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_53>(TheRewriter);
    }

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationSwap_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>(("TemplateInstantiation"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getAsTypeLoc().getSourceLocation()))
        return;
      //Get the source code text of target node
      auto templateArgs = MT->getTemplateName().getAsTemplateDecl();
      if (!templateArgs)
        return;
      std::string templateArgsStr;
      for (auto arg : MT->template_arguments()) {
        templateArgsStr += arg.getAsType().getAsString();
        templateArgsStr += ", ";
      }
      templateArgsStr.pop_back();
      templateArgsStr.pop_back();
      //Perform mutation on the source code text by applying string replacement
      SourceLocation startLoc = MT->getAsTypeLoc().getSourceLocation();
      SourceLocation endLoc = MT->getAsTypeLoc().getEndLoc();
      std::string mutatedCode = "/*mut53*/" + templateArgs->getNameAsString() + "<" + templateArgsStr + ">";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getCharRange(startLoc, endLoc), mutatedCode);
    }
}

void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher matcher = templateSpecializationType().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}