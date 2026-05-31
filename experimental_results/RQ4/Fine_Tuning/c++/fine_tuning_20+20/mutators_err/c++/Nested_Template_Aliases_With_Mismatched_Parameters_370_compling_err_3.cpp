//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Aliases_With_Mismatched_Parameters_370
 */ 
class MutatorFrontendAction_370 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(370)

private:
    class MutatorASTConsumer_370 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_370(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateDecl *> cur_templates;
    };
};

//source file
#include "../include/nested_template_aliases_with_mismatched_parameters_370.h"

// ========================================================================================================
#define MUT370_OUTPUT 1

void MutatorFrontendAction_370::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DL->getLocation()))
          return;
        cur_templates.push_back(DL);
      } else if (auto *AL =
                     Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Aliases")) {
        if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       AL->getLocation()))
          return;
        if (cur_templates.empty())
          return;
        auto DL = cur_templates.back();
        if (!DL->isTemplated())
          return;
        auto params = DL->getTemplateParameters();
        auto paramstr = stringutils::rangetoStr(*(Result.SourceManager),
                                                params->getSourceRange());
        if (params->size() == 0)
          return;
        if (params->size() == 1) {
          paramstr += "<>";
        } else {
          paramstr += "<";
          for (std::size_t i = 1; i < params->size(); ++i) {
            auto param = params->getParam(i);
            auto paramname = stringutils::rangetoStr(
                *(Result.SourceManager), param->getSourceRange());
            paramstr += paramname;
            if (i != params->size() - 1)
              paramstr += ",";
          }
          paramstr += ">";
        }
        paramstr = "/*mut370*/" + paramstr;
        Rewrite.ReplaceText(AL->getBeginLoc(), 0, paramstr);
      }
}
  
void MutatorFrontendAction_370::MutatorASTConsumer_370::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = clang::ast_matchers::templateDecl().bind("Templates");
    auto alias_matcher = clang::ast_matchers::typeAliasDecl().bind("Aliases");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(alias_matcher, &callback);
    matchFinder.matchAST(Context);
}