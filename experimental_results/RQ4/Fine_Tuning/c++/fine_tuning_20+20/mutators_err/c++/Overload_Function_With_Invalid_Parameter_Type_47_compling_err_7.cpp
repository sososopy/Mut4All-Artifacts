//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_Function_With_Invalid_Parameter_Type_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)

private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> cur_functions;
    };
};

//source file
#include "../include/overload_function_with_invalid_parameter_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
    if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FD->getLocation()))
      return;
    if (FD->isOverloadedOperator())
      return;
    if (FD->getNameAsString() == "")
      return;
    if (FD->getNameAsString() == "main")
      return;
    cur_functions.push_back(FD);
  } else if (auto *OD =
                 Result.Nodes.getNodeAs<clang::FunctionDecl>("Overloads")) {
    if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   OD->getLocation()))
      return;
    if (OD->isOverloadedOperator())
      return;
    if (OD->getNameAsString() == "")
      return;
    if (OD->getNameAsString() == "main")
      return;
    if (OD->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (OD->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == "")
      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("DL")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isOverloadedOperator())
      return;
    if (DL->getNameAsString() == ""
)      return;
    if (DL->getNameAsString() == "main")
      return;
    if (DL->getNameAsString() != cur_functions.back()->getNameAsString())
      return;
    if (DL->getNameAsString() == cur_functions.back()->getNameAsString())
      cur_functions.pop_back();
  } else if (auto *DL = Result.Nodes