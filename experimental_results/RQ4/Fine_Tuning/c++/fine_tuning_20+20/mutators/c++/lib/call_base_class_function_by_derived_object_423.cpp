//source file
#include "../include/call_base_class_function_by_derived_object_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Base")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      base_class = CL;
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Derived")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      derived_class = CL;
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "BaseMethod")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      base_method = DL;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "MainFunction")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (base_class == nullptr || derived_class == nullptr ||
          base_method == nullptr)
        return;
      auto base_name = base_class->getNameAsString();
      auto derived_name = derived_class->getNameAsString();
      auto method_name = base_method->getNameAsString();
      auto ins = "/*mut423*/class " + base_name + "{};\nclass " +
                 derived_name + " : public " + base_name + "{};\n";
      ins += "auto mut423_obj = " + derived_name + "{};\n";
      ins += "decltype(auto) mut423_res=mut423_obj." + method_name + "();\n";
      llvm::outs() << ins << '\n';
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, ins);
    }
  }

void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto base_matcher =
        cxxRecordDecl(has(cxxMethodDecl(hasName("mut423_base")))).bind("Base");
    auto derived_matcher = cxxRecordDecl(has(cxxMethodDecl(hasName(
                                              "mut423_derived"))))
                               .bind("Derived");
    auto base_method_matcher = cxxMethodDecl(hasName("mut423_base")).bind("BaseMethod");
    auto main_matcher = functionDecl(hasName("main")).bind("MainFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(base_method_matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}