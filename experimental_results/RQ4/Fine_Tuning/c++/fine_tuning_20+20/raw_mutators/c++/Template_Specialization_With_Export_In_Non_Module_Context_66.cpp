//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_With_Export_In_Non_Module_Context_66
 */ 
class MutatorFrontendAction_66 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(66)

private:
    class MutatorASTConsumer_66 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_66(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::NamedDecl *> cur_types;
    };
};

//source file
#include "../include/template_specialization_with_export_in_non_module_context_66.h"

// ========================================================================================================
#define MUT66_OUTPUT 1

void MutatorFrontendAction_66::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TP = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      cur_types.push_back(TP);
    } else if (auto *TY = Result.Nodes.getNodeAs<clang::NamedDecl>("Types")) {
      if (!TY || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TY->getLocation()))
        return;
      cur_types.push_back(TY);
    } else if (auto *SP = Result.Nodes.getNodeAs<clang::NamedDecl>("Specializations")) {
      if (!SP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SP->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), SP->getSourceRange());
      if (content.find("export") == string::npos)
        content = "/*mut66*/export " + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SP->getSourceRange()),
                          content);
    }
}

void MutatorFrontendAction_66::MutatorASTConsumer_66::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto template_matcher = classTemplateDecl().bind("Templates");
    auto type_matcher = namedDecl().bind("Types");
    auto specialization_matcher = namedDecl().bind("Specializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(type_matcher, &callback);
    matchFinder.addMatcher(specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}