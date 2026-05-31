//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_parameter_list_511
 */ 
class MutatorFrontendAction_511 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(511)

private:
    class MutatorASTConsumer_511 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_511(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameter_list_511.h"

// ========================================================================================================
#define MUT511_OUTPUT 1

void MutatorFrontendAction_511::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      auto templateParams = FTD->getTemplateParameters();
      if (templateParams->size() < 2)
        return;

      auto sourceRange = FTD->getSourceRange();
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      size_t pos = declaration.find('>');
      if (pos != std::string::npos) {
        declaration.insert(pos, ", template<typename> class T");
        pos = declaration.find('{');
        if (pos != std::string::npos) {
          declaration.insert(pos + 1, "\nusing NewType = T<Base>;\nstd::make_unique<NewType>();\n");
        }
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), declaration);
      }
    }
}
  
void MutatorFrontendAction_511::MutatorASTConsumer_511::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasTemplateParameterList(
        has(templateTypeParmDecl()), has(templateTemplateParmDecl()))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}