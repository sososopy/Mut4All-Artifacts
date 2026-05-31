//header file
#pragma once
#include "Mutator_base.h"

/**
 * Out-of-Class_Definition_with_Trailing_Requires_Clause_400
 */ 
class MutatorFrontendAction_400 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(400)

private:
    class MutatorASTConsumer_400 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_400(Rewriter &R) : TheRewriter(R) {}
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
        const CXXRecordDecl *cur_class;
        std::string class_name;
        std::string template_part;
    };
};

//source file
#include "../include/Out-of-Class_Definition_with_Trailing_Requires_Clause_400.h"

// ========================================================================================================
#define MUT400_OUTPUT 1

void MutatorFrontendAction_400::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      llvm::outs() << CL->getNameAsString() << '\n';
      cur_class = CL;
      class_name = CL->getNameAsString();
      if (cur_class->getDescribedTemplate() != nullptr) {
        template_part = stringutils::rangetoStr(
            *(Result.SourceManager),
            cur_class->getDescribedTemplate()->getSourceRange());
      }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "FunctionWithTrailingRequires")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isOutOfLineDefinition())
        return;
      llvm::outs() << FD->getNameAsString() << '\n';
      auto func_name = FD->getNameAsString();
      auto func_decl = stringutils::rangetoStr(*(Result.SourceManager),
                                               FD->getSourceRange());
      auto func_body = stringutils::rangetoStr(*(Result.SourceManager),
                                               FD->getBody()->getSourceRange());
      llvm::outs() << func_decl << '\n';
      llvm::outs() << func_body << '\n';
      auto pos = func_decl.find(func_name);
      auto new_func_decl = func_decl;
      if (cur_class->getDescribedTemplate() != nullptr) {
        new_func_decl = template_part + " " + new_func_decl;
      }
      new_func_decl.insert(pos, class_name + "::");
      new_func_decl += " " + func_body;
      llvm::outs() << new_func_decl << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()),
                          "");
      Rewrite.ReplaceText(cur_class->getEndLoc(), 0, "\n" + new_func_decl);
    }
  }

void MutatorFrontendAction_400::MutatorASTConsumer_400::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto function_with_trailing_requires_matcher =
        functionDecl(hasTrailingRequiresClause()).bind(
            "FunctionWithTrailingRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(function_with_trailing_requires_matcher, &callback);
    matchFinder.matchAST(Context);
}