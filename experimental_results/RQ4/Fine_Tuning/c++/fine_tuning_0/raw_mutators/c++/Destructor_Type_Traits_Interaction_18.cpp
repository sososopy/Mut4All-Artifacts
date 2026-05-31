//header file
#pragma once
#include "Mutator_base.h"

/**
 * Destructor_Type_Traits_Interaction_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *target_class;
        const clang::CXXDestructorDecl *target_destructor;
    };
};

//source file
#include "../include/Destructor_Type_Traits_Interaction_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isLambda())
        return;
      target_class = DL;
      llvm::outs() << "mut18: Found target class: " << DL->getNameAsString()
                   << "\n";
    } else if (auto *DT = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>(
                   "Destructors")) {
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getLocation()))
        return;
      if (DT->isUserProvided() == false)
        return;
      target_destructor = DT;
      llvm::outs() << "mut18: Found target destructor: "
                   << DT->getNameAsString() << "\n";
    } else if (auto *FT =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      if (FT->getNameAsString() != "main")
        return;
      llvm::outs() << "mut18: Found target function: " << FT->getNameAsString()
                   << "\n";
      auto class_name = target_class->getNameAsString();
      auto destructor_name = target_destructor->getNameAsString();
      auto destructor_range = target_destructor->getSourceRange();
      auto destructor_str = stringutils::rangetoStr(
          *(Result.SourceManager), target_destructor->getSourceRange());
      llvm::outs() << "mut18: Destructor string: " << destructor_str << "\n";
      auto new_destructor_str = destructor_str;
      new_destructor_str.insert(destructor_str.find('=') + 1, " ");
      llvm::outs() << "mut18: New destructor string: " << new_destructor_str
                   << "\n";
      Rewrite.ReplaceText(destructor_range, new_destructor_str);
      std::vector<string> type_traits = {
          "std::is_trivially_constructible", "std::is_trivially_copyable",
          "std::is_trivially_default_constructible",
          "std::is_trivially_destructible", "std::is_trivially_move_assignable",
          "std::is_trivially_move_constructible"};
      string new_code = "/*mut18*/\n";
      for (auto type_trait : type_traits) {
        new_code += "std::cout << " + type_trait + "<" + class_name +
                    ">::value << std::endl;\n";
      }
      llvm::outs() << "mut18: New code: " << new_code << "\n";
      Rewrite.ReplaceText(FT->getBody()->getSourceRange(),
                          "{" + new_code + "}");
    }
  }

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto destructor_matcher = cxxDestructorDecl().bind("Destructors");
    auto function_matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(destructor_matcher, &callback);
    matchFinder.addMatcher(function_matcher, &callback);
    matchFinder.matchAST(Context);
}