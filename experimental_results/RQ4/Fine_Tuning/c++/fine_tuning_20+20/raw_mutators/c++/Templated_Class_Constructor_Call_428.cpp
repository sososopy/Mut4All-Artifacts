//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Callable-Templated_Class_Constructor_Call_428
 */ 
class MutatorFrontendAction_428 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(428)

private:
    class MutatorASTConsumer_428 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_428(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> template_names;
    };
};

//source file
#include "../include/Mutate_Callable-Templated_Class_Constructor_Call_428.h"

// ========================================================================================================
#define MUT428_OUTPUT 1

void MutatorFrontendAction_428::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstructExpr")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->getNumArgs() == 0)
        return;
      if (!MT->getConstructor()->isTemplated())
        return;
      auto DL = MT->getConstructor()->getParent();
      if (!DL->isTemplated())
        return;
      if (DL->getDescribedClassTemplate() == nullptr)
        return;
      auto DLname = DL->getNameAsString();
      llvm::outs() << DLname << '\n';
      if (std::find(template_names.begin(), template_names.end(), DLname) ==
          template_names.end())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto pos = content.find('(');
      string template_args = "";
      auto TDL = DL->getDescribedClassTemplate();
      auto params = TDL->getTemplateParameters();
      for (auto param : *params) {
        if (param->isTemplateParameter() == false)
          continue;
        if (template_args != "")
          template_args += ',';
        auto TP = dyn_cast<TemplateTypeParmDecl>(param);
        if (TP == nullptr)
          continue;
        auto index = TP->getIndex();
        llvm::outs() << index << '\n';
        template_args += "typename";
      }
      llvm::outs() << template_args << '\n';
      if (pos != string::npos && template_args != "")
        content.insert(pos, '<' + template_args + '>');
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxConstructExpr().bind("ConstructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder matchFinder2;
    auto matcher2 = classTemplateDecl().bind("TemplateDecl");
    matchFinder2.addMatcher(matcher2, &callback);
    matchFinder2.matchAST(Context);
}