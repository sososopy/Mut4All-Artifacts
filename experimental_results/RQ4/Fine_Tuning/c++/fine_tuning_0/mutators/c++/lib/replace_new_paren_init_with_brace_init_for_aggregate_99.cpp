//source file
#include "../include/Replace_New_Paren_Init_With_Brace_Init_For_Aggregate_99.h"

// ========================================================================================================
#define MUT99_OUTPUT 1

void MutatorFrontendAction_99::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isAggregate())
        cur_classes.push_back(CL);
    } else if (auto *NE =
                   Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
      if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NE->getBeginLoc()))
        return;
      if (NE->getNumPlacementArgs() != 0)
        return;
      if (NE->isArray())
        return;
      auto construct_expr = NE->getConstructExpr();
      if (construct_expr->isListInitialization())
        return;
      auto type = NE->getAllocatedType();
      if (type->isBuiltinType())
        return;
      auto type_name = type.getAsString();
      for (auto cur_class : cur_classes) {
        if (cur_class->getNameAsString() == type_name) {
          auto args = construct_expr->getArgs();
          if (construct_expr->getNumArgs() == 0)
            return;
          auto args_text = stringutils::rangetoStr(*(Result.SourceManager),
                                                   NE->getSourceRange());
          llvm::outs() << args_text << '\n';
          args_text.insert(args_text.find('(') + 1, "{");
          args_text.insert(args_text.rfind(')'), "}");
          llvm::outs() << args_text << '\n';
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()), args_text);
          return;
        }
      }
    }
}
  
void MutatorFrontendAction_99::MutatorASTConsumer_99::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto newexpr_matcher = cxxNewExpr().bind("NewExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(newexpr_matcher, &callback);
    matchFinder.matchAST(Context);
}