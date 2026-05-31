//source file
#include "../include/Change_First_Parameter_Type_Of_Member_Operator_144.h"

// ========================================================================================================
#define MUT144_OUTPUT 1

void MutatorFrontendAction_144::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return; 
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      cur_classes.push_back(CL);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Operators")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isOverloadedOperator())
        return;
      auto params = FD->parameters();
      if (params.size() == 0)
        return;
      auto first_param = params[0];
      if (first_param->getType()->isRecordType())
        return;
      llvm::outs() << first_param->getType().getAsString() << '\n';
      int choice = getrandom::getRandomIndex(cur_classes.size() - 1);
      auto target_class = cur_classes[choice];
      auto target_class_name = target_class->getNameAsString();
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(first_param->getSourceRange()),
          target_class_name);
    }
}
  
void MutatorFrontendAction_144::MutatorASTConsumer_144::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto operator_matcher = cxxMethodDecl().bind("Operators");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(operator_matcher, &callback);
    matchFinder.matchAST(Context);
}