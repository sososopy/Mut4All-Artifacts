//source file
#include "../include/modify_macro_in_constexpr_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      //Get the source code text of target node
      auto macroText = stringutils::rangetoStr(*(Result.SourceManager),
                                               CE->getSourceRange());

      if (auto *ME = dyn_cast<MemberExpr>(CE->getArg(1))) {
          if (auto *FD = dyn_cast<FieldDecl>(ME->getMemberDecl())) {
              //Perform mutation on the source code text by applying string replacement
              std::string mutatedText = macroText + " + 1";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedText);
          }
      }
    }
}

void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("min"))), 
                            hasArgument(1, memberExpr(hasObjectExpression(
                                declRefExpr(to(varDecl(hasType(pointsTo(
                                    recordDecl(has(fieldDecl())))))))))
                            )).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}