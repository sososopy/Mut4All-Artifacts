//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_Member_Operator_Delete_With_Custom_Type_144
 */ 
class MutatorFrontendAction_144 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(144)

private:
    class MutatorASTConsumer_144 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_144(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Overload_Member_Operator_Delete_With_Custom_Type_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->getNameAsString() != "operator delete")
        return;
      auto params = MT->parameters();
      if (params.size() == 0)
        return;
      auto first_param = params[0];
      if (first_param->getType().getAsString() == "void *" ||
          first_param->getType().getAsString() == "size_t") {
        if (cur_classes.empty()) {
          Rewrite.ReplaceText(first_param->getSourceRange(), "S");
        } else {
          size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
          Rewrite.ReplaceText(first_param->getSourceRange(),
                              cur_classes[index]->getNameAsString());
        }
      }
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) { // If no custom class is present, insert a new
                                 // struct S
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(

                   "Decl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (cur_classes.empty()) {
        Rewrite.ReplaceText(DL->getEndLoc(), 0, "\n/*mut144*/struct S{};\n");
      }
    }
  }

void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl().bind("Methods");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto decl_matcher = cxxRecordDecl().bind("Decl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}