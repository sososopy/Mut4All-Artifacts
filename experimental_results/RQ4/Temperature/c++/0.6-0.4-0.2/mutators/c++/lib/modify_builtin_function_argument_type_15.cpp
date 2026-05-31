//source file
#include "../include/modify_builtin_function_argument_type_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(Call->getBeginLoc()))
            return;

        if (Call->getNumArgs() < 1)
            return;

        auto *Arg = Call->getArg(0);
        if (!Arg->getType()->isIntegerType())
            return;

        std::string argText = Lexer::getSourceText(CharSourceRange::getTokenRange(Arg->getSourceRange()), 
                                                   *Result.SourceManager, 
                                                   Result.Context->getLangOpts()).str();

        std::string pointerType = "unsigned char*";
        std::string newArgText = "(" + pointerType + ")&" + argText;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Arg->getSourceRange()), newArgText);
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}