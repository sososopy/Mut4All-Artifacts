//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Member_Accessor_261
 */ 
class MutatorFrontendAction_261 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(261)

private:
    class MutatorASTConsumer_261 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_261(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXRecordDecl *> cur_specializations;
        std::vector<const clang::CXXMethodDecl *> cur_methods;
        std::vector<const clang::VarDecl *> cur_vars;
    };
};

//source file
#include "../include/Modify_Template_Member_Accessor_261.h"

// ========================================================================================================
#define MUT261_OUTPUT 1

void MutatorFrontendAction_261::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (!CL->isTemplated())
      return;
    if (CL->getTemplateSpecializationKind() !=
        TemplateSpecializationKind::TSK_ExplicitSpecialization)
      return;
    llvm::outs() << "mut261: find specialization class "
                 << CL->getNameAsString() << '\n';
    cur_specializations.push_back(CL);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Template")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (!DL->isCompleteDefinition())
      return;
    if (DL->getDescribedClassTemplate() == nullptr)
      return;
    llvm::outs() << "mut261: find template class " << DL->getNameAsString()
                 << '\n';
    cur_classes.push_back(DL);
  } else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Method")) {
    if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   MD->getLocation()))
      return;
    if (MD->getTemplateSpecializationKind() !=
        TemplateSpecializationKind::TSK_ExplicitSpecialization)
      return;
    llvm::outs() << "mut261: find specialized method "
                 << MD->getNameAsString() << '\n';
    cur_methods.push_back(MD);
  } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variable")) {
    if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VD->getLocation()))
      return;
    if (VD->getTemplateSpecializationKind() !=
        TemplateSpecializationKind::TSK_ExplicitSpecialization)
      return;
    llvm::outs() << "mut261: find specialized variable "
                 << VD->getNameAsString() << '\n';
    cur_vars.push_back(VD);
  } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
    if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TD->getLocation()))
      return;
    llvm::outs() << "mut261: find specialized typedef "
                 << TD->getNameAsString() << '\n';
  } else if (auto *AL =
                 Result.Nodes.getNodeAs<clang::AccessSpecDecl>("Accessor")) {
    if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   AL->getLocation()))
      return;
    llvm::outs() << "mut261: find accessor\n";
    if (cur_specializations.empty())
      return;
    auto target = cur_specializations.back();
    if (cur_methods.empty() && cur_vars.empty())
      return;
    int choice = getrandom::getRandomIndex(1);
    if (cur_methods.empty())
      choice = 1;
    else if (cur_vars.empty())
      choice = 0;
    if (choice == 0) {
      auto method = cur_methods.back();
      auto rettype = method->getReturnType().getAsString();
      auto paramtype = method->getParamDecl(0)->getType().getAsString();
      auto replace = stringutils::rangetoStr(*(Result.SourceManager),
                                             method->getSourceRange());
      llvm::outs() << "mut261: method " << replace << '\n';
      auto dice = getrandom::getRandomIndex(1);
      if (dice == 0) {
        auto newtype = getrandom::getRandomType();
        llvm::outs() << "mut261: new return type " << newtype << '\n';
        stringutils::strReplace(replace, rettype, newtype);
      } else {
        auto newtype = getrandom::getRandomType();
        llvm::outs() << "mut261: new param type " << newtype << '\n';
        stringutils::strReplace(replace, paramtype, newtype);
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target->getBeginLoc(), method->getEndLoc()),
                          replace);
    } else {
      auto var = cur_vars.back();
      auto vartype = var->getType().getAsString();
      auto replace = stringutils::rangetoStr(*(Result.SourceManager),
                                             var->getSourceRange());
      llvm::outs() << "mut261: var " << replace << '\n';
      auto newtype = getrandom::getRandomType();
      llvm::outs() << "mut261: new var type " << newtype << '\n';
      stringutils::strReplace(replace, vartype, newtype);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              target->getBeginLoc(), var->getEndLoc()),
                          replace);
    }
  }
}

void MutatorFrontendAction_261::MutatorASTConsumer_261::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto template_matcher = cxxRecordDecl().bind("Template");
    auto method_matcher = cxxMethodDecl().bind("Method");
    auto var_matcher = varDecl().bind("Variable");
    auto typedef_matcher = typedefDecl().bind("Typedef");
    auto accessor_matcher = accessSpecDecl().bind("Accessor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.addMatcher(accessor_matcher, &callback);
    matchFinder.matchAST(Context);
}