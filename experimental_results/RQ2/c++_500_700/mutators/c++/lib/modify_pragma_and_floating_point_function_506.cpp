//source file
#include "../include/modify_pragma_and_floating_point_function_506.h"

// ========================================================================================================
#define MUT506_OUTPUT 1

void MutatorFrontendAction_506::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FloatingPointFunc")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
        return;

      if (FD->hasBody()) {
        auto body = stringutils::rangetoStr(*(Result.SourceManager), FD->getBody()->getSourceRange());
        if (body.find("#pragma STDC FP_CONTRACT") == std::string::npos) {
          body.insert(0, "#pragma STDC FP_CONTRACT ON\n");
        } else {
          size_t pos = body.find("#pragma STDC FP_CONTRACT");
          body.replace(pos, 26, "#pragma STDC FP_CONTRACT ON");
        }
        body.insert(body.find_last_of('}'), "\n  double temp = 0.0; temp += 1.0; temp *= 2.0;\n");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), body);
      }
    }
}

void MutatorFrontendAction_506::MutatorASTConsumer_506::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(hasDescendant(floatLiteral())).bind("FloatingPointFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}