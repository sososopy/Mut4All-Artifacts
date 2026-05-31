//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Variadic_Template_Call_382
 */ 
class MutatorFrontendAction_382 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(382)

private:
    class MutatorASTConsumer_382 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_382(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl *> variadic_templates;
    };
};

//source file
#include "../include/Add_Variadic_Template_Call_382.h"

// ========================================================================================================
#define MUT382_OUTPUT 1

void MutatorFrontendAction_382::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("VariadicTemplates")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->getTemplateParameters()->size() == 1)
        return;
      variadic_templates.push_back(FT);
    }
    else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isMain())
        return;
      auto DL_name = DL->getNameAsString();
      if (DL_name != "test")
        return;
      if (variadic_templates.empty())
        return;
      auto index = getrandom::getRandomIndex(variadic_templates.size() - 1);
      auto target = variadic_templates[index];
      auto target_name = target->getNameAsString();
      auto param_size = target->getTemplateParameters()->size();
      string ins = target_name + "<";
      for (size_t i = 0; i < param_size; ++i) {
        ins += "int";
        if (i != param_size - 1)
          ins += ",";
      }
      ins += ">(";
      auto DL_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                DL->getSourceRange());
      ins += DL_content;
      ins += ")";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(DL->getEndLoc(), 0, "/*mut382*/" + ins + ";\n");
    }
}
  
void MutatorFrontendAction_382::MutatorASTConsumer_382::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto variadic_template_matcher =
        functionTemplateDecl(has(cxxMethodDecl(hasDescendant(
                                 exprWithCleanups(hasDescendant(packExpansionExpr()))))))
            .bind("VariadicTemplates");
    auto test_decl_matcher = functionDecl().bind("DL");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(variadic_template_matcher, &callback);
    matchFinder.addMatcher(test_decl_matcher, &callback);
    matchFinder.matchAST(Context);
}