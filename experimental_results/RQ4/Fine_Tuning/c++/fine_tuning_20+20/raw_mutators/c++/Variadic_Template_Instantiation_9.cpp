//header file
#pragma once
#include "Mutator_base.h"

/**
 * Variadic_Template_Instantiation_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_templates;
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/variadic_template_instantiation_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
      return;
    auto params = TD->getTemplateParameters()->size();
    if (params <= 1)
      return;
    auto spec = TD->getTemplatedDecl();
    if (!spec->isCompleteDefinition())
      return;
    auto partial = TD->getPartialSpecializations();
    if (!partial.empty())
      return;
    cur_templates.push_back(TD);
  } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    cur_classes.push_back(CL);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (DL->getDescribedClassTemplate() == nullptr)
      return;
    auto DL_name = DL->getNameAsString();
    if (DL_name == "")
      return;
    for (std::size_t i = 0; i < cur_templates.size(); ++i) {
      if (cur_templates[i]->getNameAsString() == DL_name) {
        auto DL_params = DL->getTemplateArgsAsWritten();
        if (DL_params == nullptr)
          return;
        auto DL_size = DL_params->arguments().size();
        if (DL_size <= 1)
          return;
        int dice = getrandom::getRandomIndex(1);
        std::string ins = "";
        if (dice == 0) {
          if (DL_size == 2)
            return;
          ins = DL_name + "<int,int>";
        } else {
          if (cur_classes.empty())
            return;
          size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
          ins = DL_name + "<" + cur_classes[index]->getNameAsString() + ">";
        }
        ins = "/*mut9*/template<> struct " + ins + "{};\n";
        Rewrite.ReplaceText(DL->getEndLoc(), 0, ins);
      }
    }
  }
}
  
void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("Templates");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto derived_matcher = cxxRecordDecl(hasAnyTemplateArgument(anything())).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}