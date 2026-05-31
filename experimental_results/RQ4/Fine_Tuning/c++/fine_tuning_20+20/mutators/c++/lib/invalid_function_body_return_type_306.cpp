//source file
#include "../include/invalid_function_body_return_type_306.h"

// ========================================================================================================
#define MUT306_OUTPUT 1

void MutatorFrontendAction_306::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      if (FD->isMain())
        return;
      functions.push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>( "DLFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile( DL->getLocation())) return;
      if (!DL->hasBody())
        return;
      if (DL->isMain())
        return;
      auto returntype = DL->getReturnType();
      auto body = DL->getBody();
      if (returntype->isVoidType()) {
        llvm::outs() << "void\n";
        if (isa<ReturnStmt>(*body->child_begin())) {
          llvm::outs() << "return\n";
          return;
        }
      }
      if (returntype->isIntegerType()) {
        llvm::outs() << "int\n";
        if (isa<ReturnStmt>(*body->child_begin())) {
          llvm::outs() << "return\n";
          return;
        }
      }
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (returntype->isVoidType()) {
        llvm::outs() << "void\n";
        declaration.replace(0, 4, "int");
      } else if (returntype->isIntegerType()) {
        llvm::outs() << "int\n";
        declaration.replace(0, 3, "void");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), declaration);
    }
  }
  
void MutatorFrontendAction_306::MutatorASTConsumer_306::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto DLmatcher = functionDecl().bind("DLFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(DLmatcher, &callback);
    matchFinder.matchAST(Context);
}