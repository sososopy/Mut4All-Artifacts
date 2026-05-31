//source file
#include "../include/modify_function_return_type_to_void_622.h"

// ========================================================================================================
#define MUT622_OUTPUT 1

void MutatorFrontendAction_622::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (!FD->getReturnType()->isVoidType() && FD->hasBody()) {
        functionsToModify.push_back(FD);
        auto functionRange = FD->getSourceRange();
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), functionRange);
        
        // Modify the function return type to void
        auto returnTypeRange = FD->getReturnTypeSourceRange();
        Rewrite.ReplaceText(returnTypeRange, "void");

        // Modify return statements within the function
        for (auto &stmt : FD->getBody()->children()) {
          if (auto *returnStmt = llvm::dyn_cast<clang::ReturnStmt>(stmt)) {
            Rewrite.ReplaceText(returnStmt->getSourceRange(), "return;");
          }
        }
      }
    }
}

void MutatorFrontendAction_622::MutatorASTConsumer_622::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(returns(voidType()))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}