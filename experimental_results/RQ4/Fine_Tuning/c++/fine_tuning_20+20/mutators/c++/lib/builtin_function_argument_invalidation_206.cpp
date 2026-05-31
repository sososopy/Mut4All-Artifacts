//source file
#include "../include/Builtin_Function_Argument_Invalidation_206.h"

// ========================================================================================================
#define MUT206_OUTPUT 1

void MutatorFrontendAction_206::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCalls")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      builtin_calls.push_back(CE);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCalls2")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      size_t index = getrandom::getRandomIndex(builtin_calls.size() - 1);
      llvm::outs() << "Mut206: " << index << '\n';
      auto target = builtin_calls[index];
      auto args = target->getArgs();
      size_t arg_index = getrandom::getRandomIndex(target->getNumArgs() - 1);
      auto target_arg = args[arg_index];
      auto target_arg_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                    target_arg->getSourceRange());
      std::string ins = target_arg_str + " + " + target_arg_str + " + " + target_arg_str;
      llvm::outs() << "Mut206: " << ins << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(target_arg->getSourceRange()), ins);
    }
}
  
void MutatorFrontendAction_206::MutatorASTConsumer_206::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify"))))
                       .bind("BuiltinCalls");
    auto matcher2 = callExpr(callee(functionDecl(hasName("__builtin_fpclassify"))))
                        .bind("BuiltinCalls2");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    MatchFinder finder2;
    finder2.addMatcher(matcher2, &callback);
    finder2.matchAST(Context);
}