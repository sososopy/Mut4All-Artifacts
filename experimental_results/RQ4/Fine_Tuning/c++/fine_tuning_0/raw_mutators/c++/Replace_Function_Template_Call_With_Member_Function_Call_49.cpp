//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Call_With_Member_Function_Call_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl *> cur_classes;
        std::vector<const clang::CXXMethodDecl *> cur_methods;
    };
};

//source file
#include "../include/Replace_Function_Template_Call_With_Member_Function_Call_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      llvm::outs() << "Class: " << CL->getNameAsString() << '\n';
      cur_classes.push_back(CL);
    } else if (auto *MT =
                   Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->hasBody() && !MT->isVirtual()) {
        cur_methods.push_back(MT);
      }
    } else if (auto *FC = Result.Nodes.getNodeAs<clang::CallExpr>("Funcalls")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getBeginLoc()))
        return;
      if (cur_methods.empty())
        return;
      auto cur_method = cur_methods.back();
      if (!cur_method->getBody()->getSourceRange().fullyContains(
              FC->getSourceRange()))
        return;
      if (FC->getDirectCallee() == nullptr)
        return;
      auto callee = FC->getDirectCallee();
      if (!callee->isFunctionTemplateSpecialization())
        return;
      auto cur_class = cur_classes.back();
      auto methods = cur_class->methods();
      for (auto method : methods) {
        if (method->getNameAsString() == callee->getNameAsString())
          return;
      }
      auto calltext = stringutils::rangetoStr(*(Result.SourceManager),
                                              FC->getSourceRange());
      calltext = cur_class->getNameAsString() + "::" + calltext;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FC->getSourceRange()),
                          calltext);
    }
  }

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto method_matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    auto funccall_matcher = callExpr().bind("Funcalls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(method_matcher, &callback);
    matchFinder.addMatcher(funccall_matcher, &callback);
    matchFinder.matchAST(Context);
}