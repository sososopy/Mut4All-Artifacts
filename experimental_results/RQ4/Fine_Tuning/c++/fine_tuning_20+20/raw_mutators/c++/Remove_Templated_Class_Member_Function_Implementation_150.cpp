//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Templated_Class_Member_Function_Implementation_150
 */ 
class MutatorFrontendAction_150 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(150)

private:
    class MutatorASTConsumer_150 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_150(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_class = nullptr;
        std::vector<const clang::CXXMethodDecl *> cur_methods;
    };
};

//source file
#include "../include/remove_templated_class_member_function_implementation_150.h"

// ========================================================================================================
#define MUT150_OUTPUT 1

void MutatorFrontendAction_150::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return;
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    if (!CL->isCompleteDefinition())
      return;
    if (CL->isTemplated() == false)
      return;
    cur_class = CL;
    cur_methods.clear();
    auto methods = CL->methods();
    for (auto method : methods) {
      if (method->isUserProvided() == false)
        continue;
      if (method->isDefinedOutsideFunctionOrMethod())
        cur_methods.push_back(method);
    }
  } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isTemplated() == false)
      return;
    if (cur_class == nullptr)
      return;
    if (cur_methods.empty())
      return;
    for (auto method : cur_methods) {
      if (method->getNameAsString() == FD->getNameAsString()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               FD->getSourceRange());
        content = "/*mut150*/" + content;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                            content);
      }
    }
  }
}
  
void MutatorFrontendAction_150::MutatorASTConsumer_150::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}