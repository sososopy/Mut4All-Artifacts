//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Function_Templates_With_Unknown_Return_Types_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> target_templates;
    };
};

//source file
#include "../include/target_function_templates_with_unknown_return_types_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplatedDecl()->getReturnType().getAsString() != "auto")
        return;
      if (FT->getTemplatedDecl()->hasBody() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
      if (content.find("decltype") != string::npos)
        return;
      target_templates.push_back(FT);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::DeclStmt>("DeclStmts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (target_templates.empty())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("decltype") != string::npos)
        return;
      auto target = target_templates[getrandom::getRandomIndex(
          target_templates.size() - 1)];
      auto decl = "/*mut19*/decltype(auto) g = &" +
                  target->getNameAsString() + "<int>;";
      content = content + "\n" + decl;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionTemplateDecl().bind("FuncTemplates");
    auto decl_matcher = declStmt().bind("DeclStmts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}