//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Template_Parameter_Mutation_265
 */ 
class MutatorFrontendAction_265 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(265)

private:
    class MutatorASTConsumer_265 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_265(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Template_Parameter_Mutation_265.h"

// ========================================================================================================
#define MUT265_OUTPUT 1

void MutatorFrontendAction_265::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;

      if (auto *TTPD = dyn_cast<TemplateTemplateParmDecl>(TD)) {
        auto tpl = stringutils::rangetoStr(*(Result.SourceManager),
                                           TTPD->getSourceRange());
        llvm::outs() << tpl << '\n';
        if (tpl.find("typename") != string::npos)
          tpl = std::regex_replace(tpl, std::regex("typename"), "int");
        else if (tpl.find("class") != string::npos)
          tpl = std::regex_replace(tpl, std::regex("class"), "int");
        else
          return;
        tpl = "/*mut265*/" + tpl;
        Rewrite.ReplaceText(TTPD->getSourceRange(), tpl);
      }
    }
}
  
void MutatorFrontendAction_265::MutatorASTConsumer_265::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = templateDecl(has(templateTemplateParmDecl())).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}