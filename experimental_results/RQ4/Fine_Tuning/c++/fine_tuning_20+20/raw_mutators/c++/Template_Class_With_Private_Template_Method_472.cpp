//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Class_With_Private_Template_Method_472
 */ 
class MutatorFrontendAction_472 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(472)

private:
    class MutatorASTConsumer_472 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_472(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<string> target_classes;
    };
};

//source file
#include "../include/template_class_with_private_template_method_472.h"

// ========================================================================================================
#define MUT472_OUTPUT 1

void MutatorFrontendAction_472::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isTemplated() == false)
        return;
      auto DL = MT->getParent();
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() || DL->isClass()) {
        auto class_name = DL->getNameAsString();
        if (DL->isLambda() == false) {
          target_classes.push_back(class_name);
          llvm::outs() << "mut472: " << class_name << '\n';
        }
      }
      if (MT->getAccess() == AccessSpecifier::AS_private)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto pos = content.find("template");
      if (pos == string::npos)
        return;
      content.insert(pos, "private:");
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (CL->isCompleteDefinition() == false)
        return;
      auto class_name = CL->getNameAsString();
      for (size_t i = 0; i < target_classes.size(); ++i) {
        if (class_name == target_classes[i]) {
          auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 CL->getSourceRange());
          auto pos = content.find(class_name);
          if (pos == string::npos)
            return;
          content.insert(pos, "/*mut472*/");
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(CL->getSourceRange()), content);
        }
      }
    }
}

void MutatorFrontendAction_472::MutatorASTConsumer_472::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("Methods");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}